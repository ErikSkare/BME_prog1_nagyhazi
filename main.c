#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "funkciok.h"

int main(int argc, char *argv[])
{
    if(argc != 2) {
        fprintf(stderr, "A program megfelelo hivasa: etterem.exe [etterem neve]!\n");
        return 1;
    }

    //F�jlnevek l�trehoz�sa
    char asztalok_fajlnev[255], menu_fajlnev[255], rendelesek_fajlnev[255];
    strcpy(asztalok_fajlnev, argv[1]);
    strcat(asztalok_fajlnev, "_asztalok.csv");
    strcpy(menu_fajlnev, argv[1]);
    strcat(menu_fajlnev, "_menu.csv");
    strcpy(rendelesek_fajlnev, argv[1]);
    strcat(rendelesek_fajlnev, "_rendelesek.csv");

    //Adatok beolvas�sa �s hibakezel�s
    Asztalok asztalok;
    Menu menu;

    bool asztal_hozzaad_hiba = asztalok_beolvas(asztalok_fajlnev, &asztalok) == 2;
    bool menupont_hozzaad_hiba = menu_beolvas(menu_fajlnev, &menu) == 2;
    bool rendeles_hozzaad_hiba = rendelesek_beolvas(rendelesek_fajlnev, &menu, &asztalok) == 2;

    if(asztal_hozzaad_hiba || menupont_hozzaad_hiba || rendeles_hozzaad_hiba) {
        fprintf(stderr, "Nem sikerult a listak letrehozasa!\n");
        asztalok_felszabadit(&asztalok);
        menu_felszabadit(&menu);
        return 2;
    }

    //Konzolos men�vel vez�rl�s (m�g k�sz�l�ben)

    //Adatok ki�r�sa
    asztalok_kiir(asztalok_fajlnev, &asztalok);
    menu_kiir(menu_fajlnev, &menu);
    rendelesek_kiir(rendelesek_fajlnev, &asztalok);

    //Felszabad�t�s
    asztalok_felszabadit(&asztalok);
    menu_felszabadit(&menu);
    return 0;
}
