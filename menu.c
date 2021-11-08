#include <stdio.h>
#include "menu.h"
#include "deps/debugmalloc.h"

/**
 * @brief Dinamikusan lefoglal egy menüpontot az adatokkal.
 *
 * @param azonosito: A menüpont azonosítója.
 * @param ar: A menüpont ára.
 * @param nev: A menüpont neve.
 *
 * @return Visszaadja a dinamikusan lefoglalt menüpont címét.
 *         A hívó felelõssége a felszabadítás!
 */
Menupont *menupont_foglal(int azonosito, int ar, char *nev) {
    Menupont *uj = (Menupont*) malloc(sizeof(Menupont));
    strcpy(uj->nev, nev);
    uj->azonosito = azonosito;
    uj->ar = ar;
    uj->kov = NULL;
    return uj;
}

/**
 * @brief Felszabadítja a menü elemeit.
 *
 * @param menu: A menü struktúrára mutató pointer.
 */
void menu_felszabadit(const Menu *menu) {
    Menupont *aktualis = menu->eleje;
    while(aktualis != NULL) {
        Menupont *kov = aktualis->kov;
        free(aktualis);
        aktualis = kov;
    }
}

/**
 * @brief Beolvassa a menüt egy megadott fájlból.
 *
 * @param fajl: A fájl útvonalát tartalmazó sztring.
 * @param menu: A menü struktúrára mutató pointer.
 *
 * @return Paraméterlistán beállítja a menu értékét.
 *         Visszatérési értéke 1, ha nem sikerült megnyitni a fájlt.
 *         0, ha sikerült.
 */
int menu_beolvas(char *fajl, Menu *menu) {
    FILE *fp = fopen(fajl, "r");
    if(fp == NULL)
        return 1;

    *menu = (Menu) {NULL, NULL};
    char buffer[255];
    while(fgets(buffer, 255, fp) != NULL)
        menu_sor_hozzaad(buffer, menu);

    fclose(fp);
    return 0;
}

/**
 * @brief Kiírja a menüt egy megadott fájlba.
 *
 * @param fajl: A fájl útvonalát tartalmazó sztring.
 * @param menu: A menü struktúrára mutató pointer.
 *
 * @return Visszatérési értéke 1, ha nem sikerült megnyitni a fájlt.
 *         0, ha sikerült.
 */
int menu_kiir(char *fajl, const Menu *menu) {
    FILE *fp = fopen(fajl, "w");
    if(fp == NULL)
        return 1;

    Menupont *aktualis = menu->eleje;
    while(aktualis != NULL) {
        fprintf(fp, "%s;%d\n", aktualis->nev, aktualis->ar);
        aktualis = aktualis->kov;
    }

    fclose(fp);
    return 0;
}

/**
 * @brief Hozzáadja a láncolt listához a menüpontot,
 *        amelyet egy sztringbõl képez.
 *
 * @param sor: A menüpont adatait tartalmazó sztring.
 * @param menu: A menü struktúrára mutató pointer.
 */
static void menu_sor_hozzaad(char *sor, Menu *menu) {
    char *resz, nev[50 + 1];
    int ar;
    resz = strtok(sor, ";");
    strcpy(nev, resz);
    resz = strtok(NULL, ";");
    sscanf(resz, "%d", &ar);
    menupont_hozzaad(ar, nev, menu);
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
 */
void menupont_hozzaad(int ar, char *nev, Menu *menu) {
    Menupont *uj;
    if(menu->eleje == NULL && menu->vege == NULL) {
        uj = menupont_foglal(0, ar, nev);
        menu->eleje = uj;
    } else {
        uj = menupont_foglal(menu->vege->azonosito + 1, ar, nev);
        menu->vege->kov = uj;
    }
    menu->vege = uj;
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
