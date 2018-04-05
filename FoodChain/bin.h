#ifndef BIN_H_INCLUDED
#define BIN_H_INCLUDED
#include <string>
using namespace std;

class Edge_bin{

public:
    int m_indice;
    int m_vert1;    //from
    int m_vert2;    //to
    int m_weight;

    Edge_bin(int a, int b, int c, int d);
    ~Edge_bin();
};

class Vertex_bin{

public:
    int m_indice; //a
    int m_value;    //b
    int m_x;    //c
    int m_y;    //d
    std::string m_pic_name; //e
    int m_pic_idx; //f

    Vertex_bin(int a, int b, int c, int d, std::string e, int f);
    ~Vertex_bin();

};
#endif // BIN_H_INCLUDED
