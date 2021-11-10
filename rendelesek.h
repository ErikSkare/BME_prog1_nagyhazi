#ifndef RENDELESEK_H
#define RENDELESEK_H
#include "menu.h"

/**
 * @param eleje: A l�ncolt lista els� elem�re mutat� pointer.
 * @param vege: A l�ncolt lista utols� elem�re mutat� pointer.
 */
typedef struct Rendelesek {
    struct Rendeles *eleje;
    struct Rendeles *vege;
} Rendelesek;

/**
 * @param termek: A rendelt term�k men�pontj�ra mutat� pointer.
 * @param darab: H�ny darab lett rendelve a term�kb�l.
 * @param kov: A k�vetkez� rendel�sre mutat� pointer.
 */
typedef struct Rendeles {
    Menupont *termek;
    int darab;
    struct Rendeles *kov;
} Rendeles;

//<Mem�riakezel� f�ggv�nyek>
Rendeles *rendeles_foglal(Menupont *termek, int darab);
void rendelesek_felszabadit(const Rendelesek *rendelesek);

//<Hivatkoz�sok>
extern int rendeles_hozzaad(int termek_azonosito, int darab, const Menu *menu, Rendelesek *rendelesek);

#endif
