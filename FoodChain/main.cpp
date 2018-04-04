#include "grman/grman.h"
#include <iostream>

#include "graph.h"
#include "Interface.h"

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;
    g.chargement_fichier_c();

    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();

        ///Affichage barre outil
        affichage_outil();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();

        ///Appel les fonctions pour gérer les actions sur les bouttons
        acces_G1(g);
        acces_G2(g);
        acces_G3(g);

        g.add_espece();
        g.delete_espece();
        exit(g);

//        g.get_vertices()[g.get_edges[2].m_from].get_interface()=nullptr;
    }

    grman::fermer_allegro();

    g.sauv_graphec();

    return 0;
}
END_OF_MAIN();


