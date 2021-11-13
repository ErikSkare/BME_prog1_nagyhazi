#include <stdio.h>
#include "rendelesek.h"
#include "deps/debugmalloc.h"

/**
 * @brief Dinamikusan lefoglal egy rendel�st.
 *
 * @param termek: A term�k men�pontj�ra mutat� pointer.
 * @param darab: Ah�ny darabot rendeltek a term�kb�l.
 *
 * @return Visszaadja a dinamikusan foglalt rendel�s c�m�t, vagy NULL-t,
 *         ha nem siker�lt a foglal�s. A h�v� felel�ss�ge a felszabad�t�s.
 */
Rendeles *rendeles_foglal(Menupont *termek, int darab) {
    Rendeles *uj = (Rendeles*) malloc(sizeof(Rendeles));
    if(uj == NULL)
        return NULL;
    uj->termek = termek;
    uj->darab = darab;
    uj->kov = NULL;
    return uj;
}

/**
 * @brief Felszabad�tja a rendel�sek l�ncolt list�t.
 *
 * @param rendelesek: A rendel�sek strukt�r�ra mutat� pointer.
 */
void rendelesek_felszabadit(const Rendelesek *rendelesek) {
    Rendeles *aktualis = rendelesek->eleje;
    while(aktualis != NULL) {
        Rendeles *kov = aktualis->kov;
        free(aktualis);
        aktualis = kov;
    }
}
