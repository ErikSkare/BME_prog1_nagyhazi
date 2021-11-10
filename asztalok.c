#include <stdio.h>
#include "asztalok.h"
#include "deps/debugmalloc.h"

/**
 * @brief Dinamikusan lefoglal egy asztalt.
 *
 * @param azonosito: Az asztal azonosítója.
 * @param pozicio: Az asztal pozíciója az étteremben.
 * @param statusz: Az asztal státusza (szabad vagy foglalt).
 * @param ferohely: Az asztal helyeinek száma.
 *
 * @return Visszaadja a dinamikusan lefoglalt asztal címét.
 *         A hívó felelőssége a felszabadítás!
 */
Asztal *asztal_foglal(int azonosito, struct Pozicio pozicio, enum Statusz statusz, int ferohely) {
    Asztal *uj = (Asztal*) malloc(sizeof(Asztal));
    uj->azonosito = azonosito;
    uj->pozicio = pozicio;
    uj->statusz = statusz;
    uj->ferohely = ferohely;
    uj->hely_rendelesek = (Rendelesek*) malloc(ferohely * sizeof(Rendelesek));
    for(int i = 0; i < ferohely; ++i)
        uj->hely_rendelesek[i] = (Rendelesek) {NULL, NULL};
    uj->kov = NULL;
    return uj;
}

/**
 * @brief Felszabadítja az asztalok listáját.
 *
 * @param asztalok: Az asztalok struktúrára mutató pointer.
 */
void asztalok_felszabadit(const Asztalok *asztalok) {
    Asztal *aktualis = asztalok->eleje;
    while(aktualis != NULL) {
        Asztal *kov = aktualis->kov;
        for(int i = 0; i < aktualis->ferohely; ++i)
            rendelesek_felszabadit(&aktualis->hely_rendelesek[i]);
        free(aktualis->hely_rendelesek);
        free(aktualis);
        aktualis = kov;
    }
}

/**
 * @brief Beolvassa az asztalokat a megadott fájlból.
 *
 * @param fajl: A fájl útvonalát tartalmazó sztring.
 * @param asztalok: Az asztalok struktúrára mutató pointer.
 *
 * @return Paraméterlistán beállítja az asztalok értékét.
 *         Visszatérési értéke 1, ha nem sikerült megnyitni a fájlt.
 *         0, ha sikerült.
 */
int asztalok_beolvas(char *fajl, Asztalok *asztalok) {
    FILE *fp = fopen(fajl, "r");
    *asztalok = (Asztalok) {NULL, NULL};
    if(fp == NULL)
        return 1;

    char buffer[255];
    while(fgets(buffer, 255, fp) != NULL)
        asztal_sor_hozzaad(buffer, asztalok);

    fclose(fp);
    return 0;
}

/**
 * @brief Kiírja az asztalokat a megadott fájlba.
 *
 * @param fajl: A fájl útvonalát tartalmazó sztring.
 * @param asztalok: Az asztalok struktúrára mutató pointer.
 *
 * @return Visszatérési értéke 1, ha nem sikerült megnyitni a fájlt.
 *         0, ha sikerült.
 */
int asztalok_kiir(char *fajl, const Asztalok *asztalok) {
    FILE *fp = fopen(fajl, "w");
    if(fp == NULL)
        return 1;

    Asztal *aktualis = asztalok->eleje;
    while(aktualis != NULL) {
        fprintf(fp, "%d;%d;%d;%d\n", aktualis->pozicio.X, aktualis->pozicio.Y, aktualis->ferohely, (int) aktualis->statusz);
        aktualis = aktualis->kov;
    }

    fclose(fp);
    return 0;
}

/**
 * @brief Beolvassa a rendeléseket a megadott fájlból.
 *
 * @param fajl: A fájl útvonalát tartalmazó sztring.
 * @param menu: A menü struktúrára mutató pointer.
 * @param asztalok: Az asztalok struktúrára mutató pointer.
 *
 * @return Visszatérési értéke 1, ha nem sikerült megnyitni a fájlt.
 *         0, ha sikerült.
 */
int rendelesek_beolvas(char *fajl, const Menu *menu, Asztalok *asztalok) {
    FILE *fp = fopen(fajl, "r");
    if(fp == NULL)
        return 1;

    char buffer[255];
    while(fgets(buffer, 255, fp) != NULL)
        rendeles_sor_hozzaad(buffer, menu, asztalok);

    fclose(fp);
    return 0;
}

/**
 * @brief Kiírja a rendeléseket a megadott fájlba.
 *
 * @param fajl: A fájl útvonalát tartalmazó sztring.
 * @param asztalok: Az asztalok struktúrára mutató pointer.
 *
 * @return Visszatérési értéke 1, ha nem sikerült megnyitni a fájlt.
 *         0, ha sikerült.
 */
int rendelesek_kiir(char *fajl, const Asztalok *asztalok) {
    FILE *fp = fopen(fajl, "w");
    if(fp == NULL)
        return 1;

    Asztal *a_aktualis = asztalok->eleje;
    while(a_aktualis != NULL) {
        for(int i = 0; i < a_aktualis->ferohely; ++i) {
            Rendeles *r_aktualis = a_aktualis->hely_rendelesek[i].eleje;
            while(r_aktualis != NULL) {
                fprintf(fp, "%d;%d;%d;%d\n", a_aktualis->azonosito, i, r_aktualis->termek->azonosito, r_aktualis->darab);
                r_aktualis = r_aktualis->kov;
            }
        }
        a_aktualis = a_aktualis->kov;
    }

    fclose(fp);
    return 0;
}

/**
 * @brief Hozzáadja a láncolt listához az asztalt,
 *        amelyet egy sztringbõl képez.
 *
 * @param sor: Az asztal adatait tartalmazó sztring.
 * @param asztalok: Az asztalok struktúrára mutató pointer.
 */
static void asztal_sor_hozzaad(char *sor, Asztalok *asztalok) {
    struct Pozicio pozicio;
    int statusz, ferohely;
    sscanf(sor, "%d;%d;%d;%d\n", &pozicio.X, &pozicio.Y, &ferohely, &statusz);
    asztal_hozzaad(pozicio, (enum Statusz) statusz, ferohely, asztalok);
}

/**
 * @brief Hozzáadja a megfelelő asztal megfelelő hely rendelések láncolt listájához a rendelést,
 *        amelyet egy sztringből képez.
 *
 * @param sor: A rendelés adatait tartalmazó sztring.
 * @param menu: A menü struktúrára mutató pointer.
 * @param asztalok: Az asztalok struktúrára mutató pointer.
 */
static void rendeles_sor_hozzaad(char *sor, const Menu *menu, Asztalok *asztalok) {
    int a_azonosito, hely_sorszam, t_azonosito, darab;
    sscanf(sor, "%d;%d;%d;%d\n", &a_azonosito, &hely_sorszam, &t_azonosito, &darab);
    rendeles_hozzaad(t_azonosito, darab, menu, &asztal_keres(a_azonosito, asztalok)->hely_rendelesek[hely_sorszam]);
}
