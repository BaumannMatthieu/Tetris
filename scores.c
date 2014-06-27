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
#include "scores.h"
#include "liste.h"

void scores(SDL_Surface* screen, struct liste* _liste_scores)
{
    struct Input in;
    memset(&in, 0, sizeof(struct Input));

    TTF_Font* police = TTF_OpenFont("Data/Graphiques/Tetris.ttf", 26);
    SDL_Color blanc = {255, 255, 255};

    SDL_Surface* scores_s[SCORES_MAX] = {NULL};
    char scores_chaine[SCORES_MAX][100];

    int i;

    SDL_Surface* scores_font_s = IMG_Load("Data/Graphiques/scores.png");

    /* Trier la liste en fonction des scores de chaques joueurs */
    trierScores(_liste_scores);

    /* Limiter la liste des scores à 10 membres*/
    while(taille(_liste_scores) > 10)
    {
        supprimmer(&_liste_scores, (taille(_liste_scores) - 1));
    }

    while(!in.quit && !in.key[SDLK_ESCAPE])
    {
        UpdateEvents(&in);

        for(i = 0; i < SCORES_MAX; i++)
        {
            if(taille(_liste_scores) > i)
            {
                sprintf(scores_chaine[i], "%d ] %s : %d pts", (i + 1), ((struct Scores*)(donner(_liste_scores, i)->var))->pseudo, ((struct Scores*)(donner(_liste_scores, i)->var))->points);
            }
            else
            {
                sprintf(scores_chaine[i], "%d ] aucun", (i + 1));
            }

            SDL_FreeSurface(scores_s[i]);
            scores_s[i] = TTF_RenderText_Blended(police, scores_chaine[i], blanc);
        }

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

        SDL_BlitSurface(scores_font_s, NULL, screen, NULL);

        for(i = 0; i < SCORES_MAX; i++)
        {
            SDL_Rect pos_score_actu;
            pos_score_actu.x = 80;
            pos_score_actu.y = 150 + 30*i;

            SDL_BlitSurface(scores_s[i], NULL, screen, &pos_score_actu);
        }

        SDL_Flip(screen);
    }
}

struct Scores* creerScore(const char* _pseudo, const int _points)
{
    struct Scores* nouveauScore = (struct Scores*)malloc(sizeof(struct Scores));

    nouveauScore->points = _points;
    strcpy(nouveauScore->pseudo, _pseudo);

    return nouveauScore;
}

void trierScores(struct liste* _scores)
{
    size_t i = 0, min = 0;

    struct liste* current = _scores;
    struct Scores* scoreMax = NULL;

    for(size_t j = 0; j < taille(_scores); j++)
    {
        scoreMax = (struct Scores*)(donner(_scores, i)->var);

        for(size_t k = i; k < taille(_scores); k++)
        {
            if(k == i)
                min = i;

            if(((struct Scores*)(current->var))->points > scoreMax->points)
            {
                scoreMax = (struct Scores*)current->var;
                min = k;
            }

            current = current->suivant;
        }

        swap(_scores, min, i);
        i++;

        current = donner(_scores, i);
    }
}

void sauvegarderScores(struct liste* _liste_scores)
{
    char buffer[1000], chaineScores[100], chaineNombreScores[5];

    sprintf(chaineNombreScores, "%d\n", (int)taille(_liste_scores));
    strcat(buffer, chaineNombreScores);

    struct liste* current = _liste_scores;

    while(current != NULL)
    {
        sprintf(chaineScores, "%s  %d\n", ((struct Scores*)(current->var))->pseudo, ((struct Scores*)(current->var))->points);
        strcat(chaineScores, "\0");

        strcat(buffer, chaineScores);

        current = current->suivant;
    }

    strcat(buffer, "\0");

    ecrireFichier(buffer, "Data/scores.txt");
}

struct liste* chargerScores()
{
    int n_scores = 0;

    FILE* file_load = fopen("Data/scores.txt", "r+");
    struct liste* liste_scores = NULL;

    if(file_load != NULL)
    {
        if(fscanf(file_load, "%d\n", &(n_scores)) == EOF)
            exit(EXIT_FAILURE);

        for(size_t i = 0; i < n_scores; i++)
        {
            char pseudo[100];
            int points;

            if(fscanf(file_load, "%s  %d\n", pseudo, &points) == EOF)
                exit(EXIT_FAILURE);

            if(liste_scores == NULL)
            {
                liste_scores = creerListeDeScores(creerScore(pseudo, points));
            }
            else
            {
                ajouter(&liste_scores, creerScore(pseudo, points));
            }
        }

        fclose(file_load);
    }
    else
    {
        fprintf(stderr, "Fichier inexistant\n");
    }

    return liste_scores;
}

void ecrireFichier(const char* str, const char* nomFichier)
{
    FILE* fichier = fopen(nomFichier, "w+");

    if(fichier != NULL)
    {
        fputs(str, fichier);
    }
    else
    {
        fprintf(stderr, "Fichier de sauvegarde inexistant\n");
    }

    fclose(fichier);
}
