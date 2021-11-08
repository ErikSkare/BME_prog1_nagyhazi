#include <stdio.h>
#include "menu.h"
#include "deps/debugmalloc.h"

/**
 * @brief Dinamikusan lefoglal egy men�pontot az adatokkal.
 *
 * @param azonosito: A men�pont azonos�t�ja.
 * @param ar: A men�pont �ra.
 * @param nev: A men�pont neve.
 *
 * @return Visszaadja a dinamikusan lefoglalt men�pont c�m�t.
 *         A h�v� felel�ss�ge a felszabad�t�s!
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
 * @brief Felszabad�tja a men� elemeit.
 *
 * @param menu: A men� strukt�r�ra mutat� pointer.
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
 * @brief Beolvassa a men�t egy megadott f�jlb�l.
 *
 * @param fajl: A f�jl �tvonal�t tartalmaz� sztring.
 * @param menu: A men� strukt�r�ra mutat� pointer.
 *
 * @return Param�terlist�n be�ll�tja a menu �rt�k�t.
 *         Visszat�r�si �rt�ke 1, ha nem siker�lt megnyitni a f�jlt.
 *         0, ha siker�lt.
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
 * @brief Ki�rja a men�t egy megadott f�jlba.
 *
 * @param fajl: A f�jl �tvonal�t tartalmaz� sztring.
 * @param menu: A men� strukt�r�ra mutat� pointer.
 *
 * @return Visszat�r�si �rt�ke 1, ha nem siker�lt megnyitni a f�jlt.
 *         0, ha siker�lt.
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
 * @brief Hozz�adja a l�ncolt list�hoz a men�pontot,
 *        amelyet egy sztringb�l k�pez.
 *
 * @param sor: A men�pont adatait tartalmaz� sztring.
 * @param menu: A men� strukt�r�ra mutat� pointer.
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
 * @brief Hozz�adja a l�ncolt list�hoz a men�pontot,
 *        a megadott adatokkal.
 *
 * Megfelel�en be�ll�tja a men�pont index�t a k�vetkez�re.
 *
 * @param ar: A men�pont �ra.
 * @param nev: A men�pont neve.
 * @param menu: A men� strukt�r�ra mutat� pointer.
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
 * @brief Megkeres egy men�pontot a megadott azonos�t�val.
 *
 * @param azonosito: A keresett men�pont azonos�t�ja.
 * @param menu: A men� strukt�r�ra mutat� pointer.
 *
 * @return Visszaadja a keresett men�pont c�m�t, vagy NULL-t,
 *         ha nem tal�lt semmit.
 */
Menupont *menupont_keres(int azonosito, const Menu *menu) {
    Menupont *aktualis = menu->eleje;
    while(aktualis != NULL && aktualis->azonosito != azonosito)
        aktualis = aktualis->kov;
    return aktualis;
}
