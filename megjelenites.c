#include <stdio.h>
#include "megjelenites.h"
#include "deps/econio.h"

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
    printf("8 - Kil�p�s\n\n");

    int utasitas;
    printf("Parancs: ");
    while(szam_beolvas(&utasitas) == 1 || utasitas < 1 || utasitas > 8)
        printf("Parancs: ");

    return megerosites(utasitas);
}

MenuAllapot asztal_hozzaad_vezerlo(Asztalok *asztalok) {
    econio_clrscr();
    printf("Asztal hozz�ad�sa\n\n");

    int x_poz;
    printf("X poz�ci�: ");
    while(szam_beolvas(&x_poz) == 1)
        printf("X poz�ci�: ");

    int y_poz;
    printf("Y poz�ci�: ");
    while(szam_beolvas(&y_poz) == 1)
        printf("Y poz�ci�: ");

    int ferohely;
    printf("F�r�hely: ");
    while(szam_beolvas(&ferohely) == 1)
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
    scanf("%s", &nev);

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
