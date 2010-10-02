/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*
 *   skinedit - a skin editor for the TiEmu emulator
 *   Copyright (C) 2002 Julien BLACHE <jb@tilp.info>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#ifndef __WIN32__
#include <byteswap.h>
#endif

#include <glib.h>

#include "support.h"
#include "struct.h"

#include "vti_export.h"
#include "skinops.h"
#include "utils.h"

int
export_skin_vti(const char *path, unsigned int version)
{
  FILE *fp = NULL;
  FILE *jpeg = NULL;
  unsigned char *jpeg_data = NULL;
  unsigned int jpeg_length;
  char buf[64];
  int ret = -1;

  fp = fopen(path, "wb");
  
  if (fp != NULL)
    {
      if (skin_infos.type == SKIN_TYPE_NEW)
        {
          jpeg = fopen(skin_infos.jpeg_path, "rb");
        }
      else
        {
          jpeg = fopen(skin_infos.skin_path, "rb");
        }

      if (jpeg == NULL)
        {
          fclose(fp);

          return -1;
        }

      jpeg_data = read_image(jpeg, &jpeg_length);

      if (jpeg_data == NULL) goto exit;

      if (version == EXPORT_VTI_2_1)
        {
          if (fwrite("VTIv2.1 ", 8, 1, fp) < 1) goto exit;

          memset(buf, 0, 64);

          if (skin_infos.name != NULL)
            {
              strncpy(buf, skin_infos.name, 64);
            }

          if (fwrite(buf, 64, 1, fp) < 1) goto exit;
        }
      else if (version == EXPORT_VTI_2_5)
        {
          if (fwrite("VTIv2.5 ", 8, 1, fp) < 1) goto exit;

          memset(buf, 0, 64);

          if (skin_infos.name != NULL)
            {
              strncpy(buf, skin_infos.name, 64);
            }

          if (fwrite(buf, 64, 1, fp) < 1) goto exit;

          memset(buf, 0, 64);

          if (skin_infos.author != NULL)
            {
              strncpy(buf, skin_infos.author, 64);
            }


          if (fwrite(buf, 64, 1, fp) < 1) goto exit;
        }
      else
        {
          goto exit;
        }

      write_vti_core(fp);

      if (fwrite(jpeg_data, jpeg_length, 1, fp) < 1) goto exit;

      ret = 0;

exit:
      free(jpeg_data); 
      fclose(fp);
      fclose(jpeg);
    }
  else
    {
      return -1;
    }

  if (ret != -1)
    {
      sbar_print(_("Skin exported as %s (%s)"), ((version == EXPORT_VTI_2_1) ? "VTiv2.1" : "VTiv2.5"), g_basename(path));
    }

  return ret;
}


int
write_vti_core(FILE *fp)
{
  uint32_t tmpint;
  int i;
  int ret = -1;

  if (strcmp(skin_infos.calc, CALC_TI73) == 0)
    {
      tmpint = 73;
    }
  else if (strcmp(skin_infos.calc, CALC_TI82) == 0)
    {
      tmpint = 82;
    }
  else if (strcmp(skin_infos.calc, CALC_TI83) == 0)
    {
      tmpint = 83;
    }
  else if (strcmp(skin_infos.calc, CALC_TI83P) == 0)
    {
      tmpint = 84;
    }
  else if (strcmp(skin_infos.calc, CALC_TI85) == 0)
    {
      tmpint = 85;
    }
  else if (strcmp(skin_infos.calc, CALC_TI86) == 0)
    {
      tmpint = 86;
    } 
  else if (strcmp(skin_infos.calc, CALC_TI89) == 0)
    {
      tmpint = 89;
    }
  else if (strcmp(skin_infos.calc, CALC_TI92) == 0)
    {
      tmpint = 92;
    }
  else if (strcmp(skin_infos.calc, CALC_TI92P) == 0)
    {
      tmpint = 94;
    }

#if G_BYTE_ORDER == G_BIG_ENDIAN
  tmpint = bswap_32(tmpint);
  if (fwrite(&tmpint, 4, 1, fp) < 1) goto exit; /* calc type */

  tmpint = bswap_32(skin_infos.colortype);
  if (fwrite(&tmpint, 4, 1, fp) < 1) goto exit;

  tmpint = bswap_32(skin_infos.lcd_white);
  if (fwrite(&tmpint, 4, 1, fp) < 1) goto exit;

  tmpint = bswap_32(skin_infos.lcd_black);
  if (fwrite(&tmpint, 4, 1, fp) < 1) goto exit;

  tmpint = bswap_32(skin_infos.lcd_pos.top);
  if (fwrite(&tmpint, 4, 1, fp) < 1) goto exit;
  tmpint = bswap_32(skin_infos.lcd_pos.bottom);
  if (fwrite(&tmpint, 4, 1, fp) < 1) goto exit;
  tmpint = bswap_32(skin_infos.lcd_pos.left);
  if (fwrite(&tmpint, 4, 1, fp) < 1) goto exit;
  bswap_32(skin_infos.lcd_pos.right);
  if (fwrite(&tmpint, 4, 1, fp) < 1) goto exit;

  for (i = 0; i < 80; i++)
    {
      tmpint = bswap_32(skin_infos.keys_pos[i].top);
      if (fwrite(&tmpint, 4, 1, fp) < 1) goto exit;
      tmpint = bswap_32(skin_infos.keys_pos[i].bottom);
      if (fwrite(&tmpint, 4, 1, fp) < 1) goto exit;
      tmpint = bswap_32(skin_infos.keys_pos[i].left);
      if (fwrite(&tmpint, 4, 1, fp) < 1) goto exit;
      tmpint = bswap_32(skin_infos.keys_pos[i].right);
      if (fwrite(&tmpint, 4, 1, fp) < 1) goto exit;
    }
#else
  if (fwrite(&tmpint, 4, 1, fp) < 1) goto exit;
  if (fwrite(&skin_infos.colortype, 4, 1, fp) < 1) goto exit;
  if (fwrite(&skin_infos.lcd_white, 4, 1, fp) < 1) goto exit;
  if (fwrite(&skin_infos.lcd_black, 4, 1, fp) < 1) goto exit;

  if (fwrite(&skin_infos.lcd_pos.left, 4, 1, fp) < 1) goto exit;
  if (fwrite(&skin_infos.lcd_pos.top, 4, 1, fp) < 1) goto exit;
  if (fwrite(&skin_infos.lcd_pos.right, 4, 1, fp) < 1) goto exit;
  if (fwrite(&skin_infos.lcd_pos.bottom, 4, 1, fp) < 1) goto exit;

  for (i = 0; i < 80; i++)
    {
      if (fwrite(&skin_infos.keys_pos[i].left, 4, 1, fp) < 1) goto exit;
      if (fwrite(&skin_infos.keys_pos[i].top, 4, 1, fp) < 1) goto exit;
      if (fwrite(&skin_infos.keys_pos[i].right, 4, 1, fp) < 1) goto exit;
      if (fwrite(&skin_infos.keys_pos[i].bottom, 4, 1, fp) < 1) goto exit;
    }
#endif

  ret = 0;
 
exit:
  return ret;
}
