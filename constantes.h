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

#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <stdio.h>
#include <stdlib.h>

#define SCREEN_X 500
#define SCREEN_Y 528

#define FALSE 0
#define TRUE 1

#define DIM_BLOCK 24
#define N_BLOCK_PER_PIECE 4

#define SCORES_MAX 10

enum{I, O, T, L, J, Z, S, VIDE, BORD, PERDU};
enum{LEFT, RIGHT, DOWN, UP}; // Deplacement

enum{JEU, SCORES, QUITTER};

#endif
