#include <stdio.h>
#include <string.h>
#include "megjelenites.h"
#include "deps/econio.h"

static int szam_beolvas(int *hova);
static MenuAllapot megerosites(MenuAllapot kovetkezo);
static MenuAllapot hibauzenet(MenuAllapot honnan, char *uzenet);
static MenuAllapot siker(char *uzenet);
static void szamla_kiir(const Asztal *megjelenitendo);
static void foglaltsag_kiir(const Asztalok *asztalok, int offsetX, int offsetY);

MenuAllapot fomenu_vezerlo() {
    econio_clrscr();
    printf("Fõmenü\n\n");
    printf("1 - Asztal hozzáadása\n");
    printf("2 - Asztal törlése\n");
    printf("3 - Asztal megnyitása\n");
    printf("4 - Asztal lezárása\n");
    printf("5 - Menüpont hozzáadása\n");
    printf("6 - Menüpont törlése\n");
    printf("7 - Rendelés felvétele\n");
    printf("8 - Számla megjelenítése\n");
    printf("9 - Foglaltsági térkép megjelenítése\n");
    printf("10 - Kilépés\n\n");

    int utasitas;
    printf("Parancs: ");
    while(szam_beolvas(&utasitas) == 1 || utasitas < 1 || utasitas > 10)
        printf("Parancs: ");

    return megerosites(utasitas);
}

MenuAllapot asztal_hozzaad_vezerlo(Asztalok *asztalok) {
    econio_clrscr();
    printf("Asztal hozzáadása\n\n");

    int x_poz;
    printf("X pozíció: ");
    while(szam_beolvas(&x_poz) == 1 || x_poz < 0)
        printf("X pozíció: ");

    int y_poz;
    printf("Y pozíció: ");
    while(szam_beolvas(&y_poz) == 1 || y_poz < 0)
        printf("Y pozíció: ");

    int ferohely;
    printf("Férõhely: ");
    while(szam_beolvas(&ferohely) == 1 || ferohely < 1)
        printf("Férõhely: ");

    int eredmeny = asztal_hozzaad((struct Pozicio){x_poz, y_poz}, SZABAD, ferohely, asztalok);
    if(eredmeny == 1)
        return hibauzenet(ASZTAL_HOZZAAD, "Nem sikerült hozzáadni az asztalt!");
    return siker("Sikerült hozzáadni az asztalt!");
}

MenuAllapot asztal_torol_vezerlo(Asztalok *asztalok) {
    econio_clrscr();
    printf("Asztal törlése\n\n");

    int asztal_az;
    printf("Azonosító: ");
    while(szam_beolvas(&asztal_az) == 1)
        printf("Azonosító: ");

    int eredmeny = asztal_torol(asztal_az, asztalok);
    if(eredmeny == 1)
        return hibauzenet(ASZTAL_TOROL, "Nem található az asztal, vagy foglalt a státusza!");
    return siker("Sikerült törölni az asztalt!");
}

MenuAllapot asztal_megnyit_vezerlo(Asztalok *asztalok) {
    econio_clrscr();
    printf("Asztal megnyitása\n\n");

    int asztal_az;
    printf("Azonosító: ");
    while(szam_beolvas(&asztal_az) == 1)
        printf("Azonosító: ");

    int eredmeny = asztal_megnyit(asztal_az, asztalok);
    if(eredmeny == 1)
        return hibauzenet(ASZTAL_MEGNYIT, "Nem található a megadott azonosítóval asztal!");
    return siker("Sikerült megnyitni az asztalt!");
}

MenuAllapot asztal_lezar_vezerlo(Asztalok *asztalok) {
    econio_clrscr();
    printf("Asztal lezárása\n\n");

    int asztal_az;
    printf("Azonosító: ");
    while(szam_beolvas(&asztal_az) == 1)
        printf("Azonosító: ");

    int eredmeny = asztal_lezar(asztal_az, asztalok);
    if(eredmeny == 1)
        return hibauzenet(ASZTAL_LEZAR, "Nem található a megadott azonosítóval asztal!");
    return siker("Sikerült lezárni az asztalt!");
}

MenuAllapot menupont_hozzaad_vezerlo(Menu *menu) {
    econio_clrscr();
    printf("Menüpont hozzáadása\n\n");

    char nev[50 + 1];
    printf("Név: ");
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
    fgets(nev, 51, stdin);
    nev[strcspn(nev, "\n")] = 0;

    int ar;
    printf("Ár: ");
    while(szam_beolvas(&ar) == 1 || ar < 0)
        printf("Ár: ");

    int eredmeny = menupont_hozzaad(ar, nev, menu);
    if(eredmeny == 1)
        return hibauzenet(MENUPONT_HOZZAAD, "Nem sikerült hozzáadni a menüpontot!");
    return siker("Sikerült hozzáadni a menüpontot!");
}

MenuAllapot menupont_torol_vezerlo(Menu *menu, const Asztalok *asztalok) {
    econio_clrscr();
    printf("Menüpont törlése\n\n");

    int menupont_az;
    printf("Azonosító: ");
    while(szam_beolvas(&menupont_az) == 1)
        printf("Azonosító: ");

    int eredmeny = menupont_torol(menupont_az, menu, asztalok);
    if(eredmeny == 1)
        return hibauzenet(MENUPONT_TOROL, "Nem található a menüpont, vagy van hozzá tartozó megrendelés!");
    return siker("Sikerült törölni a menüpontot!");
}

MenuAllapot rendeles_felvetel_vezerlo(const Menu *menu, const Asztalok *asztalok) {
    econio_clrscr();
    printf("Rendelés felvétele\n\n");

    int asztal_az;
    printf("Asztal azonosító: ");
    while(szam_beolvas(&asztal_az) == 1)
        printf("Asztal azonosító: ");

    int hely_sorszam;
    printf("Hely sorszáma: ");
    while(szam_beolvas(&hely_sorszam) == 1)
        printf("Hely sorszáma: ");

    int termek_az;
    printf("Termék azonosító: ");
    while(szam_beolvas(&termek_az) == 1)
        printf("Termék azonosító: ");

    int darab;
    printf("Darabszám: ");
    while(szam_beolvas(&darab) == 1 || darab < 1)
        printf("Darabszám: ");

    Asztal *asztal = asztal_keres(asztal_az, asztalok);
    if(asztal == NULL || asztal->statusz == SZABAD)
        return hibauzenet(RENDELES_FELVETEL, "Nem található asztal a megadott azonosítóval, vagy nincs megnyitva!");
    if(hely_sorszam >= asztal->ferohely)
        return hibauzenet(RENDELES_FELVETEL, "Nem található hely a megadott sorszámmal!");

    int eredmeny = rendeles_hozzaad(termek_az, darab, menu, &asztal->hely_rendelesek[hely_sorszam]);
    if(eredmeny == 1)
        return hibauzenet(RENDELES_FELVETEL, "Nem sikerült felvenni a rendelést!");
    return siker("Sikerült felvenni a rendelést!");
}

MenuAllapot szamla_megjelenit_vezerlo(const Asztalok *asztalok) {
    econio_clrscr();
    printf("Számla megjelenítése\n\n");

    int asztal_az;
    printf("Asztal azonosító: ");
    while(szam_beolvas(&asztal_az) == 1)
        printf("Asztal azonosító: ");

    Asztal *megjelenitendo = asztal_keres(asztal_az, asztalok);
    if(megjelenitendo == NULL)
        return hibauzenet(SZAMLA_MEGJELENIT, "Nem található asztal a megadott azonosítóval!");

    szamla_kiir(megjelenitendo);

    printf("\n1 - Vissza a fõmenübe\n\n");
    int utasitas;
    printf("Parancs: ");
    while(szam_beolvas(&utasitas) == 1 || utasitas != 1)
        printf("Parancs: ");
    return FOMENU;
}

MenuAllapot foglaltsagi_terkep_vezerlo(const Asztalok *asztalok) {
    econio_clrscr();
    printf("Foglaltsági térkép megjelenítése");

    foglaltsag_kiir(asztalok, 2, 2);

    printf("\n1- Vissza a fõmenübe\n\n");

    int utasitas;
    printf("Parancs: ");
    while(szam_beolvas(&utasitas) == 1 || utasitas != 1)
        printf("Parancs: ");

    return FOMENU;
}

static int szam_beolvas(int *hova) {
    int sikerult = scanf("%d", hova);
    if(sikerult != 1) {
        scanf("%*[^\n]");
        return 1;
    }
    return 0;
}

static MenuAllapot megerosites(MenuAllapot kovetkezo) {
    econio_clrscr();
    printf("Biztosan tovább szeretne lépni?\n\n");
    printf("1 - Igen\n");
    printf("2 - Nem\n\n");

    int utasitas;
    printf("Parancs: ");
    while(szam_beolvas(&utasitas) == 1 || utasitas < 1 || utasitas > 2)
        printf("Parancs: ");

    if(utasitas == 2)
        return FOMENU;
    else
        return kovetkezo;
}

static MenuAllapot hibauzenet(MenuAllapot honnan, char *uzenet) {
    econio_clrscr();
    printf("%s\n\n", uzenet);
    printf("1 - Újrapróbálkozás\n");
    printf("2 - Vissza a fõmenübe\n\n");

    int utasitas;
    printf("Parancs: ");
    while(szam_beolvas(&utasitas) == 1 || utasitas < 1 || utasitas > 2)
        printf("Parancs: ");

    if(utasitas == 1)
        return honnan;
    else
        return FOMENU;
}

static MenuAllapot siker(char *uzenet) {
    econio_clrscr();
    printf("%s\n\n", uzenet);
    printf("1 - Vissza a fõmenübe\n\n");

    int utasitas;
    printf("Parancs: ");
    while(szam_beolvas(&utasitas) == 1 || utasitas != 1)
        printf("Parancs: ");

    return FOMENU;
}

static void szamla_kiir(const Asztal *megjelenitendo) {
    printf("\n------------------------------\n");
    int vegosszeg = 0;
    for(int i = 0; i < megjelenitendo->ferohely; ++i) {
        Rendeles *futo = megjelenitendo->hely_rendelesek[i].eleje;
        if(futo != NULL)
            printf(" %d. hely:\n", i);

        while(futo != NULL) {
            printf("  %dx %s - %dFt\n", futo->darab, futo->termek->nev, futo->darab * futo->termek->ar);
            vegosszeg += futo->darab * futo->termek->ar;
            futo = futo->kov;
        }
    }
    if(vegosszeg == 0)
        printf(" Nem volt rendelés!\n");
    else
        printf("\n Végösszeg: %dFt\n", vegosszeg);
    printf("------------------------------\n");
}

static void foglaltsag_kiir(const Asztalok *asztalok, int offsetX, int offsetY) {
    econio_gotoxy(offsetX, offsetY);

    int max_X, max_Y;
    max_koordinatak(asztalok, &max_X, &max_Y);

    if(max_X == -1 && max_Y == -1) {
        econio_gotoxy(offsetX, offsetY);
        printf("Nincsen asztal az étteremben!\n");
        return;
    }

    for(int i = 0; i <= max_X; ++i) {
        for(int j = 0; j <= max_Y; ++j) {
            econio_gotoxy(offsetX + i, offsetY + j);
            printf("-");
        }
    }

    Asztal *futo = asztalok->eleje;
    while(futo != NULL) {
        econio_gotoxy(offsetX + futo->pozicio.X,
                      offsetY + futo->pozicio.Y);
        if(futo->statusz == FOGLALT)
            printf("F");
        else
            printf("S");
        futo = futo->kov;
    }

    econio_gotoxy(0, offsetY + max_Y + 1);
}
