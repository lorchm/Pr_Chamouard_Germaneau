#include "graph.h"
#include <fstream>

using namespace std;

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx, bool presence)
{

    m_presence = presence;

    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
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
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
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

    //Boutton 1 de la tool box, pour le graphe 1
    m_tool_box.add_child(m_buttonG1);
    m_buttonG1.set_frame(2,0,80,80);
    m_buttonG1.set_bg_color(ROSE);
    m_buttonG1.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    //Boutton 2 de la tool box, pour le graphe 2
    m_tool_box.add_child(m_buttonG2);
    m_buttonG2.set_frame(2,80,80,80);
    m_buttonG2.set_bg_color(VERTCLAIR);

    //Boutton 3 de la tool box, pour le graphe 3
    m_tool_box.add_child(m_buttonG3);
    m_buttonG3.set_frame(2,160,80,80);
    m_buttonG3.set_bg_color(ORANGECLAIR);

    //Boutton ajout espèce de la tool box
    m_tool_box.add_child(m_buttonAdd);
    m_buttonAdd.set_frame(2,486,80,80);
    m_buttonAdd.set_bg_color(FUCHSIACLAIR);

    //Boutton enlever espece de la tool box
    m_tool_box.add_child(m_buttonDelete);
    m_buttonDelete.set_frame(2,566,80,80);
    m_buttonDelete.set_bg_color(VIOLETCLAIR);

    //Boutton exit de la tool box
    m_tool_box.add_child(m_buttonExit);
    m_buttonExit.set_frame(2,646,80,80);
    m_buttonExit.set_bg_color(BLANCROSE);
}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
void Graph::chargement_fichier_a()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    ifstream file("fich_graphea.txt", ios::in);

    int nb_sommets;
    int var_nb_pop;
    int var_coordx;
    int var_coordy;
    std::string var_image;

    int nb_aretes;
    int som1;
    int som2;
    float var_arc;

    if(file)
    {
        file >> nb_sommets;

        for(int i=0 ; i<nb_sommets ; i++)
        {
            file >> var_nb_pop >> var_coordx >> var_coordy >> var_image;
            add_interfaced_vertex(i, var_nb_pop , var_coordx , var_coordy, var_image);
        }

        file >> nb_aretes;

        for(int j=0 ; j<nb_aretes ; j++)
        {
            file >> som1 >> som2 >> var_arc;
            add_interfaced_edge(j, som1, som2, var_arc);
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

    int nb_sommets;
    int var_nb_pop;
    int var_coordx;
    int var_coordy;
    std::string var_image;

    int nb_aretes;
    int som1;
    int som2;
    float var_arc;

    if(file)
    {
        file >> nb_sommets;

        for(int i=0 ; i<nb_sommets ; i++)
        {
            file >> var_nb_pop >> var_coordx >> var_coordy >> var_image;
            add_interfaced_vertex(i+1, var_nb_pop , var_coordx , var_coordy, var_image);
        }

        file >> nb_aretes;

        for(int j=0 ; j<nb_aretes ; j++)
        {
            file >> som1 >> som2 >> var_arc;
            add_interfaced_edge(j+1, som1, som2, var_arc);
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

    int nb_sommets;
    int var_nb_pop;
    int var_coordx;
    int var_coordy;
    std::string var_image;

    int nb_aretes;
    int som1;
    int som2;
    float var_arc;

    if(file)
    {
        file >> nb_sommets;

        for(int i=0 ; i<nb_sommets ; i++)
        {
            file >> var_nb_pop >> var_coordx >> var_coordy >> var_image;
            add_interfaced_vertex(i+1, var_nb_pop , var_coordx , var_coordy, var_image);
        }

        file >> nb_aretes;

        for(int j=0 ; j<nb_aretes ; j++)
        {
            file >> som1 >> som2 >> var_arc;
            add_interfaced_edge(j+1, som1, som2, var_arc);
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
            file << elt.second.m_value << " " << elt.second.m_interface->m_top_box.get_frame().pos.x << " " << elt.second.m_interface->m_top_box.get_frame().pos.y << " " << elt.second.m_interface->m_img.get_pic_name()<< std::endl;
        }

        ///Ecrire le nombre de edges
        file << std::endl << m_edges.size() << std::endl << std::endl ;

        ///Ecrire les sommets et le poids de l'arc
        for (auto &it : m_edges)
        {
           file << it.second.m_from << " " << it.second.m_to << " " << it.second.m_weight << std::endl;
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
            file << elt.second.m_value << " " << elt.second.m_interface->m_top_box.get_frame().pos.x << " " << elt.second.m_interface->m_top_box.get_frame().pos.y << " " << elt.second.m_interface->m_img.get_pic_name()<< std::endl;
        }

        ///Ecrire le nombre de edges
        file << std::endl << m_edges.size() << std::endl << std::endl ;

        ///Ecrire les sommets et le poids de l'arc
        for (auto &it : m_edges)
        {
           file << it.second.m_from << " " << it.second.m_to << " " << it.second.m_weight << std::endl;
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
            file << elt.second.m_value << " " << elt.second.m_interface->m_top_box.get_frame().pos.x << " " << elt.second.m_interface->m_top_box.get_frame().pos.y << " " << elt.second.m_interface->m_img.get_pic_name()<< std::endl;
        }

        ///Ecrire le nombre de edges
        file << std::endl << m_edges.size() << std::endl << std::endl ;

        ///Ecrire les sommets et le poids de l'arc
        for (auto &it : m_edges)
        {
           file << it.second.m_from << " " << it.second.m_to << " " << it.second.m_weight << std::endl;
        }


        file.close();

    }
    else
    {
       std::cerr << "Sauvegarde impossible" << std::endl;
    }

}

///Ajouter et supprimer une espèce
void Graph::add_espece()
{
    //On voit si y a un clique sur les bouttons de la barre outils
    m_interface->get_buttonAdd().interact_focus();

    if(m_interface->get_buttonAdd().clicked())
    {
        std::cout << "Ajouter une espece " << std::endl;
    }
}

void Graph::delete_espece()
{
    //On voit si y a un clique sur les bouttons de la barre outils
    m_interface->get_buttonDelete().interact_focus();

    if(m_interface->get_buttonDelete().clicked())
    {
        std::cout << "Supprimer une espece " << std::endl;

       // m_vertices.erase(2);
    }
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
void Graph::add_interfaced_vertex(int idx, int value, int x, int y, std::string pic_name, int pic_idx, bool presence )
{
    /*parcours les indices de sommet de la map pour voir si le sommet a pas déjà été crée, si c le cas y un message d'erreur*/
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx, presence);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
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
}

