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
 * @return Visszaadja a dinamikusan lefoglalt menüpont címét, vagy NULL-t,
 *         ha nem sikerült a foglalás. A hívó felelõssége a felszabadítás!
 */
Menupont *menupont_foglal(int azonosito, int ar, char *nev) {
    Menupont *uj = (Menupont*) malloc(sizeof(Menupont));
    if(uj == NULL)
        return NULL;
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
 *         2, ha nem sikerült a listához hozzáadás.
 *         0, ha sikerült a beolvasás.
 */
int menu_beolvas(char *fajl, Menu *menu) {
    FILE *fp = fopen(fajl, "r");
    *menu = (Menu) {NULL, NULL};
    if(fp == NULL)
        return 1;

    char buffer[255];
    while(fgets(buffer, 255, fp) != NULL)
        if(menu_sor_hozzaad(buffer, menu) == 1)
            return 2;

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
 *
 * @return Visszatérési értéke a menü hozzáadás sikeressége.
 */
static int menu_sor_hozzaad(char *sor, Menu *menu) {
    char *resz, nev[50 + 1];
    int ar;
    resz = strtok(sor, ";");
    strcpy(nev, resz);
    resz = strtok(NULL, ";");
    sscanf(resz, "%d", &ar);
    return menupont_hozzaad(ar, nev, menu);
}
