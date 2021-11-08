#ifndef ASZTALOK_H
#define ASZTALOK_H
#include "rendelesek.h"

/**
 * @param eleje: A láncolt lista elsõ elemére mutató pointer.
 * @param vege: A láncolt lista utolsó elemére mutató pointer.
 */
typedef struct Asztalok {
    struct Asztal *eleje;
    struct Asztal *vege;
} Asztalok;

enum Statusz {
    SZABAD = 0,
    FOGLALT = 1
};

struct Pozicio {
    int X, Y;
};

/**
 * @param azonosito: Az asztal azonosítója 0-tól számozva.
 * @param pozicio: Az asztal elhelyezkedését szimbolizáló (X, Y) koordináta.
 * @param statusz: Az asztal szabad, vagy foglalt.
 * @param ferohely: Az asztalhoz tartozó székek száma.
 * @param hely_rendelesek: A helyenként egy rendelések listát tartalmazó tömb.
 * @param kov: A következõ asztalra mutató pointer.
 */
typedef struct Asztal {
    int azonosito;
    struct Pozicio pozicio;
    enum Statusz statusz;
    int ferohely;
    Rendelesek *hely_rendelesek;
    struct Asztal *kov;
} Asztal;

//<Memóriakezelõ függvények>
Asztal *asztal_foglal(int azonosito, struct Pozicio pozicio, enum Statusz statusz, int ferohely);
void asztalok_felszabadit(const Asztalok *asztalok);

//<Fájlkezelõ függvények>
int asztalok_beolvas(char *fajl, Asztalok *asztalok);
int asztalok_kiir(char *fajl, const Asztalok *asztalok);
int rendelesek_beolvas(char *fajl, const Menu *menu, Asztalok *asztalok);
int rendelesek_kiir(char *fajl, const Asztalok *asztalok);
static void asztal_sor_hozzaad(char *sor, Asztalok *asztalok);
static void rendeles_sor_hozzaad(char *sor, const Menu *menu, Asztalok *asztalok);

//<Asztalkezelõ függvények>
void asztal_hozzaad(struct Pozicio pozicio, enum Statusz statusz, int ferohely, Asztalok *asztalok);
Asztal *asztal_keres(int azonosito, const Asztalok *asztalok);

#endif
