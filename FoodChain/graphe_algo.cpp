#include "graph.h"
#include <stack>
#include <queue>


/**
 * \file graphe_algo.cpp
 * \brief Programme d'ajout/suppression d'espece, recherche de composante fortement connexe, recherhce de composante connexe et deconnexion du graphe.
 * \author Laure Chamouard et Barbara Germaneau et Monsieur Fercop (pour Remove_Edge)
 * \version 1.0
 * \date 8 Avril 2018
 *
 * Programme d'ajout et suppression d'une espece (sommet + arete), recherche de composante fortement connexe, recherhce de composante connexe et deconnexion du graphe.
 *
 */

/*************************************************************
AJOUTS ET SUPPRESSION
*************************************************************/
/* add_espece1 :ajoute une espèce sur le graphe 1
Entrée : /
Sortie : /
*/
/**
 * \fn void Graph::add_espece1()
 * \brief Fonction d'ajout d'une espece pour le graphe 1.
 */
void Graph::add_espece1()
{
    int add;

    //On voit si y a un clique sur les bouttons de la barre outils
    m_interface->get_buttonAdd().interact_focus();

    if(m_interface->get_buttonAdd().clicked())
    {
        //Affichage des espèces du graphe
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

            //Blindage
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

            //Pour tous les sommets de la bin
            for(unsigned int i = 0 ; i< m_bin_vertices.size() ; i++)
            {
                //Si l'indice du sommet correspond à celui qu'on veut add
                if(m_bin_vertices[i].m_indice == add)
                {
                    //On le crée + interface
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

                    //On efface l'arête
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

/* add_espece 2 : ajoute une espèce sur le graphe 2
Entrée : /
Sortie : /
*/
/**
 * \fn void Graph::add_espece2()
 * \brief Fonction d'ajout d'une espece pour le graphe 2.
 */
void Graph::add_espece2()
{
    int add;

    //On voit si y a un clique sur les bouttons de la barre outils
    m_interface->get_buttonAdd().interact_focus();

    if(m_interface->get_buttonAdd().clicked())
    {
        //Affichage des espèces
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

            //Blindage
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

            //Pour tous les sommets
            for(unsigned int i = 0 ; i< m_bin_vertices.size() ; i++)
            {
                //Si l'indice du sommet correspond à celui qu'on veut suppr
                if(m_bin_vertices[i].m_indice == add)
                {
                    //On le crée avec une interface
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

                    //On supprime l'arête
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

/* add_espece3 : ajoute une espèce sur le graphe 3
Entrée : /
Sortie : /
*/
/**
 * \fn void Graph::add_espece3()
 * \brief Fonction d'ajout d'une espece pour le graphe 3.
 */
void Graph::add_espece3()
{
    int add;

    //On voit si y a un clique sur les bouttons de la barre outils
    m_interface->get_buttonAdd().interact_focus();

    if(m_interface->get_buttonAdd().clicked())
    {
        //Affichage de toutes les espèces
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

            //Blindage
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

            //Parcours des sommets supprimés
            for(unsigned int i = 0 ; i< m_bin_vertices.size() ; i++)
            {
                //Si un des sommets supprimés correspond à celui qu'on veut ajouter
                if(m_bin_vertices[i].m_indice == add)
                {
                    //Création du sommet avec une interface
                    add_interfaced_vertex(add, m_bin_vertices[i].m_value, m_bin_vertices[i].m_x, m_bin_vertices[i].m_y, m_bin_vertices[i].m_pic_name, m_bin_vertices[i].m_pic_idx,m_bin_vertices[i].m_taux_repro);


                    int a = m_bin_edges.size();
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

                    //Suppresion de l'arête
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

/* delete_espece : supprime l'espèce choisit grace à son indice
Entrée : /
Sortie : /
*/
/**
 * \fn void Graph::delete_espece()
 * \brief Fonction qui supprime une espece presente sur le graphe.
 */
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

/* remove_edge : enlève l'edge d'indice passé en paramètre (source Monsieur Fercoq)
Entrée : int eidx (indice de l'arête à retirer)
Sortie : /
*/
/**
 * \fn void Graph::remove_edge(int eidx)
 * \brief Fonction qui supprime l'arete associe au sommet voulant etre supprime.
 * \author Monsieur Fercoq
 * \param eidx Indice de l'arete a supprimer
 */
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
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );

    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );

    //on ajoute l'arête dans une autre map contenant les aretes supprimées
    Edge_bin e_bin(eidx, remed.getFrom(), remed.getTo(), remed.getWeight());
    m_bin_edges.push_back(e_bin);

    //Enfin on suppr l'arete de la map contenant les aretes du graphe
    m_edges.erase( eidx );

}


/************************************************************
ALGOS ET FONCTIONS DE RECHERCHE
************************************************************/

/* get_indice : retourne l'indice d'un sommet
Entrée : Vertex V : Sommet
Sortie : int : indice sommet
*/
/**
 * \fn int Graph::get_indice(Vertex V)
 * \brief Fonction qui retrouve l'indice d'un sommet.
 *
 * \param V Sommet pour lequel on cherche l'indice.
 * \return Retourne l'indice (int) du sommet voulu.
 */
int Graph::get_indice(Vertex V)
{
    //Si le nombre d'arc entrant n'est pas égal à 0
    if(V.m_in.size() != 0)
    {
        //On retourne l'indice m_to du premier arc entrant du sommet
        return m_edges[V.m_in[0]].m_to;
    }
    else
    {
        //On retourne l'indice m_from du premier arc sortant du sommet
        return m_edges[V.m_out[0]].m_from;
    }
}

/*CFC = recherche de composante(s) fortement connexe(s)
Entrées : sommet ancre à partir duquel la recherche commencera
Sorties : retourne un booléen indiquant s'il s'agit bien d'un composante fortement connexe (true)*/
/**
 * \fn bool Graph::CFC(int sommet_ancre)
 * \brief Fonction qui va stocker dans 2 vecteurs les sommets accessible a partir des arcs entrant et sortant en fonction du sommet ancre,
 * utilisation d'un parcours en profondeur du graphe (DFS). Puis appel la fonction marquage, a qui l'on passe les 2 vecteurs
 *
 * \param sommet_ancre Indice du sommet a partir duquel on commence la recherche des composantes fortement connexes
 * \return Retourne un booléen indiquant s'il s'agit bien d'un composante fortement connexe (true)
 */
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


/*marquage : sert a retrouver les sommets appartenant aux comosantes fortement connexe
Entrée : 2 vecteurs contenant les indices des sommets, on va comparer ces 2 vecteurs pour retrouver les sommets de la compo
Sortie : retourne un booléen qui indique s'il existe bien une compo fortement connexe
*/
/**
 * \fn bool Graph::marquage(std::vector<int> v1, std::vector<int> v2 )
 * \brief Cherche les indices de sommets en communs dans les vecteus et determine s'il s'agit bien d'une composante fortement connexe.
 *
 * \param v1 et v2, 2 vecteurs contenant des indices de sommets, l'un les indices de sommets accessible via les arcs entrants et l'autre via les arcs sortants, le tout par rapport au sommet ancre
 * \return un booleen, true s'il existe un composante fort. connexe sinon false
 */
bool Graph::marquage(std::vector<int> v1, std::vector<int> v2 )
{
    std::vector<int> v;
    bool compo_existe = false;

    for ( unsigned int i = 0 ; i < v1.size() ; i++ )
    {
        for (unsigned int j = 0 ; j < v2.size() ; j ++ )
        {
            if( v1[i] == v2[j] )
                v.push_back(v1[i]); //on met ds v les indice de sommet se retour à la fois ds v1 et v2, se sont les indices des sommets appartenant à la composante
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
        }


        std::vector<Vertex> vect_temp;
        for ( unsigned int i = 0 ; i < v.size() ; i++ )
        {
            vect_temp.push_back(m_vertices[v[i]]);
        }

        //on mets les sommets de la compo dans un vecteur de vecteur qui repertorira tt le monde
        m_vect_composantes.push_back( vect_temp );

        //reinitialise
        vect_temp.clear();
    }

    return compo_existe;

}

/*reset_marquages : enlève les marquages pour le marquage de l'algo de Tarjan pour les CDC
Entrée : /
Sortie : /
*/
/**
 * \fn void Graph::reset_marquages()
 * \brief Remets e false les attributs m_1 et m_2 d'un sommet, donc de la classe Vertex.
 */
void Graph::reset_marquages()
{
    //Pour tous les sommets
    for (auto &elt : m_vertices)
    {
        //Met les bool de marquage à false
        elt.second.set_Bool1(false);
        elt.second.set_Bool2(false);
    }
}

/*Marquer_composantes : Recherche les composantes fortement connexes
Entrée : /
Sortie : /
*/
/**
 * \fn void Graph::Marquer_composantes()
 * \brief Permet d'effectuer la recherche de composante fortement connexe pour chaque sommet du graphe et affiche en orange les arcs appartenant e la composante
 */
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

            //Si la compo existe on l'affiche sur la console
            if( compo_existe )
                afficher();
        }
    }

    ///Affiche les aretes faisant partie de la compo fort connexe ds une autre couleur
    //Pour toutes les composantes fortement connexes
    for(unsigned int i = 0 ; i< m_vect_composantes.size() ; i++)
    {
        //Pour tous les sommets de la composante i
        for(unsigned int j = 0 ; j < m_vect_composantes[i].size() ; j++)
        {
            //Pour toutes les arêtes
            for(auto &elem : m_edges)
            {
                //Si l'indice du sommet est le m_from de l'arête
                if(get_indice(m_vect_composantes[i][j]) == elem.second.m_from)
                {
                    //Pour tous les sommets de la composante i
                    for(unsigned int k = 0 ; k < m_vect_composantes[i].size() ; k++)
                    {
                        //Si un sommet correspond au m_to de l'arete
                        if(get_indice(m_vect_composantes[i][k]) == elem.second.m_to)
                        {
                            //On change la couleur de l'arête
                            elem.second.m_interface->m_top_edge.set_color(ORANGECLAIR);
                        }
                    }
                }
                //Si l'indice du sommet est le m_to de l'arête
                else if(get_indice(m_vect_composantes[i][j]) == elem.second.m_to)
                {
                    //Pour tous les sommets de la composante
                    for(unsigned int k = 0 ; k < m_vect_composantes[i].size() ; k++)
                    {
                        //Si l'autre sommet est le m_from de l'arête
                        if(get_indice(m_vect_composantes[i][k]) == elem.second.m_from)
                        {
                            //On change la couleur de l'arête
                            elem.second.m_interface->m_top_edge.set_color(ORANGECLAIR);
                        }
                    }
                }
            }
        }
    }
}

/*Rechercher_connexe :  calcul s'il les composantes connexes à partir d'un sommet, utilisation du BFS
Entree : sommet à partir duquel on va chercher les composantes
Sortie : /
*/
/**
 * \fn void Graph::Rechercher_connexe(int sommet_ancre)
 * \brief Utilisation d'un parcours en largeur (BFS) pour determiner une composante connexe a partir d'un sommet ancre
 *
 * \param sommet_ancre Indice du sommet a partir duquel on commmence la recherche
 */
void Graph::Rechercher_connexe(int sommet_ancre)
{
    //Création variable
    std::queue<int> file;

    /// 1. On part du sommet donné en paramètre
    //On enfile le 1er sommet (son indice)
    file.push(sommet_ancre);

    ///2. Marquage du sommet de départ
    m_vertices[sommet_ancre].set_Bool1(true);

    ///3. on fait tourner tant que la file n'est pas vide
    //tant que la file n'est pas vide on continu
    while (!file.empty())
    {
        //On utilise une variable temp qui prend la valeur de l'indice du sommet en tête de file
        int id_sommet_actuel = file.front();

        ///5. défiler le sommet en tête de file
        file.pop();

        ///POUR VOISINS AVEC ARC ENTRANTS
        ///6. enfile sommet(s) voisin(s) si pas déjà marqué
        //on fait en fonction du nombre de voisin du sommet en tête de file
        for(unsigned int i=0; i<  m_vertices[id_sommet_actuel].m_in.size(); i++)
        {
            ///7. On vérifie si les sommets adj sont déjà marqué si c'est le cas il ne faut pas les enfiler et s'il est présent sur le graph temporaire
            if( m_vertices[m_edges[m_vertices[id_sommet_actuel].m_in[i]].m_from].m_1 == false && m_vertices[m_edges[m_vertices[id_sommet_actuel].m_in[i]].m_from].m_prst_graph == true )
            {
                // std::cout << "s" << m_edges[m_vertices[id_sommet_actuel].m_in[i]].m_from << " b1=" <<m_vertices[m_edges[m_vertices[id_sommet_actuel].m_in[i]].m_from].m_1<< " prst g=" <<m_vertices[m_edges[m_vertices[id_sommet_actuel].m_in[i]].m_from].m_prst_graph << std::endl;

                //m_adjacent contient indice du sommet adj
                ///8. enfile sommet(s) voisin(s) non marqué(s)
                file.push( m_edges[m_vertices[id_sommet_actuel].m_in[i]].m_from );

                ///9. marquage
                m_vertices[m_edges[m_vertices[id_sommet_actuel].m_in[i]].m_from].set_Bool1(true);
            }
        }

        ///POUR VOISINS AVEC ARC SORTANTS
        ///6. enfile sommet(s) voisin(s) si pas déjà marqué
        //on fait en fonction du nombre de voisin du sommet en tête de file
        for(unsigned int i=0; i<  m_vertices[id_sommet_actuel].m_out.size() ; i++)
        {
            ///7. On vérifie si les sommets adj sont déjà marqué si c'est le cas il ne faut pas les enfiler et s'il est présent sur le graph temporaire
            if( m_vertices[m_edges[m_vertices[id_sommet_actuel].m_out[i]].m_to].m_1 == false && m_vertices[m_edges[m_vertices[id_sommet_actuel].m_out[i]].m_to].m_prst_graph == true )
            {
                //std::cout << "s" << m_edges[m_vertices[id_sommet_actuel].m_out[i]].m_to << " b1=" <<m_vertices[m_edges[m_vertices[id_sommet_actuel].m_out[i]].m_to].m_1<< " prst g=" <<m_vertices[m_edges[m_vertices[id_sommet_actuel].m_out[i]].m_to].m_prst_graph << std::endl;
                //m_adjacent contient indice du sommet adj
                ///8. enfile sommet(s) voisin(s) non marqué(s)
                file.push( m_edges[m_vertices[id_sommet_actuel].m_out[i]].m_to );

                ///9. marquage
                m_vertices[m_edges[m_vertices[id_sommet_actuel].m_out[i]].m_to].set_Bool1(true);
            }
        }
    }
}

/*Rechercher_connexes : calcul si quand on enlève un groupe de sommet (ou un sommet) le graphe n'est plus connecté
Entrée : /
Sortie : renvoit un booléen, true si il a plus plusieurs compo connexe (ce qui signifie que le uplet de sommet deconnecte le graphe) sinon retourne false
*/
/**
 * \fn bool Graph::Rechercher_connexes()
 * \brief Permet d'effectuer la recherche de composante connexe pour chaque sommet du graphe et retourne un booleen en fonction du nombre de composantes trouvees
 *
 * \param sommet_ancre Indice du sommet a partir duquel on commmence la recherche
 * \return true s'il y a plus de 2 composantes, sinon false
 */
bool Graph::Rechercher_connexes()
{
    //effacer marquage
    reset_marquages();

    //pour chaque sommets
    for(auto &elt : m_vertices)
    {
        //si  pas marque (pas ds compo connexe) et si present ds le graph
        if ( elt.second.m_1 == false && elt.second.m_prst_graph == true)
        {
            //enregistrer l'indice de ce sommet
            m_indice_sommet_ancre.push_back(elt.first);

            //marquer_composante (de ce sommet)
            Rechercher_connexe( elt.first );
        }
    }

    //s'il y a plus de 1 composante connexe alors le sommet que l'on testait permet bien de deconnecter le graphe
    if ( m_indice_sommet_ancre.size() > 1 )
    {
        std::cout << "Il y a " << m_indice_sommet_ancre.size() << " composantes connexes" <<std::endl;
        m_indice_sommet_ancre.clear();
        return true;
    }

    else
    {
        m_indice_sommet_ancre.clear();
        std::cout << "Il y a 1 composante connexe" << std::endl;
        return false;

    }
}

/*k_connexite = trouver le minimum de sommet à enlever pour déconnecter le graphe
Entrée : /
Sortie : Affichage en console des k_connexités
*/
/**
 * \fn void Graph::k_connexite()
 * \brief Recherche si le graphe se déconnecte lorsqu'on enleve un k-uplet de sommets et enregistre ces sommets
 */
void Graph::k_connexite()
{
    bool kmin_ok = false;
    int kmin=0;

    std::vector<std::vector<int>> combi;
    std::vector<std::vector<int>> k_uplet;

    ///  Pour chaque k de 0 à ordre et tant que kmin non trouvé
    for (unsigned int k  = 1; k < m_vertices.size() ; k++)
    {

        if ( kmin_ok == false)
        {
            ///    Pour chaque k-plet de sommets
            //calcul du coeff binomial et recup vecteur qui contient les conbinaisaons
            combi = Coeff_binomial(k);
            std::cout << "les combi :" << std::endl;

            for (unsigned int i =0 ; i < combi.size(); i++)
            {
                for (unsigned int j =0 ; j < combi[i].size(); j++)
                {
                    std::cout << combi[i][j] << " ";
                }
                std::cout << std::endl;
            }

            //pour chaque combinaison on met bool prst ds graphe à false
            for (unsigned int i =0 ; i < combi.size(); i++)
            {
                std::cout << "i=" <<i << std::endl;
                for (unsigned int j =0 ; j < combi[i].size(); j++)
                {
                    std::cout << "j=" <<j << std::endl;
                    m_vertices[ combi[i][j] ].set_prst_graph(false);

                }
                //calcul nb de connexite avec Rechercher_connexes
                bool many_compo = Rechercher_connexes();

                //si nb de connexite > 1
                //true a été retourné alors c'est que k-plet deconnecte le graph
                if ( many_compo == true )
                {
                    for (unsigned int j =0 ; j < combi[i].size(); j++)
                    {
                        m_vertices[ combi[i][j] ].set_deconnect(true);
                        std::cout << "indice sommet a true " << combi[i][j] << std::endl;

                    }

                    //Enregistrer ce k-plet de sommets
                    std::vector<int> v_temp1;
                    for (auto &elem: m_vertices)
                    {
                        if ( elem.second.m_prst_graph == false )
                        {
                            v_temp1.push_back( elem.first );
                            std::cout << "vtemp =" << elem.first << std::endl;
                        }
                    }
                    k_uplet.push_back(v_temp1);

                    v_temp1.clear();

                    //mets ds un vecteur de vecteur les sommets avec bool prst ds graph à true
                    //On rempli le vecteur avec les sommets qui reste apres avoir suppr le k-uplet qui deconnecte, c'est pour visualiser apres le graph simplifie
                    std::vector<Vertex> v_temp2;
                    for (auto &elem: m_vertices)
                    {
                        if ( elem.second.m_prst_graph == true )
                        {
                            v_temp2.push_back( elem.second );
                        }
                    }

                    //vecteur qui servira à afficher les composantes connexes sans les k-uplet du coup
                    m_connexe.push_back( v_temp2 );

                    v_temp2.clear();

                    for (unsigned int j =0 ; j < combi[i].size(); j++)
                    {
                        m_vertices[ combi[i][j] ].set_prst_graph(true);
                    }

                    for (auto &elem: m_vertices)
                    {
                        if ( elem.second.m_prst_graph == false )
                        {
                            std::cout<< "prst g = false  " << elem.first << std::endl;
                        }
                    }

                    /// Si le fait de neutraliser ces k sommet déconnecte le graphe alors
                    //kmin trouvé : kmin = k (mais on termine bien la boucle « Pour chaque k-plet... »)
                    kmin = k;
                    std::cout << "kmin ok" << std::endl;
                    kmin_ok = true;
                }
                for (unsigned int j =0 ; j < combi[i].size(); j++)
                {
                    m_vertices[ combi[i][j] ].set_prst_graph(true);
                }
            }
        }

    }

    std::cout << "Il y a " << k_uplet.size() << " " << kmin << "-uplet" << std::endl;

    for (unsigned int i =0 ; i < k_uplet.size(); i++)
    {
        std::cout << i+1 << "er/eme uplet : ";
        for (unsigned int j =0 ; j < k_uplet[i].size(); j++)
        {
            std::cout << k_uplet[i][j] << " ";
        }

        std::cout << std::endl;
    }
}

/* Coeff_binomial : retourne les différentes combinaisons de k_plets (source : http://www.commentcamarche.net/forum/affich-4567474-algo-affichage-combinaison-de-p-elemt-parmi-n)
Entrée : int k
Sortie : retourne les combinaisons de k_plets dans des vecteurs d'indices
*/
/**
 * \fn std::vector<std::vector<int>> Graph::Coeff_binomial(const int k)
 * \brief Calcul le coefficient binomial
 *
 * \param k nombre test qui va nous permettre de recuperer les differentes combinaisons de sommets
 * \return un vecteur de vecteur contenant toutes les combinaisons de sommets possible selon k
 */
std::vector<std::vector<int>> Graph::Coeff_binomial(const int k)
{
    //Initialisation des vecteurs qui vont contenir les indices
    std::vector<std::vector <int>> grogro;
    std::vector<int> lala;

    //Definition des variables
    const int n = m_vertices.size();
    int comb[40] = {0};
    int i = 0;


    while (i >= 0)
    {
        if (comb[i] < n + i - k + 1)
        {
            comb[i]++;
            if (i == k - 1)
            {
                //Pour tous les indices du k_plet
                for (int j = 0; j < k; j++)
                {
                    //On ajoute les indices dans un vecteur
                    lala.push_back(comb[j]-1);
                }
                //On ajoute le vecteur dans le vecteur de vecteur
                grogro.push_back(lala);
                //On vide le vecteur des indices
                lala.clear();

            }
            else
            {
                comb[++i] = comb[i - 1];
            }
        }
        else
            i--;
    }

    //On retourne le vecteur de vecteur d'indice
    return grogro;
}

