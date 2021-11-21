#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef _WIN32
    #include <windows.h>
#endif
#include "funkciok.h"
#include "megjelenites.h"

int main(int argc, char *argv[])
{
#ifdef _WIN32
    SetConsoleCP(1250);
    SetConsoleOutputCP(1250);
#endif
    if(argc != 2) {
        fprintf(stderr, "A program megfelelo hivasa: etterem.exe [etterem neve]!\n");
        return 1;
    }

    //Fájlnevek létrehozása
    char asztalok_fajlnev[255], menu_fajlnev[255], rendelesek_fajlnev[255];
    strcpy(asztalok_fajlnev, argv[1]);
    strcat(asztalok_fajlnev, "_asztalok.csv");
    strcpy(menu_fajlnev, argv[1]);
    strcat(menu_fajlnev, "_menu.csv");
    strcpy(rendelesek_fajlnev, argv[1]);
    strcat(rendelesek_fajlnev, "_rendelesek.csv");

    //Adatok beolvasása és hibakezelés
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

    //Konzolos menüvel vezérlés (még készülõben)
    MenuAllapot m_allapot = FOMENU;
    while(m_allapot != KILEP) {
        switch(m_allapot) {
            case FOMENU:
                m_allapot = fomenu_vezerlo();
                break;
            case ASZTAL_HOZZAAD:
                m_allapot = asztal_hozzaad_vezerlo(&asztalok);
                break;
            case ASZTAL_TOROL:
                m_allapot = asztal_torol_vezerlo(&asztalok);
                break;
            case ASZTAL_MEGNYIT:
                m_allapot = asztal_megnyit_vezerlo(&asztalok);
                break;
            case ASZTAL_LEZAR:
                m_allapot = asztal_lezar_vezerlo(&asztalok);
                break;
            case MENUPONT_HOZZAAD:
                m_allapot = menupont_hozzaad_vezerlo(&menu);
                break;
            case MENUPONT_TOROL:
                m_allapot = menupont_torol_vezerlo(&menu, &asztalok);
                break;
            case RENDELES_FELVETEL:
                m_allapot = rendeles_felvetel_vezerlo(&menu, &asztalok);
                break;
            case SZAMLA_MEGJELENIT:
                m_allapot = szamla_megjelenit_vezerlo(&asztalok);
                break;
            default:
                m_allapot = FOMENU;
                break;
        }
    }

    //Adatok kiírása
    asztalok_kiir(asztalok_fajlnev, &asztalok);
    menu_kiir(menu_fajlnev, &menu);
    rendelesek_kiir(rendelesek_fajlnev, &asztalok);

    //Felszabadítás
    asztalok_felszabadit(&asztalok);
    menu_felszabadit(&menu);
    return 0;
}
