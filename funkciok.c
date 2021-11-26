#include "funkciok.h"
#include "deps/debugmalloc.h"

static int rendeles_menuponttal(int azonosito, const Asztalok *asztalok);

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
 * @brief Megnyitja az adott azonosítójú asztalt.
 *
 * @param azonosito: A megnyitandó asztal azonosítója.
 * @param asztalok: Az asztalok struktúrára mutató pointer.
 *
 * @return Visszatérési értéke 1, ha az adott azonosítóval nem található asztal.
 *         0, ha sikerült megnyitni az asztalt.
 */
int asztal_megnyit(int azonosito, Asztalok *asztalok) {
    Asztal *megnyitando = asztal_keres(azonosito, asztalok);
    if(megnyitando == NULL)
        return 1;
    megnyitando->statusz = FOGLALT;
    return 0;
}

/**
 * @brief Lezárja az adott azonosítójú asztalt.
 *
 * A státuszát SZABAD-ra állítja, és törli a hozzá tartozó megrendeléseket.
 *
 * @param azonosito: A lezárandó asztal azonosítója.
 * @param asztalok: Az asztalok struktúrára mutató pointer.
 *
 * @return Visszatérési értéke 1, ha az adott azonosítóval nem található asztal.
 *         0, ha sikerült lezárni az asztalt.
 */
int asztal_lezar(int azonosito, Asztalok *asztalok) {
    Asztal *lezarando = asztal_keres(azonosito, asztalok);
    if(lezarando == NULL)
        return 1;
    lezarando->statusz = SZABAD;
    for(int i = 0; i < lezarando->ferohely; ++i) {
        rendelesek_felszabadit(&lezarando->hely_rendelesek[i]);
        lezarando->hely_rendelesek[i] = (Rendelesek) {NULL, NULL};
    }
    return 0;
}

/**
 * @brief Töröl egy asztalt a láncolt listából.
 *
 * Továbbá csökkenti az utána következő asztalok azonosítóját 1-el.
 *
 * @param azonosito: A törölendő asztal azonosítója.
 * @param asztalok: Az asztalok struktúrára mutató pointer.
 *
 * @return Visszatérési értéke 1, ha nem található az asztal, vagy foglalt a státusza.
 *         0, ha sikerült a törlés.
 */
int asztal_torol(int azonosito, Asztalok *asztalok) {
    Asztal *torolendo = asztal_keres(azonosito, asztalok);
    if(torolendo == NULL || torolendo->statusz == FOGLALT)
        return 1;

    //A következő asztalok azonosítójának csökkentése.
    Asztal *futo = torolendo->kov;
    while(futo != NULL) {
        --futo->azonosito;
        futo = futo->kov;
    }

    //Törlés a listából
    Asztal *elotte = asztal_keres(azonosito - 1, asztalok);
    if(elotte != NULL)
        elotte->kov = torolendo->kov;
    else
        asztalok->eleje = torolendo->kov;
    if(torolendo->kov == NULL)
        asztalok->vege = elotte;

    //Felszabadítás
    for(int i = 0; i < torolendo->ferohely; ++i)
        rendelesek_felszabadit(&torolendo->hely_rendelesek[i]);
    free(torolendo->hely_rendelesek);
    free(torolendo);
    return 0;
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
 * @brief Töröl egy menüpontot a megadott azonosítóval.
 *
 * Továbbá csökkenti az utána következő menüpontok azonosítóját 1-el.
 *
 * @param azonosito: A törölendő menüpont azonosítója.
 * @param menu: A menü struktúrára mutató pointer.
 * @param asztalok: Az asztalok struktúrára mutató pointer.
 *
 * @return Visszatérési értéke 1, ha nem található a megadott azonosítóval menüpont,
 *         vagy, ha van a megadott azonosítóval nyilvántartott rendelés.
 *         0, ha sikerült a törlés.
 */
int menupont_torol(int azonosito, Menu *menu, const Asztalok *asztalok) {
    Menupont *torolendo = menupont_keres(azonosito, menu);
    if(torolendo == NULL || rendeles_menuponttal(azonosito, asztalok) == 1)
        return 1;

    //A következő menüpontok azonosítójának csökkentés
    Menupont *futo = torolendo->kov;
    while(futo != NULL) {
        --futo->azonosito;
        futo = futo->kov;
    }

    //Törlés a listából
    Menupont *elotte = menupont_keres(azonosito - 1, menu);
    if(elotte != NULL)
        elotte->kov = torolendo->kov;
    else
        menu->eleje = torolendo->kov;
    if(torolendo->kov == NULL)
        menu->vege = elotte;

    //Felszabadítás
    free(torolendo);
    return 0;
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

/**
 * @brief Dinamikusan beolvas egy sort a megadott fájlból.
 *
 * @param fp: A fájlra mutató pointer.
 *
 * @return Visszaadja a dinamikusan foglalt sorra mutató pointer.
 *         A hívó felelőssége a felszabadítás!
 */
char *din_sor_beolvas(FILE *fp) {
    char *sor = NULL;
    int ch, hossz = 1;

    while((ch = getc(fp)) != EOF && ch != '\n') {
        sor = (char*) realloc(sor, (hossz + 1) * sizeof(char));
        sor[hossz - 1] = ch;
        hossz++;
    }

    if(ch == EOF)
        return NULL;
    sor[hossz - 1] = '\0';
    return sor;
}

/**
 * @brief Megmondja, hogy van-e nyilvántartott rendelés az adott menüpont azonosítóval.
 *
 * @param azonosito: A menüpont azonosítója.
 * @param asztalok: Az asztalok struktúrára mutató pointer.
 *
 * @return Visszatérési értéke 1, ha van rendelés az adott azonosítóval.
 *         0, ha nincs.
 */
static int rendeles_menuponttal(int azonosito, const Asztalok *asztalok) {
    Asztal *aktualis = asztalok->eleje;
    while(aktualis != NULL) {
        for(int i = 0; i < aktualis->ferohely; ++i) {
            Rendeles *futo = aktualis->hely_rendelesek[i].eleje;
            while(futo != NULL) {
                if(futo->termek->azonosito == azonosito)
                    return 1;
                futo = futo->kov;
            }
        }
        aktualis = aktualis->kov;
    }
    return 0;
}
