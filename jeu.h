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

#ifndef JEU_H
#define JEU_H

#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

struct liste;

/* Structures */
struct Case
{
    size_t x, y;
};

struct Piece
{
	struct Case cases[4];
	size_t type;

	size_t taille_box;
	int box_x, box_y;

	size_t taille_rot;
	int actu_rot;

	void (*rotation)(struct Piece* piece);
};

struct Plateau
{
    size_t rows, columns;
    int** map;
};

/* Fonctions de jeu */
    /* Fonctions de creations */
struct Plateau* creerPlateau(const size_t _c, const size_t _r);
struct Piece* creerI(struct Plateau* _p);
struct Piece* creerO(struct Plateau* _p);
struct Piece* creerT(struct Plateau* _p);
struct Piece* creerL(struct Plateau* _p);
struct Piece* creerJ(struct Plateau* _p);
struct Piece* creerS(struct Plateau* _p);
struct Piece* creerZ(struct Plateau* _p);

    /* Fonctions de rotations */
void rotationI(struct Piece* piece);
void rotationO(struct Piece* piece);
void rotationT(struct Piece* piece);
void rotationL(struct Piece* piece);
void rotationJ(struct Piece* piece);
void rotationZ(struct Piece* piece);
void rotationS(struct Piece* piece);

    /* Fonctions de traitements */
int memePiece(const struct Piece* _p, const size_t x, const size_t y);
int collision(struct Piece* _p, struct Plateau* _plat);
void deplacerPiece(struct Piece* _p, struct Plateau* _plat, size_t _d);
void viderMap(const int row, struct Plateau* p);

    /* Fonction principale */
struct Scores* jeu(SDL_Surface* screen, struct liste* liste_musiques);

#endif
