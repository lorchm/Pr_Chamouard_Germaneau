#include "grman/grman.h"
#include <iostream>

#include "graph.h"
#include "Interface.h"


/******************************** INFOS UTILES *******************************************************************************
Il faut parfois cliquer plusieurs fois pour bien faire comprendre au programme que l'on clique

Pour voir un graphe : Cliquer sur la case (G1/G2/G3) correspondante puis appuyer sur read

Pour sauvegarder les positions et les esp�ces supprim�es/ajout�es : Bouton "save"
(Changer de graphe ou quitter le programme ne sauvegarde pas)

Touche 'SPACE' = trouve les composantes fortements connexes
Touche 'P' = afficher graphe simplifi� des composantes fortements connexes, toujours le faire apr�s avoir fait 'SPACE'

Touche 'K' = recherche le nb de sommet minimum � enlever pour d�connecter le graphe
Touche 'B' = afficher les composantes connexes apr�s avoir d�connecter le graph ( apres avoir fait 'k' )

Le code a �t� construit � partir du code de Mr. Fercoq.

******************************************************************************************************************************/


int main()
{
    //Initialisations
    grman::init();
    grman::set_pictures_path("pics");

    //Pointeur pour le menu
    int n = 1;
    int* p_n = &n;

    //Initialisation du graphe
    Graph g;

    //Chargement d'un fichier (le premier qu'on affiche)
    g.chargement_fichier_a();

    //Initialisation du compteur
    int compteur = 0;

    //Boucle de jeu
    while ( !key[KEY_ESC] )
    {
        //On ralenti le temps gr�ce au compteur
        if(compteur>100000000)
        {
            //Graphe 1
            if(n==1)
            {
                g.acces_G1(p_n);
            }
            //Graphe 2
            if(n==2)
            {
                g.acces_G2(p_n);
            }
            //Graphe 3
            if(n==3)
            {
                g.acces_G3(p_n);
            }

            //On repasse le compteur � 0
            compteur = 0;

        }

        //On augmente le compteur � chaque tour de boucle
        compteur++;
    }

    //Fermeture allegro
    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


