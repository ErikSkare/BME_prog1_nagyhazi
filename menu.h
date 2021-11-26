#ifndef MENU_H
#define MENU_H

/**
 * @param eleje: A láncolt lista elsõ elemére mutató pointer.
 * @param vege: A láncolt lista utolsó elemére mutató pointer.
 */
typedef struct Menu {
    struct Menupont *eleje, *vege;
} Menu;

/**
 * @param azonosito: A menüpont azonosítója 0-tól számozva.
 * @param ar: A menüpont ára.
 * @param nev: A menüpont neve.
 * @param kov: A következõ menüpontra mutató pointer.
 */
typedef struct Menupont {
    int azonosito, ar;
    char *nev;
    struct Menupont *kov;
} Menupont;

//<Memóriakezelõ függvények>
Menupont *menupont_foglal(int azonosito, int ar, char *nev);
void menu_felszabadit(const Menu *menu);

//<Fájlkezelõ függvények>
int menu_beolvas(char *fajl, Menu *menu);
int menu_kiir(char *fajl, const Menu *menu);

//<Hivatkozások>
extern int menupont_hozzaad(int ar, char *nev, Menu *menu);
extern char *din_sor_beolvas(FILE *fp);

#endif
