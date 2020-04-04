#include "Huffmann.h"

/********MAIN********/

int main(int argc, char** argv)
{
    /*Ouverture du fichier à compresser*/
    FILE* fic = NULL;
    fic = fopen(argv[1], "rb");
    assert(fic != NULL);

    /*On créer le fichier compressé*/
    FILE* fic2 = fopen(argv[2], "wb");
    assert(NULL != fic2);

    /*On détermine la taille du texte à compresser*/
    fseek(fic, 0L, SEEK_END);
    int size = ftell(fic);
    fseek(fic, 0, SEEK_SET);

    /*On lit le fichier compressé et on stocke son contenu*/
    unsigned char * texte;
    texte = (unsigned char *) malloc(( size) * sizeof(unsigned char));
    fread(texte, sizeof(unsigned char), size, fic);
    printf("Lecture du texte : OK\n");

    /*On compte les occurences de chaque caractère*/
    tableau *T = comptage(texte, size);
    printf("Calcul des occurrences : OK\n");

    /*On crée l'arbre binaire permettant le codage*/
    noeud *racine = creer_arbre(T);
    printf("Creation de l'arbre de codage : OK\n");

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

    /*On stocke l'arbre de codage en entête*/
    stocker_occurrences(fic2, T);

    printf("Ecriture de l'entete : OK\n");

    int position_texte = 0, position_code = 0, position =0;
    char *code;
    unsigned char codage = 0;

    /*On récupère le code du premier octet lu*/
    code = rechercher(texte[position_texte], racine);


    printf("Compression : en cours\n");
    while ( position_texte < size )
    {
      for (position = 0; (position < 8) && (position_texte < size); position++)
      {
          if (position_code < strlen(code))
          {
              codage += ( pow(2, 7-position) * (code[position_code] - '0') );
              position_code++;
          }
          else
          {
            position_texte++;
            if ( position_texte < size)
            {
              code = rechercher(texte[position_texte], racine);
              position_code = 0;
            }

            position--;
          }
      }
      fwrite(&codage, sizeof(unsigned char), 1, fic2);
      codage = 0;
    }

    /*On enregistre le nombre de bits utiles dans le dernier octet*/
    fwrite(&position, sizeof(int), 1, fic2);

    printf("Compression : OK\n");

    /*On ferme les trois fichiers*/
    fclose(fic);
    fclose(fic2);

    /*On libère la mémoire du tableau*/
    free_tableau(T);

    return EXIT_SUCCESS;
}
