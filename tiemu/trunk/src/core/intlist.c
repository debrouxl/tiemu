/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2002, Romain Lievin, Julien Blache
 *  Copyright (c) 2003-2004, Romain Liévin
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

/*
  Specific linked lists: should be replaced with glib
*/

#include <stdlib.h>

#include "intlist.h"

int getSize(struct intlist* list) 
{
  if (!list) 
    return 0;
  else 
    return (getSize(list->next)+1);
}

void addEnd(struct intlist** list,int val) 
{
  struct intlist *l;

  if (!(*list)) 
    addAt(list,0,val);
  else 
    {
      if ((*list)->next) addEnd(&(*list)->next,val);
      else 
	{
	  l=(struct intlist*)malloc(sizeof(struct intlist));
	  (*list)->next = l;
	  l->next = 0;
	  l->val = val;
	}
    }
}

void add2End(struct intlist** list,int val,int val2) 
{
  struct intlist *l;
 
  if (!(*list)) 
    addAt(list,0,val);
  else 
    {
      if ((*list)->next) add2End(&(*list)->next,val,val2);
      else 
	{
	  l=(struct intlist*)malloc(sizeof(struct intlist));
	  (*list)->next = l;
	  l->next = 0;
	  l->val = val;
	  l->val2 = val2;
	}
    }
}

void addAt(struct intlist** list,int i,int val) 
{
  struct intlist* l;
  if (!*list) 
    {
      *list=(struct intlist*)malloc(sizeof(struct intlist));
      (*list)->next=0;
      (*list)->val=val;
      return;
    }
  if (i>0) 
    addAt(&(*list)->next,i-1,val);
  else 
    {
      l=(struct intlist*)malloc(sizeof(struct intlist));
      l->val = val;
      l->next = *list;
      *list = l;
    }
}

void add2At(struct intlist** list,int i,int val,int val2) 
{
  struct intlist* l;
  if (!*list) 
    {
      *list=(struct intlist*)malloc(sizeof(struct intlist));
      (*list)->next=0;
      (*list)->val=val;
      (*list)->val2=val2;
      return;
    }
  if (i>0) 
    add2At(&(*list)->next,i-1,val,val2);
  else 
    {
      l=(struct intlist*)malloc(sizeof(struct intlist));
      l->val = val;
      l->val2 = val2;
      l->next = *list;
      *list = l;
    }
}

void delList(struct intlist** list) 
{
  if (!list) return;
  delList(&(*list)->next);
  free(list);
  *list=0;
}
	
void delAt(struct intlist** list,int i) 
{
  if (!*list) return;
  if (i>0) 
    delAt(&(*list)->next,i-1);
  else 
    {
      struct intlist* temp = (*list)->next;
      free(*list);
      *list = temp;
    }
}

void delEnd(struct intlist** list) 
{
  if (!*list) return;
  if ((*list)->next) 
    delEnd(&(*list)->next);
  else 
    {
      free(*list);
      *list=0;
    }
}
