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

#ifndef LISTE_H
#define LISTE_H

struct Scores;

struct liste
{
    void * var;
    struct liste* suivant;
};

struct liste* creerListeDeScores(struct Scores* _scores);
struct liste* creerListeDeMusiques(Mix_Music* _mus);

struct liste* donner(struct liste* debut, size_t i);

void ajouter(struct liste** debut, void * _var);

size_t taille(struct liste* debut);

void assign(struct liste* debut, size_t i, void * _var);

void swap(struct liste* debut, size_t i, size_t j);

void inserer(struct liste* debut, size_t i, void * _var);

void supprimmer(struct liste** debut, size_t i);

void viderListe(struct liste** debut);

void detruire(struct liste** liste);

int id(struct liste* debut, void * _var, int t);

#endif

