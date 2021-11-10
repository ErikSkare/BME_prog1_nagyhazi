#ifndef RENDELESEK_H
#define RENDELESEK_H
#include "menu.h"

/**
 * @param eleje: A láncolt lista elsõ elemére mutató pointer.
 * @param vege: A láncolt lista utolsó elemére mutató pointer.
 */
typedef struct Rendelesek {
    struct Rendeles *eleje;
    struct Rendeles *vege;
} Rendelesek;

/**
 * @param termek: A rendelt termék menüpontjára mutató pointer.
 * @param darab: Hány darab lett rendelve a termékbõl.
 * @param kov: A következõ rendelésre mutató pointer.
 */
typedef struct Rendeles {
    Menupont *termek;
    int darab;
    struct Rendeles *kov;
} Rendeles;

//<Memóriakezelõ függvények>
Rendeles *rendeles_foglal(Menupont *termek, int darab);
void rendelesek_felszabadit(const Rendelesek *rendelesek);

//<Hivatkozások>
extern int rendeles_hozzaad(int termek_azonosito, int darab, const Menu *menu, Rendelesek *rendelesek);

#endif
