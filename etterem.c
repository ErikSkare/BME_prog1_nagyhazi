#include <stdbool.h>
#include "megjelenites.h"
#include "deps/debugmalloc.h"
#ifdef _WIN32
    #include <windows.h>
#endif

void fajlnevek_meghataroz(char *asztalok, char *menu, char *rendelesek, char *etterem_nev) {
    strcpy(asztalok, etterem_nev);
    strcat(asztalok, "_asztalok.csv");

    strcpy(menu, etterem_nev);
    strcat(menu, "_menu.csv");

    strcpy(rendelesek, etterem_nev);
    strcat(rendelesek, "_rendelesek.csv");
}

int beolvas(Asztalok *asztalok, Menu *menu, char *etterem_nev) {
    char asztalok_fajl[255], menu_fajl[255], rendelesek_fajl[255];
    fajlnevek_meghataroz(asztalok_fajl, menu_fajl, rendelesek_fajl, etterem_nev);

    return asztalok_beolvas(asztalok_fajl, asztalok) == 2 ||
           menu_beolvas(menu_fajl, menu) == 2 ||
           rendelesek_beolvas(rendelesek_fajl, menu, asztalok) == 2;
}

int kiir(const Asztalok *asztalok, const Menu *menu, char *etterem_nev) {
    char asztalok_fajl[255], menu_fajl[255], rendelesek_fajl[255];
    fajlnevek_meghataroz(asztalok_fajl, menu_fajl, rendelesek_fajl, etterem_nev);

    return asztalok_kiir(asztalok_fajl, asztalok) == 1 ||
           menu_kiir(menu_fajl, menu) == 1 ||
           rendelesek_kiir(rendelesek_fajl, asztalok) == 1;
}

void felszabadit(const Asztalok *asztalok, const Menu *menu) {
    asztalok_felszabadit(asztalok);
    menu_felszabadit(menu);
}

int main(int argc, char *argv[])
{
    //Beállítások
    fclose(fopen("memlog.txt", "w"));
    debugmalloc_log_file("memlog.txt");
#ifdef _WIN32
    SetConsoleCP(1250);
    SetConsoleOutputCP(1250);
#endif

    //Nem megfelelő hívás ellenőrzése
    if(argc != 2) {
        fprintf(stderr, "A program megfelelo hivasa: etterem.exe [etterem neve]!\n");
        return 1;
    }

    //Adatok beolvasása
    Asztalok asztalok;
    Menu menu;
    if(beolvas(&asztalok, &menu, argv[1]) == 1) {
        fprintf(stderr, "Nem sikerult a listak letrehozasa!\n");
        felszabadit(&asztalok, &menu);
        return 2;
    }

    //Konzolos menürendszer
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
            case FOGLALTSAG_MEGJELENIT:
                m_allapot = foglaltsagi_terkep_vezerlo(&asztalok);
                break;
            default:
                m_allapot = FOMENU;
                break;
        }
    }

    //Adatok kiírása
    if(kiir(&asztalok, &menu, argv[1]) == 1) {
        fprintf(stderr, "A fajlba kiiras nem sikerult!\n");
        felszabadit(&asztalok, &menu);
        return 3;
    }

    //Adatok felszabadítása
    felszabadit(&asztalok, &menu);

    return 0;
}
