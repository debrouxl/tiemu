#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdlib.h>
#include <gtk/gtk.h>
#include <ctype.h>

#include "debug_cb.h"
#include "debug_dbox.h"
#include "support.h"

#include "interface.h"
#include "platform.h"
#include "struct.h"
#include "ticalc.h"
#include "paths.h"
#include "support.h"
#include "utils.h"
#include "pgmentbkpt_dbox.h"
#include "pgmentbkpt_cb.h"
#include "trap_dbox.h"
#include "trap_cb.h"
#include "dbgopt_dbox.h"
#include "dbgopt_cb.h"
#include "msg_cb.h"
#include "dbkpt_dbox.h"
#include "dbkpt_cb.h"
#include "bkpt_cause.h"
#include "main.h"

#include "sysdeps.h"

#define DEBUG_LINES 20

/* Used widget */
static GtkWidget *debugger_dbox = NULL;
static GtkWidget *code_clist;
static GtkWidget *mem_clist;
static GtkWidget *stack_clist;
static GtkWidget *reg_text;
static GtkWidget *statusbar;

gint refresh_register_dbox(void);
gint refresh_code_dbox(void);
gint refresh_stack_dbox(void);
gint refresh_memory_dbox(void);


gint display_debugger_dbox(void)
{
  GtkWidget *dbox;
  GtkWidget *clist;
  GtkListStore *list;
  GtkTreeModel *model;
  GtkTreeSelection *sel;
  
  if(debugger_dbox == NULL)
    {
      dbox = create_debugger_dbox();
      debugger_dbox = dbox;

      reg_text = lookup_widget(dbox, "text5");
      
      /* Code clist */
      code_clist = lookup_widget(dbox, "clist1");
      clist = code_clist;
      list = gtk_list_store_new(4, GDK_TYPE_PIXBUF, G_TYPE_STRING,
				G_TYPE_STRING, G_TYPE_POINTER);
      model = GTK_TREE_MODEL(list);
      
      gtk_tree_view_set_model(GTK_TREE_VIEW(clist), model); 
      gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(clist), TRUE); 
      
      gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(clist), 0, _("B"),
						  gtk_cell_renderer_pixbuf_new(),
						  "pixbuf", 0, NULL);
      gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(clist), 1, _("Addr"),
						  gtk_cell_renderer_text_new(),
						  "text", 1, NULL);
      gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(clist), 2, _("Code"),
						  gtk_cell_renderer_text_new(),
						  "text", 2, NULL);
      gtk_list_store_clear(list);

      /* Memory clist */
      mem_clist = lookup_widget(dbox, "clist2");
      clist = code_clist;
      list = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING,
				G_TYPE_STRING);
      model = GTK_TREE_MODEL(list);
      
      gtk_tree_view_set_model(GTK_TREE_VIEW(clist), model); 
      gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(clist), FALSE); 
      
      gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(clist), 0, _("Addr"),
						  gtk_cell_renderer_text_new(),
						  "text", 0, NULL);
      gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(clist), 1, _("Dump"),
						  gtk_cell_renderer_text_new(),
						  "text", 1, NULL);
      gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(clist), 2, _("Char"),
						  gtk_cell_renderer_text_new(),
						  "text", 2, NULL);
      gtk_list_store_clear(list);

      /* Stack clist */
      stack_clist = lookup_widget(dbox, "clist3");
      clist = stack_clist;
      mem_clist = lookup_widget(dbox, "clist2");
      clist = code_clist;
      list = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
      model = GTK_TREE_MODEL(list);
      
      gtk_tree_view_set_model(GTK_TREE_VIEW(clist), model); 
      gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(clist), FALSE); 
      
      gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(clist), 0, _("SP"),
						  gtk_cell_renderer_text_new(),
						  "text", 0, NULL);
      gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(clist), 1, _("Value"),
						  gtk_cell_renderer_text_new(),
						  "text", 1, NULL);
      gtk_list_store_clear(list);


      statusbar = lookup_widget(dbox, "statusbar1");

      /* We need to catch the selection on the code_clist */
      sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(code_clist));
      gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);
      
      gtk_widget_show_all(dbox);

      g_signal_connect(G_OBJECT(sel), "changed",
		       G_CALLBACK(on_code_clist_selection_changed), NULL);

      display_bkpt_cause();
    }

  refresh_stack_dbox();
  refresh_memory_dbox();
  refresh_code_dbox();
  refresh_register_dbox();
      
  return 0;
}


/* Local persistant variables */
GtkWidget *data_bkpt_dbox = NULL;
gint bkpt_encountered = 0;
gint data_addr = 0x000000;
gint selected_row = -1;

/* Some linked-list for breakpoints */
GList *bkpt_address_list = NULL;
GList *bkpt_access_list = NULL;
GList *bkpt_vector_list = NULL;
GList *bkpt_trap_list = NULL;


/*****************/
/* Debugger dbox */
/*****************/

static int put_in_sb(gchar *s)
{
  gint id;

  id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), s);
  gtk_statusbar_push(GTK_STATUSBAR(statusbar), id, s);

  return id;
}


/*
  Called when the Debugger Dbox is destroyed 
*/
void
on_debugger_dbox_destroy               (GtkObject       *object,
                                        gpointer         user_data)
{
  gint i;
  CODE_BKPT *s;

  // The debugger dbox is not active any longer
  code_clist = NULL;
  debugger_dbox = NULL;

  // Restore breakpoints
  // Bkpts are removed when the debugger is launched so
  // that we can 'single step'
  for(i=0; i<g_list_length(bkpt_address_list); i++)
    {
      s = (CODE_BKPT *)g_list_nth_data(bkpt_address_list, i);
      
      ti68k_setBreakpointAddress(s->address);
    }
  
  // Emulation core in free running
  unhalt();
}


/*
  Initialize the 'Registers' box
*/
gint refresh_register_dbox(void)
{
  GtkWidget *text = reg_text;
  GtkTextBuffer *buf;
  GtkTextIter start, end;
  GtkTextTag *red_tag;
  gint i;
  gchar buffer[MAXCHARS];
  static gint prev_Dx[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  static gint prev_Ax[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  static gint prev_SP = 0;
  static gint prev_SR = 0;
  static gint prev_PC = 0;

  buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
  gtk_text_buffer_set_text(buf, "", -1);
  gtk_text_buffer_get_start_iter(buf, &start);
  gtk_text_buffer_get_end_iter(buf, &end);

  red_tag = gtk_text_buffer_create_tag(buf, "red_text", "foreground", "red", NULL);

  /* Display the Dx & Ax registers */
  for(i=0; i<8; i++)
    {
      sprintf(buffer, "D%i=%08X", i, ti68k_getDataRegister(i));
      gtk_text_buffer_insert(buf, &end, buffer, -1);
      if(prev_Dx[i] != ti68k_getDataRegister(i))
	gtk_text_buffer_apply_tag(buf, red_tag, &start, &end);
      
      start = end;

      sprintf(buffer, "  ");
      gtk_text_buffer_insert(buf, &end, buffer, -1);
      
      sprintf(buffer, "A%i=%08X\n", i, ti68k_getAddressRegister(i));
      gtk_text_buffer_insert(buf, &end, buffer, -1);
      if(prev_Ax[i] != ti68k_getAddressRegister(i))
	gtk_text_buffer_apply_tag(buf, red_tag, &start, &end);
      
      start = end;
    }

  gtk_text_buffer_insert(buf, &end, "\n", -1);
  start = end;
  
  /* Display the SP register */
  sprintf(buffer, "SP=%06X\n", ti68k_getSpRegister());
  gtk_text_buffer_insert(buf, &end, buffer, -1);
  if(prev_SP != ti68k_getSpRegister())
    gtk_text_buffer_apply_tag(buf, red_tag, &start, &end);

  start = end;

  /* Display the SR register */
  sprintf(buffer, "SR=%04X      ", ti68k_getSrRegister());
  gtk_text_buffer_insert(buf, &end, buffer, -1);
  if(prev_SR != ti68k_getSrRegister())
    gtk_text_buffer_apply_tag(buf, red_tag, &start, &end);

  start = end;

  /* Display the PC register */
  sprintf(buffer, "PC=%06X  \n", ti68k_getPcRegister());
  gtk_text_buffer_insert(buf, &end, buffer, -1);
  if(prev_PC != ti68k_getPcRegister())
    gtk_text_buffer_apply_tag(buf, red_tag, &start, &end);

  /* Display the status flag register */
  /*
  sprintf(buffer, "T=%i  S=%i  M=%i  I=%i  \n", 0, 0, 0, 0);
  gtk_text_buffer_insert(buf, &end, buffer, -1);

  sprintf(buffer, "X=%i  N=%i  Z=%i  V=%i  C=%i\n", 0, 0, 0, 0, 0);
  gtk_text_buffer_insert(buf, &end, buffer, -1);

  */
  gtk_text_buffer_insert(buf, &end, ti68k_getFlagRegister(), -1);

  /* Store old values for colour display */
  for(i=0; i<8; i++)
    {
      prev_Dx[i] = ti68k_getDataRegister(i);
      prev_Ax[i] = ti68k_getAddressRegister(i);
    }
  prev_SR = ti68k_getSrRegister();
  prev_SP = ti68k_getSpRegister();
  prev_PC = ti68k_getPcRegister();

  return 0;
}

/*
  Initialize the 'code/disasm' box
*/

/* Utility function */
gint refresh_breakpoints(void)
{
  GtkWidget *clist = code_clist;
  GtkTreeModel *model;
  GtkListStore *list;
  GtkTreePath *path;
  GtkTreeIter iter;
  GdkPixbuf *run, *bkpt, *none;
  gint i;
  CODE_BKPT *bp;
  gint addr_bp, addr_active, addr;
  gpointer addrp;

  path = gtk_tree_path_new_from_indices(0, -1);
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(clist));
  list = GTK_LIST_STORE(model);
  if (gtk_tree_model_get_iter(model, &iter, path) == FALSE)
    {
      gtk_tree_path_free(path);
      return -1;
    }

  gtk_tree_path_free(path);

  /* Load pixbufs */
  run = create_pixbuf("run.xpm");
  bkpt = create_pixbuf("bkpt.xpm");
  none = create_pixbuf("void.xpm");
 
  addr_active = ti68k_getPcRegister();
  //fprintf(stdout, "PC in refresh: %06X\n", addr_active);

  /* Fill the clist */
  do
    {
      /* Get addr from the clist */
      gtk_tree_model_get(model, &iter, 3, &addrp, -1);
      addr = GPOINTER_TO_INT(addrp);

      //printf("Looking for a breakpoint at 0x%06x\n", addr);

      /* Look for a matching breakpoint */
      for (i = 0; i < g_list_length(bkpt_address_list); i++)
	{
	  bp = (CODE_BKPT *)g_list_nth_data(bkpt_address_list, i);
	  addr_bp = bp->address;

	  if (addr == addr_active)
	    {
	      /* set the run pixmap */
	      //printf("Address is active: Ox%06x\n", addr);
	      gtk_list_store_set(list, &iter, 0, run, -1);
	    }
	  else if (addr == addr_bp)
	    {
	      /* set the bkpt pixmap */
	      //printf("Breakpoint found at 0x%06x\n", addr);
	      gtk_list_store_set(list, &iter, 0, bkpt, -1);
	    }
	  else
	    {
	      /* set the void pixmap */
	      //printf("No breakpoint at 0x%06x\n", addr);
	      gtk_list_store_set(list, &iter, 0, none, -1);
	    }
	}
    }
  while (gtk_tree_model_iter_next(model, &iter) == TRUE);

  g_object_unref(run);
  g_object_unref(bkpt);
  g_object_unref(none);

  return 0;
}

gint refresh_code_dbox(void)
{
  GtkWidget *clist = code_clist;
  GtkTreeModel *model;
  GtkListStore *list;
  GtkTreeIter iter;
  GtkTreePath *path;
  gchar *text[2];
  gint i;
  gint addr_active = ti68k_getPcRegister();
  gint addr;
  gpointer addrp;
  gint offset;
  gchar buffer[MAXCHARS];
  gboolean found;
  
  path = gtk_tree_path_new_from_indices(0, -1);
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(clist));
  list = GTK_LIST_STORE(model);
  if (gtk_tree_model_get_iter(model, &iter, path) == FALSE)
    {
      gtk_tree_path_free(path);
      return -1;
    }

  gtk_tree_path_free(path);

  found = FALSE;
  do
    {
      /* Get addr from the clist */
      gtk_tree_model_get(model, &iter, 3, &addrp, -1);
      addr = GPOINTER_TO_INT(addrp);

      if (addr == addr_active)
	{
	  found = TRUE;
	  break;
	}
    }
  while (gtk_tree_model_iter_next(model, &iter) == TRUE);

  if (found == TRUE)
    {
      /* row found, no need to reload */
      refresh_breakpoints();
      return 0;
    }

  /* Reload the CList */
  list = GTK_LIST_STORE(model);
  gtk_list_store_clear(list);

  for(i=0; i<options.code_lines; i++)
    {
      offset = ti68k_disasm(addr, buffer);
      text[0] = g_strdup(buffer);
      text[0][9] = '\0';
      text[1] = g_strdup(buffer+10);

      gtk_list_store_append(list, &iter);
      gtk_list_store_set(list, &iter, 1, text[0],
			 2, text[1], 3, GINT_TO_POINTER(addr),
			 -1);
      //fprintf(stderr, "%i: %06X\n", i, addr);

      addr += offset;

      g_free(text[0]);
      g_free(text[1]);
    }

  /* Refresh breakpoints */
  refresh_breakpoints();

  return 0;
}

/*
  Initialize the 'memory' box
*/
gint refresh_memory_dbox(void)
{
  GtkWidget *clist = mem_clist;
  GtkTreeModel *model;
  GtkListStore *list;
  GtkTreeIter iter;
  gchar *text[3];
  gint i, k;
  UBYTE *ti_ram = (UBYTE *)ti68k_getRamPtr();
  gint addr = data_addr;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(clist));
  list = GTK_LIST_STORE(model);
  
  gtk_list_store_clear(list);

  /* Fill the clist */
  for(i = 0; i < options.mem_lines; i++, addr+=8)
    {
      text[0] = g_strdup_printf("%06X", addr);
      text[1] = g_strdup_printf("%02X %02X %02X %02X %02X %02X %02X %02X ",
				ti_ram[addr+0], ti_ram[addr+1],
				ti_ram[addr+2], ti_ram[addr+3],
				ti_ram[addr+4], ti_ram[addr+5],
				ti_ram[addr+6], ti_ram[addr+7]);
      
      text[2] = g_strdup_printf("%c%c%c%c%c%c%c%c", 
				isprint(ti_ram[addr+0]) ? ti_ram[addr+0] : '.',
				isprint(ti_ram[addr+1]) ? ti_ram[addr+1] : '.',
				isprint(ti_ram[addr+2]) ? ti_ram[addr+2] : '.',
				isprint(ti_ram[addr+3]) ? ti_ram[addr+3] : '.',
				isprint(ti_ram[addr+4]) ? ti_ram[addr+4] : '.',
				isprint(ti_ram[addr+5]) ? ti_ram[addr+5] : '.',
				isprint(ti_ram[addr+6]) ? ti_ram[addr+6] : '.',
				isprint(ti_ram[addr+7]) ? ti_ram[addr+7] : '.');
      
      gtk_list_store_append(list, &iter);
      gtk_list_store_set(list, &iter,
			 0, text[0], 1, text[1],
			 2, text[2], -1);

      for(k = 0; k < 3; k++)
	{
	  //fprintf(stderr, "<%i %p %s>\n", k, row_text[k], row_text[k]); // bug here !!!
	  g_free(text[k]);
	}
    }

  return 0;
}

/*
  Initialize the 'stack' box
*/
gint refresh_stack_dbox(void)
{
  GtkWidget *clist = stack_clist;
  GtkTreeModel *model;
  GtkListStore *list;
  GtkTreeIter iter;
  gchar *text[2];
  gint i;
  gint sp = ti68k_getSpRegister();
  UWORD *ti_ram = (UWORD *)ti68k_getRamPtr();
  gint addr;

  model = gtk_tree_view_get_model(GTK_TREE_VIEW(clist));
  list = GTK_LIST_STORE(model);
  
  gtk_list_store_clear(list);

  /* Fill the clist */
  for(i = 0, addr = sp; i < 10; i++, addr += 2)
    {
      text[0] = g_strdup_printf("%06X:", addr);
      text[1] = g_strdup_printf("%04X", ti_ram[addr]);

      gtk_list_store_append(list, &iter);
      gtk_list_store_set(list, &iter,
			 0, text[0], 1, text[1],
			 -1);
    
      g_free(text[0]);
      g_free(text[1]);
    }

  return 0;
}


/********************************/
/* Debugger dbox menu callbacks */
/********************************/

/*
  Debug|Run (F5)
*/
void
on_run1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  put_in_sb(_("Running..."));

  // Run the emulation core
  unhalt();
}


/*
  Debug|Step (F7)
*/

void
on_step1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gint addr;

  put_in_sb(_("One step (F7)"));
  DISPLAY("Step (F7)\n");

  // Set up an internal trap (DBTRACE) which will launch/refresh the
  // debugger when encountered
  addr = ti68k_getPcRegister();
  DISPLAY("addr=$%06x\n", addr);
  ti68k_doSingleStep(); // set trap
  unhalt();    // emulator in free running
}


void
on_step_over1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  put_in_sb(_("Step over."));
  fprintf(stderr, "Step over (F8): not yet implemented\n");

  /* Execute one instruction */
  //bkpt_encountered = !doInstructions(1, 0);

  refresh_code_dbox();

  /* Refres_breakpoints */
  refresh_breakpoints();
}


/*
  Run up to cursor (F4)
*/
void
on_run_to_cursor1_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkTreeModel *model;
  GtkTreePath *path;
  GtkTreeIter iter;
  gint addr_to_go, next_addr;
  gint i;
  gpointer addrp;
  gchar buffer[64];

  put_in_sb(_("Ran up to cursor."));
  fprintf(stderr, "Run to cursor (F4)\n");

  /* Execute some instructions */
  if(selected_row != -1)
    {
      printf("PC: 0x%06x\n", ti68k_getPcRegister());

      model = gtk_tree_view_get_model(GTK_TREE_VIEW(code_clist));
      path = gtk_tree_path_new_from_indices(selected_row, -1);
      gtk_tree_model_get_iter(model, &iter, path);
      gtk_tree_path_free(path);
      gtk_tree_model_get(model, &iter, 3, &addrp, -1);
      addr_to_go = GPOINTER_TO_INT(addrp);

      printf("addr to go: 0x%06x\n", addr_to_go);
      for(i=1, next_addr = ti68k_getPcRegister(); next_addr < addr_to_go; i++)
	{
	  next_addr += ti68k_disasm(next_addr, buffer);
	  printf("-> buffer: <%s>\n", buffer);
	  if(i > options.code_lines)
	    break;
	}
      //bkpt_encountered = !doInstructions(i, 0);
    }
  else
    return; 

  refresh_code_dbox();
  
  /* Refresh breakpoints */
  refresh_breakpoints();
}


/*
  Debug|Break (F11)
*/
void
on_break1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gint i;
  gint addr;
  //GtkWidget *clist = code_clist;
  CODE_BKPT *s;

  put_in_sb(_("Break !"));
  halt();
  fprintf(stderr, "Break (F11)\n");

  /* Place a breakpoint */
  addr = ti68k_getPcRegister();
  //addr = getPcRegister() + disasm(getPcRegister(), buffer);
  i = ti68k_setBreakpointAddress(addr);

  s = (CODE_BKPT *)g_malloc(sizeof(CODE_BKPT));
  s->address = addr;
  s->id = i;
  bkpt_address_list = g_list_append(bkpt_address_list, s);

  refresh_code_dbox();
  
  /* Refresh breakpoints */
  refresh_breakpoints();
}


/* Quit the 'Debugger' window */
void
on_exit2_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gtk_widget_destroy(debugger_dbox);
}


void
on_set_breakpoint_at_selection1_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkTreeModel *model;
  GtkListStore *list;
  GtkTreeIter iter;
  GtkTreePath *path;
  GdkPixbuf *bkpt;
  gint addr;
  gpointer addrp;
  gint i;
  CODE_BKPT *s, *s1;

  put_in_sb(_("Breakpoint added."));
  fprintf(stderr, "Breakpoints|Set bkpt at selection (F2)\n");
  /* Retrieve the selected line and its address */
  if(selected_row != -1)
    {
      model = gtk_tree_view_get_model(GTK_TREE_VIEW(code_clist));
      path = gtk_tree_path_new_from_indices(selected_row, -1);
      gtk_tree_model_get_iter(model, &iter, path);
      gtk_tree_path_free(path);
      gtk_tree_model_get(model, &iter, 3, &addrp, -1);
      addr = GPOINTER_TO_INT(addrp);

      /* Check whether we already have a breakpoint */      
      for(i=0; i<g_list_length(bkpt_address_list); i++)
	{
	  s1 = (CODE_BKPT *)g_list_nth_data(bkpt_address_list, i);
	  if(addr == s1->address)
	    {
	      /* If yes, then remove it (toggle bkpt) */
	      ti68k_delBreakpointAddress(s1->id);
	      bkpt_address_list = g_list_remove(bkpt_address_list, s1);
	      g_free(s1);
	      printf("Bkpt toggled\n");
	      refresh_breakpoints();

	      return;
	    }
	}

      bkpt = create_pixbuf("bkpt.xpm");
      list = GTK_LIST_STORE(model);
      gtk_list_store_set(list, &iter,
			 0, bkpt, -1);
      g_object_unref(bkpt);
    }
  else
    {
      addr = ti68k_getPcRegister();
    }
    
  /* Place a breakpoint */
  fprintf(stdout, "Place a breakpoint at %06X\n", addr);
  i = ti68k_setBreakpointAddress(addr);
  
  s = (CODE_BKPT *)g_malloc(sizeof(CODE_BKPT));
  s->address = addr;
  s->id = i;
  bkpt_address_list = g_list_append(bkpt_address_list, s);
}


void
on_set_breakpoint_on_program_entry1_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_pgmentrybkpt_dbox ();
}


void
on_set_breakpoint_on_trap_exception1_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_trap_dbox();
}


void
on_set_data_breakpoint1_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  //gtk_widget_show(create_add_data_bkpt_dbox());
  display_data_bkpts_dbox();
}

/* Local functions used below */
static void clear_all_bkpt_address(gpointer data, gpointer user_data)
{
  CODE_BKPT *s = (CODE_BKPT *)data;
  ti68k_delBreakpointAddress(s->id);

  g_free(s);
}

void
on_clear_all_breakpoints1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  fprintf(stderr, "Breakpoints|Clear all bkpts\n");
  g_list_foreach(bkpt_address_list, clear_all_bkpt_address, NULL);
  g_list_free(bkpt_address_list);
  bkpt_address_list = NULL;

  /* Refresh breakpoints */
  refresh_breakpoints();
}


void
on_log1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  
}


void
on_view_log1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  msg_box2(_("Information"), 
	   _("This function is not available yet."));
}


void
on_edit_logged_addresses1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  msg_box2(_("Information"), 
	  _("This function is not available yet."));
}


void
on_log_link_port_send1_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  // to do
}


void
on_log_link_port_receive1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  // to do
}


void
on_clear_all1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  msg_box2(_("Information"), 
	   _("This function is not available yet."));
}


void
on_enable_logging1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  // to do
}


gboolean
on_clist1_button_press_event           (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  GdkEventButton *bevent;
  gboolean valid;

  valid = gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(widget),
					(gint)event->x, (gint)event->y,
					NULL, NULL, NULL, NULL);

  if (valid == FALSE)
    return FALSE;

  switch(event->type)
    {
    case GDK_BUTTON_PRESS:
      if(event->button==3)
	{
	  bevent=(GdkEventButton *)(event);
	  gtk_menu_popup(GTK_MENU(create_code_popup_menu()), 
			 NULL, NULL, NULL, NULL, 
			 bevent->button, bevent->time);

	  return TRUE;
	}
      break;
    case GDK_2BUTTON_PRESS:
      break;
    default:
      break;        
    }
  
  return FALSE;
}


void 
on_code_clist_selection_changed (GtkTreeSelection *sel, 
				 gpointer user_data)
{
  GList *paths;
  GtkTreeModel *model;

  paths = gtk_tree_selection_get_selected_rows(sel, &model);

  if (paths != NULL)
    selected_row = *gtk_tree_path_get_indices(g_list_nth_data(paths, 0));
  else
    selected_row = -1;

  g_list_foreach(paths, (GFunc)gtk_tree_path_free, NULL);
  g_list_free(paths);
}


gboolean
on_clist2_key_press_event              (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{
  //gint row, column;

  printf("Key pressed\n");
  /*
  if (gtk_clist_get_selection_info (GTK_CLIST (widget), event->x, event->y, 
  &row, &column))
    {
      printf("at row %i, column %i\n", row, column);
    }
  */
  return FALSE;
}


gboolean
on_clist2_button_press_event           (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  GdkEventButton *bevent;
  gboolean valid;

  valid = gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(widget),
					(gint)event->x, (gint)event->y,
					NULL, NULL, NULL, NULL);

  if (valid == FALSE)
    return FALSE;

  switch(event->type)
    {
    case GDK_BUTTON_PRESS:
      if(event->button==3)
	{
	  bevent=(GdkEventButton *)(event);
	  gtk_menu_popup(GTK_MENU(create_memory_popup_menu()), 
			 NULL, NULL, NULL, NULL, 
			 bevent->button, bevent->time);

	  return TRUE;
	}
      break;
    case GDK_2BUTTON_PRESS:
      break;
    default:
      break;        
    }
  
  return FALSE;
}

/*******************/
/* Code popup menu */
/*******************/
//begin

void
on_go_to_address1_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gtk_widget_show_all(create_gotocode_dbox());
}


void
on_go_to_pc1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


/* Right button menu in the code part of the Debugger Window */
void
on_set_breakpoint1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  // Equivalent to the menu
  on_set_breakpoint_at_selection1_activate(menuitem, code_clist);
}


void
on_set_pc_to_selection1_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkTreeModel *model;
  GtkTreePath *path;
  GtkTreeIter iter;
  gint addr;
  gpointer addrp;

  /* Retrieve the selected line and its address */
  if(selected_row != -1)
    {
      model = gtk_tree_view_get_model(GTK_TREE_VIEW(code_clist));
      path = gtk_tree_path_new_from_indices(selected_row, -1);
      gtk_tree_model_get_iter(model, &iter, path);
      gtk_tree_path_free(path);
      gtk_tree_model_get(model, &iter, 3, &addrp, -1);
      addr = GPOINTER_TO_INT(addrp);
      
      ti68k_setPcRegister(addr);
      refresh_register_dbox();
    }
}


void
on_force_refresh1_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  refresh_code_dbox();
  
  /* Refresh breakpoints */
  refresh_breakpoints();
}

/********************/
/* Memoy popup menu */
/********************/

void
on_edit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  
}


void
on_go_to1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gtk_widget_show_all(create_gotodata_dbox());
}


/********************************/
/* 'Address to go' dialog boxes */
/********************************/

static gint code_addr_to_go = 0x000000;
static gint data_addr_to_go = 0x000000;

void
on_entry10_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{
  gchar *s;

  s = gtk_editable_get_chars(editable, 0, -1);
  if(sscanf(s, "0x%6x", &code_addr_to_go) < 1)
    code_addr_to_go =  0x000000;

  g_free(s);
}


void
on_button45_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  ti68k_setPcRegister(code_addr_to_go);
  refresh_register_dbox();

  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "gotocode_dbox"));
}

/* 'Address mem to go' dialog box */

void
on_entry11_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{
  gchar *s;

  s = gtk_editable_get_chars(editable, 0, -1);
  if(sscanf(s, "0x%6x", &data_addr_to_go) < 1)
    data_addr_to_go =  0x000000;

  g_free(s);
}


void
on_button99_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  data_addr = data_addr_to_go;
  refresh_memory_dbox();

  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "gotodata_dbox"));
}


void
on_options1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  display_dbgopt_dbox();
}

/* Memory box events for test purposes */
gboolean
on_clist2_event                        (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  /*
  GdkEventButton *bevent = (GdkEventButton *)event;
  int x, y;
  int row, column;

  if( (event->type >= 4) && (event->type <= 9))
    {
      x = bevent->x;
      y = bevent->y;
      gtk_clist_get_selection_info (GTK_CLIST(widget), x, y, &row, &column);
      fprintf(stdout, "event %i, x=%i, y=%i, row=%i, column=%i\n",
	      event->type, x, y, row, column);
    }
  */
  return FALSE;
}

void
on_gotocode_dbox_destroy               (GtkObject       *object,
                                        gpointer         user_data)
{
  //unhalt();
}


void
on_gotodata_dbox_destroy               (GtkObject       *object,
                                        gpointer         user_data)
{
  //unhalt();
}

/**/

