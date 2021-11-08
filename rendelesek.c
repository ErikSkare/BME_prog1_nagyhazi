#include <stdio.h>
#include "rendelesek.h"
#include "deps/debugmalloc.h"

/**
 * @brief Dinamikusan lefoglal egy rendelést.
 *
 * @param termek: A termék menüpontjára mutató pointer.
 * @param darab: Ahány darabot rendeltek a termékbõl.
 *
 * @return Visszaadja a dinamikusan foglalt rendelés címét.
 *         A hívó felelõssége a felszabadítás.
 */
Rendeles *rendeles_foglal(Menupont *termek, int darab) {
    Rendeles *uj = (Rendeles*) malloc(sizeof(Rendeles));
    uj->termek = termek;
    uj->darab = darab;
    uj->kov = NULL;
    return uj;
}

/**
 * @brief Felszabadítja a rendelések láncolt listát.
 *
 * @param rendelesek: A rendelések struktúrára mutató pointer.
 */
void rendelesek_felszabadit(const Rendelesek *rendelesek) {
    Rendeles *aktualis = rendelesek->eleje;
    while(aktualis != NULL) {
        Rendeles *kov = aktualis->kov;
        free(aktualis);
        aktualis = kov;
    }
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
    if(rendelesek->eleje == NULL && rendelesek->vege == NULL)
        rendelesek->eleje = uj;
    else
        rendelesek->vege->kov = uj;

    rendelesek->vege = uj;
    return 0;
}
