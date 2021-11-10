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
