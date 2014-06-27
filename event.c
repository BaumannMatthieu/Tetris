/* Tetris_SDL
Copyright (C) 2012 BAUMANN Matthieu

Tetris_SDL is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "event.h"

void UpdateEvents(struct Input* in)
{
    SDL_Event event;

    in->mousebuttons[SDL_BUTTON_WHEELUP] = 0;
    in->mousebuttons[SDL_BUTTON_WHEELDOWN] = 0;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                in->quit = 1;
            break;
            case SDL_KEYDOWN:
                in->key[event.key.keysym.sym] = 1;
            break;
            case SDL_KEYUP:
                in->key[event.key.keysym.sym] = 0;
            break;
            case SDL_MOUSEBUTTONDOWN:
                in->mousebuttons[event.button.button] = 1;
                in->mouseclicx = event.button.x;
                in->mouseclicy = event.button.y;
            break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button != SDL_BUTTON_WHEELDOWN && event.button.button != SDL_BUTTON_WHEELUP)
                    in->mousebuttons[event.button.button] = 0;
            break;
            case SDL_MOUSEMOTION:
                in->mousex = event.motion.x;
                in->mousey = event.motion.y;
                in->mousexrel = event.motion.xrel;
                in->mouseyrel = event.motion.yrel;
            break;
            default:
            break;
        }
    }
}
