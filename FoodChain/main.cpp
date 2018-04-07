#include "grman/grman.h"
#include <iostream>

#include "graph.h"
#include "Interface.h"


/******************************** INFOS UTILES *******************************************************************************
Touche 'SPACE' = trouve les composantes fortements connexes
Touche 'P' = afficher graphe simplifié des composantes fortements connexes, toujours le faire après avoir fait 'SPACE'

Touche 'K' = recherche le nb de sommet minimum à enlever pour déconnecter le graphe
Touche 'B' = afficher les composantes connexes après avoir déconnecter le graph ( apres avoir fait 'k' )



******************************************************************************************************************************/


int main()
{
    ///Initialisations
    grman::init();
    grman::set_pictures_path("pics");
    //Pointeur pour le menu
    int n = 3;
    int* p_n = &n;

    Graph g;

    g.chargement_fichier_c();

    int compteur = 0;

    while ( !key[KEY_ESC] )
    {

        if(compteur>1000)
        {
            if(n==1)
            {
                g.acces_G1(p_n);
            }

            if(n==2)
            {
                g.acces_G2(p_n);
            }

            if(n==3)
            {
                g.acces_G3(p_n);
            }

            compteur = 0;
            std::cout << "BAM " << std::endl;

        }

        compteur++;
    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


