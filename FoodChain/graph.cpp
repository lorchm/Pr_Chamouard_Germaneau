#include "graph.h"
#include <fstream>

using namespace std;

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
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


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
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

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    ///Si y'a pas d'interface
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
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

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise en place du Graphe
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
    m_buttonG1.set_frame(2,0,80,80);
    m_buttonG1.set_bg_color(ROSE);
    m_buttonG1.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    //Bouton 2 de la tool box, pour le graphe 2
    m_tool_box.add_child(m_buttonG2);
    m_buttonG2.set_frame(2,80,80,80);
    m_buttonG2.set_bg_color(VERTCLAIR);

    //Bouton 3 de la tool box, pour le graphe 3
    m_tool_box.add_child(m_buttonG3);
    m_buttonG3.set_frame(2,160,80,80);
    m_buttonG3.set_bg_color(ORANGECLAIR);

    //Bouton ajout esp�ce de la tool box
    m_tool_box.add_child(m_buttonAdd);
    m_buttonAdd.set_frame(2,486,80,80);
    m_buttonAdd.set_bg_color(FUCHSIACLAIR);

    //Bouton enlever espece de la tool box
    m_tool_box.add_child(m_buttonDelete);
    m_buttonDelete.set_frame(2,566,80,80);
    m_buttonDelete.set_bg_color(VIOLETCLAIR);

    //Bouton exit de la tool box
    m_tool_box.add_child(m_buttonExit);
    m_buttonExit.set_frame(2,646,80,80);
    m_buttonExit.set_bg_color(BLANCROSE);

    //Bouton save de la tool box
    m_tool_box.add_child(m_save);
    m_save.set_frame(2,286,80,80);
    m_save.set_bg_color(BLANC);

    //Bouton read de la tool box
    m_tool_box.add_child(m_lect);
    m_lect.set_frame(2,366,80,80);
    m_save.set_bg_color(BLANC);

}


/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.
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
            file >> idx >>var_nb_pop >> var_coordx >> var_coordy >> var_image;
            add_interfaced_vertex(idx, var_nb_pop, var_coordx, var_coordy, var_image);

        }

        file >> nb_sommets_supp;

        for(int k=0; k<nb_sommets_supp ; k++)
        {
            file >> idx >> var_nb_pop >> var_coordx >> var_coordy >> var_image;
            Vertex_bin temp = Vertex_bin(idx,var_nb_pop, var_coordx, var_coordy, var_image,0);
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
            file >> idx >> var_nb_pop >> var_coordx >> var_coordy >> var_image;
            add_interfaced_vertex(idx, var_nb_pop, var_coordx, var_coordy, var_image);

        }

        file >> nb_sommets_supp;

        for(int k=0; k<nb_sommets_supp ; k++)
        {
            file >> idx >>var_nb_pop >> var_coordx >> var_coordy >> var_image;
            Vertex_bin temp = Vertex_bin(idx,var_nb_pop, var_coordx, var_coordy, var_image,0);
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
            file >> idx >> var_nb_pop >> var_coordx >> var_coordy >> var_image;
            add_interfaced_vertex(idx, var_nb_pop, var_coordx, var_coordy, var_image);

        }

        file >> nb_sommets_supp;

        for(int k=0; k<nb_sommets_supp ; k++)
        {
            file >> idx >> var_nb_pop >> var_coordx >> var_coordy >> var_image;
            Vertex_bin temp = Vertex_bin(idx,var_nb_pop, var_coordx, var_coordy, var_image,0);
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
            file << elt.first << " " << elt.second.m_value << " " << elt.second.m_interface->m_top_box.get_frame().pos.x << " " << elt.second.m_interface->m_top_box.get_frame().pos.y << " " << elt.second.m_interface->m_img.get_pic_name()<< std::endl;
        }

        ///Nombre vertices dans la bin
        file << std::endl << m_bin_vertices.size() << std::endl << std::endl;

        ///Ecrire la valeur et positions des sommets
        for (unsigned int i = 0; i< m_bin_vertices.size() ; i++)
        {
            file << m_bin_vertices[i].m_indice << " " <<  m_bin_vertices[i].m_value << " " << m_bin_vertices[i].m_x << " " << m_bin_vertices[i].m_y << " " << m_bin_vertices[i].m_pic_name << std::endl;
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
            file << elt.first << " " << elt.second.m_value << " " << elt.second.m_interface->m_top_box.get_frame().pos.x << " " << elt.second.m_interface->m_top_box.get_frame().pos.y << " " << elt.second.m_interface->m_img.get_pic_name()<< std::endl;
        }

        ///Nombre vertices dans la bin
        file << m_bin_vertices.size() << std::endl << std::endl;

        ///Ecrire la valeur et positions des sommets
        for (unsigned int i = 0 ; i<m_bin_vertices.size() ; i++)
        {
            file << m_bin_vertices[i].m_indice << " " << m_bin_vertices[i].m_value << " " << m_bin_vertices[i].m_x << " " << m_bin_vertices[i].m_y << " " << m_bin_vertices[i].m_pic_name << std::endl;
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
            file << elt.first << " " <<elt.second.m_value << " " << elt.second.m_interface->m_top_box.get_frame().pos.x << " " << elt.second.m_interface->m_top_box.get_frame().pos.y << " " << elt.second.m_interface->m_img.get_pic_name()<< std::endl;
        }

        ///Nombre vertices dans la bin
        file << m_bin_vertices.size() << std::endl << std::endl;

        ///Ecrire la valeur et positions des sommets
        for (unsigned int i = 0 ; i<m_bin_vertices.size() ; i++)
        {
            file << m_bin_vertices[i].m_indice << " " << m_bin_vertices[i].m_value << " " << m_bin_vertices[i].m_x << " " << m_bin_vertices[i].m_y << " " << m_bin_vertices[i].m_pic_name << std::endl;
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
        // m_vertices[indice] correspond � la valeur associ� � indice, soit le Sommet

        Vertex &remove_vertex = m_vertices[indice];

        ///on supprimer les aretes de ce sommet
        //les aretes sortantes et entrantes
        std::cout <<"1) Parcours des aretes sortantes, il y en a " <<remove_vertex.m_out.size()<< std::endl;

        //taille des vecteurs contenant aretes sprtantes/entratnte
        int a = remove_vertex.m_out.size();
        int b = remove_vertex.m_in.size();


        if ( !remove_vertex.m_out.empty() )
        {
             for( unsigned int i= 0; i < a ; i++)
            {
                //suppression des aretes sortantes, on passe tjrs [0] car qd on erase les case du vesteur se d�place
                //donc le prochain indice a trait� se situera tjrs � l'indice 0
                remove_edge( remove_vertex.m_out[0] );
            }
        }

        //meme chose mais pour les aretes entrantes
        if ( !remove_vertex.m_in.empty() )
        {
            for( unsigned int i= 0; i < b ; i++)
            {
                //suppression des aretes entrantes, m�me principe que pr�c�demment
                remove_edge( remove_vertex.m_in[0] );
            }
        }

        //Il faut retirer l'interface de ce sommet de la main box
        if (m_interface && remove_vertex.m_interface)
        {
            m_interface->m_main_box.remove_child( remove_vertex.m_interface->m_top_box );
        }

        //Puis ajouter le sommet � la map poubelle retirer le sommet de la map normale
        Vertex_bin v_bin(indice, remove_vertex.m_value, remove_vertex.m_interface->m_top_box.get_frame().pos.x,
                         remove_vertex.m_interface->m_top_box.get_frame().pos.y, remove_vertex.m_interface->m_img.get_pic_name(), 0);
        m_bin_vertices.push_back(v_bin);

        ///On supprime le sommet
        m_vertices.erase( indice );

    }
}



/*Source principale Monsieur Fercoq*/
void Graph::remove_edge(int eidx)
{
    // r�f�rence vers le Edge � enlever
    Edge &remed=m_edges.at(eidx);

    //Affichage de l'arete qui va etre supprimer
    std::cout << "Suppr arete " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

    //Il faut retir� l'arete de la main box
    //On v�rifie que le grape et l'arete on une interface pour ensuite retir� l'arete
    if (m_interface && remed.m_interface)
    {
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }

    //Reference sur les vecteurs respectifs des 2 sommets concern�s par l'arete
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;

    //on retrouve l'arete � suppr dans le vecteur m_out ou m_in la contenant
    for(unsigned int i=0; i< vefrom.size(); i++)
    {
        //Qd on l'a retrouv� on la supprime du vecteur
        if( vefrom[i] == eidx) vefrom.erase(vefrom.begin() + i);
    }

    //idem
    for(unsigned int i=0; i< veto.size(); i++)
    {
        if( vefrom[i] == eidx) veto.erase(veto.begin() + i);
    }

    //on ajoute l'ar�te dans une autre map contenant les aretes supprim�es
    Edge_bin e_bin(eidx, remed.getFrom(), remed.getTo(), remed.getWeight());
    m_bin_edges.push_back(e_bin);

    //Enfin on suppr l'arete de la map contenant les aretes du graphe
    m_edges.erase( eidx );

}

/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
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

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, int value, int x, int y, std::string pic_name, int pic_idx)
{
    /*parcours les indices de sommet de la map pour voir si le sommet a pas d�j� �t� cr�e, si c le cas y un message d'erreur*/
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide � l'ajout d'arcs interfac�s
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
    grman::show_picture(grman::page, "add.bmp", 24,486,0);
    grman::show_picture(grman::page, "delete.bmp", 24,566,0);
    grman::show_picture(grman::page, "logout.bmp", 24,646,0);

    grman::show_picture(grman::page,"save.bmp",24,290,0);
    grman::show_picture(grman::page,"read.bmp",24,370,0);


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
        chargement_fichier_a();
    }

    ///SAUVEGARDE
    get_interface()->get_save().interact_focus();

    if(get_interface()->get_save().clicked())
    {
        std::cout << "Sauvegarde du fichier 1" << std::endl;
        sauv_graphea();
    }

    /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
    update();

    ///Affichage barre outil
    affichage_outil();

    /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
    grman::mettre_a_jour();

    add_espece();
    delete_espece();
    sortie();

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

    /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
    update();

    ///Affichage barre outil
    affichage_outil();

    /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
    grman::mettre_a_jour();

    add_espece();
    delete_espece();
    sortie();

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

}

void Graph::acces_G3(int* n)
{

    std::cout << "Bienvenue sur le graphe 3 :) " << std::endl;

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

    /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
    update();

    ///Affichage barre outil
    affichage_outil();

    /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
    grman::mettre_a_jour();

    add_espece();
    delete_espece();
    sortie();

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

