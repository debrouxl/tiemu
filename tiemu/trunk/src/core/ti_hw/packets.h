/*  gtktiemu - a TI89/92/92+ emulator
 *  (c) Copyright 2000-2001, Romain Lievin and Thomas Corvazier
 *  (c) Copyright 2001-2002, Romain Lievin
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

#ifndef __TI68K_PACKETS__
#define __TI68K_PACKETS__

/* Types */

typedef struct 
{
  char name[10];
  UBYTE type;
  int size;
} tifile;

/* Functions */

extern int init_protocol(void);
extern void reset_protocol(void);
extern void prot_receive_byte(UBYTE arg);
extern UBYTE prot_get_byte(void);
extern void start_send_backup(UBYTE *ptr, int size);
extern void start_send_variable(UBYTE *var, char *name, UBYTE type, int size);
extern void start_get_directory(tifile *buffer , int size);
extern int prot_bytes_left(void);
extern int send_ti_file(char *fname);
extern int query_libs(char *fname, char *libBuf, char **libsPtrs);

void receive_packet(int type, int size, UBYTE *data);
void resend_lastpacket(void);
void send_packet(int type, int size, UBYTE *data);
void send_varheader(int vartype, int size, char *name);

/* Constants */

#define PC_TI92  0x09
#define TI92_PC  0x89
#define PC_TI89  0x08
#define TI89_PC  0x98
#define PC_TI92p 0x08
#define TI92p_PC 0x88

#define PKT_VARHEAD   0x06
#define PKT_EXPECT    0x09
#define PKT_VARDATA   0x15
#define PKT_READY     0x56
#define PKT_CONT      0x78
#define PKT_EOT       0x92
#define PKT_VARREQ    0xA2

#define ACT_VARHEAD  1
#define ACT_EOF      2
#define ACT_VARDATA  3
#define ACT_NOACTION 4
#define ACT_CONTEOT  5

#define TXM_BACKUP  1
#define TXM_SENDVAR 2
#define TXM_GETDIR  3

#define VARTYPE_BACKUP 0x1d

#define LINK_RUNNING 0
#define LINK_STOP   -1
#define LINK_FAIL   -2
#define LINK_BACKUP -3
#define LINK_DIR    -4
#define LINK_FILE   -5

#endif
