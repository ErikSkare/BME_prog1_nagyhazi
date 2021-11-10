#ifndef FUNKCIOK_H
#define FUNKCIOK_H
#include "asztalok.h"

//<Asztalkezelõ függvények>
void asztal_hozzaad(struct Pozicio pozicio, enum Statusz statusz, int ferohely, Asztalok *asztalok);
Asztal *asztal_keres(int azonosito, const Asztalok *asztalok);

//<Menükezelõ függvények>
void menupont_hozzaad(int ar, char *nev, Menu *menu);
Menupont *menupont_keres(int azonosito, const Menu *menu);

//<Rendeléskezelõ függvények>
int rendeles_hozzaad(int termek_azonosito, int darab, const Menu *menu, Rendelesek *rendelesek);

#endif
