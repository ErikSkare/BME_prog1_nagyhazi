#ifndef ASZTALOK_H
#define ASZTALOK_H
#include "rendelesek.h"

/**
 * @param eleje: A l�ncolt lista els� elem�re mutat� pointer.
 * @param vege: A l�ncolt lista utols� elem�re mutat� pointer.
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
 * @param azonosito: Az asztal azonos�t�ja 0-t�l sz�mozva.
 * @param pozicio: Az asztal elhelyezked�s�t szimboliz�l� (X, Y) koordin�ta.
 * @param statusz: Az asztal szabad, vagy foglalt.
 * @param ferohely: Az asztalhoz tartoz� sz�kek sz�ma.
 * @param hely_rendelesek: A helyenk�nt egy rendel�sek list�t tartalmaz� t�mb.
 * @param kov: A k�vetkez� asztalra mutat� pointer.
 */
typedef struct Asztal {
    int azonosito;
    struct Pozicio pozicio;
    enum Statusz statusz;
    int ferohely;
    Rendelesek *hely_rendelesek;
    struct Asztal *kov;
} Asztal;

//<Mem�riakezel� f�ggv�nyek>
Asztal *asztal_foglal(int azonosito, struct Pozicio pozicio, enum Statusz statusz, int ferohely);
void asztalok_felszabadit(const Asztalok *asztalok);

//<F�jlkezel� f�ggv�nyek>
int asztalok_beolvas(char *fajl, Asztalok *asztalok);
int asztalok_kiir(char *fajl, const Asztalok *asztalok);
int rendelesek_beolvas(char *fajl, const Menu *menu, Asztalok *asztalok);
int rendelesek_kiir(char *fajl, const Asztalok *asztalok);
static void asztal_sor_hozzaad(char *sor, Asztalok *asztalok);
static void rendeles_sor_hozzaad(char *sor, const Menu *menu, Asztalok *asztalok);

//<Asztalkezel� f�ggv�nyek>
void asztal_hozzaad(struct Pozicio pozicio, enum Statusz statusz, int ferohely, Asztalok *asztalok);
Asztal *asztal_keres(int azonosito, const Asztalok *asztalok);

#endif
