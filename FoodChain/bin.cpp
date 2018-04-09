#include "bin.h"

/**
*\file bin.cpp
*\brief Contient les constructeurs surcharg�s et destructeurs des classes Vertex_bin et Edge_bin.
*\author Laure Chamouard et Barbara Germaneau
*\date 5 Avril 2018
*/

/*********************
EDGE BIN
*********************/

/**
 * \fn Edge_bin(int a, int b, int c, int d)
 *\brief Fonction permettant d'instancier un objet de la classe Edge_bin, il s'agit d'un constructeur surcharg�.
 * \param a Indice de l'arc, b indice du sommet d'o� provient l'arc, c indice du sommet o� va l'arc et d le poids de l'arc
 */
//Constructeur surcharg�
Edge_bin::Edge_bin(int a, int b, int c, int d)
:m_indice(a), m_vert1(b), m_vert2(c), m_weight(d)
{

}

/**
 *\fn ~Edge_bin()
 * \brief Fonction permettant de d�truire un objet de la classe Edge_bin.
 */
//Destructeur
Edge_bin::~Edge_bin()
{

}

/************************
VERTEX BIN
************************/


/**
 *\fn Vertex_bin(int a, int b, int c, int d, std::string e, int f, int g)
 * \brief Fonction permettant d'instancier un objet de la classe Vertex_bin, il s'agit d'un constructeur surcharg�.
 * \param a Indice du sommet, b taille de la population, c coordonn�e x de la box contentant les informations du sommet
 * d coordonn�e y de la box, e nom de l'image dans le fichier .txt, f nbr d'image de l'animation, g coefficient de reproduction du sommet
 */
//Constructeur surcharg�
Vertex_bin::Vertex_bin(int a, int b, int c, int d, std::string e, int f, int g)
:m_indice(a), m_value(b), m_x(c), m_y(d), m_pic_name(e), m_pic_idx(f), m_taux_repro(g)
{

}


/**
 *\fn ~Vertex_bin()
 * \brief Fonction permettant de d�truire un objet de la classe Vertex_bin.
 */
//Destructeur
Vertex_bin::~Vertex_bin()
{

}
