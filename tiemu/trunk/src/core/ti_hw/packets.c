/*  gtktiemu - a TI89/92/92+ emulator
 *  (c) Copyright 2000-2001, Romain Lievin and Thomas Corvazier
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
  Old linkport management
*/

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "uae.h"

#include "packets.h"
#include "hardware.h"
#include "lib68k.h"


int txMode  = 0;
int nextAction = 0;

UBYTE *backupPtr = NULL;
int backupSize = 0;
int backupBlock = 0;

UBYTE *varPtr = NULL;
int varSize = 0;

int dataSize = 0;

int outPos = 0;
int outBytes = 0;
int inBytes = 0;
int lastPacketPos = 0;

int count=0;

UBYTE *outBuffer;
UBYTE *inBuffer;

tifile *dirPtr = NULL;

#define OUT_SIZE 32768
#define IN_SIZE 32768

UBYTE prot_get_byte(void) 
{
  if(outBytes > outPos)
    return outBuffer[outPos++];
  else
    return 0;
}

int prot_bytes_left(void) 
{
  if(!(outBytes-outPos) && nextAction == ACT_NOACTION)
    lc_internal = 0;
  return (outBytes-outPos);
}

int init_protocol(void) 
{
  if(outBuffer == NULL)
    outBuffer = malloc(OUT_SIZE);
  if(inBuffer == NULL)
    inBuffer = malloc(IN_SIZE);
  return (inBuffer && outBuffer);
}

void reset_protocol(void) 
{
  outPos = outBytes = inBytes = lastPacketPos = 0;
  dataSize = txMode = nextAction = 0;
}

/* 
   Parse and store one byte from the TI92. If we have a complete
   packet, we call receive_packet() to handle it.
*/
void prot_receive_byte(UBYTE arg) 
{
  inBuffer[inBytes++] = arg;
  if(inBytes == 4) 
    {
      switch(inBuffer[1]) 
	{
	case PKT_VARDATA :
	case PKT_VARHEAD :
	  dataSize = (((UWORD)inBuffer[3])<<8)|inBuffer[2];
	  break;
	case PKT_READY :
	case PKT_EXPECT :
	default:
	  receive_packet(inBuffer[1], inBuffer[3], NULL);
	  inBytes = 0;
	  break;
	}
    }
  else if(inBytes == dataSize+6) 
    {
      receive_packet(inBuffer[1], dataSize, &inBuffer[4]);
      inBytes = dataSize = 0;
    }
}

void receive_packet(int type, int size, UBYTE *data) 
{
  if(type == PKT_READY && size) 
    {
      fprintf(stdout, _("Resending packet!\n"));
      resend_lastpacket();
      return;
    }
  outPos = outBytes = 0;
  
    if(txMode == TXM_GETDIR) 
      {
	if(nextAction == ACT_VARHEAD && type == PKT_VARHEAD) 
	  {
	    send_packet(PKT_READY, 0, NULL);
	    send_packet(PKT_EXPECT, 0xffff, NULL);
	    nextAction = ACT_VARDATA;
	  }
	else
	  if(nextAction == ACT_CONTEOT) 
	    {
	      if(type == PKT_EOT) 
		{
		  send_packet(PKT_READY, 0, NULL);
		  nextAction = ACT_NOACTION;
		  link_progress(LINK_STOP, NULL, 0);
		}
	      else if(type == PKT_CONT) 
		{
		  send_packet(PKT_READY, 0, NULL);
		  send_packet(PKT_EXPECT, 0xffff, NULL);
		  nextAction = ACT_VARDATA;
		}
	    }
	  else
	    if(nextAction == ACT_VARDATA && type == PKT_VARDATA) 
	      {
		strcpy(dirPtr->name, &data[4]);
		dirPtr->name[8] = 0;
		dirPtr->type = data[12];
		dirPtr->size = ((ULONG)data[14])|(((ULONG)data[15])<<8)|(((ULONG)data[16])<<16)|(((ULONG)data[17])<<24);
		dirPtr++;
		dirPtr->type = 255;
		send_packet(PKT_READY, 0, NULL);
		nextAction = ACT_CONTEOT;
	      }
      }
    
    if(txMode == TXM_SENDVAR) 
      {
	if(nextAction == ACT_EOF) 
	  {
	    send_packet(PKT_EOT, 0, NULL);
	    nextAction = ACT_NOACTION;
	    link_progress(LINK_STOP, NULL, 0);
	  }
	else
	  if(nextAction == ACT_VARDATA && type == PKT_EXPECT) 
	    {
	      send_packet(PKT_READY, 0, NULL);
	      send_packet(PKT_VARDATA, varSize, varPtr);
	      nextAction = ACT_EOF;
	    }
      }
   
    if(txMode == TXM_BACKUP) 
      {
	if(nextAction == ACT_EOF) 
	  {
	    send_packet(PKT_EOT, 0, NULL);
	    link_progress(LINK_STOP, NULL, 0);
	    nextAction = ACT_NOACTION;
	  }
	else
	  if(nextAction == ACT_VARHEAD && type == PKT_READY) 
	    {
	      send_varheader(VARTYPE_BACKUP, 0, NULL);
	      nextAction = ACT_VARDATA;
	    }
	  else
	    if(nextAction == ACT_VARDATA && type == PKT_EXPECT) 
	      {
		send_packet(PKT_READY, 0, NULL);
		
		if(backupSize <= backupBlock) 
		  {
		    backupBlock = backupSize;
		    nextAction = ACT_EOF;
		  }
		else
		  nextAction = ACT_VARHEAD;
		send_packet(PKT_VARDATA, backupBlock, backupPtr);
		backupPtr+=backupBlock;
		backupSize -= backupBlock;
		link_progress(LINK_RUNNING, NULL, backupSize);
	      }
      }
}

void resend_lastpacket(void) 
{
  outBytes = lastPacketPos;
}

void send_packet(int type, int size, UBYTE *data) 
{
  lastPacketPos = outBytes;
  outBuffer[outBytes++] = (getCalcType() == TI92) ? PC_TI92 : PC_TI89;
  outBuffer[outBytes++] = type;
  outBuffer[outBytes++] = size&0xff;
  outBuffer[outBytes++] = size>>8;
  if(data) 
    {
      int i;
      UWORD checksum = 0;
      for(i=0; i<size; i++)
	checksum+=data[i];
      memcpy(&outBuffer[outBytes], data, size);
      outBytes += size;
      outBuffer[outBytes++] = checksum&0xff;
      outBuffer[outBytes++] = checksum>>8;
    }  
  /*  fprintf(stdout, "To TI92:\n", type,size);
      k = outBytes - lastPacketPos;
      if(k>16) k=16;
      for(i=0; i<k; i++)
      fprintf(stdout, "%02x ", outBuffer[lastPacketPos+i]&0xff);
      fprintf(stdout, "\n");*/

  return;
}

void send_varheader(int vartype, int size, char *name) 
{
  UBYTE head[24];
  
  head[3] = size>>24;
  head[2] = (size>>16)&0xff;
  head[1] = (size>>8)&0xff;
  head[0] = size&0xff;
  head[4] = vartype;
  if(name) 
    {
      int len = strlen(name);
      if(len && len<18) 
	{ 
	  head[5] = len;
	  strcpy(&head[6], name);
	  send_packet(PKT_VARHEAD, 6+len, head);
	}
    }
  else
    send_packet(PKT_VARHEAD, 5, head);
}

void request_var(char *name, int type) 
{
  UBYTE head[24];
  
  head[3] = 0;
  head[2] = 0;
  head[1] = 0;
  head[0] = 0;
  head[4] = type;
  if(name) 
    {
      int len = strlen(name);
      if(len<18) 
	{ 
	  head[5] = len;
	  if(len)
	    strcpy(&head[6], name);
	  send_packet(PKT_VARREQ, 6+len, head);
	}
    }
  else
    send_packet(PKT_VARREQ, 5, head);
}

void start_send_backup(UBYTE *ptr, int size) 
{
  backupPtr = ptr;
  backupSize = size;
  backupBlock = 1024;
  txMode = TXM_BACKUP;
  lc_internal = 1;
  link_progress(LINK_BACKUP, "backup", backupSize);

  send_varheader(VARTYPE_BACKUP, size, NULL);
  nextAction = ACT_VARHEAD;
}

void start_send_variable(UBYTE *var, char *name, UBYTE type, int size) 
{
  txMode = TXM_SENDVAR;
  varSize = size;
  varPtr = var;
  lc_internal = 1;
  link_progress(LINK_FILE, name, size);

  send_varheader(type, size, name);
  nextAction = ACT_VARDATA;
}

void start_get_directory(tifile *buffer, int bufsize) 
{
  txMode = TXM_GETDIR;
  dirPtr = buffer;
  dirPtr->type = 255;
  varSize = bufsize;
  lc_internal = 1;
  link_progress(LINK_DIR, NULL, 0);

  request_var("", 0x19);
  nextAction = ACT_VARHEAD;
}

int query_libs(char *fname, char *libBuf, char **libPtrs) 
{
  FILE *fp;
  UBYTE len[2];
  UWORD offs;
  int i, libcnt = -1;
  
  if((fp = fopen(fname, "rb"))) 
    {
      fseek(fp, 0x5E, SEEK_SET);
      fread(len, 1, 2, fp);
      offs = (((UWORD)len[0])<<8) | len[1];
      fseek(fp, offs+0x59, SEEK_SET);
      fread(len, 1, 1, fp);
      libcnt = len[0];
      for(i=0; i<len[0]; i++) 
	{
	  libPtrs[i] = libBuf;
	  do 
	    {
	      fread(libBuf, 1, 1, fp);
	    } 
	  while(*libBuf++);
	}
      fclose(fp);    
    }
  return libcnt;
}


int send_ti_file(char *fname) 
{
  FILE *fp;
  char vname[10];
  static UBYTE *backBuf = NULL;
  int filesize = 0;

  if((fp = fopen(fname, "rb"))) 
    {
      int len;
      char c;

      fseek(fp, 0, SEEK_END);
      filesize = ftell(fp);
      fseek(fp, 0, SEEK_SET);

      if(backBuf != NULL)
	free(backBuf);
      backBuf = malloc(filesize * sizeof(char));

      c = toupper(fname[strlen(fname)-1]);
      if(c == 'B') 
	{
	  fseek(fp, 0x52, SEEK_SET);
	  len = fread(backBuf, 1, 128*1024, fp);
	  fclose(fp);
	  if(len > 0) 
	    {
	      lc_speedy = 1;
	      start_send_backup(backBuf, len-2);
	    }
	}
      else 
	{
	  UBYTE vType;
	  fseek(fp, 0x40, SEEK_SET);
	  fread(vname, 1, 8, fp);
	  vname[8] = (char)0;
	  fread(&vType, 1, 1, fp);
	  fseek(fp, 0x52, SEEK_SET);
	  len = fread(backBuf, 1, 128*1024, fp);
	  fclose(fp);
	  if(len > 0) 
	    {
	      lc_speedy = 1;
	      start_send_variable(backBuf, vname, vType, len-2);
	    }
	}
      return 1;
    }
  return 0;
}
