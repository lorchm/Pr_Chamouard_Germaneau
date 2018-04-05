#include "bin.h"

Edge_bin::Edge_bin(int a, int b, int c, int d)
:m_indice(a), m_vert1(b), m_vert2(c), m_weight(d)
{

}

Edge_bin::~Edge_bin()
{

}

Vertex_bin::Vertex_bin(int a, int b, int c, int d, std::string e, int f)
:m_indice(a), m_value(b), m_x(c), m_y(d), m_pic_name(e), m_pic_idx(f)
{

}

Vertex_bin::~Vertex_bin()
{

}
