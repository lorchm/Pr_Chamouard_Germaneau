#include "grman/grman.h"
#include <iostream>

#include "graph.h"
#include "Interface.h"

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

        if(compteur>100000000)
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


