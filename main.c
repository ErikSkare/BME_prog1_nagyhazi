#include <stdio.h>
#include <stdlib.h>
#include "asztalok.h"

int main()
{
    Asztalok asztalok;
    Menu menu;

    menu_beolvas("menu.csv", &menu);
    asztalok_beolvas("asztalok.csv", &asztalok);
    rendelesek_beolvas("rendelesek.csv", &menu, &asztalok);

    Rendeles *aktualis = asztal_keres(1, &asztalok)->hely_rendelesek[0].eleje;
    while(aktualis != NULL) {
        printf("%s %d\n", aktualis->termek->nev, aktualis->darab);
        aktualis = aktualis->kov;
    }

    rendelesek_kiir("rendelesek.csv", &asztalok);
    menu_kiir("menu.csv", &menu);
    asztalok_kiir("asztalok.csv", &asztalok);

    menu_felszabadit(&menu);
    asztalok_felszabadit(&asztalok);
    return 0;
}
