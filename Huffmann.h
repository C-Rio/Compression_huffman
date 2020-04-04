#ifndef __HUFFMANN_H__
#define __HUFFMANN_H__

/********BIBLIOTHEQUES********/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/********STRUCTURES********/

typedef struct code_s
{
    unsigned char nom;
    char *code;
} code;

typedef struct noeud_s
{
    _Bool active;
    struct noeud_s *parent;
    struct noeud_s *gauche;
    struct noeud_s *droite;
    struct code_s code;
    int poids;
} noeud;

typedef struct tableau_t
{
    int taille;
    int nb_actif;
    noeud** tab;
}tableau;


/********DECLERATION DES FONCTIONS********/

noeud* init_noeud(unsigned char, int);
tableau* init_tableau();
void free_tableau(tableau*);
tableau* comptage(unsigned char *, int);
noeud* recherche_min(tableau*);
noeud* fusion_noeuds(noeud*, noeud*);
noeud* creer_arbre(tableau*);
_Bool est_feuille(noeud*);
void codage(noeud *n);
int max(int, int);
int presence(unsigned char, noeud*);
char * rechercher(unsigned char, noeud*);
void stocker_occurrences(FILE*, tableau*);
tableau* lire_occurrences(FILE*);
void char_to_bin(unsigned char, char*);


#endif
