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

#include "liste.h"

struct liste* creerListeDeScores(struct Scores* _scores)
{
    struct liste* nouvelle = (struct liste*)malloc(sizeof(struct liste));

    nouvelle->var = (struct Scores*)_scores;
    nouvelle->suivant = NULL;

    return nouvelle;
}

struct liste* creerListeDeMusiques(Mix_Music* _mus)
{
    struct liste* nouvelle = (struct liste*)malloc(sizeof(struct liste));

    nouvelle->var = (Mix_Music*)_mus;
    nouvelle->suivant = NULL;

    return nouvelle;
}

struct liste* donner(struct liste* debut, size_t i)
{
    if(i == 0)
        return debut;
    else
        return donner(debut->suivant, (i - 1));
}

size_t taille(struct liste* debut)
{
    struct liste* current = debut;
    size_t taille = 0;

    while(current != NULL)
    {
        taille = taille + 1;
        current = current->suivant;
    }

    return taille;
}


void ajouter(struct liste** debut, void * _var) // Double pointeur car on veut allouer l'adresse du pointeur designé par *debut. Le DOuble pointeur nous permet d'obtenir cet adresse !
{
    if(*debut == NULL)
    {
        *debut = (struct liste*)malloc(sizeof(struct liste));
        (*debut)->suivant = NULL;
        (*debut)->var = _var;
    }
    else
    {
        ajouter(&((*debut)->suivant), _var);
    }
}

void assign(struct liste* debut, size_t i, void * _var)
{
    if(i == 0)
        debut->var = _var;
    else
        assign(debut->suivant, (i - 1), _var);
}

void swap(struct liste* debut, size_t i, size_t j)
{
    struct liste* swap = (struct liste*)malloc(sizeof(struct liste));

    swap->var = donner(debut, i)->var;
    assign(debut, i, donner(debut, j)->var);
    assign(debut, j, swap->var);
}

void inserer(struct liste* debut, size_t i, void * _var)
{
    if(i == 0)
    {
        ajouter(donner(debut, (taille(debut) - 1))->var, &debut);

        size_t j = taille(debut) - 1;

        while(debut != donner(debut, j))
        {
            assign(debut, j, donner(debut, (j - 1))->var);
            j = j - 1;
        }

        debut->var = _var;
    }
    else
    {
        inserer(debut->suivant, (i - 1), _var);
    }
}

void supprimmer(struct liste** debut, size_t i)
{
    if(taille(*debut) == 1)
    {
        detruire(debut);
    }
    else
    {
        while(i < (taille(*debut) - 1))
        {
            assign(*debut, i, (donner(*debut, (i + 1))->var));
            i++;
        }

        detruire(&(donner(*debut, (taille(*debut) - 2))->suivant));
    }
}

void detruire(struct liste** liste)
{
    free(*liste);
    *liste = NULL;
}

void viderListe(struct liste** debut)
{
    for(size_t t = 0; t < taille(*debut); t++)
    {
        supprimmer(debut, 0);
    }
}

int id(struct liste* debut, void * _var, int t)
{
    if(debut->var == _var)
        return t;
    else
        return id(debut->suivant, _var, (t + 1));
}
