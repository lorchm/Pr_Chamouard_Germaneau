#include "Interface.h"

void affichage_outil(BITMAP* buffer)
{
        BITMAP *supprimer, *ajouter, *G1, *G2, *G3, *exit;

        supprimer=load_bitmap("cross.bmp",NULL);
        ajouter=load_bitmap("plus.bmp",NULL);
        G1=load_bitmap("pics/G1.bmp",NULL);
        G2=load_bitmap("G2.bmp",NULL);
        G3=load_bitmap("G3.bmp",NULL);
        exit=load_bitmap("exit.bmp",NULL);

//        grman::WidgetImage g1("G1.jpg");
//         std::cout << "cc1" << std::endl;
//        g1.draw();
//masked_blit(G1,, 0, 0, 24, 6, 80, 80);

        // blit(G1,,0,0, 0, 0, 80, 80);
        //  blit(grman::page,screen,0,0,0, 0, 1024, 720);

        if (mouse_b)
        {
            std::cout << mouse_x <<" " << mouse_y;
            //if(mouse_x < & mouse_y)
        }

}

void suppr_espece()
{
}
