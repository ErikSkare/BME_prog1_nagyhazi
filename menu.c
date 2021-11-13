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
 * @return Visszaadja a dinamikusan lefoglalt men�pont c�m�t, vagy NULL-t,
 *         ha nem siker�lt a foglal�s. A h�v� felel�ss�ge a felszabad�t�s!
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
 *         2, ha nem siker�lt a list�hoz hozz�ad�s.
 *         0, ha siker�lt a beolvas�s.
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
 *
 * @return Visszat�r�si �rt�ke a men� hozz�ad�s sikeress�ge.
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
