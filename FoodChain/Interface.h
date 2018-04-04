#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "grman/widget.h"
#include "Graph.h"


void affichage_outil();


void acces_G1(Graph& g);
void acces_G2(Graph& g);
void acces_G3(Graph& g);

void exit(Graph& g);

#endif // INTERFACE_H_INCLUDED
