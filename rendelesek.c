#include <stdio.h>
#include "rendelesek.h"
#include "deps/debugmalloc.h"

/**
 * @brief Dinamikusan lefoglal egy rendel�st.
 *
 * @param termek: A term�k men�pontj�ra mutat� pointer.
 * @param darab: Ah�ny darabot rendeltek a term�kb�l.
 *
 * @return Visszaadja a dinamikusan foglalt rendel�s c�m�t.
 *         A h�v� felel�ss�ge a felszabad�t�s.
 */
Rendeles *rendeles_foglal(Menupont *termek, int darab) {
    Rendeles *uj = (Rendeles*) malloc(sizeof(Rendeles));
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

/**
 * @brief Hozz�ad egy rendel�st a l�ncolt list�hoz.
 *
 * @param termek_azonosito: A term�k azonos�t�ja, amelyet hozz� szeretn�nk adni.
 * @param darab: Ah�ny darabot szeretn�nk rendelni a term�kb�l.
 * @param menu: A men� strukt�r�ra mutat� pointer.
 * @param rendelesek: A rendel�sek strukt�r�ra mutat� pointer.
 *
 * @return Visszat�r�si �rt�ke 1, ha nem siker�lt felvenni a rendel�st.
 *         0, ha sikeres volt a felv�tel.
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
