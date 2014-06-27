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

#ifndef SCORES_H
#define SCORES_H

struct Scores
{
    char pseudo[10];
    int points;
};

struct liste;

struct Scores* creerScore(const char* _pseudo, const int _points);

void scores(SDL_Surface* screen, struct liste* _liste_scores);

void trierScores(struct liste* _scores);

void sauvegarderScores(struct liste* _liste_scores);
struct liste* chargerScores();

void ecrireFichier(const char* str, const char* nomFichier);

#endif
