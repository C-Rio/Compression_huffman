#include "Huffmann.h"

/********DEFINITION DES FONCTIONS********/

noeud* init_noeud(unsigned char c, int p)
{
    noeud *N = (noeud*)malloc(sizeof(noeud));
    N->active = 0;
    N->parent = NULL;
    N->gauche = NULL;
    N->droite = NULL;
    N->code.nom = c;
    N->code.code = (char*)calloc(30, sizeof(char));
    N->poids = p;
    return N;
}

tableau* init_tableau()
{
    int i;
    tableau *T = (tableau*)malloc(sizeof(tableau));
    T->tab = (noeud**)malloc(256 * sizeof(noeud*));
    for(i=0; i <= 255; i++)
    {
        T->tab[i] = init_noeud(i, 0);
    }
    T->taille = 256;
    T->nb_actif = 0;
    return T;
}

void free_tableau(tableau *T)
{
    int i;
    for(i=0; i < T->taille; i++)
    {
        free(T->tab[i]);
    }
    free(T->tab);
    free(T);
}

tableau* comptage( unsigned char * texte, int size)
{
    tableau *T = init_tableau();
    int position = 0;

    while( position < size )
    {

      if(!(T->tab[(int) texte[position]]->active))
      {
          T->tab[(int) texte[position]]->active ++;
          T->nb_actif ++;
      }

      T->tab[(int) texte[position]]->poids ++;
      position++;
    }

    return T;
}

tableau* lire_occurrences( FILE * fic)
{
    tableau *T = init_tableau();
    int occurrence = 0;
    int i;
    for (i = 0; i < 256 ; i++)
    {
        fread(&occurrence, sizeof(int), 1 , fic);
        T->tab[i]->poids = occurrence;

        if (occurrence > 0)
        {
            T->tab[i]->active++;
            T->nb_actif++;
        }
    }

    return T;
}

noeud* recherche_min(tableau *T)
{
    int indice;
    int i=0;
    while(i < T->taille && T->tab[i]->active == 0)
        i++;

    indice = i;
    for(i = indice+1; i < T->taille; i++)
    {
        if(T->tab[i]->active && T->tab[i]->poids < T->tab[indice]->poids)
            indice = i;
    }
    return T->tab[indice];
}

noeud* fusion_noeuds(noeud *a, noeud *b)
{
    noeud *fusion = init_noeud(124, a->poids + b->poids);
    fusion->gauche = a;
    fusion->droite = b;
    a->parent = fusion;
    b->parent = fusion;
    fusion->active ++;
    return fusion;
}

noeud* creer_arbre(tableau *T)
{
    assert(T->nb_actif != 0);
    noeud *min1, *min2;
    int cpt = 0;

    if(T->nb_actif == 1)
    {
        return recherche_min(T);
    }
    else
    {
        while(T->nb_actif > 1)
        {
            cpt ++;
            min1 = recherche_min(T);
            min1->active = 0;
            min2 = recherche_min(T);
            min2->active = 0;
            T->tab = (noeud**)realloc(T->tab, (256 + cpt) * sizeof(noeud*));
            T->tab[255 + cpt] = fusion_noeuds(min1, min2);
            T->taille ++;
            T->nb_actif --;
        }
    }
    return T->tab[T->taille -1];
}

_Bool est_feuille(noeud *n)
{
    return (n->gauche == NULL || n->droite == NULL);
}

void codage(noeud *n)
{
    if(!est_feuille(n))
    {
        if(n->gauche != NULL)
        {
            strcpy(n->gauche->code.code, n->code.code);
            n->gauche->code.code = strcat(n->gauche->code.code, "0");
            codage(n->gauche);
        }
        if(n->droite != NULL)
        {
            strcpy(n->droite->code.code, n->code.code);
            n->droite->code.code = strcat(n->droite->code.code, "1");
            codage(n->droite);
        }
    }
}

int max(int a, int b)
{
    if(a > b)
        return a;
    return b;
}

int presence(unsigned char nom, noeud* n)
{
    if(n == NULL)
        return 0;
    else
    {
        if(n->code.nom == nom && est_feuille(n))
            return 1;
        else
            return max(presence(nom, n->gauche), presence(nom, n->droite));
    }
}

char * rechercher(unsigned char nom, noeud* n)
{
    if(n->code.nom == nom && est_feuille(n))
        return n->code.code;
    else
    {
        if(presence(nom, n->gauche))
            return rechercher(nom, n->gauche);
        if(presence(nom, n->droite))
            return rechercher(nom, n->droite);
    }

    assert(0);
}

void stocker_occurrences(FILE* fic, tableau * T)
{
    int i;
    for (i = 0; i < 256; i++)
    {
        fwrite(&T->tab[i]->poids, sizeof(int), 1, fic);
    }
}

void char_to_bin(unsigned char c, char * binaire)
{
    binaire[0] = (c&0x80)?'1':'0';
    binaire[1] = (c&0x40)?'1':'0';
    binaire[2] = (c&0x20)?'1':'0';
    binaire[3] = (c&0x10)?'1':'0';
    binaire[4] = (c&0x08)?'1':'0';
    binaire[5] = (c&0x04)?'1':'0';
    binaire[6] = (c&0x02)?'1':'0';
    binaire[7] = (c&0x01)?'1':'0';
}
