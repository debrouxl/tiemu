/*  TiEmu - a TI calculator emulator
 *
 *  Character set conversion routine
 *  Copyright (c) 2006 Kevin Kofler
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
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

void utf16_to_ti(const unsigned short *utf16, char *ti)
{
  const unsigned short *p=utf16;
  unsigned char *q=(unsigned char *)ti;
  while (*p) {
#ifdef __GNUC__
    switch (*(p++)) {
      case 0 ... 10:
      case 12 ... 13:
      case 32 ... 126:
      case 161 ... 167:
      case 169 ... 172:
      case 174 ... 186:
      case 191 ... 255:
        *(q++)=p[-1];
        break;
      case 0x2934:
        *(q++)=11;
        break;
      case 0x2693:
        *(q++)=14;
        break;
      case 0x2713:
        *(q++)=15;
        break;
      case 0x25fe:
        *(q++)=16;
        break;
      case 0x25c2:
        *(q++)=17;
        break;
      case 0x25b8:
        *(q++)=18;
        break;
      case 0x25b4:
        *(q++)=19;
        break;
      case 0x25be:
        *(q++)=20;
        break;
      case 0x2190:
        *(q++)=21;
        break;
      case 0x2192:
        *(q++)=22;
        break;
      case 0x2191:
        *(q++)=23;
        break;
      case 0x2193:
        *(q++)=24;
        break;
      case 0x25c0:
        *(q++)=25;
        break;
      case 0x25b6:
        *(q++)=26;
        break;
      case 0x2b06:
        *(q++)=27;
        break;
      case 0x222a:
        *(q++)=28;
        break;
      case 0x2229:
        *(q++)=29;
        break;
      case 0x2282:
        *(q++)=30;
        break;
      case 0x2208:
        *(q++)=31;
        break;
      case 0x25c6:
        *(q++)=127;
        break;
      case 0x3b1:
        *(q++)=128;
        break;
      case 0x3b2:
        *(q++)=129;
        break;
      case 0x393:
        *(q++)=130;
        break;
      case 0x3b3:
        *(q++)=131;
        break;
      case 0x394:
        *(q++)=132;
        break;
      case 0x3b4:
        *(q++)=133;
        break;
      case 0x3b5:
        *(q++)=134;
        break;
      case 0x3b6:
        *(q++)=135;
        break;
      case 0x3b8:
        *(q++)=136;
        break;
      case 0x3bb:
        *(q++)=137;
        break;
      case 0x3be:
        *(q++)=138;
        break;
      case 0x3a0:
        *(q++)=139;
        break;
      case 0x3c0:
        *(q++)=140;
        break;
      case 0x3c1:
        *(q++)=141;
        break;
      case 0x3a3:
        *(q++)=142;
        break;
      case 0x3c3:
        *(q++)=143;
        break;
      case 0x3c4:
        *(q++)=144;
        break;
      case 0x3c6:
        *(q++)=145;
        break;
      case 0x3c8:
        *(q++)=146;
        break;
      case 0x3a9:
        *(q++)=147;
        break;
      case 0x3c9:
        *(q++)=148;
        break;
      case 0x212f:
        *(q++)=150;
        break;
      case 0x2b3:
        *(q++)=152;
        break;
      case 0x22ba:
        *(q++)=153;
        break;
      case 0x2264:
        *(q++)=156;
        break;
      case 0x2260:
        *(q++)=157;
        break;
      case 0x2265:
        *(q++)=158;
        break;
      case 0x2220:
        *(q++)=159;
        break;
      case 0x2026:
        *(q++)=160;
        break;
      case 0x221a:
        *(q++)=168;
        break;
      case 0x2212:
        *(q++)=173;
        break;
      case 0x207a:
        *(q++)=187;
        break;
      case 0x2202:
        *(q++)=188;
        break;
      case 0x222b:
        *(q++)=189;
        break;
      case 0x221e:
        *(q++)=190;
        break;
      case 0x305:
        if (*p==0x78) {
          *(q++)=154;
          p++;
        } else if (*p==0x79) {
          *(q++)=155;
          p++;
        } else {
          *(q++)='?';
        }
        break;
      case 0xd875:
        if (*p==0xdda4) {
          *(q++)=149;
          p++;
          break;
        } else if (*p==0xdc8a) {
          *(q++)=151;
          p++;
          break;
        }
      case 0xd800 ... 0xd874:
      case 0xd876 ... 0xdbff:
        p++;
      default:
        *(q++)='?';
        break;
    }
#else
    *(q++)=*(p++); // sorry
#endif
  }
  *q=0;
}
