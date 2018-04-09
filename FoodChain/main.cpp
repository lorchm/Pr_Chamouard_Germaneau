/**
*\file main.cpp
*\brief appelles les sous programmes qui font fonctionner le programme
*\author Barbara Germaneau + Laure Chamouard
*\version 2.0
*\date 2 avril 2018
**/


#include "grman/grman.h"
#include <iostream>

#include "graph.h"
#include "Interface.h"


/******************************** INFOS UTILES ***********************************************************************************************
Il faut parfois cliquer plusieurs fois pour bien faire comprendre au programme que l'on clique (et bien appuyer sur la partie gauche du bouton ça marche mieux)

Pour voir un graphe : Cliquer sur la case (G1/G2/G3) correspondante puis appuyer sur read

Pour sauvegarder les positions et les espèces supprimées/ajoutées : Bouton "save"
(Changer de graphe ou quitter le programme ne sauvegarde pas)

Touche 'P' = afficher graphe simplifié des composantes fortements connexes, toujours le faire après avoir cliquer sur 'CFC'

Touche 'B' = afficher les composantes connexes après avoir déconnecter le graph ( apres avoir fait 'k-connex' )

Le code a été construit à partir du code de Mr. Fercoq.

*************************************************************************************************************************************************/

/**
*\fn int main()
*\brief appel des sous-programmes qui font fonctionner le projet
*\return retourne 0 pour finir le programme
*/
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
        if(compteur>1000000)
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

            //On repasse le compteur à 0
            compteur = 0;

        }

        //On augmente le compteur à chaque tour de boucle
        compteur++;
    }

    //Fermeture allegro
    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


