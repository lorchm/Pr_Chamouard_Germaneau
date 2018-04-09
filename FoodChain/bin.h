#ifndef BIN_H_INCLUDED
#define BIN_H_INCLUDED
#include <string>
using namespace std;

/**
*\file bin.h
*\brief Contient nos classes permettant de gérer les sommets et arêtes associées supprimés du graphe mais aussi de mieux les gérer lors de l'ajout
*\author Laure Chamouard et Barbara Germaneau
*\date 5 Avril 2018
*/

/**
*\class <Edge_bin>
*
* Cette classe nous permet de manipuler plus facilement les arêtes supprimer pour ensuite les ajouter au graphe
*/
class Edge_bin{

    public:

        ///Attributs
        int m_indice;
        int m_vert1;    //from
        int m_vert2;    //to
        int m_weight;

        ///Constructeur/Destructeur
        Edge_bin(int a, int b, int c, int d);
        ~Edge_bin();
};

/**
*\class <Vertex_bin>
*
* Cette classe nous permet de manipuler plus facilement les sommets supprimer pour ensuite les ajouter au graphe
*/
class Vertex_bin{

    public:

        ///Attributs
        int m_indice; //a
        int m_value;    //b
        int m_x;    //c
        int m_y;    //d
        std::string m_pic_name; //e
        int m_pic_idx; //f
        int m_taux_repro;

        ///Constructeur/Destructeur
        Vertex_bin(int a, int b, int c, int d, std::string e, int f, int g);
        ~Vertex_bin();

};
#endif // BIN_H_INCLUDED
