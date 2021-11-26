#ifndef MENU_H
#define MENU_H

/**
 * @param eleje: A l�ncolt lista els� elem�re mutat� pointer.
 * @param vege: A l�ncolt lista utols� elem�re mutat� pointer.
 */
typedef struct Menu {
    struct Menupont *eleje, *vege;
} Menu;

/**
 * @param azonosito: A men�pont azonos�t�ja 0-t�l sz�mozva.
 * @param ar: A men�pont �ra.
 * @param nev: A men�pont neve.
 * @param kov: A k�vetkez� men�pontra mutat� pointer.
 */
typedef struct Menupont {
    int azonosito, ar;
    char *nev;
    struct Menupont *kov;
} Menupont;

//<Mem�riakezel� f�ggv�nyek>
Menupont *menupont_foglal(int azonosito, int ar, char *nev);
void menu_felszabadit(const Menu *menu);

//<F�jlkezel� f�ggv�nyek>
int menu_beolvas(char *fajl, Menu *menu);
int menu_kiir(char *fajl, const Menu *menu);

//<Hivatkoz�sok>
extern int menupont_hozzaad(int ar, char *nev, Menu *menu);
extern char *din_sor_beolvas(FILE *fp);

#endif
