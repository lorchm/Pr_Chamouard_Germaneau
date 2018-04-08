#include "bin.h"

/*********************
EDGE BIN
*********************/
//Constructeur surchargé
Edge_bin::Edge_bin(int a, int b, int c, int d)
:m_indice(a), m_vert1(b), m_vert2(c), m_weight(d)
{

}

//Destructeur
Edge_bin::~Edge_bin()
{

}

/************************
VERTEX BIN
************************/
//Constructeur surchargé
Vertex_bin::Vertex_bin(int a, int b, int c, int d, std::string e, int f, int g)
:m_indice(a), m_value(b), m_x(c), m_y(d), m_pic_name(e), m_pic_idx(f), m_taux_repro(g)
{

}

//Destructeur
Vertex_bin::~Vertex_bin()
{

}
