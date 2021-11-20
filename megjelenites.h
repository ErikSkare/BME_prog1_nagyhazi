#ifndef MEGJELENITES_H
#define MEGJELENITES_H
#include "funkciok.h"

typedef enum MenuAllapot {
    FOMENU = -1,
    MEGEROSITES = 0,
    ASZTAL_HOZZAAD = 1,
    ASZTAL_TOROL = 2,
    ASZTAL_MEGNYIT = 3,
    ASZTAL_LEZAR = 4,
    MENUPONT_HOZZAAD = 5,
    MENUPONT_TOROL = 6,
    RENDELES_FELVETEL = 7,
    KILEP = 8
} MenuAllapot;

//<Vez�rl� f�ggv�nyek>
MenuAllapot fomenu_vezerlo();
MenuAllapot asztal_hozzaad_vezerlo(Asztalok *asztalok);
MenuAllapot asztal_torol_vezerlo(Asztalok *asztalok);
MenuAllapot asztal_megnyit_vezerlo(Asztalok *asztalok);
MenuAllapot asztal_lezar_vezerlo(Asztalok *asztalok);
MenuAllapot menupont_hozzaad_vezerlo(Menu *menu);
MenuAllapot menupont_torol_vezerlo(Menu *menu, const Asztalok *asztalok);
MenuAllapot rendeles_felvetel_vezerlo(const Menu *menu, const Asztalok *asztalok);

//<Seg�d f�ggv�nyek>
static int szam_beolvas(int *hova);
static MenuAllapot megerosites(MenuAllapot kovetkezo);
static MenuAllapot hibauzenet(MenuAllapot honnan, char *uzenet);
static MenuAllapot siker(char *uzenet);

#endif