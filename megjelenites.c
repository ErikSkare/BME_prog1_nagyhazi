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
    printf("F�men�\n\n");
    printf("1 - Asztal hozz�ad�sa\n");
    printf("2 - Asztal t�rl�se\n");
    printf("3 - Asztal megnyit�sa\n");
    printf("4 - Asztal lez�r�sa\n");
    printf("5 - Men�pont hozz�ad�sa\n");
    printf("6 - Men�pont t�rl�se\n");
    printf("7 - Rendel�s felv�tele\n");
    printf("8 - Sz�mla megjelen�t�se\n");
    printf("9 - Foglalts�gi t�rk�p megjelen�t�se\n");
    printf("10 - Kil�p�s\n\n");

    int utasitas;
    printf("Parancs: ");
    while(szam_beolvas(&utasitas) == 1 || utasitas < 1 || utasitas > 10)
        printf("Parancs: ");

    return megerosites(utasitas);
}

MenuAllapot asztal_hozzaad_vezerlo(Asztalok *asztalok) {
    econio_clrscr();
    printf("Asztal hozz�ad�sa\n\n");

    int x_poz;
    printf("X poz�ci�: ");
    while(szam_beolvas(&x_poz) == 1 || x_poz < 0)
        printf("X poz�ci�: ");

    int y_poz;
    printf("Y poz�ci�: ");
    while(szam_beolvas(&y_poz) == 1 || y_poz < 0)
        printf("Y poz�ci�: ");

    int ferohely;
    printf("F�r�hely: ");
    while(szam_beolvas(&ferohely) == 1 || ferohely < 1)
        printf("F�r�hely: ");

    int eredmeny = asztal_hozzaad((struct Pozicio){x_poz, y_poz}, SZABAD, ferohely, asztalok);
    if(eredmeny == 1)
        return hibauzenet(ASZTAL_HOZZAAD, "Nem siker�lt hozz�adni az asztalt!");
    return siker("Siker�lt hozz�adni az asztalt!");
}

MenuAllapot asztal_torol_vezerlo(Asztalok *asztalok) {
    econio_clrscr();
    printf("Asztal t�rl�se\n\n");

    int asztal_az;
    printf("Azonos�t�: ");
    while(szam_beolvas(&asztal_az) == 1)
        printf("Azonos�t�: ");

    int eredmeny = asztal_torol(asztal_az, asztalok);
    if(eredmeny == 1)
        return hibauzenet(ASZTAL_TOROL, "Nem tal�lhat� az asztal, vagy foglalt a st�tusza!");
    return siker("Siker�lt t�r�lni az asztalt!");
}

MenuAllapot asztal_megnyit_vezerlo(Asztalok *asztalok) {
    econio_clrscr();
    printf("Asztal megnyit�sa\n\n");

    int asztal_az;
    printf("Azonos�t�: ");
    while(szam_beolvas(&asztal_az) == 1)
        printf("Azonos�t�: ");

    int eredmeny = asztal_megnyit(asztal_az, asztalok);
    if(eredmeny == 1)
        return hibauzenet(ASZTAL_MEGNYIT, "Nem tal�lhat� a megadott azonos�t�val asztal!");
    return siker("Siker�lt megnyitni az asztalt!");
}

MenuAllapot asztal_lezar_vezerlo(Asztalok *asztalok) {
    econio_clrscr();
    printf("Asztal lez�r�sa\n\n");

    int asztal_az;
    printf("Azonos�t�: ");
    while(szam_beolvas(&asztal_az) == 1)
        printf("Azonos�t�: ");

    int eredmeny = asztal_lezar(asztal_az, asztalok);
    if(eredmeny == 1)
        return hibauzenet(ASZTAL_LEZAR, "Nem tal�lhat� a megadott azonos�t�val asztal!");
    return siker("Siker�lt lez�rni az asztalt!");
}

MenuAllapot menupont_hozzaad_vezerlo(Menu *menu) {
    econio_clrscr();
    printf("Men�pont hozz�ad�sa\n\n");

    char nev[50 + 1];
    printf("N�v: ");
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
    fgets(nev, 51, stdin);
    nev[strcspn(nev, "\n")] = 0;

    int ar;
    printf("�r: ");
    while(szam_beolvas(&ar) == 1 || ar < 0)
        printf("�r: ");

    int eredmeny = menupont_hozzaad(ar, nev, menu);
    if(eredmeny == 1)
        return hibauzenet(MENUPONT_HOZZAAD, "Nem siker�lt hozz�adni a men�pontot!");
    return siker("Siker�lt hozz�adni a men�pontot!");
}

MenuAllapot menupont_torol_vezerlo(Menu *menu, const Asztalok *asztalok) {
    econio_clrscr();
    printf("Men�pont t�rl�se\n\n");

    int menupont_az;
    printf("Azonos�t�: ");
    while(szam_beolvas(&menupont_az) == 1)
        printf("Azonos�t�: ");

    int eredmeny = menupont_torol(menupont_az, menu, asztalok);
    if(eredmeny == 1)
        return hibauzenet(MENUPONT_TOROL, "Nem tal�lhat� a men�pont, vagy van hozz� tartoz� megrendel�s!");
    return siker("Siker�lt t�r�lni a men�pontot!");
}

MenuAllapot rendeles_felvetel_vezerlo(const Menu *menu, const Asztalok *asztalok) {
    econio_clrscr();
    printf("Rendel�s felv�tele\n\n");

    int asztal_az;
    printf("Asztal azonos�t�: ");
    while(szam_beolvas(&asztal_az) == 1)
        printf("Asztal azonos�t�: ");

    int hely_sorszam;
    printf("Hely sorsz�ma: ");
    while(szam_beolvas(&hely_sorszam) == 1)
        printf("Hely sorsz�ma: ");

    int termek_az;
    printf("Term�k azonos�t�: ");
    while(szam_beolvas(&termek_az) == 1)
        printf("Term�k azonos�t�: ");

    int darab;
    printf("Darabsz�m: ");
    while(szam_beolvas(&darab) == 1 || darab < 1)
        printf("Darabsz�m: ");

    Asztal *asztal = asztal_keres(asztal_az, asztalok);
    if(asztal == NULL || asztal->statusz == SZABAD)
        return hibauzenet(RENDELES_FELVETEL, "Nem tal�lhat� asztal a megadott azonos�t�val, vagy nincs megnyitva!");
    if(hely_sorszam >= asztal->ferohely)
        return hibauzenet(RENDELES_FELVETEL, "Nem tal�lhat� hely a megadott sorsz�mmal!");

    int eredmeny = rendeles_hozzaad(termek_az, darab, menu, &asztal->hely_rendelesek[hely_sorszam]);
    if(eredmeny == 1)
        return hibauzenet(RENDELES_FELVETEL, "Nem siker�lt felvenni a rendel�st!");
    return siker("Siker�lt felvenni a rendel�st!");
}

MenuAllapot szamla_megjelenit_vezerlo(const Asztalok *asztalok) {
    econio_clrscr();
    printf("Sz�mla megjelen�t�se\n\n");

    int asztal_az;
    printf("Asztal azonos�t�: ");
    while(szam_beolvas(&asztal_az) == 1)
        printf("Asztal azonos�t�: ");

    Asztal *megjelenitendo = asztal_keres(asztal_az, asztalok);
    if(megjelenitendo == NULL)
        return hibauzenet(SZAMLA_MEGJELENIT, "Nem tal�lhat� asztal a megadott azonos�t�val!");

    szamla_kiir(megjelenitendo);

    printf("\n1 - Vissza a f�men�be\n\n");
    int utasitas;
    printf("Parancs: ");
    while(szam_beolvas(&utasitas) == 1 || utasitas != 1)
        printf("Parancs: ");
    return FOMENU;
}

MenuAllapot foglaltsagi_terkep_vezerlo(const Asztalok *asztalok) {
    econio_clrscr();
    printf("Foglalts�gi t�rk�p megjelen�t�se");

    foglaltsag_kiir(asztalok, 2, 2);

    printf("\n1- Vissza a f�men�be\n\n");

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
    printf("Biztosan tov�bb szeretne l�pni?\n\n");
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
    printf("1 - �jrapr�b�lkoz�s\n");
    printf("2 - Vissza a f�men�be\n\n");

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
    printf("1 - Vissza a f�men�be\n\n");

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
        printf(" Nem volt rendel�s!\n");
    else
        printf("\n V�g�sszeg: %dFt\n", vegosszeg);
    printf("------------------------------\n");
}

static void foglaltsag_kiir(const Asztalok *asztalok, int offsetX, int offsetY) {
    econio_gotoxy(offsetX, offsetY);

    int max_X, max_Y;
    max_koordinatak(asztalok, &max_X, &max_Y);

    if(max_X == -1 && max_Y == -1) {
        econio_gotoxy(offsetX, offsetY);
        printf("Nincsen asztal az �tteremben!\n");
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
