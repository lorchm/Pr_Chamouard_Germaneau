#include "graph.h"
#include <fstream>
#include <stack>

using namespace std;

/***************************************************
                    VERTEX
****************************************************/

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

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}

///GETTERS
int Edge::getFrom()
{
    return m_from;
}

int Edge::getTo()
{
    return m_to;
}

/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise en place du Graphe
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
    m_save.set_bg_color(BLEU);
}

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

///Ajouter et supprimer une espèce dans le graphe 1
void Graph::add_espece1()
{
    int add;

    //On voit si y a un clique sur les bouttons de la barre outils
    m_interface->get_buttonAdd().interact_focus();

    if(m_interface->get_buttonAdd().clicked())
    {
        if(m_bin_vertices.size() != 0 )
        {
            std::cout << " Quelle espèce voulez vous ajouter ? " << std::endl;
            std::cout << "0. Feuilles" << std::endl;
            std::cout << "1. Herbe " << std::endl;
            std::cout << "2. Chenille " << std::endl;
            std::cout << "3. Sauterelle " << std::endl;
            std::cout << "4. Lapin " << std::endl;
            std::cout << "5. Serpent " << std::endl;
            std::cout << "6. Geai bleu " << std::endl;
            std::cout << "7. Aigle " << std::endl;
            std::cout << "8. Chouette " << std::endl;
            std::cout << "9. Decompositeur" << std::endl;
            std::cout << "10. Plante carnivore" << std::endl;
            std::cin >> add;

            while(add < 0 || add > 10 )
            {
                std::cout << "Vous ne pouvez pas ajouter cette espèce" << std::endl;
                std::cout << " Quelle espèce voulez vous ajouter ? " << std::endl;
                std::cout << "0. Feuilles" << std::endl;
                std::cout << "1. Herbe " << std::endl;
                std::cout << "2. Chenille " << std::endl;
                std::cout << "3. Sauterelle " << std::endl;
                std::cout << "4. Lapin " << std::endl;
                std::cout << "5. Serpent " << std::endl;
                std::cout << "6. Geai bleu " << std::endl;
                std::cout << "7. Aigle " << std::endl;
                std::cout << "8. Chouette " << std::endl;
                std::cout << "9. Decompositeur" << std::endl;
                std::cout << "10. Plante carnivore" << std::endl;
                std::cin >> add;
            }

            for(unsigned int i = 0 ; i< m_bin_vertices.size() ; i++)
            {
                std::cout << m_bin_vertices[i].m_indice << std::endl;
                if(m_bin_vertices[i].m_indice == add)
                {
                    add_interfaced_vertex(add, m_bin_vertices[i].m_value, m_bin_vertices[i].m_x, m_bin_vertices[i].m_y, m_bin_vertices[i].m_pic_name, m_bin_vertices[i].m_pic_idx, m_bin_vertices[i].m_taux_repro);

                    int a = m_bin_edges.size();
                    std::cout << a << std::endl;
                    std::vector<int> ar_a_eff;

                    //retrouver les aretes qui vont avec le sommet
                    for (int i = 0; i<a; i++)
                    {
                        if( m_bin_edges[i].m_vert1 == add)
                        {
                            Edge_bin &e_temp = m_bin_edges[i];
                            ///AJOUTER ARETE LIE AU SOMMET A INTERFACE
                            add_interfaced_edge(e_temp.m_indice, e_temp.m_vert1, e_temp.m_vert2, e_temp.m_weight);
                            ///SUPPRIME L'ARETE DU VECTEUR POUBELLE
                            ar_a_eff.push_back(e_temp.m_indice);
                        }

                        //2eme cas possible

                        if( m_bin_edges[i].m_vert2 == add)
                        {
                            Edge_bin &e_temp = m_bin_edges[i];
                            ///AJOUTER ARETE LIE AU SOMMET A INTERFACE
                            add_interfaced_edge(e_temp.m_indice, e_temp.m_vert1, e_temp.m_vert2, e_temp.m_weight);
                            ///SUPPRIME L'ARETE DU VECTEUR POUBELLE
                            ar_a_eff.push_back(e_temp.m_indice);

                        }
                    }

                    for(unsigned int b = 0; b<m_bin_edges.size() ; b++)
                    {
                        for(unsigned int c = 0 ; c < ar_a_eff.size() ; c++)
                        {
                            if(m_bin_edges[b].m_indice == ar_a_eff[c])
                            {
                                m_bin_edges.erase(m_bin_edges.begin() + b);
                            }
                        }
                    }

                    ///SUPPRIME LE SOMMET DU VECTEUR POUBELLE
                    m_bin_vertices.erase( m_bin_vertices.begin() + i);

                }
                else
                {
                    std::cout << "Deja present sur le graphe" << std::endl;
                }
            }
        }
    }
}

///Ajouter et supprimer une espèce dans le graphe 2
void Graph::add_espece2()
{
    int add;

    //On voit si y a un clique sur les bouttons de la barre outils
    m_interface->get_buttonAdd().interact_focus();

    if(m_interface->get_buttonAdd().clicked())
    {

        if (m_bin_vertices.size() != 0)
        {
            std::cout << " Quelle espèce voulez vous ajouter ? " << std::endl;
            std::cout << "0. Baleine bleue" << std::endl;
            std::cout << "1. Poisson " << std::endl;
            std::cout << "2. Mouette " << std::endl;
            std::cout << "3. Phytoplanctons " << std::endl;
            std::cout << "4. Zooplanctons " << std::endl;
            std::cout << "5. Crevette " << std::endl;
            std::cout << "6. Leopard de mer " << std::endl;
            std::cout << "7. Crabe " << std::endl;
            std::cout << "8. Poulpe " << std::endl;
            std::cout << "9. Pingouins " << std::endl;
            std::cout << "10. Elephant de mer " << std::endl;
            std::cout << "11. Orque" << std::endl;
            std::cin >> add;

            while(add < 0 || add > 11 )
            {
                std::cout << "Vous ne pouvez pas ajouter cette espèce" << std::endl;
                std::cout << " Quelle espèce voulez vous ajouter ? " << std::endl;
                std::cout << "0. Baleine bleue" << std::endl;
                std::cout << "1. Poisson " << std::endl;
                std::cout << "2. Mouette " << std::endl;
                std::cout << "3. Phytoplanctons " << std::endl;
                std::cout << "4. Zooplanctons " << std::endl;
                std::cout << "5. Crevette " << std::endl;
                std::cout << "6. Leopard de mer " << std::endl;
                std::cout << "7. Crabe " << std::endl;
                std::cout << "8. Poulpe " << std::endl;
                std::cout << "9. Pingouins " << std::endl;
                std::cout << "10. Elephant de mer " << std::endl;
                std::cout << "11. Orque" << std::endl;
                std::cin >> add;
            }

            for(unsigned int i = 0 ; i< m_bin_vertices.size() ; i++)
            {
                std::cout << m_bin_vertices[i].m_indice << std::endl;
                if(m_bin_vertices[i].m_indice == add)
                {
                    add_interfaced_vertex(add, m_bin_vertices[i].m_value, m_bin_vertices[i].m_x, m_bin_vertices[i].m_y, m_bin_vertices[i].m_pic_name, m_bin_vertices[i].m_pic_idx, m_bin_vertices[i].m_taux_repro);

                    int a = m_bin_edges.size();
                    std::cout << a << std::endl;
                    std::vector<int> ar_a_eff;

                    //retrouver les aretes qui vont avec le sommet
                    for (int i = 0; i<a; i++)
                    {
                        if( m_bin_edges[i].m_vert1 == add)
                        {
                            Edge_bin &e_temp = m_bin_edges[i];
                            ///AJOUTER ARETE LIE AU SOMMET A INTERFACE
                            add_interfaced_edge(e_temp.m_indice, e_temp.m_vert1, e_temp.m_vert2, e_temp.m_weight);
                            ///SUPPRIME L'ARETE DU VECTEUR POUBELLE
                            ar_a_eff.push_back(e_temp.m_indice);
                        }

                        //2eme cas possible
                        if( m_bin_edges[i].m_vert2 == add)
                        {
                            Edge_bin &e_temp = m_bin_edges[i];
                            ///AJOUTER ARETE LIE AU SOMMET A INTERFACE
                            add_interfaced_edge(e_temp.m_indice, e_temp.m_vert1, e_temp.m_vert2, e_temp.m_weight);
                            ///SUPPRIME L'ARETE DU VECTEUR POUBELLE
                            ar_a_eff.push_back(e_temp.m_indice);
                        }
                    }

                    for(unsigned int b = 0; b<m_bin_edges.size() ; b++)
                    {
                        for(unsigned int c = 0 ; c < ar_a_eff.size() ; c++)
                        {
                            if(m_bin_edges[b].m_indice == ar_a_eff[c])
                            {
                                m_bin_edges.erase(m_bin_edges.begin() + b);
                            }
                        }
                    }

                    ///SUPPRIME LE SOMMET DU VECTEUR POUBELLE
                    m_bin_vertices.erase( m_bin_vertices.begin() + i);
                }
                else
                {
                    std::cout << "Deja present sur le graphe" << std::endl;
                }
            }
        }
    }
}

///Ajouter et supprimer une espèce dans le graphe 3
void Graph::add_espece3()
{
    int add;

    //On voit si y a un clique sur les bouttons de la barre outils
    m_interface->get_buttonAdd().interact_focus();

    if(m_interface->get_buttonAdd().clicked())
    {
        if(m_bin_vertices.size() != 0)
        {
            std::cout << " Quelle espèce voulez vous ajouter ? " << std::endl;
            std::cout << "0. Chien sauvage" << std::endl;
            std::cout << "1. Hyène " << std::endl;
            std::cout << "2. Lion " << std::endl;
            std::cout << "3. Pantère " << std::endl;
            std::cout << "4. Chacal " << std::endl;
            std::cout << "5. Serval " << std::endl;
            std::cout << "6. Vautour de Ruppell" << std::endl;
            std::cout << "7. Aigle ravisseur " << std::endl;
            std::cout << "8. Oryctérope " << std::endl;
            std::cout << "9.  Mangouste" << std::endl;
            std::cout << "10. Gnou " << std::endl;
            std::cout << "11. Gazelle de Thompson" << std::endl;
            std::cout << "12. Antilope" << std::endl;
            std::cout << "13. Souris" << std::endl;
            std::cin >> add;

            if( add <0 || add>13)
            {
                std::cout << "Vous ne pouvez pas ajouter cette espèce" << std::endl;
                std::cout << "0. Chien sauvage" << std::endl;
                std::cout << "1. Hyène " << std::endl;
                std::cout << "2. Lion " << std::endl;
                std::cout << "3. Pantère " << std::endl;
                std::cout << "4. Chacal " << std::endl;
                std::cout << "5. Serval " << std::endl;
                std::cout << "6. Vautour de Ruppell" << std::endl;
                std::cout << "7. Aigle ravisseur " << std::endl;
                std::cout << "8. Oryctérope " << std::endl;
                std::cout << "9.  Mangouste" << std::endl;
                std::cout << "10. Gnou " << std::endl;
                std::cout << "11. Gazelle de Thompson" << std::endl;
                std::cout << "12. Antilope" << std::endl;
                std::cout << "13. Souris" << std::endl;
                std::cin >> add;
            }

            for(unsigned int i = 0 ; i< m_bin_vertices.size() ; i++)
            {
                std::cout << m_bin_vertices[i].m_indice << std::endl;
                if(m_bin_vertices[i].m_indice == add)
                {
                    add_interfaced_vertex(add, m_bin_vertices[i].m_value, m_bin_vertices[i].m_x, m_bin_vertices[i].m_y, m_bin_vertices[i].m_pic_name, m_bin_vertices[i].m_pic_idx,m_bin_vertices[i].m_taux_repro);

                    int a = m_bin_edges.size();
                    std::cout << a << std::endl;
                    std::vector<int> ar_a_eff;

                    //retrouver les aretes qui vont avec le sommet
                    for (int i = 0; i<a; i++)
                    {
                        if( m_bin_edges[i].m_vert1 == add)
                        {
                            Edge_bin &e_temp = m_bin_edges[i];
                            ///AJOUTER ARETE LIE AU SOMMET A INTERFACE
                            add_interfaced_edge(e_temp.m_indice, e_temp.m_vert1, e_temp.m_vert2, e_temp.m_weight);
                            ///SUPPRIME L'ARETE DU VECTEUR POUBELLE
                            ar_a_eff.push_back(e_temp.m_indice);
                        }

                        //2eme cas possible
                        if( m_bin_edges[i].m_vert2 == add)
                        {
                            Edge_bin &e_temp = m_bin_edges[i];
                            ///AJOUTER ARETE LIE AU SOMMET A INTERFACE
                            add_interfaced_edge(e_temp.m_indice, e_temp.m_vert1, e_temp.m_vert2, e_temp.m_weight);
                            ///SUPPRIME L'ARETE DU VECTEUR POUBELLE
                            ar_a_eff.push_back(e_temp.m_indice);
                        }
                    }

                    for(unsigned int b = 0; b<m_bin_edges.size() ; b++)
                    {
                        for(unsigned int c = 0 ; c < ar_a_eff.size() ; c++)
                        {
                            if(m_bin_edges[b].m_indice == ar_a_eff[c])
                            {
                                m_bin_edges.erase(m_bin_edges.begin() + b);
                            }
                        }
                    }

                    ///SUPPRIME LE SOMMET DU VECTEUR POUBELLE
                    m_bin_vertices.erase( m_bin_vertices.begin() + i);
                }
                else
                {
                    std::cout << "Deja present sur le graphe" << std::endl;
                }
            }
        }
    }
}

void Graph::delete_espece()
{
    //On voit si y a un clique sur les bouttons de la barre outils
    m_interface->get_buttonDelete().interact_focus();

    if(m_interface->get_buttonDelete().clicked())
    {
        std::cout << "Indiquez l'indice de l'espece a supprimer : " << std::endl;
        int indice;
        std::cin>>indice;

        std::cout << "voici l'indice choisit "<< indice << std::endl;
        // m_vertices[indice] correspond à la valeur associé à indice, soit le Sommet

        Vertex &remove_vertex = m_vertices[indice];

        ///on supprimer les aretes de ce sommet
        //les aretes sortantes et entrantes
        std::cout <<"1) Parcours des aretes sortantes, il y en a " <<remove_vertex.m_out.size()<< std::endl;

        //taille des vecteurs contenant aretes sprtantes/entratnte
        int a = remove_vertex.m_out.size();
        int b = remove_vertex.m_in.size();


        if ( !remove_vertex.m_out.empty() )
        {
            for(int i= 0; i < a ; i++)
            {
                //suppression des aretes sortantes, on passe tjrs [0] car qd on erase les case du vesteur se déplace
                //donc le prochain indice a traité se situera tjrs à l'indice 0
                remove_edge( remove_vertex.m_out[0] );
            }
        }

        //meme chose mais pour les aretes entrantes
        if ( !remove_vertex.m_in.empty() )
        {
            for(int i= 0; i < b ; i++)
            {
                //suppression des aretes entrantes, même principe que précédemment
                remove_edge( remove_vertex.m_in[0] );
            }
        }

        //Il faut retirer l'interface de ce sommet de la main box
        if (m_interface && remove_vertex.m_interface)
        {
            m_interface->m_main_box.remove_child( remove_vertex.m_interface->m_top_box );
        }

        //Puis ajouter le sommet à la map poubelle retirer le sommet de la map normale
        Vertex_bin v_bin(indice, remove_vertex.m_value, remove_vertex.m_interface->m_top_box.get_frame().pos.x, remove_vertex.m_interface->m_top_box.get_frame().pos.y, remove_vertex.m_interface->m_img.get_pic_name(), 0, remove_vertex.m_taux_repro);
        m_bin_vertices.push_back(v_bin);

        ///On supprime le sommet
        m_vertices.erase( indice );

    }
}

void Graph::reset_marquages()
{
    for (auto &elt : m_vertices)
    {
        elt.second.set_Bool1(false);
        elt.second.set_Bool2(false);
//elt.second.set_present_ds_compo(true);
    }
}

/*recherche les composantes*/
void Graph::Marquer_composantes()
{
    reset_marquages();
    //pour chaque sommets
    for(auto &elt : m_vertices)
    {
        //si non marqué
       if ( elt.second.m_present_ds_compo == false )
        {
            //enregistrer l'indice de ce sommet
            reset_marquages();

            //marquer_composante (de ce sommet)
            bool compo_existe = CFC(elt.first);

            if( compo_existe ) afficher();
        }
    }

}
/*******************************************


PENSER PEUT ETRE A VIDER M_VEC_COPOSANTE

EFFACER LES MARQUAGES D APPARTENANCE A COMPO MAIS FAIT BEUGS

************************************************/
void Graph::afficher()
{
    std::cout << "Voici les sommets de la composante fortement connexe : ";
    for ( auto &elt : m_vertices )
    {
        if ( elt.second.m_1 == true && elt.second.m_2 == true && elt.second.m_present_ds_compo == true )
        {
            std::cout << elt.first << " ";
        }
    }
    std::cout << std::endl;
}

bool Graph::CFC(int sommet_ancre)
{
    //piles pour traiter les arcs sortants et entrants
    std::stack<int> pile1;
    std::stack<int> pile2;

    // On part du sommet donné en paramètre
    //On enfile le 1er sommet (son indice)
    pile1.push(sommet_ancre);
    pile2.push(sommet_ancre);

    //vecteur qui serviront à retrouver les composantes fortement connexe
    std::vector<int> v1;
    std::vector<int> v2;

    v1.push_back(sommet_ancre);
    v2.push_back(sommet_ancre);

    // Marquage du sommet de départ
    m_vertices[sommet_ancre].set_Bool1(true);
    m_vertices[sommet_ancre].set_Bool2(true);

    ///PARCOURS DES ARCS ENTRANTS
    //tant que la pile n'est pas vide on continu
    while ( !pile1.empty() )
    {
        //On utilise une variable temp qui prend la valeur de l'indice du sommet en tête de pile
        int id_sommet_actuel = pile1.top();

        // depile sommet en tête de pile
        pile1.pop();

        //empiler sommets predecessuer ( contenu m_in -> m_from )
        for( unsigned int i=0; i < m_vertices[id_sommet_actuel].m_in.size() ; i++)
        {
            //on verifie si le sommet est pas dejà marqué
            if ( m_vertices[m_edges[m_vertices[id_sommet_actuel].m_in[i]].m_from].m_1 == false )
            {
                //on ajoute indice du sommet predecessuer soit le m_from de m_in[i]
                pile1.push( m_edges[m_vertices[id_sommet_actuel].m_in[i]].m_from );

                v1.push_back( m_edges[m_vertices[id_sommet_actuel].m_in[i]].m_from );

                //on le marque et il devient le nouveau sommet_actuel
                m_vertices[m_edges[m_vertices[id_sommet_actuel].m_in[i]].m_from].set_Bool1(true);
            }
        }
    }

    ///PARCOURS DES ARCS SORTANTS
    while (!pile2.empty())
    {
        //On utilise une variable temp qui prend la valeur de l'indice du sommet en tête de pile
        int id_sommet_actuel = pile2.top();

        /// depile sommet en tête de pile
        pile2.pop();

        //empiler sommets predecessuer ( contenu m_in -> m_from )
        for( unsigned int i=0; i < m_vertices[id_sommet_actuel].m_out.size() ; i++)
        {
            //on verifie si le sommet est pas dejà marqué
            if (m_vertices[m_edges[m_vertices[id_sommet_actuel].m_out[i]].m_to].m_2  == false )
            {
                //on ajoute indice du sommet predecessuer soit le m_from de m_in[i]
                pile2.push( m_edges[m_vertices[id_sommet_actuel].m_out[i]].m_to );

                v2.push_back( m_edges[m_vertices[id_sommet_actuel].m_out[i]].m_to );

                //on le marque et il devient le nouveau sommet_actuel
                m_vertices[m_edges[m_vertices[id_sommet_actuel].m_out[i]].m_to].set_Bool2(true);
            }
        }
    }

    //marquer les sommet faisant partie de la composante fort connexe
    bool compo_existe = marquage(v1, v2);

    return compo_existe;
}

bool Graph::marquage(std::vector<int> v1, std::vector<int> v2 )
{
    std::vector<int> v;
    int cpt=0;
    bool compo_existe = false;

    for ( unsigned int i = 0 ; i < v1.size() ; i++ )
    {
        for (unsigned int j = 0 ; j < v2.size() ; j ++ )
        {
            if( v1[i] == v2[j] ) v.push_back(v1[i]); //on met ds v les indice de sommet se retour à la fois ds v1 et v2, se sont les indices des sommets appartenant à la composante
        }
    }

    //s'il y a + de 1 sommet dans la composante fort. connexe alors on marque ces sommets comme faisant parti d'une compo sinon non
    if ( v.size() > 1 )
    {
        //a chaque fois qu'on entre ds ce if cela signifie qu'il existe une nvelle compo connexe, le cpt nous permettra de savoir cb il y en a
        compo_existe = true ;

        for ( unsigned int i = 0 ; i < v.size() ; i++ )
        {
            m_vertices[v[i]].set_present_ds_compo(true);
            m_vect_composantes[cpt].push_back( m_vertices[v[i]]);
        }
        cpt++;
    }


    return compo_existe;
}


/*Source principale Monsieur Fercoq*/
void Graph::remove_edge(int eidx)
{
    // référence vers le Edge à enlever
    Edge &remed=m_edges.at(eidx);

    //Affichage de l'arete qui va etre supprimer
    std::cout << "Suppr arete " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

    //Il faut retiré l'arete de la main box
    //On vérifie que le grape et l'arete on une interface pour ensuite retiré l'arete
    if (m_interface && remed.m_interface)
    {
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }

    //Reference sur les vecteurs respectifs des 2 sommets concernés par l'arete
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;

    //on retrouve l'arete à suppr dans le vecteur m_out ou m_in la contenant
    for(unsigned int i=0; i< vefrom.size(); i++)
    {
        //Qd on l'a retrouvé on la supprime du vecteur
        if( vefrom[i] == eidx)
            vefrom.erase(vefrom.begin() + i);
    }

    //idem
    for(unsigned int i=0; i< veto.size(); i++)
    {
        if( vefrom[i] == eidx)
            veto.erase(veto.begin() + i);
    }

    //on ajoute l'arête dans une autre map contenant les aretes supprimées
    Edge_bin e_bin(eidx, remed.getFrom(), remed.getTo(), remed.getWeight());
    m_bin_edges.push_back(e_bin);

    //Enfin on suppr l'arete de la map contenant les aretes du graphe
    m_edges.erase( eidx );

}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
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

/// Aide à l'ajout de sommets interfacés
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

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, int weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);
    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;

    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}

/*****************************************************
Affichage tour de boucle
******************************************************/
void Graph::affichage_outil()
{
    //affichage/chargement des images pour la tool bar
    grman::show_picture(grman::page, "G1.bmp", 24,6,0);
    grman::show_picture(grman::page, "G2.bmp", 24,86,0);
    grman::show_picture(grman::page, "G3.bmp", 24,166,0);
    grman::show_picture(grman::page,"save.bmp",24,246,0);
    grman::show_picture(grman::page,"read.bmp",24,326,0);
    grman::show_picture(grman::page,"cfc.bmp",24, 406,0);
    grman::show_picture(grman::page, "add.bmp", 24,486,0);
    grman::show_picture(grman::page, "delete.bmp", 24,566,0);
    grman::show_picture(grman::page, "logout.bmp", 24,646,0);
}

void Graph::acces_G1(int* n)
{
    std::cout << "Bienvenue sur le graphe 1 :) " << std::endl;

    ///LECTURE
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

    ///SAUVEGARDE
    get_interface()->get_save().interact_focus();

    if(get_interface()->get_save().clicked())
    {
        std::cout << "Sauvegarde du fichier 1" << std::endl;
        sauv_graphea();
    }

    /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
    update();

    ///Affichage barre outil
    affichage_outil();

    if (key[KEY_SPACE])
    {
        std::cout << "RECHERCHE DE COMPOSANTE FORTEMENT CONNEXE" << std::endl;
        Marquer_composantes();
    }

    /// Mise à jour générale (clavier/souris/buffer etc...)
    grman::mettre_a_jour();

    add_espece1();
    delete_espece();
    sortie();

    if(key[KEY_P])
    {
        //graph_simpl();
    }

    get_interface()->get_buttonG3().interact_focus();

    if(get_interface()->get_buttonG3().clicked())
    {
        *n=3;
    }

    get_interface()->get_buttonG2().interact_focus();

    if(get_interface()->get_buttonG2().clicked())
    {
        *n=2;
    }

    //var_temps();

}

void Graph::acces_G2(int* n)
{

    std::cout << "Bienvenue sur le graphe 2 :) " << std::endl;

    ///LECTURE
    get_interface()->get_lect().interact_focus();

    if(get_interface()->get_lect().clicked())
    {
        std::cout << "Lecture du fichier 2" << std::endl;
        m_edges.clear();
        m_vertices.clear();
        chargement_fichier_b();
    }

    ///SAUVEGARDE
    get_interface()->get_save().interact_focus();

    if(get_interface()->get_save().clicked())
    {
        std::cout << "Sauvegarde du fichier 2" << std::endl;
        sauv_grapheb();
    }

    /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
    update();

    ///Affichage barre outil
    affichage_outil();

    /// Mise à jour générale (clavier/souris/buffer etc...)
    grman::mettre_a_jour();

    add_espece2();
    delete_espece();
    sortie();

    while(key[KEY_P])
    {
        //graph_simpl();
    }


    get_interface()->get_buttonG1().interact_focus();

    if(get_interface()->get_buttonG1().clicked())
    {
        *n=1;
    }

    get_interface()->get_buttonG3().interact_focus();

    if(get_interface()->get_buttonG3().clicked())
    {
        *n=3;
    }

    //var_temps();

}

void Graph::acces_G3(int* n)
{

    //std::cout << "Bienvenue sur le graphe 3 :) " << std::endl;

    ///LECTURE
    get_interface()->get_lect().interact_focus();

    if(get_interface()->get_lect().clicked())
    {
        std::cout << "Lecture du fichier 3" << std::endl;
        m_edges.clear();
        m_vertices.clear();
        chargement_fichier_c();
    }

    ///SAUVEGARDE
    get_interface()->get_save().interact_focus();

    if(get_interface()->get_save().clicked())
    {
        std::cout << "Sauvegarde du fichier 3" << std::endl;
        sauv_graphec();
    }

    /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
    update();

    ///Affichage barre outil
    affichage_outil();

    /// Mise à jour générale (clavier/souris/buffer etc...)
    grman::mettre_a_jour();

    add_espece3();
    delete_espece();
    sortie();

    while(key[KEY_P])
    {
        //graph_simpl();
    }

    get_interface()->get_buttonG1().interact_focus();

    if(get_interface()->get_buttonG1().clicked())
    {
        *n=1;
    }

    get_interface()->get_buttonG2().interact_focus();

    if(get_interface()->get_buttonG2().clicked())
    {
        *n=2;
    }

    //var_temps();

}

void Graph::sortie()
{
    //On voit si y a un clique sur les bouttons de la barre outils
    get_interface()->get_buttonExit().interact_focus();

    if(get_interface()->get_buttonExit().clicked())
    {
        std::cout << "A bientot" << std::endl;
        exit(0);
    }
}

void Graph::graph_simpl(std::vector<std::vector<Vertex>> cfc)
{

    std::cout << "aff graphe simplifi" << std::endl;

    ///Créer le buffer
    BITMAP* buffer = create_bitmap(908,720);

    ///Faire les aretes
    for(unsigned int i = 0 ; i< cfc.size() ; i++)
    {
        for(unsigned int j = 0 ; j < cfc[i].size() ; j++)
        {
            for(auto &elem : m_edges)
            {
                if(get_indice(cfc[i][j]) == elem.second.m_from)
                {
                    for(unsigned int k = 0 ; k < cfc.size() ; k++)
                    {
                        for(unsigned int l = 0 ; l < cfc.size() ; l++)
                        {
                            if(get_indice(cfc[k][l]) == elem.second.m_to)
                            {
                                line(buffer, cfc[i][j].m_interface->m_top_box.get_frame().pos.x+50,cfc[i][j].m_interface->m_top_box.get_frame().pos.y+50, cfc[k][l].m_interface->m_top_box.get_frame().pos.x+50,cfc[k][l].m_interface->m_top_box.get_frame().pos.y+50, NOIR);
                                textprintf_ex(buffer, font, ((cfc[i][j].m_interface->m_top_box.get_frame().pos.x+50+cfc[k][l].m_interface->m_top_box.get_frame().pos.x+50)/2), ((cfc[i][j].m_interface->m_top_box.get_frame().pos.y+50+cfc[k][l].m_interface->m_top_box.get_frame().pos.y+50)/2), NOIR, -1, " de %d a %d", get_indice(cfc[i][j]),get_indice(cfc[k][l]));
                            }
                        }
                    }
                }
                else if(get_indice(cfc[i][j]) == elem.second.m_to)
                {
                    for(unsigned int k = 0 ; k < cfc.size() ; k++)
                    {
                        for(unsigned int l = 0 ; l < cfc.size() ; l++)
                        {
                            if(get_indice(cfc[k][l]) == elem.second.m_from)
                            {
                                line(buffer, cfc[k][l].m_interface->m_top_box.get_frame().pos.x+50,cfc[k][l].m_interface->m_top_box.get_frame().pos.y+50,cfc[i][j].m_interface->m_top_box.get_frame().pos.x+50,cfc[i][j].m_interface->m_top_box.get_frame().pos.y+50, NOIR);
                                textprintf_ex(buffer, font, ((cfc[i][j].m_interface->m_top_box.get_frame().pos.x+50+cfc[k][l].m_interface->m_top_box.get_frame().pos.x+50)/2), ((cfc[k][l].m_interface->m_top_box.get_frame().pos.y+50+cfc[i][j].m_interface->m_top_box.get_frame().pos.y+50)/2), NOIR, -1, " de %d a %d", get_indice(cfc[k][l]),get_indice(cfc[i][j]));
                            }
                        }
                    }
                }
            }
        }
    }

    ///Affichage des sommets de fortes connexités
    for(unsigned int i = 0 ; i< cfc.size() ; i++)
    {
        for(unsigned int j = 0 ; j < cfc[i].size() ; j++)
        {
            //Affichage du sommet
            rectfill(buffer,cfc[i][j].m_interface->m_top_box.get_frame().pos.x+45, cfc[i][j].m_interface->m_top_box.get_frame().pos.y+45,cfc[i][j].m_interface->m_top_box.get_frame().pos.x+55,cfc[i][j].m_interface->m_top_box.get_frame().pos.y+55, BLANC );
            rect(buffer,cfc[i][j].m_interface->m_top_box.get_frame().pos.x+45, cfc[i][j].m_interface->m_top_box.get_frame().pos.y+45,cfc[i][j].m_interface->m_top_box.get_frame().pos.x+55,cfc[i][j].m_interface->m_top_box.get_frame().pos.y+55, NOIR);

            textprintf_ex(buffer, font, cfc[i][j].m_interface->m_top_box.get_frame().pos.x+50, cfc[i][j].m_interface->m_top_box.get_frame().pos.y+50, NOIR, -1, "%d", get_indice(cfc[i][j]));
        }
    }

    ///Affichage du buffer
    blit(buffer,grman::page,0,0, 100, 5,908,720);


}

int Graph::get_indice(Vertex V)
{
    if(V.m_in.size() != 0)
    {
        return m_edges[V.m_in[0]].m_to;
    }
    else
    {
        return m_edges[V.m_out[0]].m_from;
    }
}

void Graph::var_temps()
{
    ///On modifie tous les K
    for(auto &elem : m_vertices)
    {
        elem.second.m_K = 0;

        for(unsigned int i = 0 ; i < elem.second.m_in.size() ; i++ )
        {

            elem.second.m_K = elem.second.m_K + ( m_edges[elem.second.m_in[i]].m_weight * m_vertices[m_edges[elem.second.m_in[i]].m_from].m_value );
            std::cout << elem.second.m_K << std::endl;
        }
    }


    ///On modifie tous les N
    for(auto &elem : m_vertices)
    {
        elem.second.m_value = elem.second.m_value + (elem.second.m_taux_repro*elem.second.m_value*(1 - (elem.second.m_value/elem.second.m_K)));
        for(unsigned int i = 0 ; i < elem.second.m_out.size() ; i++)
        {
            elem.second.m_value = elem.second.m_value - ( m_edges[elem.second.m_out[i]].m_weight * m_vertices[m_edges[elem.second.m_out[i]].m_to].m_value );
        }
    }
}




