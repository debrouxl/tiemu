/* tclhelp.c -- TCL interface to help.
   Copyright (C) 1997 Cygnus Solutions.
   Written by Ian Lance Taylor <ian@cygnus.com>.  */

#include "config.h"

#include <stdio.h>
#include <ctype.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <errno.h>
#ifndef errno
extern int errno;
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#else
#ifdef HAVE_STRINGS_H
#include <strings.h>
#else
extern char *strerror ();
#endif
#endif

#ifdef _WIN32
/* We avoid warnings by including this before tcl.h.  */
#include <windows.h>
#include <winuser.h>
#endif

#include <tcl.h>
#include <tk.h>

#include "guitcl.h"
#include "subcommand.h"

/* This file defines one TCL command with subcommands.  This command
   may be used to bring up a help window.

   ide_help initialize ...
       Initialize the help system.

       On Windows, this takes two arguments: the name of the help
       file, and the name of the header file which may be used to map
       help topics to numbers.  This header files is created by the
       help system.

       The Unix help system has not yet been implemented.

   ide_help topic TOPIC
       Brings up a help window for the particular topic.  The topic is
       a string.

   ide_help toc
       Brings up a help window for the main table of contents.

   ide_help display_file FILENAME TOPIC_ID
       The "display_file" subcommand was added as a hack to get the Foundry Tour to
       launch.  The help system can't handle more than one help file and should
       be rewritten */

#ifdef _WIN32

/* Windows implementation.  This uses WinHelp.  */

/* We use an instance of this structure as the client data for the
   ide_help command.  */

struct help_command_data
{
  /* The name of the help file.  */
  char *filename;
  /* The name of the help header file which we use to map topic
     strings to numbers.  */
  char *header_filename;
  /* A hash table mapping help topic strings to numbers.  */
  Tcl_HashTable topic_hash;
  /* Nonzero if the hash table has been initialized.  */
  int hash_initialized;
  /* The window we are passing to WinHelp.  */
  HWND window;
};

/* This function is called as an exit handler.  */

static void
help_command_atexit (ClientData cd)
{
  struct help_command_data *hdata = (struct help_command_data *) cd;

  /* Tell Windows we don't need any more help.  */
  if (hdata->window != NULL)
    WinHelp (hdata->window, hdata->filename, HELP_QUIT, 0);
}

/* This function is called if the ide_help command is deleted.  */

static void
help_command_deleted (ClientData cd)
{
  struct help_command_data *hdata = (struct help_command_data *) cd;

  /* Run the exit handler now, rather than when we exit.  */
  help_command_atexit (cd);
  Tcl_DeleteExitHandler (help_command_atexit, cd);

  if (hdata->filename != NULL)
    ckfree (hdata->filename);
  if (hdata->header_filename != NULL)
    ckfree (hdata->header_filename);
  if (hdata->hash_initialized)
    Tcl_DeleteHashTable (&hdata->topic_hash);
  ckfree ((char *) hdata);
}

/* Initialize the help system: choose a window, and set up the topic
   hash table.  We don't set up the topic hash table when the command
   is created, because there's no point wasting time on it at program
   startup time if the help system is not going to be used.  */

static int
help_initialize (Tcl_Interp *interp, struct help_command_data *hdata)
{
  if (hdata->filename == NULL || hdata->header_filename == NULL)
    {
      Tcl_SetResult (interp, "help system has not been initialized",
		     TCL_STATIC);
      return TCL_ERROR;
    }

  if (hdata->window == NULL)
    {
      HWND window, parent;

      /* We don't really care what window we use, although it should
         probably be one that will last until the application exits.  */
      window = GetActiveWindow ();
      if (window == NULL)
	window = GetFocus ();
      if (window == NULL)
	{
	  Tcl_SetResult (interp, "can't find window to use for help",
			 TCL_STATIC);
	  return TCL_ERROR;
	}

      while ((parent = GetParent (window)) != NULL)
	window = parent;

      hdata->window = window;

      Tcl_CreateExitHandler (help_command_atexit, (ClientData) hdata);
    }

  if (! hdata->hash_initialized)
    {
      FILE *e;
      char buf[200];

      e = fopen (hdata->header_filename, "r");
      if (e == NULL)
	{
	  Tcl_AppendResult (interp, "can't open help file \"",
			    hdata->header_filename, "\": ",
			    strerror (errno), (char *) NULL);
	  return TCL_ERROR;
	}

      Tcl_InitHashTable (&hdata->topic_hash, TCL_STRING_KEYS);
      hdata->hash_initialized = 1;

      /* We expect the format of the header file to be tightly
	 constrained: the lines of interest will look like
	     #define TOPIC_STRING TOPIC_NUMBER
	 We ignore all other lines.  We assume that topic strings have
	 a limited length, since they are created by humans, so for
	 simplicity we use fgets with a fixed size buffer.  The error
	 checking is minimal, but that's OK, because this file is part
	 of the application; it is not created by the user.  */

      while (fgets (buf, sizeof buf, e) != NULL)
	{
	  char *s, *topic;
	  int number;
	  Tcl_HashEntry *he;
	  int new;

	  if (strncmp (buf, "#define", 7) != 0)
	    continue;

	  s = buf + 7;
	  while (isspace ((unsigned char) *s))
	    ++s;
	  topic = s;
	  while (! isspace ((unsigned char) *s))
	    ++s;
	  *s = '\0';

	  number = atoi (s + 1);
	  if (number == 0)
	    continue;

	  he = Tcl_CreateHashEntry (&hdata->topic_hash, topic, &new);
	  Tcl_SetHashValue (he, (ClientData) number);
	}

      fclose (e);
    }

  return TCL_OK;
}

/* Implement the ide_help initialize command.  We don't actually look
   at the files now; we only do that if the user requests help.  */

static int
help_initialize_command (ClientData cd, Tcl_Interp *interp, int argc,
			 char **argv)
{
  struct help_command_data *hdata = (struct help_command_data *) cd;

  hdata->filename = ckalloc (strlen (argv[2]) + 1);
  strcpy (hdata->filename, argv[2]);
  hdata->header_filename = ckalloc (strlen (argv[3]) + 1);
  strcpy (hdata->header_filename, argv[3]);
  return TCL_OK;
}

#define INIT_MINARGS (4)
#define INIT_MAXARGS (4)

/* Implement the ide_help topic command.  */

static int
help_topic_command (ClientData cd, Tcl_Interp *interp, int argc, char **argv)
{
  struct help_command_data *hdata = (struct help_command_data *) cd;
  Tcl_HashEntry *he;

  if (help_initialize (interp, hdata) != TCL_OK)
    return TCL_ERROR;

  he = Tcl_FindHashEntry (&hdata->topic_hash, argv[2]);
  if (he == NULL)
    {
      Tcl_AppendResult (interp, "unknown help topic \"", argv[2], "\"",
			(char *) NULL);
      return TCL_ERROR;
    }

  if (! WinHelp (hdata->window, hdata->filename, HELP_CONTEXT,
		 (DWORD) Tcl_GetHashValue (he)))
    {
      char buf[200];

      FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError (), 0,
		     buf, 200, NULL);
      Tcl_AppendResult (interp, "WinHelp failed: ", buf, (char *) NULL);
      return TCL_ERROR;
    }

  return TCL_OK;
}

/* Implement the ide_help toc command.  */

static int
help_toc_command (ClientData cd, Tcl_Interp *interp, int argc, char **argv)
{
  struct help_command_data *hdata = (struct help_command_data *) cd;

  if (help_initialize (interp, hdata) != TCL_OK)
    return TCL_ERROR;

  if (! WinHelp (hdata->window, hdata->filename, HELP_FINDER, 0))
    {
      char buf[200];

      FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError (), 0,
		     buf, 200, NULL);
      Tcl_AppendResult (interp, "WinHelp failed: ", buf, (char *) NULL);
      return TCL_ERROR;
    }

  return TCL_OK;
}

/* Implement the ide_help display_file command.  */
/* This is a hack to display an external help file, */
/* by 'external' I mean not part of Foundry Help. */
/* This was added specifically to display the Foundry */
/* Tour help file and should be made less hacky in the future */
/* The "display_file" subcommand was added as a hack to get the Foundry Tour to */
/* launch.  The help system can't handle more than one help file and should */
/* be rewritten */

static int
help_display_file_command (ClientData cd, Tcl_Interp *interp, int argc, char **argv)
{
  struct help_command_data *hdata = (struct help_command_data *) cd;
  FILE *e;
  int id = 0;
  DWORD   topic_id; /* default topic id is 0 which brings up the find dialog */

  /* We call Help initialize just to make sure the window handle is setup */
  /* We don't care about the finding the main help file and checking the */
  /* hash table but we do it anyway because this is a hack. */
  if (help_initialize (interp, hdata) != TCL_OK)
    return TCL_ERROR;

  /* We should check to see if the help file we want exists since function */
  /* 'help_initialize' checked the wrong file (it checked the main help file) */
  e = fopen (argv[2], "r");
  if (e == NULL)
  {
	  Tcl_AppendResult (interp, "can't open help file \"",
                argv[2], "\": ",
			    strerror (errno), (char *) NULL);
	  return TCL_ERROR;
  }
  fclose (e);
  if (argc > 3)
  {
      if ( Tcl_GetInt (interp, argv[3], &id) != TCL_OK )
        return TCL_ERROR;
  }

  topic_id = (DWORD) id;
  if (! WinHelp (hdata->window, argv[2], HELP_CONTEXT, topic_id))
  {
      char buf[200];

      FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError (), 0,
		     buf, 200, NULL);
      Tcl_AppendResult (interp, "WinHelp failed: ", buf, (char *) NULL);
      return TCL_ERROR;
  }

  return TCL_OK;
}

/* Initialize the help command structure.  */

struct help_command_data *
hdata_initialize ()
{
  struct help_command_data *hdata;

  hdata = (struct help_command_data *) ckalloc (sizeof *hdata);

  hdata->filename = NULL;
  hdata->header_filename = NULL;
  hdata->hash_initialized = 0;
  hdata->window = NULL;

  return hdata;
}

#else /* ! _WIN32 */

/* The Unix help implementation. */

/* We use an instance of this structure as the client data for the
   ide_help command.  */

struct help_command_data
{
  /* path to webhelp.csh file */
  char *filename;
  /* path to foundry.hh file */
  char *header_filename;
  /* path to help directory */
  char *help_dir;
  /* A hash table mapping help topic strings to numbers. */
  Tcl_HashTable topic_hash;
  /* Nonzero if the hash table has been initialized. */
  int hash_initialized;
  /* pointer to large block of memory used for hashing */
  char *memory_block;
  };

/* This function is called if the ide_help command is deleted.  */

static void
help_command_deleted (ClientData cd)
{
  struct help_command_data *hdata = (struct help_command_data *) cd;

  if (hdata->filename != NULL)
    ckfree (hdata->filename);
  if (hdata->header_filename != NULL)
    ckfree (hdata->header_filename);
  if (hdata->help_dir != NULL)
    ckfree (hdata->help_dir);
  if (hdata->hash_initialized)
    Tcl_DeleteHashTable (&hdata->topic_hash);
  if (hdata->memory_block != NULL)
    ckfree (hdata->memory_block);
  ckfree ((char *) hdata);
}

/* Implement the ide_help initialize command.  */

static int
help_initialize_command (ClientData cd, Tcl_Interp *interp, int argc,
             char **argv)
{
  struct help_command_data *hdata = (struct help_command_data *) cd;

  hdata->filename = ckalloc (strlen (argv[2]) + 1);
  strcpy (hdata->filename, argv[2]);
  hdata->header_filename = ckalloc (strlen (argv[3]) + 1);
  strcpy (hdata->header_filename, argv[3]);
  hdata->help_dir = ckalloc (strlen (argv[4]) + 1);
  strcpy (hdata->help_dir, argv[4]);
  return TCL_OK;
}

static int
help_initialize (Tcl_Interp *interp, struct help_command_data *hdata)
{

  if (hdata->filename == NULL || hdata->header_filename == NULL)
    {
      Tcl_SetResult (interp, "help system has not been initialized",
             TCL_STATIC);
      return TCL_ERROR;
    }

  if (! hdata->hash_initialized)
    {
      FILE *e;
      char buf[200], *block_start;

      block_start = hdata->memory_block = ckalloc(6000);

      e = fopen (hdata->header_filename, "r");
      if (e == NULL)
    {
      Tcl_AppendResult (interp, "can't open help file \"",
                hdata->header_filename, "\": ",
                strerror (errno), (char *) NULL);
      return TCL_ERROR;
    }

      Tcl_InitHashTable (&hdata->topic_hash, TCL_STRING_KEYS);
      hdata->hash_initialized = 1;

      /* We expect the format of the header file to be tightly
     constrained: the lines of interest will look like
         #define TOPIC_STRING TOPIC_FILENAME
     We ignore all other lines.  We assume that topic strings have
     a limited length, since they are created by humans, so for
     simplicity we use fgets with a fixed size buffer.  The error
     checking is minimal, but that's OK, because this file is part
     of the application; it is not created by the user.  */

      while (fgets (buf, sizeof buf, e) != NULL)
    {
      char *s, *topic, *strng;
      Tcl_HashEntry *he;
      int new;

      if (strncmp (buf, "#define", 7) != 0)
        continue;

      s = buf + 7;
      while (isspace ((unsigned char) *s))
        ++s;
      topic = s;
      while (! isspace ((unsigned char) *s))
        ++s;
      *s = '\0';

      ++s;
      while (isspace ((unsigned char) *s))
        ++s;
      strng = s;
      while (! isspace ((unsigned char) *s))
        ++s;
      *s = '\0';
      strcpy (block_start, strng);

      he = Tcl_CreateHashEntry (&hdata->topic_hash, topic, &new);
      Tcl_SetHashValue (he, (ClientData) block_start);
      block_start += strlen(strng) + 2;

    }
      fclose (e);

    }

  return TCL_OK;

}

#define INIT_MINARGS 2
#define INIT_MAXARGS 5

/* Implement the ide_help topic command.  */

static int
help_topic_command (ClientData cd, Tcl_Interp *interp, int argc, char **argv)
{
  struct help_command_data *hdata = (struct help_command_data *) cd;
  Tcl_HashEntry *he;
  char htmlFile[250], htmlFile2[250];

  if (help_initialize (interp, hdata) != TCL_OK)
    return TCL_ERROR;

  he = Tcl_FindHashEntry (&hdata->topic_hash, argv[2]);
  if (he == NULL)
    {
      Tcl_AppendResult (interp, "unknown help topic \"", argv[2], "\"",
            (char *) NULL);
      return TCL_ERROR;
    }
  else
    {

     strcpy (htmlFile, hdata->help_dir);
     strcat (htmlFile, "/");
     strcat (htmlFile, Tcl_GetHashValue (he));

     ShowHelp (htmlFile, hdata->filename);
     return TCL_OK;
    }
}

/* Implement the ide_help toc command.  */

static int
help_toc_command (ClientData cd, Tcl_Interp *interp, int argc, char **argv)
{
  struct help_command_data *hdata = (struct help_command_data *) cd;
  char htmlFile[250];

  strcpy (htmlFile, hdata->help_dir);
  strcat (htmlFile, "/start.htm");

  if (! ShowHelp (htmlFile, hdata->filename))
    { Tcl_SetResult (interp, "Help not available", TCL_STATIC);
      return TCL_ERROR;
    }
}

/* Implement the ide_help display command.  */

static int
help_display_file_command (ClientData cd, Tcl_Interp *interp, int argc, char **argv)
{
  struct help_command_data *hdata = (struct help_command_data *) cd;

  if (! ShowHelp (argv[2], hdata->filename))
    { Tcl_SetResult (interp, "Help not available", TCL_STATIC);
      return TCL_ERROR;
    }
}

/* Initialize the help command structure.  */

struct help_command_data *
hdata_initialize ()
{
  struct help_command_data *hdata;

  hdata = (struct help_command_data *) ckalloc (sizeof *hdata);

  hdata->filename = NULL;
  hdata->help_dir = NULL;
  hdata->header_filename = NULL;
  hdata->hash_initialized = 0;
  hdata->memory_block = NULL;

  return hdata;
}

int
ShowHelp(const char* html_filename, const char* shellFile)
{
  int pidProcess = fork();
  if (pidProcess == 0)
    {
    /* new child process */
    execl("/bin/csh", "/bin/csh", shellFile, html_filename, 0);
    /* execl only returns if error occurred */
     _exit(-1);
    }
  /* fork failed, error number is why */
  else if (pidProcess == (-1))
    { return 0; }
}

#endif /* ! _WIN32 */

/* The subcommand table.  */
/* The "display_file" subcommand was added as a hack to get the Foundry Tour to */
/* launch.  The help system can't handle more than one help file and should */
/* be rewritten */
static const struct ide_subcommand_table help_commands[] =
{
  { "initialize",	help_initialize_command, INIT_MINARGS, INIT_MAXARGS },
  { "topic",		help_topic_command,	 3, 3 },
  { "toc",		help_toc_command,	 2, 2 },
  { "display_file",      help_display_file_command,    3, 4 },
  { NULL, NULL, 0, 0 }
};

/* This function creates the ide_help TCL command.  */

int
ide_create_help_command (Tcl_Interp *interp)
{
  struct help_command_data *hdata;

  hdata = hdata_initialize ();

  return ide_create_command_with_subcommands (interp, "ide_help",
					      help_commands,
					      (ClientData) hdata,
					      help_command_deleted);
}
