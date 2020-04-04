#include "Huffmann.h"

/********MAIN********/

int main(int argc, char** argv)
{
    /*Ouverture du fichier compressé*/
    FILE* fic_compresse = NULL;
    fic_compresse = fopen(argv[1], "rb");
    assert(fic_compresse != NULL);

    /*Création du fichier décompressé*/
    FILE* fic_decompresse = fopen(argv[2], "wb");
    assert(NULL != fic_decompresse);

    /*On détermine la taille du texte compressé*/
    fseek(fic_compresse, 0L, SEEK_END);
    int size_compresse = ftell(fic_compresse);
    fseek(fic_compresse, 0, SEEK_SET);

    /*On reconstruit l'arbre de codage en lisant l'entête*/
    tableau *T = lire_occurrences(fic_compresse);
    noeud *racine = creer_arbre(T);

    printf("Restauration de l'arbre de codage : OK\n");

    if(est_feuille(racine))
        racine->code.code = "1";
    else
    {
        strcpy(racine->gauche->code.code, "0");
        strcpy(racine->droite->code.code, "1");

        codage(racine->gauche);
        codage(racine->droite);
    }

    printf("Association caractere-code : OK\n");

    /*Chaine de caractères stockant la representation binaire de l'octet lu*/
    char * bin = (char *) malloc(8*sizeof(char));
    unsigned char c;
    fread(&c, sizeof(unsigned char), 1, fic_compresse);
    char_to_bin(c, bin);

    int position = 256*(sizeof(int));

    printf("Decompression : en cours\n");

    /*On décode chacun des octets lus*/
    while (position < (size_compresse - ( sizeof(int) + sizeof(unsigned char) )) )
    {
        int i;
         for (i = 0; i < 8; i++)
         {
             if (bin[i] == '1')
             {
                 if (racine->droite != NULL)
                 {
                   racine = racine->droite;
                 }
                 else
                 {
                     fwrite(&racine->code.nom, sizeof(unsigned char), 1, fic_decompresse);
                     while(racine->parent != NULL) racine = racine->parent;
                     racine = racine->droite;
                 }
             }

             else if(bin[i] == '0')
             {
                 if(racine->gauche != NULL)
                     racine = racine->gauche;
                 else
                 {
                     fwrite(&racine->code.nom, sizeof(unsigned char), 1, fic_decompresse);
                     while(racine->parent != NULL) racine = racine->parent;
                     racine = racine->gauche;
                 }
             }
         }
        fread(&c, sizeof(unsigned char), 1, fic_compresse);
        char_to_bin(c, bin);
        position++;
     }

    /*On lit le nombre de bits utiles sur le dernier octet*/
    fread(&position, sizeof(int), 1, fic_compresse);

    int i;
    for (i = 0; i <= position; i++)
    {
        if (bin[i] == '1')
        {
            if (racine->droite != NULL)
            {
                racine = racine->droite;
            }
            else
            {
                fwrite(&racine->code.nom, sizeof(unsigned char), 1, fic_decompresse);
                while(racine->parent != NULL) racine = racine->parent;
                racine = racine->droite;
            }
        }

        else if(bin[i] == '0')
        {
            if(racine->gauche != NULL)
                racine = racine->gauche;
            else
            {
                fwrite(&racine->code.nom, sizeof(unsigned char), 1, fic_decompresse);
                while(racine->parent != NULL) racine = racine->parent;
                racine = racine->gauche;
            }
        }
    }


    printf("Decompression : OK\n");

    /*On ferme les deux fichiers*/
    fclose(fic_compresse);
    fclose(fic_decompresse);

    /*On libère la mémoire*/
    free_tableau(T);

    return EXIT_SUCCESS;
}
