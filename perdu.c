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

#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

#include "constantes.h"

#include "event.h"
#include "perdu.h"
#include "jeu.h"

char* perdu(struct Plateau* p, SDL_Surface* screen)
{
    struct Input in;
    memset(&in, 0, sizeof(struct Input));

    SDL_Surface *s_perdu = SDL_LoadBMP("Data/Graphiques/perdu.bmp");
    SDL_Rect pos_perdu;
    pos_perdu.x = SCREEN_X/2 - s_perdu->w/2;
    pos_perdu.y = SCREEN_Y/2 - s_perdu->h/2;
    Uint8 alpha = 0;

    SDL_Surface *s[10];
	s[VIDE] = IMG_Load("Data/Graphiques/vide.png");
	s[BORD] = IMG_Load("Data/Graphiques/bord.png");
	s[I] = IMG_Load("Data/Graphiques/block_I.png");
	s[O] = IMG_Load("Data/Graphiques/block_O.png");
    s[T] = IMG_Load("Data/Graphiques/block_T.png");
    s[L] = IMG_Load("Data/Graphiques/block_L.png");
	s[J] = IMG_Load("Data/Graphiques/block_J.png");
	s[S] = IMG_Load("Data/Graphiques/block_S.png");
    s[Z] = IMG_Load("Data/Graphiques/block_Z.png");
    s[PERDU] = IMG_Load("Data/Graphiques/block_perdu.png");

    Uint32 tps_anim = SDL_GetTicks();
    Uint32 tps_perdu = SDL_GetTicks();

    while(!in.key[SDLK_RETURN])
    {
        UpdateEvents(&in);

        /* Affichage de la map */
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

        size_t i, j;

        for(i = 0; i < p->columns; i++)
        {
            for(j = 0; j < p->rows; j++)
            {
                SDL_Rect pos_block;
                pos_block.x = i * DIM_BLOCK;
                pos_block.y = j * DIM_BLOCK;


                if((SDL_GetTicks() - tps_anim) > 5 && p->map[i][j] != VIDE && p->map[i][j] != PERDU && p->map[i][j] != BORD)
                {
                    p->map[i][j] = PERDU;
                    tps_anim = SDL_GetTicks();
                }

                int block = p->map[i][j];

                SDL_BlitSurface(s[block], NULL, screen, &pos_block);
            }
        }
        /* Fin affichage de la map */

        SDL_BlitSurface(s_perdu, NULL, screen, &pos_perdu);

        if(alpha < 255 && (SDL_GetTicks() - tps_perdu) > 5)
        {
            SDL_SetAlpha(s_perdu, SDL_SRCALPHA, alpha);
            alpha += 3;

            tps_perdu = SDL_GetTicks();
        }
        else if(alpha == 255)
        {
            return readStringSDL(9, screen);
        }

        SDL_Flip(screen);
    }

    return NULL;
}

char *readStringSDL(size_t n, SDL_Surface* screen)
{
    SDL_Event event;

    char* s_dst = (char*)malloc(sizeof(char) * n);
    strcpy(s_dst, "");

    size_t pos = 0;
    int unicode;

    TTF_Font* police = TTF_OpenFont("Data/Graphiques/Tetris.ttf", 20);
    SDL_Color noir = {255, 255, 255};
    SDL_Surface* name = TTF_RenderText_Blended(police, "_", noir);

    char chaine_name[10];
    strcpy(chaine_name, "");

    SDL_EnableUNICODE(1);

    while (SDL_WaitEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_RETURN:
                {
                    s_dst[pos] = '\0';
                    return s_dst;
                }
                break;
                default:
                    unicode = event.key.keysym.unicode;
                    if (unicode == 8 && pos >= 0)
                    {
                        s_dst[pos] = '\0';

                        if(pos > 0)
                            pos--;
                    }
                    else
                    {
                        if (unicode >= 32 && unicode <= 127 && pos < n)
                        {
                            s_dst[pos] = (char)unicode;
                            pos++;
                        }
                    }

                    if(pos == 0)
                    {
                        strcpy(chaine_name, "_");
                    }
                    else
                    {
                        strcpy(chaine_name, s_dst);
                    }
                        SDL_FreeSurface(name);

                        name = TTF_RenderText_Blended(police, chaine_name, noir);

                        SDL_Rect pos_name;
                        pos_name.x = SCREEN_X/2 - name->w/2;
                        pos_name.y = SCREEN_Y/2 - 10;

                        SDL_Rect offset;
                        offset.x = SCREEN_X/2 - 81;
                        offset.y = SCREEN_Y/2 - 10;
                        offset.w = 161;
                        offset.h = 30;

                        SDL_FillRect(screen, &offset, SDL_MapRGB(screen->format, 142, 133, 133));

                        SDL_BlitSurface(name, NULL, screen, &pos_name);

                        SDL_Flip(screen);
                 break;
             }
         break;
        }
    }

    SDL_EnableUNICODE(0);

    return s_dst;
}
