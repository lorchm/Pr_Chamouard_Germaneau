/**
*\file graph.h
*\brief declarations des fonctions de Graph (classe de Sommets, Arcs, Graphe et leurs interfaces et utilisations)
*\author Barbara Germaneau + Laure Chamouard
*\version 3.0
*\date 2 avril 2018
*/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <vector>
#include <map>
#include <string>
#include <memory>

#include "grman/grman.h"
#include "grman/widget.h"

#include "bin.h"

/***************************************************
                    VERTEX
****************************************************/

/**
*\class VertexInterface
*\brief classe sur l'interface d'un sommet
*
* s'occupe de l'aspect visuel d'un sommet: sa boite, son image, le slider qui affiche la population
*
*/
class VertexInterface
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Vertex;
    friend class EdgeInterface;
    friend class Graph;

private :

    // La boite qui contient toute l'interface d'un sommet
    grman::WidgetBox m_top_box;

    // Un slider de visualisation/modification de la valeur du sommet
    grman::WidgetVSlider m_slider_value;

    // Un label de visualisation de la valeur du sommet
    grman::WidgetText m_label_value;

    // Une image de "remplissage"
    grman::WidgetImage m_img;

    // Un label indiquant l'index du sommet
    grman::WidgetText m_label_idx;

    // Une boite pour le label précédent
    grman::WidgetText m_box_label_idx;

public :

    // Le constructeur met en place les éléments de l'interface
    // voir l'implémentation dans le .cpp
    VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx);
};


/**
*\class Vertex
*\brief attributs et fonctions d'un sommet
*
* regroupe les attributs d'un sommet, le constructeur et les fonctions de mises a jour du sommet
*
*/
class Vertex
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Graph;
    friend class VertexInterface;
    friend class Edge;
    friend class EdgeInterface;

    private :

        ///Attributs du style de vie
        int m_taux_repro;

        ///bool pour appartient à la connexité et appartient à la forte connexité
        bool m_1=false;
        bool m_2=false;
        bool m_present_ds_compo=false;

        ///utile pour la recherche de k sommets connexite
        bool m_deconnect=false;
        bool m_prst_graph=true;

         std::vector<int> m_indice_sommet_ancre;

        ///K qu'on utilise pour calculer la taille de la population
        int m_K=1;

        ///Indices arcs entrants
        std::vector<int> m_in;

        ///Indices arcs sortants
        std::vector<int> m_out;

        /// un exemple de donnée associée au sommet, on peut en ajouter d'autres...
        int m_value;

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<VertexInterface> m_interface = nullptr;

    public:

        /// Constructeur de Vertex
        Vertex (int value=0, VertexInterface *interface=nullptr, int taux=0) :
            m_value(value), m_interface(interface), m_taux_repro(taux)  {  }

        ///Mise à jour des sommets
        void pre_update();
        void post_update();

        ///Getters
        bool get_Bool1(){return m_1;}
        bool get_Bool2(){return m_2;}

        ///Setters
        void set_Bool1 (bool b){ m_1 = b;}
        void set_Bool2 (bool b){ m_2 = b;}
        void set_present_ds_compo(bool p){ m_present_ds_compo = p;}
        void set_deconnect( bool d){ m_deconnect = d;}
        void set_prst_graph( bool g){ m_prst_graph = g;}

};

/***************************************************
                    EDGE
****************************************************/
/**
*\class EdgeInterface
*\brief classe sur l'interface d'une arête
*
* regroupe tout l'aspect visuel d'une arete : son slider, son affichage de son poids, et l'affichage de l'arete orientee
*
*/
class EdgeInterface
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Edge;
    friend class Graph;

    private :

        // Le WidgetEdge qui "contient" toute l'interface d'un arc
        grman::WidgetEdge m_top_edge;

        // Une boite pour englober les widgets de réglage associés
        grman::WidgetBox m_box_edge;

        // Un slider de visualisation/modification du poids valeur de l'arc
        grman::WidgetVSlider m_slider_weight;

        // Un label de visualisation du poids de l'arc
        grman::WidgetText m_label_weight;

    public :

        bool m_presence;

        // Le constructeur met en place les éléments de l'interface
        // voir l'implémentation dans le .cpp
        EdgeInterface(Vertex& from, Vertex& to);
};

/**
*\class Edge
*\brief contient tous les attributs d'un arc
*
* regroupe tous les attributs comme son poids, l'origine/arrivee et les mises a jour de l'arc pour chaques tours de boucle
*
*/
class Edge
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Graph;
    friend class EdgeInterface;

    private :

        /// Attribut(s)
        int m_weight;
        int m_from;
        int m_to;

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<EdgeInterface> m_interface = nullptr;

    public:

        /// Constructeur
        Edge (int weight=0, EdgeInterface *interface=nullptr) :
            m_weight(weight), m_interface(interface)  {  }

        ///Updates
        void pre_update();
        void post_update();

        ///Getters
        int getFrom();
        int getTo();
        int getWeight(){return m_weight;}

};


/***************************************************
                    GRAPH
****************************************************/
/**
*\class GraphInterface
*\brief contient la partie graphique du graphe
*
* interface du graphe avec l'ajout des interfaces des sommets et des arcs. gestion de la boite a outils avec l'initialisation de chaques boutons
*
*/
class GraphInterface
{
    friend class Graph;

private :

    /// La boite qui contient toute l'interface d'un graphe
    grman::WidgetBox m_top_box;

    /// Dans cette boite seront ajoutés les (interfaces des) sommets et des arcs...
    grman::WidgetBox m_main_box;

    /// Dans cette boite seront ajoutés des boutons de contrôle etc...
    grman::WidgetBox m_tool_box;

    // Boutons
    ///Bouton pour accéder aux 3 réseaux
    grman::WidgetButton m_buttonG1;
    grman::WidgetButton m_buttonG2;
    grman::WidgetButton m_buttonG3;
    ///Bouton pour ajouter/enlever une espèce
    grman::WidgetButton m_buttonAdd;
    grman::WidgetButton m_buttonDelete;
    ///Bouton pour quitter les programme
    grman::WidgetButton m_buttonExit;

    ///Bouton save/lecture
    grman::WidgetButton m_lect;
    grman::WidgetButton m_save;

    ///Boutons pour cfc et recherche k-connexités
    grman::WidgetButton m_cfc;
    grman::WidgetButton m_k_connexite;

public :

    ///Constructeur
    GraphInterface(int x, int y, int w, int h);

    ///Getters pour les bouttons button
    grman::WidgetButton &get_buttonG1()
    {
        return m_buttonG1;
    }
    grman::WidgetButton &get_buttonG2()
    {
        return m_buttonG2;
    }
    grman::WidgetButton &get_buttonG3()
    {
        return m_buttonG3;
    }

    grman::WidgetButton &get_buttonAdd()
    {
        return m_buttonAdd;
    }
    grman::WidgetButton &get_buttonDelete()
    {
        return m_buttonDelete;
    }
    grman::WidgetButton &get_buttonExit()
    {
        return m_buttonExit;
    }
    grman::WidgetButton &get_lect()
    {
        return m_lect;
    }
    grman::WidgetButton &get_save()
    {
        return m_save;
    }

    grman::WidgetButton &get_cfc()
    {
        return m_cfc;
    }

    grman::WidgetButton &get_kconnex()
    {
        return m_k_connexite;
    }

};

/**
*\class Graph
*\brief classe contenant les attributs et actions sur les graphes
*
* contient les aretes et les sommets d'un graphe et permet de les creer. Contient les differents algorithmes qui permettent d'etudier le graphe.
* lectures et sauvegardes des fichiers. Affichages des differents graphes.
*
*/
class Graph
{
    private :

        /// La "liste" des arêtes
        std::map<int, Edge> m_edges;

        /// La liste des sommets
        std::map<int, Vertex> m_vertices;

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<GraphInterface> m_interface = nullptr;

        ///les "poubelles" des aretes et sommets supprimés
        std::vector<Edge_bin> m_bin_edges;
        std::vector<Vertex_bin> m_bin_vertices;

        ///vecteur qui contient les sommet ancre des compo connexe
        std::vector<int> m_indice_sommet_ancre;

        ///vecteur qui contient les composantes fort. connexes
        std::vector<std::vector<Vertex>> m_vect_composantes;

        ///vecteur qui contient les sommets à afficher pour les différentes composantes connexes
        std::vector<std::vector<Vertex>> m_connexe;

    public:

        /// Constructeur
        Graph (GraphInterface *interface=nullptr) :
            m_interface(interface)  {  }

        ///Ajouter edge/vertex
        void add_interfaced_vertex(int idx, int value, int x, int y, std::string pic_name, int pic_idx,int taux_repro);
        void add_interfaced_edge(int idx, int vert1, int vert2, int weight=0);

        ///Methodes de lecture de fichier
        void chargement_fichier_a();
        void chargement_fichier_b();
        void chargement_fichier_c();

        ///Methodes de sauvegarde
        void sauv_graphea();
        void sauv_grapheb();
        void sauv_graphec();

        ///Ajouter et supprimer espèce
        void delete_espece();
        void remove_edge(int eidx);
        void add_espece1();
        void add_espece2();
        void add_espece3();

        ///Recherche de composantes fortement connexe
        bool CFC(int sommet_ancre);
        void Marquer_composantes();
        void reset_marquages();
        void afficher();
        bool marquage(std::vector<int> v1, std::vector<int> v2 );

        ///Rendre le graphe non connexe
        void k_connexite();

        ///Recherche graph connexe
        bool Rechercher_connexes();
        void Rechercher_connexe(int sommet_ancre);

        /// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
        void update();

        ///Getters pour m_interface
        std::shared_ptr<GraphInterface> &get_interface(){return m_interface;}
        std::map<int, Vertex> get_vertices(){return m_vertices;}
        std::map<int, Edge> get_edges(){return m_edges;}

        ///Affichage
        void affichage_outil();
        void acces_G1(int* n);
        void acces_G2(int* n);
        void acces_G3(int* n);
        void sortie();

        ///Affichages des graphes simplifiés
        void graph_simpl();
        void graph_simpl_connex();

        ///Methode pour recup indice d'un sommet
        int get_indice(Vertex V);

        ///Simulation des variations de N et K
        void var_temps();

        ///Calcul coeff binomial
        std::vector<std::vector<int>> Coeff_binomial(const int k);

};


#endif // GRAPH_H_INCLUDED
