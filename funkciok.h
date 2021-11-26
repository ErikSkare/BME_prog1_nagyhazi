#ifndef FUNKCIOK_H
#define FUNKCIOK_H
#include <stdio.h>
#include "asztalok.h"

//<Asztalkezelõ függvények>
int asztal_hozzaad(struct Pozicio pozicio, enum Statusz statusz, int ferohely, Asztalok *asztalok);
Asztal *asztal_keres(int azonosito, const Asztalok *asztalok);
int asztal_megnyit(int azonosito, Asztalok *asztalok);
int asztal_lezar(int azonosito, Asztalok *asztalok);
int asztal_torol(int azonosito, Asztalok *asztalok);

//<Menükezelõ függvények>
int menupont_hozzaad(int ar, char *nev, Menu *menu);
Menupont *menupont_keres(int azonosito, const Menu *menu);
int menupont_torol(int azonosito, Menu *menu, const Asztalok *asztalok);

//<Rendeléskezelõ függvények>
int rendeles_hozzaad(int termek_azonosito, int darab, const Menu *menu, Rendelesek *rendelesek);

//<Fájlkezelő függvények>
char *din_sor_beolvas(FILE *fp);

#endif
