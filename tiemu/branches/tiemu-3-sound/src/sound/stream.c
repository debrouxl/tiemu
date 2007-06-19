/*Sound capability library for TI-Emu
Copyright (C) 2007  Peter Fernandes
supersonicandtails@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/

#include "audio.h"

int bufpos;

void stream_audio(void *unused, Uint8 *stream, int len) {
	static int warned=1; //inverted logic
	int spill=bufpos-len;

	//allocate larger buffer.  if allocation fails, retry next time
	if(len>bufsize || !warned) {
		buffer=realloc(buffer,len);
		if(!buffer) {
			if(warned) {
				tiemu_warning(_("could not allocate bigger stream buffer, sound may not play"));
				warned=0;
			}
			return;
		}
		else warned=1;

		bufsize=len;
	}
	
	memcpy(stream,buffer,len);

	//reset the buffer
	bufpos=spill;
	memset(buffer,0,spill);
	
	memmove(buffer,buffer+len,spill);
}  

void push_amplitudes(char left, char right) {
	static int warned=1; //inverted logic to save on one operation

	SDL_LockAudio();
	
	if(bufpos<bufsize) {
		*(buffer+bufpos)=left;
		*(buffer+bufpos+1)=right;
		bufpos+=2;
	}
	else if(warned) {
		tiemu_warning(_("sound buffer full, dropping sample(s)"));
		warned=0;
	}
	
	SDL_UnlockAudio();

}

