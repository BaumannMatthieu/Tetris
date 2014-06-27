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

#include "liste.h"
#include "scores.h"
#include "event.h"
#include "perdu.h"
#include "jeu.h"

struct Scores* jeu(SDL_Surface* screen, struct liste* liste_musiques)
{
    struct Input in;
    memset(&in, 0, sizeof(struct Input));

    /* Sons & musiques*/
    Mix_Music* musique_actu = ((Mix_Music*)(donner(liste_musiques, (rand() % taille(liste_musiques)))->var));
    Mix_Chunk *clic = Mix_LoadWAV("Data/Sons/clic.wav");
    Mix_Chunk *ligne = Mix_LoadWAV("Data/Sons/ligne.wav");
    Mix_AllocateChannels(3);

	/* Chargement de la map */

    struct Plateau* map = creerPlateau(12, 22);

	SDL_Surface *s[9], *next_s[7];
	s[VIDE] = IMG_Load("Data/Graphiques/vide.png");
	s[BORD] = IMG_Load("Data/Graphiques/bord.png");
	s[I] = IMG_Load("Data/Graphiques/block_I.png");
	s[O] = IMG_Load("Data/Graphiques/block_O.png");
    s[T] = IMG_Load("Data/Graphiques/block_T.png");
    s[L] = IMG_Load("Data/Graphiques/block_L.png");
	s[J] = IMG_Load("Data/Graphiques/block_J.png");
	s[S] = IMG_Load("Data/Graphiques/block_S.png");
    s[Z] = IMG_Load("Data/Graphiques/block_Z.png");

    next_s[I] = IMG_Load("Data/Graphiques/next_I.png");
	next_s[O] = IMG_Load("Data/Graphiques/next_O.png");
    next_s[T] = IMG_Load("Data/Graphiques/next_T.png");
    next_s[L] = IMG_Load("Data/Graphiques/next_L.png");
	next_s[J] = IMG_Load("Data/Graphiques/next_J.png");
	next_s[S] = IMG_Load("Data/Graphiques/next_S.png");
    next_s[Z] = IMG_Load("Data/Graphiques/next_Z.png");

	/* Fin Chargement de la map */

	/* Chargement des pieces */

	struct Piece* actu = creerI(map);
	int next = rand() % 7;
	SDL_Rect pos_next_block;
	pos_next_block.x = 300;
	pos_next_block.y = 10;

	/* Fin Chargement des pieces */

	/* Score et level */

    TTF_Font* police = TTF_OpenFont("Data/Graphiques/Tetris.ttf", 30);
    SDL_Color blanc = {255, 255, 255};
    int score = 0;
    int level = 0;
    char chaine_score[100];
    char chaine_level[20];

    strcpy(chaine_score, "0");
    strcpy(chaine_level, "0");

    SDL_Surface* score_s = TTF_RenderText_Blended(police, "Score : 0", blanc);
    SDL_Surface* level_s = TTF_RenderText_Blended(police, "Level : 0", blanc);

    SDL_Rect pos_score;
    pos_score.x = 300;
    pos_score.y = 150;

    SDL_Rect pos_level;
    pos_level.x = 300;
    pos_level.y = 220;

	/* Fin score et level */

	/* Definition du temps */
	Uint32 tps_prec = SDL_GetTicks();

	Mix_PlayMusic(musique_actu, 1);


    while(!in.quit)
    {
        UpdateEvents(&in);

        if(SDL_GetTicks() - tps_prec > (1000 - 50*level))
        {
            if((collision(actu, map) == 3 || collision(actu, map) == 5 || collision(actu, map) == 8 || !collision(actu, map)))
            {
                deplacerPiece(actu, map, DOWN);
            }

            tps_prec = SDL_GetTicks();
        }

        if(in.key[SDLK_LEFT] && (collision(actu, map) == 1 || collision(actu, map) == 5 || collision(actu, map) == 6 || !collision(actu, map)))
        {
            Mix_PlayChannel(1, clic, 0);
            deplacerPiece(actu, map, LEFT);

            in.key[SDLK_LEFT] = 0;
        }

        if(in.key[SDLK_RIGHT] && (collision(actu, map) == 1 || collision(actu, map) == 3 || collision(actu, map) == 4 || !collision(actu, map)))
        {
            Mix_PlayChannel(1, clic, 0);
            deplacerPiece(actu, map, RIGHT);

            in.key[SDLK_RIGHT] = 0;
        }

        if(in.key[SDLK_SPACE] && (collision(actu, map) == 3 || collision(actu, map) == 5 || collision(actu, map) == 8 || !collision(actu, map)))
        {
            Mix_PlayChannel(1, clic, 0);

            while(in.key[SDLK_SPACE] && (collision(actu, map) == 3 || collision(actu, map) == 5 || collision(actu, map) == 8 || !collision(actu, map)))
            {
                deplacerPiece(actu, map, DOWN);
            }

            in.key[SDLK_SPACE] = 0;
        }

        /* Rotation de la piece */

        if(in.key[SDLK_UP])
        {
            size_t i, j;
            int rot = 1;

            /* Collision de la box */

            if(actu->box_x < 0 || (actu->box_x + actu->taille_box) > (map->columns - 1))
            {
                rot = 0;
            }
            else
            {
                for(i = actu->box_x; i < (actu->box_x + actu->taille_box); i++)
                {
                    for(j = actu->box_y; j < (actu->box_y + actu->taille_box); j++)
                    {
                        if(map->map[i][j] != VIDE && !memePiece(actu, i, j))
                        {
                            rot = 0;
                            break;
                        }
                    }
                }
            }

            /* Fin collision de la labox */

            if(rot)
            {
                Mix_PlayChannel(1, clic, 0);
                // Effacer piece avant rotation
                for(i = 0; i < N_BLOCK_PER_PIECE; i++)
                {
                    size_t loc_x = actu->cases[i].x;
                    size_t loc_y = actu->cases[i].y;

                    map->map[loc_x][loc_y] = VIDE;
                }

                // Rotation de la piece
                actu->rotation(actu);

                // Reactualiser la piece dans la map
                for(i = 0; i < N_BLOCK_PER_PIECE; i++)
                {
                    size_t loc_x = actu->cases[i].x;
                    size_t loc_y = actu->cases[i].y;

                    map->map[loc_x][loc_y] = actu->type;
                }
            }

            in.key[SDLK_UP] = 0;
        }

        /* Fin rotation de la piece */

        /* Piece actu est-elle en bas ? */
        size_t i = 0, j = 0;
        int clear_line = 0;

        if(collision(actu, map) == 1 || collision(actu, map) == 6 || collision(actu, map) == 4 || collision(actu, map) == 9)
        {
            in.key[SDLK_DOWN] = 0;
            /* Test perdre */

            for(i = 0; i < map->columns; i++)
            {
                if(map->map[i][1] != VIDE && map->map[i][1] != BORD)
                {
                    char* pseudo_actu = perdu(map, screen);

                    return (struct Scores*)creerScore(pseudo_actu, score);
                }
            }

            /* Fin test perdu */

            /* Vider map si ligne complete */

            for(j = 1; j < (map->rows - 1); j++)
            {
                int row_full = 1;

                for(i = 0; i < map->columns; i++)
                {
                    if(map->map[i][j] == VIDE)
                    {
                        row_full = 0;
                    }
                }

                if(row_full) // Si la ligne est pleine
                {
                    Mix_PlayChannel(1, ligne, 0);
                    viderMap(j, map);
                    clear_line++;
                }
            }
            /* Fin Vider map */
            switch(next)
            {
                case I:
                    actu = creerI(map);
                break;
                case O:
                    actu = creerO(map);
                break;
                case T:
                    actu = creerT(map);
                break;
                case L:
                    actu = creerL(map);
                break;
                case J:
                    actu = creerJ(map);
                break;
                case S:
                    actu = creerS(map);
                break;
                case Z:
                    actu = creerZ(map);
                break;
                default:
                break;
            }

            next = rand() % 7;
        }

        /* Affichage de la map */

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

        for(i = 0; i < map->columns; i++)
        {
            for(j = 0; j < map->rows; j++)
            {
                SDL_Rect pos_block;
                pos_block.x = i * DIM_BLOCK;
                pos_block.y = j * DIM_BLOCK;

                int block = map->map[i][j];

                SDL_BlitSurface(s[block], NULL, screen, &pos_block);
            }
        }
        /* Fin affichage de la map */

        // Affichage next
        SDL_BlitSurface(next_s[next], NULL, screen, &pos_next_block);

        // Affichage du texte
        switch(clear_line)
        {
            case 1:
                score += 40*(level + 1);
            break;
            case 2:
                score += 100*(level + 1);
            break;
            case 3:
                score += 300*(level + 1);
            break;
            case 4:
                score += 1200*(level + 1);
            break;
            default:
            break;
        }

        SDL_FreeSurface(score_s);
        SDL_FreeSurface(level_s);

        sprintf(chaine_score, "Score : %d", score);
        sprintf(chaine_level, "Level : %d", level);

        score_s = TTF_RenderText_Blended(police, chaine_score, blanc);
        level_s = TTF_RenderText_Blended(police, chaine_level, blanc);


        if(score >= 2500*(level + 1))
        {
            level++;
            //Mix_PauseMusic();
            //Mix_FreeMusic(musique_actu);

            musique_actu = ((Mix_Music*)(donner(liste_musiques, (rand() % taille(liste_musiques)))->var));
            Mix_PlayMusic(musique_actu, 1);
        }


        SDL_BlitSurface(score_s, NULL, screen, &pos_score);
        SDL_BlitSurface(level_s, NULL, screen, &pos_level);

        SDL_Flip(screen);
    }

    return NULL;
}
struct Plateau* creerPlateau(const size_t _c, const size_t _r)
{
    struct Plateau* plateau = (struct Plateau*)malloc(sizeof(struct Plateau));

    plateau->rows = _r;
    plateau->columns = _c;

    size_t i, j;

	plateau->map = (int**)malloc(sizeof(int*) * plateau->columns);

	for(i = 0; i < plateau->columns; i++)
	{
	    plateau->map[i] = (int*)malloc(sizeof(int) * plateau->rows);

	    for(j = 0; j < plateau->rows; j++)
        {
            plateau->map[i][j] = VIDE;

            if(i == 0 || j == 0 || i == (plateau->columns - 1) || j == (plateau->rows - 1))
            {
                plateau->map[i][j] = BORD;
            }
        }
	}

    return plateau;
}

void rotationI(struct Piece* piece)
{
    switch(piece->actu_rot)
    {
        case 0:
        {
            piece->cases[0].x = piece->box_x;
            piece->cases[0].y = piece->box_y + 1;

            piece->cases[1].x = piece->box_x + 1;
            piece->cases[1].y = piece->box_y + 1;

            piece->cases[2].x = piece->box_x + 2;
            piece->cases[2].y = piece->box_y + 1;

            piece->cases[3].x = piece->box_x + 3;
            piece->cases[3].y = piece->box_y + 1;
        }
        break;
        case 1:
        {
            piece->cases[0].x = piece->box_x + 2;
            piece->cases[0].y = piece->box_y;

            piece->cases[1].x = piece->box_x + 2;
            piece->cases[1].y = piece->box_y + 1;

            piece->cases[2].x = piece->box_x + 2;
            piece->cases[2].y = piece->box_y + 2;

            piece->cases[3].x = piece->box_x + 2;
            piece->cases[3].y = piece->box_y + 3;
        }
        break;
        default:
        break;
    }

    piece->actu_rot = (piece->actu_rot + 1) % (piece->taille_rot);
}
void rotationO(struct Piece* piece)
{

}
void rotationT(struct Piece* piece)
{
    switch(piece->actu_rot)
    {
        case 0:
        {
            piece->cases[0].x = piece->box_x;
            piece->cases[0].y = piece->box_y;

            piece->cases[1].x = piece->box_x;
            piece->cases[1].y = piece->box_y + 1;

            piece->cases[2].x = piece->box_x + 1;
            piece->cases[2].y = piece->box_y + 1;

            piece->cases[3].x = piece->box_x;
            piece->cases[3].y = piece->box_y + 2;
        }
        break;
        case 1:
        {
            piece->cases[0].x = piece->box_x;
            piece->cases[0].y = piece->box_y;

            piece->cases[1].x = piece->box_x + 1;
            piece->cases[1].y = piece->box_y;

            piece->cases[2].x = piece->box_x + 2;
            piece->cases[2].y = piece->box_y;

            piece->cases[3].x = piece->box_x + 1;
            piece->cases[3].y = piece->box_y + 1;
        }
        break;
        case 2:
        {
            piece->cases[0].x = piece->box_x + 2;
            piece->cases[0].y = piece->box_y;

            piece->cases[1].x = piece->box_x + 2;
            piece->cases[1].y = piece->box_y + 1;

            piece->cases[2].x = piece->box_x + 1;
            piece->cases[2].y = piece->box_y + 1;

            piece->cases[3].x = piece->box_x + 2;
            piece->cases[3].y = piece->box_y + 2;
        }
        break;
        case 3:
        {
            piece->cases[0].x = piece->box_x + 2;
            piece->cases[0].y = piece->box_y + 2;

            piece->cases[1].x = piece->box_x + 1;
            piece->cases[1].y = piece->box_y + 1;

            piece->cases[2].x = piece->box_x + 1;
            piece->cases[2].y = piece->box_y + 2;

            piece->cases[3].x = piece->box_x;
            piece->cases[3].y = piece->box_y + 2;
        }
        break;
        default:
        break;
    }

    piece->actu_rot = (piece->actu_rot + 1) % (piece->taille_rot);
}
void rotationL(struct Piece* piece)
{
    switch(piece->actu_rot)
    {
        case 0:
        {
            piece->cases[0].x = piece->box_x;
            piece->cases[0].y = piece->box_y + 1;

            piece->cases[1].x = piece->box_x + 1;
            piece->cases[1].y = piece->box_y + 1;

            piece->cases[2].x = piece->box_x + 2;
            piece->cases[2].y = piece->box_y + 1;

            piece->cases[3].x = piece->box_x;
            piece->cases[3].y = piece->box_y + 2;
        }
        break;
        case 1:
        {
            piece->cases[0].x = piece->box_x;
            piece->cases[0].y = piece->box_y;

            piece->cases[1].x = piece->box_x + 1;
            piece->cases[1].y = piece->box_y;

            piece->cases[2].x = piece->box_x + 1;
            piece->cases[2].y = piece->box_y + 1;

            piece->cases[3].x = piece->box_x + 1;
            piece->cases[3].y = piece->box_y + 2;
        }
        break;
        case 2:
        {
            piece->cases[0].x = piece->box_x;
            piece->cases[0].y = piece->box_y + 1;

            piece->cases[1].x = piece->box_x + 1;
            piece->cases[1].y = piece->box_y + 1;

            piece->cases[2].x = piece->box_x + 2;
            piece->cases[2].y = piece->box_y + 1;

            piece->cases[3].x = piece->box_x + 2;
            piece->cases[3].y = piece->box_y;
        }
        break;
        case 3:
        {
            piece->cases[0].x = piece->box_x + 1;
            piece->cases[0].y = piece->box_y;

            piece->cases[1].x = piece->box_x + 1;
            piece->cases[1].y = piece->box_y + 1;

            piece->cases[2].x = piece->box_x + 1;
            piece->cases[2].y = piece->box_y + 2;

            piece->cases[3].x = piece->box_x + 2;
            piece->cases[3].y = piece->box_y + 2;
        }
        break;
        default:
        break;
    }

    piece->actu_rot = (piece->actu_rot + 1) % (piece->taille_rot);
}
void rotationJ(struct Piece* piece)
{
    switch(piece->actu_rot)
    {
        case 0:
        {
            piece->cases[0].x = piece->box_x;
            piece->cases[0].y = piece->box_y;

            piece->cases[1].x = piece->box_x;
            piece->cases[1].y = piece->box_y + 1;

            piece->cases[2].x = piece->box_x + 1;
            piece->cases[2].y = piece->box_y + 1;

            piece->cases[3].x = piece->box_x + 2;
            piece->cases[3].y = piece->box_y + 1;
        }
        break;
        case 1:
        {
            piece->cases[0].x = piece->box_x + 1;
            piece->cases[0].y = piece->box_y;

            piece->cases[1].x = piece->box_x + 1;
            piece->cases[1].y = piece->box_y + 1;

            piece->cases[2].x = piece->box_x + 1;
            piece->cases[2].y = piece->box_y + 2;

            piece->cases[3].x = piece->box_x + 2;
            piece->cases[3].y = piece->box_y;
        }
        break;
        case 2:
        {
            piece->cases[0].x = piece->box_x;
            piece->cases[0].y = piece->box_y + 1;

            piece->cases[1].x = piece->box_x + 1;
            piece->cases[1].y = piece->box_y + 1;

            piece->cases[2].x = piece->box_x + 2;
            piece->cases[2].y = piece->box_y + 1;

            piece->cases[3].x = piece->box_x + 2;
            piece->cases[3].y = piece->box_y + 2;
        }
        break;
        case 3:
        {
            piece->cases[0].x = piece->box_x + 1;
            piece->cases[0].y = piece->box_y;

            piece->cases[1].x = piece->box_x + 1;
            piece->cases[1].y = piece->box_y + 1;

            piece->cases[2].x = piece->box_x + 1;
            piece->cases[2].y = piece->box_y + 2;

            piece->cases[3].x = piece->box_x;
            piece->cases[3].y = piece->box_y + 2;
        }
        break;
        default:
        break;
    }

    piece->actu_rot = (piece->actu_rot + 1) % (piece->taille_rot);
}
void rotationZ(struct Piece* piece)
{
    switch(piece->actu_rot)
    {
        case 0:
        {
            piece->cases[0].x = piece->box_x + 2;
            piece->cases[0].y = piece->box_y;

            piece->cases[1].x = piece->box_x + 2;
            piece->cases[1].y = piece->box_y + 1;

            piece->cases[2].x = piece->box_x + 1;
            piece->cases[2].y = piece->box_y + 1;

            piece->cases[3].x = piece->box_x + 1;
            piece->cases[3].y = piece->box_y + 2;
        }
        break;
        case 1:
        {
            piece->cases[0].x = piece->box_x;
            piece->cases[0].y = piece->box_y + 1;

            piece->cases[1].x = piece->box_x + 1;
            piece->cases[1].y = piece->box_y + 1;

            piece->cases[2].x = piece->box_x + 1;
            piece->cases[2].y = piece->box_y + 2;

            piece->cases[3].x = piece->box_x + 2;
            piece->cases[3].y = piece->box_y + 2;
        }
        break;
        default:
        break;
    }

    piece->actu_rot = (piece->actu_rot + 1) % (piece->taille_rot);
}
void rotationS(struct Piece* piece)
{
    switch(piece->actu_rot)
    {
        case 0:
        {
            piece->cases[0].x = piece->box_x;
            piece->cases[0].y = piece->box_y;

            piece->cases[1].x = piece->box_x;
            piece->cases[1].y = piece->box_y + 1;

            piece->cases[2].x = piece->box_x + 1;
            piece->cases[2].y = piece->box_y + 1;

            piece->cases[3].x = piece->box_x + 1;
            piece->cases[3].y = piece->box_y + 2;
        }
        break;
        case 1:
        {
            piece->cases[0].x = piece->box_x;
            piece->cases[0].y = piece->box_y + 2;

            piece->cases[1].x = piece->box_x + 1;
            piece->cases[1].y = piece->box_y + 2;

            piece->cases[2].x = piece->box_x + 1;
            piece->cases[2].y = piece->box_y + 1;

            piece->cases[3].x = piece->box_x + 2;
            piece->cases[3].y = piece->box_y + 1;
        }
        break;
        default:
        break;
    }

    piece->actu_rot = (piece->actu_rot + 1) % (piece->taille_rot);
}

struct Piece* creerI(struct Plateau* _p)
{
	struct Piece* I_block = (struct Piece*)malloc(sizeof(struct Piece));
	size_t t = 0;

	I_block->cases[0].x = 6;
    I_block->cases[0].y = 1;

    I_block->cases[1].x = 6;
    I_block->cases[1].y = 2;

    I_block->cases[2].x = 6;
    I_block->cases[2].y = 3;

    I_block->cases[3].x = 6;
    I_block->cases[3].y = 4;

    I_block->box_x = 4;
    I_block->box_y = 1;
    I_block->taille_box = 4;

    I_block->taille_rot = 2;
    I_block->actu_rot = 0;

	for(t = 0; t < N_BLOCK_PER_PIECE; t++)
	{
        size_t loc_x = I_block->cases[t].x;
        size_t loc_y = I_block->cases[t].y;

		_p->map[loc_x][loc_y] = I;
	}

	I_block->type = I;
	I_block->rotation = &rotationI;

	return I_block;
}

struct Piece* creerO(struct Plateau* _p)
{
	struct Piece* O_block = (struct Piece*)malloc(sizeof(struct Piece));
	size_t t = 0;

	O_block->cases[0].x = 6;
    O_block->cases[0].y = 1;

    O_block->cases[1].x = 7;
    O_block->cases[1].y = 1;

    O_block->cases[2].x = 6;
    O_block->cases[2].y = 2;

    O_block->cases[3].x = 7;
    O_block->cases[3].y = 2;

    O_block->box_x = 6;
    O_block->box_y = 1;
    O_block->taille_box = 2;

    for(t = 0; t < N_BLOCK_PER_PIECE; t++)
	{
        size_t loc_x = O_block->cases[t].x;
        size_t loc_y = O_block->cases[t].y;

		_p->map[loc_x][loc_y] = O;
	}

	O_block->type = O;
	O_block->rotation = &rotationO;

	return O_block;
}

struct Piece* creerT(struct Plateau* _p)
{
	struct Piece* T_block = (struct Piece*)malloc(sizeof(struct Piece));
	size_t t = 0;

	T_block->cases[0].x = 6;
    T_block->cases[0].y = 1;

    T_block->cases[1].x = 6;
    T_block->cases[1].y = 2;

    T_block->cases[2].x = 7;
    T_block->cases[2].y = 2;

    T_block->cases[3].x = 5;
    T_block->cases[3].y = 2;

    T_block->box_x = 5;
    T_block->box_y = 0;
    T_block->taille_box = 3;

    T_block->taille_rot = 4;
    T_block->actu_rot = 0;

	for(t = 0; t < N_BLOCK_PER_PIECE; t++)
	{
        size_t loc_x = T_block->cases[t].x;
        size_t loc_y = T_block->cases[t].y;

		_p->map[loc_x][loc_y] = T;
	}

	T_block->type = T;
	T_block->rotation = &rotationT;

	return T_block;
}

struct Piece* creerL(struct Plateau* _p)
{
	struct Piece* L_block = (struct Piece*)malloc(sizeof(struct Piece));
	size_t t = 0;

	L_block->cases[0].x = 6;
    L_block->cases[0].y = 1;

    L_block->cases[1].x = 6;
    L_block->cases[1].y = 2;

    L_block->cases[2].x = 6;
    L_block->cases[2].y = 3;

    L_block->cases[3].x = 7;
    L_block->cases[3].y = 3;

    L_block->box_x = 5;
    L_block->box_y = 1;
    L_block->taille_box = 3;

    L_block->taille_rot = 4;
    L_block->actu_rot = 0;

	for(t = 0; t < N_BLOCK_PER_PIECE; t++)
	{
        size_t loc_x = L_block->cases[t].x;
        size_t loc_y = L_block->cases[t].y;

		_p->map[loc_x][loc_y] = L;
	}

	L_block->type = L;
	L_block->rotation = &rotationL;

	return L_block;
}

struct Piece* creerJ(struct Plateau* _p)
{
	struct Piece* J_block = (struct Piece*)malloc(sizeof(struct Piece));
	size_t t = 0;

	J_block->cases[0].x = 6;
    J_block->cases[0].y = 1;

    J_block->cases[1].x = 6;
    J_block->cases[1].y = 2;

    J_block->cases[2].x = 6;
    J_block->cases[2].y = 3;

    J_block->cases[3].x = 5;
    J_block->cases[3].y = 3;

    J_block->box_x = 5;
    J_block->box_y = 1;
    J_block->taille_box = 3;

    J_block->taille_rot = 4;
    J_block->actu_rot = 0;

    for(t = 0; t < N_BLOCK_PER_PIECE; t++)
	{
        size_t loc_x = J_block->cases[t].x;
        size_t loc_y = J_block->cases[t].y;

		_p->map[loc_x][loc_y] = J;
	}

	J_block->type = J;
	J_block->rotation = &rotationJ;

	return J_block;
}

struct Piece* creerS(struct Plateau* _p)
{
	struct Piece* S_block = (struct Piece*)malloc(sizeof(struct Piece));
	size_t t = 0;

	S_block->cases[0].x = 6;
    S_block->cases[0].y = 1;

    S_block->cases[1].x = 5;
    S_block->cases[1].y = 1;

    S_block->cases[2].x = 5;
    S_block->cases[2].y = 2;

    S_block->cases[3].x = 4;
    S_block->cases[3].y = 2;

    S_block->box_x = 4;
    S_block->box_y = 0;
    S_block->taille_box = 3;

    S_block->taille_rot = 2;
    S_block->actu_rot = 0;

	for(t = 0; t < N_BLOCK_PER_PIECE; t++)
	{
        size_t loc_x = S_block->cases[t].x;
        size_t loc_y = S_block->cases[t].y;

		_p->map[loc_x][loc_y] = S;
	}

	S_block->type = S;
	S_block->rotation = &rotationS;

	return S_block;
}

struct Piece* creerZ(struct Plateau* _p)
{
	struct Piece* Z_block = (struct Piece*)malloc(sizeof(struct Piece));
	size_t t = 0;

	Z_block->cases[0].x = 6;
    Z_block->cases[0].y = 1;

    Z_block->cases[1].x = 7;
    Z_block->cases[1].y = 1;

    Z_block->cases[2].x = 7;
    Z_block->cases[2].y = 2;

    Z_block->cases[3].x = 8;
    Z_block->cases[3].y = 2;

    Z_block->box_x = 6;
    Z_block->box_y = 0;
    Z_block->taille_box = 3;

    Z_block->taille_rot = 2;
    Z_block->actu_rot = 0;

	for(t = 0; t < N_BLOCK_PER_PIECE; t++)
	{
        size_t loc_x = Z_block->cases[t].x;
        size_t loc_y = Z_block->cases[t].y;

		_p->map[loc_x][loc_y] = Z;
	}

	Z_block->type = Z;
	Z_block->rotation = &rotationZ;

	return Z_block;
}

int memePiece(const struct Piece* _p, const size_t x, const size_t y)
{
    size_t i = 0;

    for(i = 0; i < N_BLOCK_PER_PIECE; i++)
    {
        if(_p->cases[i].x == x && _p->cases[i].y == y)
            return 1;
    }

    return 0;
}

int collision(struct Piece* _p, struct Plateau* _plat)
{
    size_t i = 0;
    size_t col_b = 0, col_l = 0, col_r = 0;

    /* collision basse */
    for(i = 0; i < N_BLOCK_PER_PIECE; i++)
    {
        size_t loc_x = _p->cases[i].x;
        size_t loc_y = _p->cases[i].y;

        if(_plat->map[loc_x][loc_y + 1] != VIDE && !memePiece(_p, loc_x, loc_y + 1))
        {
            col_b = 1;
        }
    }

    /*collision sur le cote gauche*/
    for(i = 0; i < N_BLOCK_PER_PIECE; i++)
    {
        size_t loc_x = _p->cases[i].x;
        size_t loc_y = _p->cases[i].y;

        if(_plat->map[loc_x - 1][loc_y] != VIDE && !memePiece(_p, (loc_x - 1), loc_y))
        {
            col_l = 3;
        }
    }

    /*collision sur le cote droit*/
    for(i = 0; i < N_BLOCK_PER_PIECE; i++)
    {
        size_t loc_x = _p->cases[i].x;
        size_t loc_y = _p->cases[i].y;

        if(_plat->map[loc_x + 1][loc_y] != VIDE && !memePiece(_p, (loc_x + 1), loc_y))
        {
            col_r = 5;
        }
    }

    return (col_b + col_r + col_l);
}

void deplacerPiece(struct Piece* _p, struct Plateau* _plat, size_t _d)
{
    /* Effacer la position d'origine */
    size_t i = 0;

    for(i = 0; i < N_BLOCK_PER_PIECE; i++)
    {
        size_t loc_x = _p->cases[i].x;
        size_t loc_y = _p->cases[i].y;

        _plat->map[loc_x][loc_y] = VIDE;
    }

    /* Fin Effacer */

    switch(_d)
    {
        case UP:
        {
            for(i = 0; i < N_BLOCK_PER_PIECE; i++)
            {
                _p->cases[i].y--;
            }

            _p->box_y--;
        }
        break;
        case DOWN:
        {
            for(i = 0; i < N_BLOCK_PER_PIECE; i++)
            {
                _p->cases[i].y++;
            }

            _p->box_y++;
        }
        break;
        case LEFT:
        {
            for(i = 0; i < N_BLOCK_PER_PIECE; i++)
            {
                _p->cases[i].x--;
            }

            _p->box_x--;
        }
        break;
        case RIGHT:
        {
            for(i = 0; i < N_BLOCK_PER_PIECE; i++)
            {
                _p->cases[i].x++;
            }

            _p->box_x++;
        }
        break;
        default:
        break;
    }

    /* On actualise la map */

    for(i = 0; i < N_BLOCK_PER_PIECE; i++)
    {
        size_t loc_x = _p->cases[i].x;
        size_t loc_y = _p->cases[i].y;

        _plat->map[loc_x][loc_y] = _p->type;
    }

    /* Fin modifier map */
}

void viderMap(const int row, struct Plateau* p)
{
    int i, j;

    for(i = 1; i < (p->columns - 1); i++)
    {
        for(j = row; j >= 2; j--)
        {
            p->map[i][j] = p->map[i][j - 1];
            p->map[i][j - 1] = VIDE;
        }
    }
}
