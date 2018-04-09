#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <string>
#include <iostream>
#include <regex>


#include "grman.h"
#include <allegro.h>
#include <png.h>
#include <loadpng.h>
#include <jpgalleg.h>

/**
*\file grman.cpp
*\brief Contient les fonctions graphique du namespace german
*\author Monsieur Fercoq
*\date 2 Avril 2018
*/

/**
* \namespace <grman>
* \brief bibliotheque des fonctions graphique/widgets
*/
namespace grman
{

// Permet de compiler en mode plein écran (décommenter)
#define MODE_GRAPHIQUE GFX_AUTODETECT_WINDOWED
//#define MODE_GRAPHIQUE GFX_AUTODETECT_FULLSCREEN

// Nombre de pixels horizontalement et verticalement
#define LARGEURECRAN 1024
#define HAUTEURECRAN 768

// Couleur initiale :
#define PAGE_COULEUR_INIT BLANC

// Temporisation de la boucle de jeu en ms ( rest dans mettre_a_jour )
#define TEMPO_MAJ 20

int page_color=PAGE_COULEUR_INIT;

BITMAP *page=NULL;

char key_last;
int mouse_click;
int mouse_unclick;
int key_press[KEY_MAX];
int key_unpress[KEY_MAX];

int mouse_click_x;
int mouse_click_y;

int mouse_move_x;
int mouse_move_y;

Coords mouse_pos;

Frame page_frame;
Widget *gui_over = nullptr;
Widget *gui_last_over = nullptr;
Widget *gui_focus = nullptr;
Widget *gui_leave = nullptr;


/// Gestion des contenus graphiques (fichiers images)
std::unordered_map<std::string, BITMAP *> g_pic_names;
std::unordered_set<std::string> g_failed_pic_names;
std::string g_pic_path;

/**
 *\fn unsigned get_picture_nb(std::string name)
 *\brief retourne le nombre d'une image
 * \param name Le nom de l'image
 */
unsigned get_picture_nb(std::string name)
{
    std::string delim="xx";
    std::smatch m;

    if ( std::regex_search(name, m, std::regex(delim+"([0-9]+)"+delim)) )
    {
        unsigned nb = strtoul(m[1].str().c_str(), NULL, 10);
        return nb ? nb : 1;
    }

    return 1;
}

/**
 *\fn BITMAP *get_picture(std::string pic_name)
 *\brief retourne l'image d'un sommet
 * \param pic_name le nom de l'image
 */
BITMAP *get_picture(std::string pic_name)
{
    pic_name = g_pic_path + pic_name;

    BITMAP *pic = nullptr;
    auto pr = g_pic_names.find(pic_name);
    if ( pr!=g_pic_names.end() )
        pic = pr->second;
    else
    {
        pic = load_bitmap(pic_name.c_str(), NULL);
        if (pic)
        {
            g_pic_names.insert( std::make_pair(pic_name, pic) );
            std::cout << "Loaded " << pic_name << std::endl;
        }
        else
        {
            auto it = g_failed_pic_names.find(pic_name);
            if ( it==g_failed_pic_names.end() )
            {
                g_failed_pic_names.insert( pic_name );
                std::cout << "COULD NOT LOAD " << pic_name << std::endl;
            }
            return nullptr;
        }
    }

    return pic;
}

/**
 *\fn void show_picture(BITMAP *dest, std::string pic_name, int x, int y, unsigned pic_idx)
 *\brief permet d'afficher une image sur la fenetre graphique
 * \param dest : la ou sera afficher l'image, pic_name : nom de l'image, x et y : position à laquelle on souhaite l'afficher, pic_idx : nombre d'image de l'animation
 */
void show_picture(BITMAP *dest, std::string pic_name, int x, int y, unsigned pic_idx)
{
    BITMAP *pic =get_picture(pic_name);
    if (!pic)
        return;

    unsigned nb = get_picture_nb(pic_name);
    if (nb==1)
        draw_sprite(dest, pic, x, y);
    else
        masked_blit(pic, dest, (pic_idx%nb)*pic->w/nb, 0, x, y, pic->w/nb, pic->h);

}

/**
 *\fn void set_pictures_path(std::string path_name)
 *\brief retrouve le chemin des images
 * \param path_name : nom du chemin
 */
void set_pictures_path(std::string path_name)
{
    if (path_name[path_name.length()-1]!='/')
        path_name+="/";

    g_pic_path = path_name;
}

/**
 *\fn void mettre_a_jour()
 *\brief raffraichit les images
 */
void mettre_a_jour()
{
    buf_afficher_page();
    rest(TEMPO_MAJ);

    rafraichir_clavier_souris();
    buf_effacer_page();

    /// mouse_unclick && !gui_grabbed  ||  new click select || capture left (key ENTER or KEY ESC) ?
    if (mouse_unclick)
    {
        gui_leave = gui_focus;
        gui_focus = nullptr;
    }
    else
        gui_leave = nullptr;

    gui_last_over = gui_over;
    gui_over = gui_focus;


}


/**
 *\fn void init()
 *\brief initialisation des bibliothèques graphiques
 */
void init()
{
    if (page) return;

    srand(time(NULL));

    /// Gestion des caractères accentués
    set_uformat(U_ASCII);

    allegro_init();
    install_timer();
    install_keyboard();
    set_keyboard_rate(0,0);
    install_mouse();

    register_png_file_type();
    jpgalleg_init();

    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(MODE_GRAPHIQUE,LARGEURECRAN,HAUTEURECRAN,0,0)!=0)
    {
        if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0)!=0)
        {
            allegro_message("prb gfx mode");
            allegro_exit();
            exit(EXIT_FAILURE);
        }
    }
    set_display_switch_mode(SWITCH_BACKGROUND);

    show_mouse(screen);

    page=create_bitmap(SCREEN_W,SCREEN_H);

    rafraichir_clavier_souris();
    buf_effacer_page();
    buf_afficher_page();

    page_frame.pos.x = 0;
    page_frame.pos.y = 0;
    page_frame.dim.x = SCREEN_W;
    page_frame.dim.y = SCREEN_H;
}

/**
 *\fn void fermer_allegro()
 *\brief permet de quitter la fenetre graphique
 */
void fermer_allegro()
{
    if (!page) return;
    destroy_bitmap(page);
    page=NULL;
    allegro_exit();
}

/**
 *\fn void buf_effacer_page()
 *\brief efface le buffer pour eviter les problemes graphiques
 */
void buf_effacer_page()
{
    if (!page) return;
    clear_to_color(page, page_color);
}

/**
 *\fn void buf_afficher_page()
 *\brief affiche la bitmap page, sur laquelle on affiche toute l'interface graphique
 */
void buf_afficher_page()
{
    if (!page) return;
    acquire_screen();
    blit(page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    release_screen();
}

/**
 *\fn void rafraichir_clavier_souris()
 *\brief raffraichit la souris et le clavier pour éviter des problèmes
 */
void rafraichir_clavier_souris()
{
    static int mouse_prev, mouse_now;
    static int key_prev[KEY_MAX], key_now[KEY_MAX];
    int k;

    mouse_prev = mouse_now;
    mouse_now = mouse_b;
    mouse_click = mouse_now & ~mouse_prev;
    mouse_unclick = ~mouse_now & mouse_prev;

    mouse_pos = Coords(mouse_x, mouse_y);

    for (k=0; k<KEY_MAX; k++)
    {
        key_prev[k] = key_now[k];
        key_now[k] = key[k];
        key_press[k] = key_now[k] && !key_prev[k];
        key_unpress[k] = !key_now[k] && key_prev[k];
    }

    key_last = '\0';
    while (keypressed())
        key_last = (char)readkey();

    get_mouse_mickeys(&mouse_move_x, &mouse_move_y);

    if (mouse_click)
    {
        mouse_click_x = mouse_x;
        mouse_click_y = mouse_y;
    }
}


/// Auxiliaires : compléments aux fonctions graphiques allegro
/**
 *\fn void thick_line(BITMAP *bmp, int x1, int y1, int x2, int y2, int thickness, int color)
 *\brief dessine une ligne
 *\param bmp : la ou elle va etre dessiner, x1 et y1 : coordonnees d'une extremite de la ligne, x2 et y2 : coordonnees de l'autre extremite, thickness : épaisseur de la ligne, color : sa couleur
 */
void thick_line(BITMAP *bmp, int x1, int y1, int x2, int y2, int thickness, int color)
{
    int dx=x2-x1;
    int dy=y2-y1;

    thickness = std::max(thickness, 1);
    if ( abs(dx) > abs(dy) )
        for (int i=1-thickness; i<thickness; ++i)
            line(bmp, x1, y1+i, x2, y2+i, color);
    else
        for (int i=1-thickness; i<thickness; ++i)
            line(bmp, x1+i, y1, x2+i, y2, color);
}


} /// FIN DE NAMESPACE GRMAN


