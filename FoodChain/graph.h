#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

/**************************************************************
    Ici sont propos�es 3 classes fondamentales
            Vertex (=Sommet)
            Edge (=Ar�te ou Arc)
            Graph (=Graphe)

    Les ar�tes et les sommets et le graphe qu'ils constituent
    "travaillent" �troitement ensemble : pour cette raison les
    Vertex et Edge se d�clarent amis (friend) de Graph pour que
    ce dernier puisse librement acc�der aux membres (y compris
    protected ou private) de Vertex et Edge.

    Ces Classes peuvent �tres compl�t�es. Il est �galement possible
    de les d�river mais il est malheureusement assez difficile
    de d�river le "triplet" des 3 classes en maintenant des relations
    coh�rentes ( rechercher "c++ class covariance" et "c++ parallel inheritance"
    pour commencer .. ). Il est donc sans doute pr�f�rable, si possible,
    de "customiser" ces classes de base directement, sans h�ritage.

    Le mod�le propos� permet de repr�senter un graphe orient� �ventuellement
    pond�r�, les arcs portent une ou des informations suppl�mentaire(s).
    Les relations/navigations Arcs -> Sommets et Sommets -> Arcs se font
    dans les 2 sens et utilisent des INDICES et NON PAS DES ADRESSES (pointeurs)
    de telle sorte que la topologie du graphe puisse �tre assez facilement
    lue et �crite en fichier, et b�n�ficie d'une bonne lisibilit� en cas de bugs...

    Chaque arc poss�de 2 attributs principaux (en plus de son �ventuelle pond�ration)
        -> m_from (indice du sommet de d�part de l'arc )
        -> m_to (indice du sommet d'arriv�e de l'arc )

    Chaque sommet poss�de 2 liste d'arcs (en plus de ses attributs "internes", marquages...)
        -> m_in (liste des indices des arcs arrivant au sommet : acc�s aux pr�d�cesseurs)
        -> m_out (liste des indices des arcs partant du sommet : acc�s aux successeurs)

    Cependant le probl�me des indices (par rapport aux pointeurs) et qu'en cas
    de destruction d'une entit� (un arc et/ou un sommet sont enlev�s du graphe) alors :

    - Soit il faut reprendre toute la num�rotation pour "boucher le trou"
      (par exemple on a supprim� le sommet n�4, le sommet n�5 devient le 4, 6 devient 5 etc...)
      ce qui pose des probl�mes de stabilit� et de coh�rence, et une difficult� � r�-introduire
      le(s) m�me(s) �l�ment supprim� (au m�me indice)

    - Soit on admet que la num�rotation des sommets et arcs n'est pas contigue, c�d qu'il
      peut y avoir des "trous" : sommets 0 1 5 7 8, pas de sommets 2 ni 3 ni 4 ni 6. La num�rotation
      est stable mais on ne peut plus utiliser un simple vecteur pour ranger la liste de tous
      les arcs et tous les sommets aux indices correspondants, on peut utiliser une map
      qui associe un objet arc ou sommet � des indices arbitraires (pas forc�ment contigus)

    C'est cette 2�me approche qui est propos�e ici : dans la classe graphe vous trouverez
        -> map<int, Edge>   m_edges
        -> map<int, Vertex> m_vertices    (le pluriel de vertex est vertices)

    Il faudra �tre attentif au fait que par rapport � un simple vecteur, le parcours des �l�ments
    ne pourra PAS se faire avec un simple for (int i=0; i<m_edges.size(); ++i) ...m_edges[i]...
    et que les parcours � it�rateur ne donneront pas directement des Edge ou des Vertex
    mais des pairs, l'objet d'int�r�t se trouvant dans "second" ("first" contenant l'indice)
                for (auto &it = m_edges.begin(); it!=m_edges.end(); ++it) ...it->second...
    ou bien     for (auto &e : m_edges) ...e.second...

    Il n'est pas obligatoire d'utiliser ces classes pour le projet, vous pouvez faire les votres

    Au niveau de l'interface, on dissocie une classe interface associ�e � chaque classe fondamentale
    de telle sorte qu'il soit possible de travailler avec des graphes non repr�sent�s � l'�cran
    Imaginons par exemple qu'on doive g�n�rer 1000 permutations de graphes pour tester des
    combinaisons, on ne souhaite pas repr�senter graphiquement ces 1000 graphes, et les
    interfaces p�sent lourd en ressource, avec cette organisation on est libre de r�server ou
    pas une interface de pr�sentation associ�e aux datas (d�couplage donn�es/interface)

***********************************************************************************************/

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

class VertexInterface
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Vertex;
    friend class EdgeInterface;
    friend class Graph;

private :

    /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de d�clarer
    /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
    /// le param�trer ( voir l'impl�mentation du constructeur dans le .cpp )

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

    // Une boite pour le label pr�c�dent
    grman::WidgetText m_box_label_idx;

public :

    // Le constructeur met en place les �l�ments de l'interface
    // voir l'impl�mentation dans le .cpp
    VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx);
};


class Vertex
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Graph;
    friend class VertexInterface;
    friend class Edge;
    friend class EdgeInterface;

private :


    ///Attributs du style de vie
    int m_taux_repro;

    ///bool pour appartient � la connexit� et appartient � la forte connexit�
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

    /// un exemple de donn�e associ�e au sommet, on peut en ajouter d'autres...
    int m_value;

    /// le POINTEUR sur l'interface associ�e, nullptr -> pas d'interface
    std::shared_ptr<VertexInterface> m_interface = nullptr;

    // Docu shared_ptr : https://msdn.microsoft.com/fr-fr/library/hh279669.aspx
    // La ligne pr�c�dente est en gros �quivalent � la ligne suivante :
    // VertexInterface * m_interface = nullptr;

public:

    /// Les constructeurs sont � compl�ter selon vos besoin...
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Vertex (int value=0, VertexInterface *interface=nullptr, int taux=0) :
        m_value(value), m_interface(interface), m_taux_repro(taux)  {  }

    /// Vertex �tant g�r� par Graph ce sera la m�thode update de graph qui appellera
    /// le pre_update et post_update de Vertex (pas directement la boucle de jeu)
    /// Voir l'impl�mentation Graph::update dans le .cpp
    void pre_update();
    void post_update();

    bool get_Bool1(){return m_1;}
    bool get_Bool2(){return m_2;}

    void set_Bool1 (bool b){ m_1 = b;}
    void set_Bool2 (bool b){ m_2 = b;}
    void set_present_ds_compo(bool p){ m_present_ds_compo = p;}
    void set_deconnect( bool d){ m_deconnect = d;}
    void set_prst_graph( bool g){ m_prst_graph = g;}

};



/***************************************************
                    EDGE
****************************************************/

class EdgeInterface
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Edge;
    friend class Graph;

private :

    /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de d�clarer
    /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
    /// le param�trer ( voir l'impl�mentation du constructeur dans le .cpp )

    // Le WidgetEdge qui "contient" toute l'interface d'un arc
    grman::WidgetEdge m_top_edge;

    // Une boite pour englober les widgets de r�glage associ�s
    grman::WidgetBox m_box_edge;

    // Un slider de visualisation/modification du poids valeur de l'arc
    grman::WidgetVSlider m_slider_weight;

    // Un label de visualisation du poids de l'arc
    grman::WidgetText m_label_weight;

public :

    bool m_presence;

    // Le constructeur met en place les �l�ments de l'interface
    // voir l'impl�mentation dans le .cpp
    EdgeInterface(Vertex& from, Vertex& to);
};


class Edge
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Graph;
    friend class EdgeInterface;

private :

    /// un exemple de donn�e associ�e � l'arc, on peut en ajouter d'autres...
    int m_weight;

    /// le POINTEUR sur l'interface associ�e, nullptr -> pas d'interface
    std::shared_ptr<EdgeInterface> m_interface = nullptr;

public:

/// indice du sommet de d�part de l'arc
    int m_from;

    /// indice du sommet d'arriv�e de l'arc
    int m_to;

    /// Les constructeurs sont � compl�ter selon vos besoin...
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Edge (int weight=0, EdgeInterface *interface=nullptr) :
        m_weight(weight), m_interface(interface)  {  }

    /// Edge �tant g�r� par Graph ce sera la m�thode update de graph qui appellera
    /// le pre_update et post_update de Edge (pas directement la boucle de jeu)
    /// Voir l'impl�mentation Graph::update dans le .cpp
    void pre_update();
    void post_update();

    ///Setters/Getters
    int getFrom();
    int getTo();
    int getWeight(){return m_weight;}

};




/***************************************************
                    GRAPH
****************************************************/

class GraphInterface
{
    friend class Graph;

private :

    /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de d�clarer
    /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
    /// le param�trer ( voir l'impl�mentation du constructeur dans le .cpp )

    /// La boite qui contient toute l'interface d'un graphe
    grman::WidgetBox m_top_box;

    /// Dans cette boite seront ajout�s les (interfaces des) sommets et des arcs...
    grman::WidgetBox m_main_box;

    /// Dans cette boite seront ajout�s des boutons de contr�le etc...
    grman::WidgetBox m_tool_box;


    // A compl�ter �ventuellement par des widgets de d�coration ou
    // d'�dition (boutons ajouter/enlever ...)
    ///Bouton pour acc�der aux 3 r�seaux
    grman::WidgetButton m_buttonG1;
    grman::WidgetButton m_buttonG2;
    grman::WidgetButton m_buttonG3;
    ///Bouton pour ajouter/enlever une esp�ce
    grman::WidgetButton m_buttonAdd;
    grman::WidgetButton m_buttonDelete;
    ///Bouton pour quitter les programme
    grman::WidgetButton m_buttonExit;

    ///Bouton save/lecture
    grman::WidgetButton m_lect;
    grman::WidgetButton m_save;

    ///Bouton affichage graphe simplifi� des composantes fortement connexes
    grman::WidgetButton m_buttonAffSimp;

public :

    // Le constructeur met en place les �l�ments de l'interface
    // voir l'impl�mentation dans le .cpp
    GraphInterface(int x, int y, int w, int h);

    //getter pour les bouttons button
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

    grman::WidgetButton &get_buttonGr_simpl()
    {
        return m_buttonAffSimp;
    }

};


class Graph
{
private :

    /// La "liste" des ar�tes
    std::map<int, Edge> m_edges;

    /// La liste des sommets
    std::map<int, Vertex> m_vertices;

    /// le POINTEUR sur l'interface associ�e, nullptr -> pas d'interface
    std::shared_ptr<GraphInterface> m_interface = nullptr;

    ///les poubelles
    std::vector<Edge_bin> m_bin_edges;
    std::vector<Vertex_bin> m_bin_vertices;

    ///vecteur qui contient les sommet ancre des compo connexe
    std::vector<int> m_indice_sommet_ancre;

    ///vecteur qui contient les composantes fort. connexes
    std::vector<std::vector<Vertex>> m_vect_composantes;

    int m_nb_comp = 0;

    ///vecteur qui contient les sommets � afficher pour les diff�rentes composantes connexes
    std::vector<std::vector<Vertex>> m_connexe;

    public:

        /// Les constructeurs sont � compl�ter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Graph (GraphInterface *interface=nullptr) :
            m_interface(interface)  {  }

        void add_interfaced_vertex(int idx, int value, int x, int y, std::string pic_name, int pic_idx,int taux_repro);
        void add_interfaced_edge(int idx, int vert1, int vert2, int weight=0);

        /// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
        /// Voir impl�mentation dans le .cpp
        /// Cette m�thode est � enlever et remplacer par un syst�me
        /// de chargement de fichiers par exemple.
        void chargement_fichier_a();
        void chargement_fichier_b();
        void chargement_fichier_c();

        void sauv_graphea();
        void sauv_grapheb();
        void sauv_graphec();

        ///Ajouter et supprimer esp�ce
        void delete_espece();
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
        /// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
        void update();

        //getter pour m_interface
        std::shared_ptr<GraphInterface> &get_interface(){return m_interface;}
        std::map<int, Vertex> get_vertices(){return m_vertices;}
        std::map<int, Edge> get_edges(){return m_edges;}

        void remove_edge(int eidx);


        void affichage_outil();

        void acces_G1(int* n);
        void acces_G2(int* n);
        void acces_G3(int* n);

        void sortie();

        void graph_simpl();
        void graph_simpl_connex();

        int get_indice(Vertex V);

        void var_temps();

        std::vector<std::vector<int>> Coeff_binomial(const int k);

};


#endif // GRAPH_H_INCLUDED
