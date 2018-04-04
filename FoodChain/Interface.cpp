#include "Interface.h"

void affichage_outil()
{
    //affichage/chargement des images pour la tool bar
    grman::show_picture(grman::page, "G1.bmp", 24,6,0);
    grman::show_picture(grman::page, "G2.bmp", 24,86,0);
    grman::show_picture(grman::page, "G3.bmp", 24,166,0);
    grman::show_picture(grman::page, "add.bmp", 24,486,0);
    grman::show_picture(grman::page, "delete.bmp", 24,566,0);
    grman::show_picture(grman::page, "logout.bmp", 24,646,0);

}

void delete_espece(Graph& g)
{
    //On voit si y a un clique sur les bouttons de la barre outils
    g.get_interface()->get_buttonAdd().interact_focus();

    if(g.get_interface()->get_buttonAdd().clicked())
    {
        std::cout << "Ajouter une espece " << std::endl;
    }
}

void add_espece(Graph& g)
{
    //On voit si y a un clique sur les bouttons de la barre outils
    g.get_interface()->get_buttonDelete().interact_focus();

    if(g.get_interface()->get_buttonDelete().clicked())
    {
        std::cout << "Supprimer une espece " << std::endl;
    }
}

void acces_G1(Graph& g)
{
    //On voit si y a un clique sur les bouttons de la barre outils
    g.get_interface()->get_buttonG1().interact_focus();

    if(g.get_interface()->get_buttonG1().clicked())
    {
        std::cout << "Bienvenue sur le graphe 1 :) " << std::endl;
    }
}

void acces_G2(Graph& g)
{
    //On voit si y a un clique sur les bouttons de la barre outils
    g.get_interface()->get_buttonG2().interact_focus();

    if(g.get_interface()->get_buttonG2().clicked())

    {
        std::cout << "Bienvenue sur le graphe 2 :) " << std::endl;
    }
}

void acces_G3(Graph& g)
{
    //On voit si y a un clique sur les bouttons de la barre outils
    g.get_interface()->get_buttonG3().interact_focus();

    if(g.get_interface()->get_buttonG3().clicked())
    {
        std::cout << "Bienvenue sur le graphe 3 :) " << std::endl;
    }
}

void exit(Graph& g)
{
    //On voit si y a un clique sur les bouttons de la barre outils
    g.get_interface()->get_buttonExit().interact_focus();

    if(g.get_interface()->get_buttonExit().clicked())
    {
        std::cout << "A bientot" << std::endl;
        exit(0);
    }
}


