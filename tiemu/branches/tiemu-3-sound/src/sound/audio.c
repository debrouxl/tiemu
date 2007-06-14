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
#include "stream.h"

int audioerr;
int mix_volume;
char *buffer;

SDL_AudioSpec format;


//sets up everything for use
int init_audio(void) {
	
	//initialize SDL for Audio
	if(SDL_Init(SDL_INIT_AUDIO)<0) {
		fprintf(stderr,_("Unable to initialize sound: SDL: %s\n"), SDL_GetError());
		return -1;
	}
	
	atexit(SDL_Quit);


	//set up the audio format
	format.freq=44100;
	format.format=AUDIO_S8;
	format.channels=2;
	format.samples=4096;
	format.callback=stream_audio;
	format.userdata=NULL;


	return 0;

}


int enable_audio(void) {
	buffer=malloc(BUFFER_SIZE);
	
	if(!buffer) {
		fprintf(stderr,_("Unable to enable sound: Memory\n"));
		return -1;
	}
	
	memset(buffer,0,BUFFER_SIZE);
	bufpos=0;
	
	//open the audio device
	if(SDL_OpenAudio(&format,NULL)<0) {
		fprintf(stderr,_("Unable to open audio device: SDL: %s\n"),SDL_GetError());
		return -1;
	}
	
	//begin streaming audio
	SDL_PauseAudio(0);


}

void disable_audio(void) {
	//stop streaming audio
	SDL_CloseAudio();
	free(buffer);

}


