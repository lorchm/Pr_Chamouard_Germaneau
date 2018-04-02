#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main()
{


    std::cout << "Coucou Barbara, Coucou Laure!" << std::endl << std::endl << "Ce programme determine qui est la meilleure des deux" << std::endl << std::endl;

    srand(time(NULL));

    int i = rand()%200;

    if(i%2)
    {
        std::cout << "Lor c la best" << std::endl;
    }
    else
    {
        std::cout << "Barbara est un peu superieur mais pas trop quand meme" << std::endl;
    }


    return 0;
}
