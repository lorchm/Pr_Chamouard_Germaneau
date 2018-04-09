/**
*\file graph.cpp
*\brief principales fonctions d'initialisations + mise a jour et l'affichage des graphes
*\author Barbara Germaneau + Laure Chamouard + Mr. Fercoq
*\version 3.0
*\date 2 avril 2018
*/

#include "graph.h"
#include <fstream>
#include <stack>
#include <queue>
#include <ctime>

using namespace std;

/***************************************************
                    VERTEX
****************************************************/

/**
* \fn VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
* \brief Cree l'interface d'un sommet
* \param idx : Indice du sommet - x : Position x - y : Position y - pic_name : nom de l'image - pic_idx : nb d'image ds l'animation
*/
/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );

}

/**
* \fn void pre_update()
* \brief copie des valeurs locales dans les slides des sommets
*/
/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}

/**
* \fn void post_update()
* \brief reprend la valeur de m_value a partir de l'interface
*/
/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}

/***************************************************
                    EDGE
****************************************************/

/**
* \fn EdgeInterface(Vertex& from, Vertex& to)
* \brief cree l'interface de l'arc
* \param Vertex& from : Sommet de départ - Vertex& to : Sommet d'arrivé
*/
/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}

/**
* \fn void pre_update()
* \brief met a jour les valeurs sur le slider de l'edge
*/
/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    ///Si y'a pas d'interface
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/**
* \fn post_update()
* \brief recupere la valeur du slider de l'edge
*/
/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}

///Getters
/**
* \fn int getFrom()
* \brief recupere l'indice du sommet de depart
* \return retourne l'indice du sommet de depart
*/
int Edge::getFrom()
{
    return m_from;
}

/**
* \fn int getTo()
* \brief recupere l'indice du sommet d'arrivee
* \return retourne l'indice du sommet d'arrivee
*/
int Edge::getTo()
{
    return m_to;
}

/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise en place du Graphe
/**
* \fn GraphInterface(int x, int y, int w, int h)
* \brief Cree l'interface graphique du graphe
* \param int x : position x du graphe - int y : position y du graphe - int w : largeur du graphe - int h : longueur du graphe
*/
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    //Bouton 1 de la tool box, pour le graphe 1
    m_tool_box.add_child(m_buttonG1);
    m_buttonG1.set_frame(5,5,74,74);
    m_buttonG1.set_bg_color(ROSE);
    m_buttonG1.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    //Bouton 2 de la tool box, pour le graphe 2
    m_tool_box.add_child(m_buttonG2);
    m_buttonG2.set_frame(5,84,74,75);
    m_buttonG2.set_bg_color(VERTCLAIR);

    //Bouton 3 de la tool box, pour le graphe 3
    m_tool_box.add_child(m_buttonG3);
    m_buttonG3.set_frame(5,164,74,74);
    m_buttonG3.set_bg_color(ORANGECLAIR);

    //Bouton ajout espèce de la tool box
    m_tool_box.add_child(m_buttonAdd);
    m_buttonAdd.set_frame(2,484,74,74);
    m_buttonAdd.set_bg_color(FUCHSIACLAIR);

    //Bouton enlever espece de la tool box
    m_tool_box.add_child(m_buttonDelete);
    m_buttonDelete.set_frame(5,564,74,74);
    m_buttonDelete.set_bg_color(VIOLETCLAIR);

    //Bouton exit de la tool box
    m_tool_box.add_child(m_buttonExit);
    m_buttonExit.set_frame(5,644,74,74);
    m_buttonExit.set_bg_color(BLANCROSE);

    //Bouton save de la tool box
    m_tool_box.add_child(m_save);
    m_save.set_frame(5,246,74,74);
    m_save.set_bg_color(BLANC);

    //Bouton read de la tool box
    m_tool_box.add_child(m_lect);
    m_lect.set_frame(5,326,74,74);
    m_lect.set_bg_color(BLEU);

    //Bouton cfc
    m_tool_box.add_child(m_cfc);
    m_cfc.set_frame(5,404,75,18);
    m_cfc.set_bg_color(BLANC);

    //bouton k connexite
    m_tool_box.add_child(m_k_connexite);
    m_k_connexite.set_frame(5,443,75,18);
    m_k_connexite.set_bg_color(ORANGECLAIR);

}

/*********************************************************
LECTURES ET SAUVEGARDES
**********************************************************/

/**
* \fn void chargement_fichier_a()
* \brief charge le fichier du graphe 1
*/
/* chargement_fichier_a : lecture du fichier a pour initialiser le graphe 1
Entrée : Fichier txt
Sortie : /
*/
void Graph::chargement_fichier_a()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    ifstream file("fich_graphea.txt", ios::in);

    int idx;
    int nb_sommets;
    int nb_sommets_supp;
    int var_nb_pop;
    int var_coordx;
    int var_coordy;
    std::string var_image;
    int var_repro;

    int indice;
    int nb_aretes;
    int nb_aretes_supp;
    int som1;
    int som2;
    float var_arc;

    if(file)
    {
        file >> nb_sommets;

        for(int i=0 ; i<nb_sommets ; i++)
        {
            file >> idx >>var_nb_pop >> var_coordx >> var_coordy >> var_image >> var_repro;
            add_interfaced_vertex(idx, var_nb_pop, var_coordx, var_coordy, var_image, 0, var_repro);

        }

        file >> nb_sommets_supp;

        for(int k=0; k<nb_sommets_supp ; k++)
        {
            file >> idx >> var_nb_pop >> var_coordx >> var_coordy >> var_image >> var_repro;
            Vertex_bin temp = Vertex_bin(idx,var_nb_pop, var_coordx, var_coordy, var_image,0, var_repro);
            m_bin_vertices.push_back(temp);
        }

        file >> nb_aretes;

        for(int j=0 ; j<nb_aretes ; j++)
        {
            file >> indice >> som1 >> som2 >> var_arc;
            add_interfaced_edge(indice, som1, som2, var_arc);
        }

        file >> nb_aretes_supp;

        for(int l=0 ; l<nb_aretes_supp ; l++)
        {
            file >>indice >> som1 >> som2 >> var_arc;
            Edge_bin e_temp = Edge_bin(indice, som1, som2, var_arc);
            m_bin_edges.push_back(e_temp);
        }

        file.close();
    }
    else
    {
        std::cerr << "Impossible d'ouvrir le fichier du graphique 1" << std::endl;
    }

}

/**
* \fn void chargement_fichier_b()
* \brief charge le fichier du graphe 2
*/
/* chargement_fichier_b : lecture du fichier b pour initialiser le graphe 2
Entrée : fichier txt
Sortie : /
*/
void Graph::chargement_fichier_b()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    ifstream file("fich_grapheb.txt", ios::in);

    int idx;
    int nb_sommets;
    int nb_sommets_supp;
    int var_nb_pop;
    int var_coordx;
    int var_coordy;
    std::string var_image;
    int var_repro;

    int indice;
    int nb_aretes;
    int nb_aretes_supp;
    int som1;
    int som2;
    float var_arc;

    if(file)
    {
        file >> nb_sommets;

        for(int i=0 ; i<nb_sommets ; i++)
        {
            file >> idx >> var_nb_pop >> var_coordx >> var_coordy >> var_image >> var_repro;
            add_interfaced_vertex(idx, var_nb_pop, var_coordx, var_coordy, var_image, 0, var_repro);

        }

        file >> nb_sommets_supp;

        for(int k=0; k<nb_sommets_supp ; k++)
        {
            file >> idx >>var_nb_pop >> var_coordx >> var_coordy >> var_image >> var_repro;
            Vertex_bin temp = Vertex_bin(idx,var_nb_pop, var_coordx, var_coordy, var_image,0, var_repro);
            m_bin_vertices.push_back(temp);
        }

        file >> nb_aretes;

        for(int j=0 ; j<nb_aretes ; j++)
        {
            file >> indice >> som1 >> som2 >> var_arc;
            add_interfaced_edge(indice, som1, som2, var_arc);
        }

        file >> nb_aretes_supp;

        for(int l=0 ; l<nb_aretes_supp ; l++)
        {
            file >> indice >> som1 >> som2 >> var_arc;
            Edge_bin e_temp = Edge_bin(indice, som1, som2, var_arc);
            m_bin_edges.push_back(e_temp);
        }

        file.close();
    }
    else
    {
        std::cerr << "Impossible d'ouvrir le fichier du graphique 2" << std::endl;
    }

}

/**
* \fn void chargement_fichier_c()
* \brief lecture du fichier c
*/
/* chargement_fichier_c : lecture du fichier c pour initialiser le graphe 3
Entrée : Lecture du fichier
Sortie : /
*/
void Graph::chargement_fichier_c()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    ifstream file("fich_graphec.txt", ios::in);

    int idx;
    int nb_sommets;
    int nb_sommets_supp;
    int var_nb_pop;
    int var_coordx;
    int var_coordy;
    std::string var_image;
    int var_repro;

    int indice;
    int nb_aretes;
    int nb_aretes_supp;
    int som1;
    int som2;
    float var_arc;

    if(file)
    {
        file >> nb_sommets;

        for(int i=0 ; i<nb_sommets ; i++)
        {
            file >> idx >> var_nb_pop >> var_coordx >> var_coordy >> var_image >> var_repro;
            add_interfaced_vertex(idx, var_nb_pop, var_coordx, var_coordy, var_image,0,var_repro);

        }

        file >> nb_sommets_supp;

        for(int k=0; k<nb_sommets_supp ; k++)
        {
            file >> idx >> var_nb_pop >> var_coordx >> var_coordy >> var_image >> var_repro;
            Vertex_bin temp = Vertex_bin(idx,var_nb_pop, var_coordx, var_coordy, var_image,0, var_repro);
            m_bin_vertices.push_back(temp);
        }

        file >> nb_aretes;

        for(int j=0 ; j<nb_aretes ; j++)
        {
            file >> indice >> som1 >> som2 >> var_arc;
            add_interfaced_edge(indice, som1, som2, var_arc);
        }

        file >> nb_aretes_supp;

        for(int l=0 ; l<nb_aretes_supp ; l++)
        {
            file >> indice >> som1 >> som2 >> var_arc;
            Edge_bin e_temp = Edge_bin(indice, som1, som2, var_arc);
            m_bin_edges.push_back(e_temp);
        }

        file.close();
    }
    else
    {
        std::cerr << "Impossible d'ouvrir le fichier du graphique 1" << std::endl;
    }

}

/**
* \fn void sauv_graphea()
* \brief sauvegarde le fichier du graphe 1
*/
/* sauv_graphea : sauvegarde du graphe 1
Entrée : /
Sortie : Ecriture dans le fichier
*/
void Graph::sauv_graphea()
{

    ofstream file("fich_graphea.txt", ios::out | ios::trunc);

    if(file)
    {
        ///Ecrire le nombre de vertices
        file << m_vertices.size() << std::endl << std::endl;

        ///Ecrire la valeur et positions des sommets
        for (auto &elt : m_vertices)
        {
            file << elt.first << " " << elt.second.m_value << " " << elt.second.m_interface->m_top_box.get_frame().pos.x << " " << elt.second.m_interface->m_top_box.get_frame().pos.y << " " << elt.second.m_interface->m_img.get_pic_name()<< " " << elt.second.m_taux_repro <<std::endl;
        }

        ///Nombre vertices dans la bin
        file << std::endl << m_bin_vertices.size() << std::endl << std::endl;

        ///Ecrire la valeur et positions des sommets
        for (unsigned int i = 0; i< m_bin_vertices.size() ; i++)
        {
            file << m_bin_vertices[i].m_indice << " " <<  m_bin_vertices[i].m_value << " " << m_bin_vertices[i].m_x << " " << m_bin_vertices[i].m_y << " " << m_bin_vertices[i].m_pic_name << " " << m_bin_vertices[i].m_taux_repro << std::endl;
        }

        ///Ecrire le nombre de edges
        file << std::endl << m_edges.size() << std::endl << std::endl ;

        ///Ecrire les sommets et le poids de l'arc
        for (auto &it : m_edges)
        {
            file << it.first << " " <<it.second.m_from << " " << it.second.m_to << " " << it.second.m_weight << std::endl;
        }

        ///Ecrire le nombre de edges dans la bin
        file << std::endl << m_bin_edges.size() << std::endl << std::endl ;

        ///Ecrire les sommets et le poids de l'arc
        for (unsigned int i = 0 ; i<m_bin_edges.size() ; i++)
        {
            file << m_bin_edges[i].m_indice  << " " << m_bin_edges[i].m_vert1 << " " << m_bin_edges[i].m_vert2  << " " << m_bin_edges[i].m_weight << std::endl;
        }


        file.close();

    }
    else
    {
        std::cerr << "Sauvegarde impossible" << std::endl;
    }

}

/**
* \fn void sauv_grapheb()
* \brief sauvegarde le graphe 2
*/
/* sauv_grapheb : sauvegarde le graphe 2
Entrée : /
Sortie : Ecriture dans le fichier
*/
void Graph::sauv_grapheb()
{
    ofstream file("fich_grapheb.txt", ios::out | ios::trunc);

    if(file)
    {

        ///Ecrire le nombre de vertices
        file << m_vertices.size() << std::endl << std::endl;

        ///Ecrire la valeur et positions des sommets
        for (auto &elt : m_vertices)
        {
            file << elt.first << " " << elt.second.m_value << " " << elt.second.m_interface->m_top_box.get_frame().pos.x << " " << elt.second.m_interface->m_top_box.get_frame().pos.y << " " << elt.second.m_interface->m_img.get_pic_name() << " " << elt.second.m_taux_repro << std::endl;
        }

        ///Nombre vertices dans la bin
        file << m_bin_vertices.size() << std::endl << std::endl;

        ///Ecrire la valeur et positions des sommets
        for (unsigned int i = 0 ; i<m_bin_vertices.size() ; i++)
        {
            file << m_bin_vertices[i].m_indice << " " << m_bin_vertices[i].m_value << " " << m_bin_vertices[i].m_x << " " << m_bin_vertices[i].m_y << " " << m_bin_vertices[i].m_pic_name << " " << m_bin_vertices[i].m_taux_repro << " " << std::endl;
        }

        ///Ecrire le nombre de edges
        file << std::endl << m_edges.size() << std::endl << std::endl ;

        ///Ecrire les sommets et le poids de l'arc
        for (auto &it : m_edges)
        {
            file << it.first << " " <<  it.second.m_from << " " << it.second.m_to << " " << it.second.m_weight << std::endl;
        }

        ///Ecrire le nombre de edges dans la bin
        file << std::endl << m_bin_edges.size() << std::endl << std::endl ;

        ///Ecrire les sommets et le poids de l'arc poubelle
        for (unsigned int i = 0 ; i<m_bin_edges.size() ; i++ )
        {
            file << m_bin_edges[i].m_indice << " " << m_bin_edges[i].m_vert1 << " " << m_bin_edges[i].m_vert2 << " " << m_bin_edges[i].m_weight << std::endl;
        }


        file.close();

    }
    else
    {
        std::cerr << "Sauvegarde impossible" << std::endl;
    }

}

/**
* \fn sauv_graphec()
* \brief sauvegarde le graphe 3
*/
/* sauv_graphec : sauvegarde le graphe 3
Entrée : /
Sortie : Modification du fichier
*/
void Graph::sauv_graphec()
{

    ofstream file("fich_graphec.txt", ios::out | ios::trunc);

    if(file)
    {
        ///Ecrire le nombre de vertices
        file << m_vertices.size() << std::endl << std::endl;

        ///Ecrire la valeur et positions des sommets
        for (auto &elt : m_vertices)
        {
            file << elt.first << " " <<elt.second.m_value << " " << elt.second.m_interface->m_top_box.get_frame().pos.x << " " << elt.second.m_interface->m_top_box.get_frame().pos.y << " " << elt.second.m_interface->m_img.get_pic_name() << " " << elt.second.m_taux_repro << std::endl;
        }

        ///Nombre vertices dans la bin
        file << m_bin_vertices.size() << std::endl << std::endl;

        ///Ecrire la valeur et positions des sommets
        for (unsigned int i = 0 ; i<m_bin_vertices.size() ; i++)
        {
            file << m_bin_vertices[i].m_indice << " " << m_bin_vertices[i].m_value << " " << m_bin_vertices[i].m_x << " " << m_bin_vertices[i].m_y << " " << m_bin_vertices[i].m_pic_name << " " << m_bin_vertices[i].m_taux_repro << std::endl;
        }

        ///Ecrire le nombre de edges
        file << std::endl << m_edges.size() << std::endl << std::endl ;

        ///Ecrire les sommets et le poids de l'arc
        for (auto &it : m_edges)
        {
            file << it.first << " " << it.second.m_from << " " << it.second.m_to << " " << it.second.m_weight << std::endl;
        }

        ///Ecrire le nombre de edges dans la bin
        file << std::endl << m_bin_edges.size() << std::endl << std::endl ;


        ///Ecrire les sommets et le poids de l'arc
        for (unsigned int i = 0 ; i < m_bin_edges.size() ; i++)
        {
            file << m_bin_edges[i].m_indice << " " << m_bin_edges[i].m_vert1 << " " << m_bin_edges[i].m_vert2 << " " << m_bin_edges[i].m_weight << std::endl;
        }

        file.close();

    }
    else
    {
        std::cerr << "Sauvegarde impossible" << std::endl;
    }

}

/********************************************************
MISES A JOUR
********************************************************/

/**
* \fn void update()
* \brief appelle les sous-programmes d'updates
*/
/* update : appelle les fonctions de update pour les vertex/edge/interfaces
Entrée : /
Sortie : /
*/
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

}

/**
* \fn void var_temps()
* \brief fait varier les parametres K et N en fonction du temps
*/
/* var_temps : Fait varier K et N en fonction du temps
Entrée : /
Sortie : /
*/
void Graph::var_temps()
{
    ///On modifie tous les K
    for(auto &elem : m_vertices)
    {
        for(unsigned int i = 0 ; i < elem.second.m_in.size() ; i++ )
        {
            elem.second.m_K = elem.second.m_K + ( m_edges[elem.second.m_in[i]].m_weight * m_vertices[m_edges[elem.second.m_in[i]].m_from].m_value );

            //std::cout << elem.second.m_K << std::endl;
        }

        std::cout << elem.first << ". K = " <<  elem.second.m_K << std::endl;
    }


    ///On modifie tous les N
    for(auto &elem : m_vertices)
    {
        elem.second.m_value = elem.second.m_value + (elem.second.m_taux_repro*elem.second.m_value*(1 - (elem.second.m_value/elem.second.m_K)));

        for(unsigned int i = 0 ; i < elem.second.m_out.size() ; i++)
        {
            elem.second.m_value = elem.second.m_value - ( m_edges[elem.second.m_out[i]].m_weight * m_vertices[m_edges[elem.second.m_out[i]].m_to].m_value );
        }
        std::cout << elem.first << ". N = " <<  elem.second.m_value << std::endl;
    }
}

/*****************************************************
INITIALISATIONS / CONSTRUCTEURS
******************************************************/

/**
* \fn void add_interfaced_vertex(int idx, int value, int x, int y, std::string pic_name, int pic_idx, int taux_repro)
* \brief ajoute un sommet et son interface pour l'afficher
* \param int idx : indice du sommet - int value : nb population - int x : position x - int y : position y - std::string pic_name : nom de l'image
* int pic_idx : nombre d'images dans l'animation - int taux_repro : taux de reproduction
*/
/*add_interface_vertex : ajout d'un sommet et son interface
Entrée : int idx (indice du sommet), int value (nbr population), int x(pos x), int y(pos y), string pic_name (nom de l'image), int pic_idx(si image en mvmt), int taux_repro (facteur de reproduction)
Sortie : /
*/
void Graph::add_interfaced_vertex(int idx, int value, int x, int y, std::string pic_name, int pic_idx, int taux_repro)
{
    /*parcours les indices de sommet de la map pour voir si le sommet a pas déjà été crée, si c le cas y un message d'erreur*/
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi, taux_repro);
}

/**
* \fn void  add_interfaced_edge()
* \brief ajoute un arc et son interface
* \param int idx : indice - int id_vert1 : indice sommet depart - int id_vert2 : indice sommet arrivee - int weight : poids de l'arete
*/
/* add_interfaced_edge : ajout d'un arc avec une interface
Entrée : int idx (son indice), int id_vert1 (indice sommet from), int id_vert2 (indice sommet to), int weight (poids de l'arête)
Sortie : /
*/
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, int weight)
{
    //Si l'edge existe déjà
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    //Si on ne trouve pas un des sommets de l'arête
    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    //Création d'une interface et de l'edge
    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);
    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;

    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}

/*****************************************************
AFFICHAGE TOUR DE BOUCLE
******************************************************/

/**
* \fn void affichage_outil()
* \brief Affiche les images de la boite a outils
*/
/* affichage_outil : Affichage des images de la tool bar
Entrée : Images dans le dossier du projet
Sortie : Afichage allegro
*/
void Graph::affichage_outil()
{
    //affichage/chargement des images pour la tool bar
    grman::show_picture(grman::page, "G1.bmp", 24,6,0);
    grman::show_picture(grman::page, "G2.bmp", 24,86,0);
    grman::show_picture(grman::page, "G3.bmp", 24,166,0);
    grman::show_picture(grman::page,"save.bmp",24,246,0);
    grman::show_picture(grman::page,"read.bmp",24,326,0);
    grman::show_picture(grman::page, "add.bmp", 24,486,0);
    grman::show_picture(grman::page, "delete.bmp", 24,566,0);
    grman::show_picture(grman::page, "logout.bmp", 24,646,0);

    textprintf_ex(grman::page,font,32,414, NOIR,-1,"CFC");
    grman::show_picture(grman::page, "show_cfc.bmp", 24,428,0);
    textprintf_ex(grman::page,font,46,432, NOIR,-1,"Press P");

    textprintf_ex(grman::page,font,32,453, NOIR,-1,"K-CONNEX");
    grman::show_picture(grman::page, "show_kconnex.bmp", 24,467,0);
    textprintf_ex(grman::page,font,46,473, NOIR,-1,"Press B");

}

/**
* \fn void acces_G1(int* n)
* \brief affichage et actions sur le graphe 1
* \param int* n : pointeur sur le numero de graphe
*/
/* acces_G1 : Affichage et action sur le graphe 1
Entrée : /
Sortie : Affichage allegro + console
*/
void Graph::acces_G1(int* n)
{
    //Si clique sur le bouton de lecture -> lecture
    get_interface()->get_lect().interact_focus();
    if(get_interface()->get_lect().clicked())
    {
        std::cout << "Lecture du fichier 1" << std::endl;
        m_edges.clear();
        m_vertices.clear();
        m_bin_edges.clear();
        m_bin_vertices.clear();
        chargement_fichier_a();
    }

    //Si clique sur le bouton de sauvegarde -> sauvegarde
    get_interface()->get_save().interact_focus();
    if(get_interface()->get_save().clicked())
    {
        std::cout << "Sauvegarde du fichier 1" << std::endl;
        sauv_graphea();
    }

    //Méthodes d'update des objets qui comportent des widgets
    update();

    //Affichage barre à outils
    affichage_outil();

    //Recherche de composantes fortement connexes
    get_interface()->get_cfc().interact_focus();
    if(get_interface()->get_cfc().clicked())
    {
        rest(100);
        std::cout << "Recherche de composantes fortement connexes" << std::endl;
        Marquer_composantes();
    }

    //Recherche de sommet(s) à enlever pour déconnecter le graphe
    get_interface()->get_kconnex().interact_focus();
    if(get_interface()->get_kconnex().clicked())
    {
        rest(100);
        k_connexite();

    }

    //Mise à jour générale (clavier/souris/buffer etc...)
    grman::mettre_a_jour();

    //Ajout d'espèce sur le graphe 1
    add_espece1();

    //Suppression espèce
    delete_espece();

    //Test sortie du programme
    sortie();

    //Quand on appuie sur le bouton G3 on passe au graphe 3
    get_interface()->get_buttonG3().interact_focus();
    if(get_interface()->get_buttonG3().clicked())
    {
        *n=3;
    }

    //Quand on appuie sur le bouton G2 on passe au graphe 2
    get_interface()->get_buttonG2().interact_focus();
    if(get_interface()->get_buttonG2().clicked())
    {
        *n=2;
    }

    //Affiche du graphe simplifé des composantes fortement connexes
    if(key[KEY_P])
    {
        rest(100);
        if(m_vect_composantes.size() > 0 )
        {
            graph_simpl();
            m_vect_composantes.clear();
        }
    }



    //Affichage du graphe simplifé des différentes k_connexités
    if(key[KEY_B])
    {
        rest(100);
        if(m_connexe.size() > 0 )
        {
            graph_simpl_connex();
            m_connexe.clear();
        }
    }



    //Modifications K et N
    //var_temps();
}

/**
* \fn void acces_G2(int* n)
* \brief affichage et actions sur le graphe 2
* \param int* n : pointeur sur le numero du graphe
*/
/* acces_G2 : Actions et affichage du graphe 2
Entrée : /
Sortie : Affichage allegro et console
*/
void Graph::acces_G2(int* n)
{
    //Si on clique sur lecture -> lecture du fichier
    get_interface()->get_lect().interact_focus();
    if(get_interface()->get_lect().clicked())
    {
        std::cout << "Lecture du fichier 2" << std::endl;
        m_edges.clear();
        m_vertices.clear();
        chargement_fichier_b();
    }

    //Si clique sur sauvegarde -> on sauvegarde
    get_interface()->get_save().interact_focus();
    if(get_interface()->get_save().clicked())
    {
        std::cout << "Sauvegarde du fichier 2" << std::endl;
        sauv_grapheb();
    }

    //Méthodes d'update des objets qui comportent des widgets
    update();

    //Affichage barre à outils
    affichage_outil();

    //Mise à jour générale (clavier/souris/buffer etc...)
    grman::mettre_a_jour();

    //Ajouter une espèce sur le graphe 2
    add_espece2();

    //Suppression d'une espèce
    delete_espece();

    //Test sortie du programme
    sortie();

    //Recherche de composantes fortement connexes
    get_interface()->get_cfc().interact_focus();
    if(get_interface()->get_cfc().clicked())
    {
        rest(100);
        std::cout << "Recherche de composantes fortement connexes" << std::endl;
        Marquer_composantes();
    }

    //Recherche de sommet(s) à enlever pour déconnecter le graphe
    get_interface()->get_kconnex().interact_focus();
    if(get_interface()->get_kconnex().clicked())
    {
        rest(100);
        k_connexite();

    }

    //Si on clique sur P, affiche graphe simplifié des fortes connexités
    if(key[KEY_P])
    {
        rest(100);
        if(m_vect_composantes.size() > 0 )
        {
            graph_simpl();
            m_vect_composantes.clear();
        }
    }

    //Si on clique sur B, affiche graphe simplifé des k-connexités
    if(key[KEY_B])
    {
        rest(100);
        if(m_connexe.size() > 0 )
        {
            graph_simpl_connex();
            m_connexe.clear();
        }
    }

    //Si on clique sur G1 on passe au graphe 1
    get_interface()->get_buttonG1().interact_focus();
    if(get_interface()->get_buttonG1().clicked())
    {
        *n=1;
    }

    //Si on clique sur G3 on passe au graphe 3
    get_interface()->get_buttonG3().interact_focus();
    if(get_interface()->get_buttonG3().clicked())
    {
        *n=3;
    }

    //Met à jour K et N
    //var_temps();

}

/**
* \fn void acces_G3(int* n)
* \brief affichage et actions sur le graphe 3
* \param int* n : pointeur sur le numero du graphe
*/
/* acces_G3 : Effectue toutes les actions sur le graphe 3
Entrée : /
Sortie : Affichage allegro et console
*/
void Graph::acces_G3(int* n)
{
    //Si on clique sur read -> Lecture du fichier
    get_interface()->get_lect().interact_focus();
    if(get_interface()->get_lect().clicked())
    {
        std::cout << "Lecture du fichier 3" << std::endl;
        m_edges.clear();
        m_vertices.clear();
        chargement_fichier_c();
    }

    //Si on clique sur sauvegarde -> sauvegarde
    get_interface()->get_save().interact_focus();
    if(get_interface()->get_save().clicked())
    {
        std::cout << "Sauvegarde du fichier 3" << std::endl;
        sauv_graphec();
    }

    //Méthodes d'update des objets qui comportent des widgets
    update();

    //Affichage barre à outils
    affichage_outil();

    //Recherche de composantes fortement connexes
    get_interface()->get_cfc().interact_focus();
    if(get_interface()->get_cfc().clicked())
    {
        rest(100);
        std::cout << "Recherche de composantes fortement connexes" << std::endl;
        Marquer_composantes();
    }

    //Recherche de sommet(s) à enlever pour déconnecter le graphe
    get_interface()->get_kconnex().interact_focus();
    if(get_interface()->get_kconnex().clicked())
    {
        rest(100);
        k_connexite();

    }

    // Mise à jour générale (clavier/souris/buffer etc...)
    grman::mettre_a_jour();

    //Ajout d'une espèce sur le graphe 3
    add_espece3();

    //Suppression d'une espèce
    delete_espece();

    //Sort du programme si on clique sur le bouton exit
    sortie();




    //Affichage du graphe avec ses composantes fortement connexes
    if(key[KEY_P])
    {
        rest(100);
        if(m_vect_composantes.size() > 0 )
        {
            graph_simpl();
            m_vect_composantes.clear();
        }
    }

    //Si on appuie sur B
    if(key[KEY_B])
    {
        rest(100);
        if(m_connexe.size() > 0 )
        {
            //On affiche les graphes qui montre les sommets qui deconnectent le graphe
            graph_simpl_connex();
            //On vide le vector
            m_connexe.clear();
        }
    }

    //Si on clique sur G1 on affiche le graphe 1
    get_interface()->get_buttonG1().interact_focus();
    if(get_interface()->get_buttonG1().clicked())
    {
        *n=1;
    }

    //Si on clique sur G2 on affiche le graphe 2
    get_interface()->get_buttonG2().interact_focus();
    if(get_interface()->get_buttonG2().clicked())
    {
        *n=2;
    }

    //Changement des variables N et K en fonction du temps
    //var_temps();

}

/**
* \fn void sortie()
* \brief quitte le programme quand on clique sur le bouton sortie
*/
/* sortie : sortie du programme
Entrée : /
Sortie : /
*/
void Graph::sortie()
{
    //On voit si y a un clique sur le bouton Exit de la barre outils
    get_interface()->get_buttonExit().interact_focus();

    //Si le bouton a été cliqué
    if(get_interface()->get_buttonExit().clicked())
    {
        std::cout << "A bientot" << std::endl;
        //On sort du programme
        exit(0);
    }
}

/**
* \fn void graph_simpl_connex()
* \brief affiche les graphes simplifies de tous les k_sommets qui deconnectent le graphe
*/
/*graph_simpl_connex = affiche graphe simplicié avec les différentes composantes connexes (k_connexité)
Entrée : /
Sortie : Affichage allegro
*/
void Graph::graph_simpl_connex()
{
    ///Initialisation du buffer
    BITMAP* buffer = create_bitmap(908,720);

    std::cout << "aff graphe simplifi des k_connexités" << std::endl;

    ///Faire les aretes
    //Pour chaque connexité
    for(unsigned int i = 0 ; i< m_connexe.size() ; i++)
    {
        ///Nettoyage du buffer
        clear_bitmap(buffer);

        ///Fond du buffer
        rectfill(buffer,0,0,908,720,BLANCROSE);

        //Pour chaque sommet de la connexité
        for(unsigned int j = 0 ; j < m_connexe[i].size() ; j++)
        {
            //Pour chaques arêtes
            for(auto &elem : m_edges)
            {
                //Si l'indice du sommet actuel correspond à l'indice m_from de l'arête
                if(get_indice(m_connexe[i][j]) == elem.second.m_from)
                {
                    //pour chaque sommet de la connexité
                    for(unsigned int k = 0 ; k < m_connexe[i].size() ; k++)
                    {
                        //Si un indice correspond à l'indice m_to de l'arête
                        if(get_indice(m_connexe[i][k]) == elem.second.m_to)
                        {
                            //Affichage de l'arete
                            line(buffer, m_connexe[i][j].m_interface->m_top_box.get_frame().pos.x+50,m_connexe[i][j].m_interface->m_top_box.get_frame().pos.y+50, m_connexe[i][k].m_interface->m_top_box.get_frame().pos.x+50,m_connexe[i][k].m_interface->m_top_box.get_frame().pos.y+50, NOIR);
                            ///"affichage de la fleche" -> c'est des ronds parce que c'est trop long sinon
                            //si sA gauche-haut de sB
                            if(m_connexe[i][j].m_interface->m_top_box.get_frame().pos.x < m_connexe[i][k].m_interface->m_top_box.get_frame().pos.x && m_connexe[i][j].m_interface->m_top_box.get_frame().pos.y < m_connexe[i][k].m_interface->m_top_box.get_frame().pos.y )
                            {
                                circlefill(buffer, m_connexe[i][k].m_interface->m_top_box.get_frame().pos.x+50, m_connexe[i][k].m_interface->m_top_box.get_frame().pos.y+50, 10, NOIR);
                            }
                            //si sA gauche-bas de sB
                            if(m_connexe[i][j].m_interface->m_top_box.get_frame().pos.x < m_connexe[i][k].m_interface->m_top_box.get_frame().pos.x && m_connexe[i][j].m_interface->m_top_box.get_frame().pos.y > m_connexe[i][k].m_interface->m_top_box.get_frame().pos.y)
                            {
                                circlefill(buffer, m_connexe[i][k].m_interface->m_top_box.get_frame().pos.x+50, m_connexe[i][k].m_interface->m_top_box.get_frame().pos.y+50, 10, NOIR);
                            }
                            //si sA droit-bas de sB
                            if(m_connexe[i][j].m_interface->m_top_box.get_frame().pos.x > m_connexe[i][k].m_interface->m_top_box.get_frame().pos.x && m_connexe[i][j].m_interface->m_top_box.get_frame().pos.y > m_connexe[i][k].m_interface->m_top_box.get_frame().pos.y)
                            {
                                circlefill(buffer, m_connexe[i][k].m_interface->m_top_box.get_frame().pos.x+50, m_connexe[i][k].m_interface->m_top_box.get_frame().pos.y+50, 10, NOIR);
                            }
                            //si sA droit-haut de sB
                            if(m_connexe[i][j].m_interface->m_top_box.get_frame().pos.x > m_connexe[i][k].m_interface->m_top_box.get_frame().pos.x && m_connexe[i][j].m_interface->m_top_box.get_frame().pos.y < m_connexe[i][k].m_interface->m_top_box.get_frame().pos.y )
                            {
                                circlefill(buffer, m_connexe[i][k].m_interface->m_top_box.get_frame().pos.x+50, m_connexe[i][k].m_interface->m_top_box.get_frame().pos.y+50, 10, NOIR);
                            }
                        }
                    }
                }
                //Si l'indice du sommet actuel est l'indice m_to de l'arete
                else if(get_indice(m_connexe[i][j]) == elem.second.m_to)
                {
                    //Pour chaque sommet
                    for(unsigned int k = 0 ; k < m_connexe[i].size() ; k++)
                    {
                        if(get_indice(m_connexe[i][k]) == elem.second.m_from)
                        {
                            //Affichage de l'arete
                            line(buffer, m_connexe[i][k].m_interface->m_top_box.get_frame().pos.x+50,m_connexe[i][k].m_interface->m_top_box.get_frame().pos.y+50,m_connexe[i][j].m_interface->m_top_box.get_frame().pos.x+50,m_connexe[i][j].m_interface->m_top_box.get_frame().pos.y+50, NOIR);
                            ///Affichage de la flèche (simulé par un rond)
                            //si sA gauche-haut de sB
                            if(m_connexe[i][j].m_interface->m_top_box.get_frame().pos.x < m_connexe[i][k].m_interface->m_top_box.get_frame().pos.x && m_connexe[i][j].m_interface->m_top_box.get_frame().pos.y < m_connexe[i][k].m_interface->m_top_box.get_frame().pos.y )
                            {
                                circlefill(buffer, m_connexe[i][j].m_interface->m_top_box.get_frame().pos.x+58, m_connexe[i][j].m_interface->m_top_box.get_frame().pos.y+65, 10, NOIR);
                            }
                            //si sA gauche-bas de sB
                            if(m_connexe[i][j].m_interface->m_top_box.get_frame().pos.x < m_connexe[i][k].m_interface->m_top_box.get_frame().pos.x && m_connexe[i][j].m_interface->m_top_box.get_frame().pos.y > m_connexe[i][k].m_interface->m_top_box.get_frame().pos.y)
                            {
                                circlefill(buffer, m_connexe[i][j].m_interface->m_top_box.get_frame().pos.x+55, m_connexe[i][j].m_interface->m_top_box.get_frame().pos.y+45, 10, NOIR);
                            }
                            //si sA droite-bas de sB
                            if(m_connexe[i][j].m_interface->m_top_box.get_frame().pos.x > m_connexe[i][k].m_interface->m_top_box.get_frame().pos.x && m_connexe[i][j].m_interface->m_top_box.get_frame().pos.y > m_connexe[i][k].m_interface->m_top_box.get_frame().pos.y)
                            {
                                circlefill(buffer, m_connexe[i][j].m_interface->m_top_box.get_frame().pos.x+40, m_connexe[i][j].m_interface->m_top_box.get_frame().pos.y+45, 10, NOIR);
                            }
                            //si sA droit-haut de sB
                            if(m_connexe[i][j].m_interface->m_top_box.get_frame().pos.x > m_connexe[i][k].m_interface->m_top_box.get_frame().pos.x && m_connexe[i][j].m_interface->m_top_box.get_frame().pos.y < m_connexe[i][k].m_interface->m_top_box.get_frame().pos.y )
                            {
                                circlefill(buffer, m_connexe[i][j].m_interface->m_top_box.get_frame().pos.x+43, m_connexe[i][j].m_interface->m_top_box.get_frame().pos.y+55, 10, NOIR);
                            }
                        }
                    }
                }
            }
        }
        ///Affichage des sommets de fortes connexités
        // Pour tous les sommets
        for(unsigned int m = 0 ; m < m_connexe[i].size() ; m++)
        {
            //Affichage du rectangle du sommet
            rectfill(buffer,m_connexe[i][m].m_interface->m_top_box.get_frame().pos.x+40, m_connexe[i][m].m_interface->m_top_box.get_frame().pos.y+40,m_connexe[i][m].m_interface->m_top_box.get_frame().pos.x+70,m_connexe[i][m].m_interface->m_top_box.get_frame().pos.y+70, BLANC );
            rect(buffer,m_connexe[i][m].m_interface->m_top_box.get_frame().pos.x+40, m_connexe[i][m].m_interface->m_top_box.get_frame().pos.y+40,m_connexe[i][m].m_interface->m_top_box.get_frame().pos.x+70,m_connexe[i][m].m_interface->m_top_box.get_frame().pos.y+70, NOIR);

            //Affichage du numéro du sommet
            textprintf_ex(buffer, font, m_connexe[i][m].m_interface->m_top_box.get_frame().pos.x+50, m_connexe[i][m].m_interface->m_top_box.get_frame().pos.y+50, NOIR, -1, "%d", get_indice(m_connexe[i][m]));
        }

        ///Affichage du buffer
        blit(buffer,screen,0,0, 110, 5,908,720);
        rest(1000);
    }
}

/**
* \fn void graph_simpl()
* \brief affiche le graphe simplifie des composantes fortement connexes
*/
/*graph_simpl = affiche graphe simplicié avec les différentes composantes fortement connexes
Entrée : /
Sortie : Affichage allegro
*/
void Graph::graph_simpl()
{

    std::cout << "aff graphe simplifi" << std::endl;

    //Initialisation du buffer
    BITMAP* buffer = create_bitmap(908,720);
    //Remplissage fond buffer
    rectfill(buffer,0,0,908,720, CYANSOMBRE);

    //Pour toutes les composantes fortement connexes
    for(unsigned int i = 0 ; i< m_vect_composantes.size() ; i++)
    {
        //Pour tous les sommets de la composante i fortement connexe
        for(unsigned int j = 0 ; j < m_vect_composantes[i].size() ; j++)
        {
            //Pour toutes les arêtes
            for(auto &elem : m_edges)
            {
                //Si l'indice du sommet actuel correspond au m_from de l'arete actuelle
                if(get_indice(m_vect_composantes[i][j]) == elem.second.m_from)
                {
                    //Pour tous les sommets de la composante i fortement connexe
                    for(unsigned int k = 0 ; k < m_vect_composantes[i].size() ; k++)
                    {
                        //Si l'indice du sommet correspond au m_to de l'arete actuelle
                        if(get_indice(m_vect_composantes[i][k]) == elem.second.m_to)
                        {
                            //On dessine l'arete
                            line(buffer, m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.x+50,m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.y+50, m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.x+50,m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.y+50, NOIR);
                            ///Affichage de la fleche (cercle pour simplifié le code)
                            //si sA gauche-haut de sB
                            if(m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.x < m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.x && m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.y < m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.y )
                            {
                                circlefill(buffer, m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.x+50, m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.y+50, 10, NOIR);
                            }
                            //si sA gauche-bas de sB
                            if(m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.x < m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.x && m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.y > m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.y)
                            {
                                circlefill(buffer, m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.x+50, m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.y+50, 10, NOIR);
                            }
                            //si sA droite-bas de sB
                            if(m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.x > m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.x && m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.y > m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.y)
                            {
                                circlefill(buffer, m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.x+50, m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.y+50, 10, NOIR);
                            }
                            //si sA droit-haut de sB
                            if(m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.x > m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.x && m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.y < m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.y )
                            {
                                circlefill(buffer, m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.x+50, m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.y+50, 10, NOIR);
                            }
                        }
                    }
                }
                //Si l'indice du sommet actuel correspond au m_to de l'arete
                else if(get_indice(m_vect_composantes[i][j]) == elem.second.m_to)
                {
                    //Pour tous les sommets de la composante fortement connexe
                    for(unsigned int k = 0 ; k < m_vect_composantes[i].size() ; k++)
                    {
                        //Si l'indice du sommet correspond au m_from de l'arete
                        if(get_indice(m_vect_composantes[i][k]) == elem.second.m_from)
                        {
                            //Dessine l'arête
                            line(buffer, m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.x+50,m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.y+50,m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.x+50,m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.y+50, NOIR);
                            ///Affichage de la fleche
                            //si sA gauche-haut de sB
                            if(m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.x < m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.x && m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.y < m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.y )
                            {
                                circlefill(buffer, m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.x+58, m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.y+65, 10, NOIR);
                            }
                            //si sA gauche-bas de sB
                            if(m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.x < m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.x && m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.y > m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.y)
                            {
                                circlefill(buffer, m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.x+55, m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.y+45, 10, NOIR);
                            }
                            //si sA droite-bas de sB
                            if(m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.x > m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.x && m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.y > m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.y)
                            {
                                circlefill(buffer, m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.x+40, m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.y+45, 10, NOIR);
                            }
                            //si sA droit-haut de sB
                            if(m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.x > m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.x && m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.y < m_vect_composantes[i][k].m_interface->m_top_box.get_frame().pos.y )
                            {
                                circlefill(buffer, m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.x+43, m_vect_composantes[i][j].m_interface->m_top_box.get_frame().pos.y+55, 10, NOIR);
                            }
                        }
                    }
                }
            }
        }
    }

    ///Affichage des sommets de fortes connexités
    //Pour toutes les composantes fortement connexes
    for(unsigned int k = 0 ; k < m_vect_composantes.size() ; k++)
    {
        //Pour tous les sommets de la composante i
        for(unsigned int m = 0 ; m < m_vect_composantes[k].size() ; m++)
        {
            //Affichage du cadre du sommet
            rectfill(buffer,m_vect_composantes[k][m].m_interface->m_top_box.get_frame().pos.x+40, m_vect_composantes[k][m].m_interface->m_top_box.get_frame().pos.y+40,m_vect_composantes[k][m].m_interface->m_top_box.get_frame().pos.x+70,m_vect_composantes[k][m].m_interface->m_top_box.get_frame().pos.y+70, BLANC );
            rect(buffer,m_vect_composantes[k][m].m_interface->m_top_box.get_frame().pos.x+40, m_vect_composantes[k][m].m_interface->m_top_box.get_frame().pos.y+40,m_vect_composantes[k][m].m_interface->m_top_box.get_frame().pos.x+70,m_vect_composantes[k][m].m_interface->m_top_box.get_frame().pos.y+70, NOIR);
            //Affichage du numero du sommet
            textprintf_ex(buffer, font, m_vect_composantes[k][m].m_interface->m_top_box.get_frame().pos.x+50, m_vect_composantes[k][m].m_interface->m_top_box.get_frame().pos.y+50, NOIR, -1, "%d", get_indice(m_vect_composantes[k][m]));
        }
    }

    ///Affichage du buffer
    blit(buffer,screen,0,0, 110, 5,908,720);
    rest(5000);
}

/**
* \fn void afficher()
* \brief affiche les indices des sommets appartement a une composante fortement connexe
*/
/* afficher : afficher les indices des sommets appartenant à une composante fortement connexe
Entrée : /
Sortie : Affichage console
*/
void Graph::afficher()
{
    std::cout << "Voici les sommets de la composante fortement connexe : ";
    //Pour tous les sommets
    for ( auto &elt : m_vertices )
    {
        //S'il est marqué deux fois et qu'il est marqué présent dans la compo
        if ( elt.second.m_1 == true && elt.second.m_2 == true && elt.second.m_present_ds_compo == true )
        {
            //Affiche son indice
            std::cout << elt.first << " ";
        }
    }
    std::cout << std::endl;
}
