#include <stdio.h>
#include "funkciok.h"

/**
 * @brief Hozzáad egy asztalt a láncolt listához.
 *
 * Megfelelõen beállítja az asztal indexét a következõre.
 *
 * @param pozicio: Az asztal pozíciója az étteremben.
 * @param statusz: Az asztal státusza (szabad vagy foglalt).
 * @param ferohely: Az asztal helyeinek száma.
 * @param asztalok: Az asztalok struktúrára mutató pointer.
 *
 * @return Visszatérési értéke 1, ha nem sikerült hozzáadni az asztalt.
 *         0, ha sikerült.
 */
int asztal_hozzaad(struct Pozicio pozicio, enum Statusz statusz, int ferohely, Asztalok *asztalok) {
    Asztal *uj;
    if(asztalok->eleje == NULL && asztalok->vege == NULL) {
        uj = asztal_foglal(0, pozicio, statusz, ferohely);
        if(uj == NULL)
            return 1;
        asztalok->eleje = uj;
    } else {
        uj = asztal_foglal(asztalok->vege->azonosito + 1, pozicio, statusz, ferohely);
        if(uj == NULL)
            return 1;
        asztalok->vege->kov = uj;
    }
    asztalok->vege = uj;
    return 0;
}

/**
 * @brief Megkeres egy asztalt a láncolt listában.
 *
 * @param azonosito: A keresett asztal azonosítója.
 * @param asztalok: Az asztalok struktúrára mutató pointer.
 *
 * @return Visszaadja a keresett asztal címét, vagy NULL-t,
 *         ha nem talált semmit.
 */
Asztal *asztal_keres(int azonosito, const Asztalok *asztalok) {
    Asztal *aktualis = asztalok->eleje;
    while(aktualis != NULL && aktualis->azonosito != azonosito)
        aktualis = aktualis->kov;
    return aktualis;
}

/**
 * @brief Hozzáadja a láncolt listához a menüpontot,
 *        a megadott adatokkal.
 *
 * Megfelelõen beállítja a menüpont indexét a következõre.
 *
 * @param ar: A menüpont ára.
 * @param nev: A menüpont neve.
 * @param menu: A menü struktúrára mutató pointer.
 *
 * @return Visszatérési értéke 0, ha sikerült a hozzáadás,
 *         1, ha nem sikerült.
 */
int menupont_hozzaad(int ar, char *nev, Menu *menu) {
    Menupont *uj;
    if(menu->eleje == NULL && menu->vege == NULL) {
        uj = menupont_foglal(0, ar, nev);
        if(uj == NULL)
            return 1;
        menu->eleje = uj;
    } else {
        uj = menupont_foglal(menu->vege->azonosito + 1, ar, nev);
        if(uj == NULL)
            return 1;
        menu->vege->kov = uj;
    }
    menu->vege = uj;
    return 0;
}

/**
 * @brief Megkeres egy menüpontot a megadott azonosítóval.
 *
 * @param azonosito: A keresett menüpont azonosítója.
 * @param menu: A menü struktúrára mutató pointer.
 *
 * @return Visszaadja a keresett menüpont címét, vagy NULL-t,
 *         ha nem talált semmit.
 */
Menupont *menupont_keres(int azonosito, const Menu *menu) {
    Menupont *aktualis = menu->eleje;
    while(aktualis != NULL && aktualis->azonosito != azonosito)
        aktualis = aktualis->kov;
    return aktualis;
}

/**
 * @brief Hozzáad egy rendelést a láncolt listához.
 *
 * @param termek_azonosito: A termék azonosítója, amelyet hozzá szeretnénk adni.
 * @param darab: Ahány darabot szeretnénk rendelni a termékbõl.
 * @param menu: A menü struktúrára mutató pointer.
 * @param rendelesek: A rendelések struktúrára mutató pointer.
 *
 * @return Visszatérési értéke 1, ha nem sikerült felvenni a rendelést.
 *         0, ha sikeres volt a felvétel.
 */
int rendeles_hozzaad(int termek_azonosito, int darab, const Menu *menu, Rendelesek *rendelesek) {
    Menupont *termek = menupont_keres(termek_azonosito, menu);
    if(termek == NULL)
        return 1;

    Rendeles *uj = rendeles_foglal(termek, darab);
    if(uj == NULL)
        return 1;
    if(rendelesek->eleje == NULL && rendelesek->vege == NULL)
        rendelesek->eleje = uj;
    else
        rendelesek->vege->kov = uj;

    rendelesek->vege = uj;
    return 0;
}
