/*  tilp - link program for TI calculators
 *  Copyright (C) 1999-2001  Romain Lievin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef STRUCT_H
#define STRUCT_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <glib.h>

#define MAXCHARS 256

#include "params.h"
#include "paths.h"

/*
  General GtkTiEmu options
*/
struct options_
{
  // TI core field
  Ti68kParameters *params;
  TicableLinkParam *link_cable;

  gchar *skin_file;
  
  char locale[MAXCHARS];
  gint code_lines;
  gint stack_lines;
  gint mem_lines;
  gint img_type;
  gint img_format;
  gint img_size;
  gchar *screen_file;
  gint screen_counter;
  gint console;
};
typedef struct options_ TieOptions;

extern TieOptions options;

/* Misc */
struct sel
{
  gint address; // address of the breakpoint
  gint id;      // breakpoint id
};
typedef struct sel CODE_BKPT;

struct data_bkpt
{
  gint address;  // breakpoint address
  gint address2; // second bkpt address for range
  gint mode;     // breakpoint access mode
  gint id;       // breakpoint id
};
typedef struct data_bkpt DATA_BKPT;

extern InstPaths inst_paths;

#endif
