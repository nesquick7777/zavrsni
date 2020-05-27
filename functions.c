#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include <string.h>

// UNOS, ISPIS,BRISANJE I UREDIVANJE NARUDZBA I IGRA //
void fUnos(char* datoteka, unsigned int* pBroj,unsigned int* pBroj1) {

	FILE* pDatotekaDodajNaKraj = NULL;
	pDatotekaDodajNaKraj = fopen(datoteka, "rb+");

	if (pDatotekaDodajNaKraj == NULL) {
		perror("Greska");
		exit(EXIT_FAILURE);
	}
	else if (datoteka == "narudzbe.bin") {
		FILE* pDatotekaProcitaj = NULL;
		pDatotekaProcitaj = fopen("igre.bin", "rb");

		if (pDatotekaProcitaj == NULL) {
			fprintf(stderr, "Citanje datoteke %s je neuspjesno", datoteka);
			perror("");
			exit(EXIT_FAILURE);
		}
		else {
			fread(pBroj1, sizeof(unsigned int), 1, pDatotekaProcitaj);

			if (*pBroj1 == 0) {
				printf("Morate prvo unijeti igru!\n");
				fclose(pDatotekaProcitaj);
				return;
			}
			IGRA* sviKorisnici = NULL;
			sviKorisnici = (IGRA*)calloc(*pBroj1, sizeof(IGRA));
			if (sviKorisnici == NULL) {
				perror("Greska igre");
				exit(EXIT_FAILURE);
			}
			else {
				fread(sviKorisnici, sizeof(IGRA), *pBroj1, pDatotekaProcitaj);
				fclose(pDatotekaProcitaj);
				printf("Odaberite ID igre koju zelite naruciti!\n");
				unsigned int privremeniID = 0, found = 0;
				scanf("%u", &privremeniID);
				NARUDZBA privremeniClan = { 0 };
				for (unsigned int i = 0; i < *pBroj1; i++) 
				{
					if (privremeniID == (sviKorisnici + i)->id) {
						strcat(privremeniClan.naziv, (sviKorisnici + i)->naziv);
						strcat(privremeniClan.cijena, (sviKorisnici + i)->cijena);
						strcat(privremeniClan.edicija, (sviKorisnici + i)->edicija);
						strcat(privremeniClan.godina, (sviKorisnici + i)->godina);
						found = 1;
					}
				}
				if(found == 0){
					printf("\n\nUnijeli ste ID koji ne odgovara ni jednoj igri\n\n");
					return;
				}
				printf("Unesite ime korisnika\n");
				scanf("%30s", privremeniClan.ime);
				printf("Unesite prezime korisnika\n");
				scanf("%30s", privremeniClan.prezime);
				printf("Unesite e-mail korisnika\n");
				scanf(" %50[^\n]", privremeniClan.eMail);
				printf("Unesite broj mobitela bez +385\n");
				char privremeniBroj[10] = { '\0' };
				scanf("%9s", privremeniBroj);
				strcpy(privremeniClan.brojMobitela, "+385");
				strcat(privremeniClan.brojMobitela, privremeniBroj);
				privremeniClan.id = (*pBroj)++;

				fseek(pDatotekaDodajNaKraj, sizeof(unsigned int) + ((*pBroj - 1) * sizeof(NARUDZBA)), SEEK_SET);
				fwrite(&privremeniClan, sizeof(NARUDZBA), 1, pDatotekaDodajNaKraj);
				rewind(pDatotekaDodajNaKraj);
				fwrite(pBroj, sizeof(unsigned int), 1, pDatotekaDodajNaKraj);
				fclose(pDatotekaDodajNaKraj);
				free(sviKorisnici);
			}
		}
	}
	else if (datoteka == "igre.bin") {
		unsigned int dan, mjesec, godina;
		char* mjeseci[] = { "sijecnja", "veljace", "ozujka", "travnja", "svibnja", "lipnja",
							"srpnja", "kolovoza","rujna","listopada", "studenoga", "prosinca", };
		IGRA headNode = { 0 };
		printf("Unesite naziv igre!\n");
		scanf(" %30[^\n]s", headNode.naziv);
		printf("Unesite tip edicije!\n");
		scanf(" %30[^\n]s", headNode.edicija);
		printf("Unesite cijenu igre!\n");
		char privremenaCijena[6] = { 0 };
		scanf("%5s", privremenaCijena);
		strcpy(headNode.cijena, privremenaCijena);
		strcat(headNode.cijena, "kn");

		do {
			printf("Enter date(MM/DD/GGGG): \n");
			scanf("%u/%u/%u", &mjesec, &dan, &godina);
		} while (dan > 31 || dan <= 0 || mjesec > 12 || mjesec <= 0 || godina <= 2019 || godina > 9999);
		snprintf(headNode.godina, sizeof(headNode.godina), "%u. %s %u.", dan, mjeseci[mjesec - 1], godina);
		headNode.id = (*pBroj)++;

		fseek(pDatotekaDodajNaKraj, sizeof(unsigned int) + ((*pBroj - 1) * sizeof(IGRA)), SEEK_SET);
		fwrite(&headNode, sizeof(IGRA), 1, pDatotekaDodajNaKraj);
		rewind(pDatotekaDodajNaKraj);
		fwrite(pBroj, sizeof(unsigned int), 1, pDatotekaDodajNaKraj);
		fclose(pDatotekaDodajNaKraj);
	}
	else {
		perror("Greska!");
		return;
	}

}
void fIspis(char* datoteka, unsigned int* pBroj)
{
	FILE* pDatotekaProcitaj = NULL;
	pDatotekaProcitaj = fopen(datoteka, "rb");

	if (pDatotekaProcitaj == NULL) {
		printf("Citanje datoteke %s je neuspjesno", datoteka);
		return;
	}
	else {
		fread(pBroj, sizeof(unsigned int), 1, pDatotekaProcitaj);

		if (*pBroj == 0) {
			printf("Popis igara je prazan!\n");
			fclose(pDatotekaProcitaj);
			return;
		}
		else if (datoteka == "narudzbe.bin") {
			NARUDZBA* sviKorisnici = NULL;
			sviKorisnici = (NARUDZBA*)calloc(*pBroj, sizeof(NARUDZBA));
			if (sviKorisnici == NULL) {
				perror("Greska narudzbe");
				exit(EXIT_FAILURE);
			}
			else {
				fread(sviKorisnici, sizeof(NARUDZBA), *pBroj, pDatotekaProcitaj);
				fclose(pDatotekaProcitaj);
				printf("-----------------------------------------------------------------------------------------------------\n");
				printf("| %-2s | %-20s | %-20s | %-30s | %-13s |\n", "ID", "IME", "PREZIME", "E-MAIL", "MOBITEL");
				printf("-----------------------------------------------------------------------------------------------------\n");

				for (unsigned int i = 0; i < *pBroj; i++)
				{
					printf("| %-2d | %-20s | %-20s | %-30s | %-13s |\n", (sviKorisnici + i)->id, (sviKorisnici + i)->ime,
						(sviKorisnici + i)->prezime,
						(sviKorisnici + i)->eMail, (sviKorisnici + i)->brojMobitela);

				}
				printf("-----------------------------------------------------------------------------------------------------\n");
				free(sviKorisnici);
			}
		}

		else if (datoteka == "igre.bin") {
			IGRA* sviKorisnici = NULL;
			sviKorisnici = (IGRA*)calloc(*pBroj, sizeof(IGRA));
			if (sviKorisnici == NULL) {
				perror("Greska igre");
				exit(EXIT_FAILURE);
			}
			else {
				fread(sviKorisnici, sizeof(IGRA), *pBroj, pDatotekaProcitaj);
				fclose(pDatotekaProcitaj);

				printf("-------------------------------------------------------------------------------------------------\n");
				printf("| %-2s | %-30s | %-21s | %-7s | %-21s |\n", "ID", "NAZIV", "EDICIJA", "CIJENA", "DATUM IZLASKA");
				printf("-------------------------------------------------------------------------------------------------\n");

				for (unsigned int i = 0; i < *pBroj; i++)
				{
					printf("| %-2d | %-30s | %-21s | %-7s | %-21s |\n", (sviKorisnici + i)->id, (sviKorisnici + i)->naziv,
						(sviKorisnici + i)->edicija,
						(sviKorisnici + i)->cijena, (sviKorisnici + i)->godina);

				}
				printf("-------------------------------------------------------------------------------------------------\n");
				free(sviKorisnici);
			}
		}
		else {
			perror("Greska!");
			return;
		}
	}
}
void fObrisi(char* datoteka, unsigned int* pBroj) {
	
FILE* fp = NULL;
fp = fopen(datoteka, "rb");
if (!fp) {
	fprintf(stderr, "Neuspjesno otvorena datoteka %s",datoteka);
	perror("");
	exit(EXIT_FAILURE);
}
FILE* fp_tmp = NULL;
fp_tmp = fopen("tmp.bin", "wb+");
if (!fp_tmp) {
	perror("Neuspjesno kreirana datoteka tmp.bin!");
	exit(EXIT_FAILURE);
}
fread(pBroj, sizeof(unsigned int), 1, fp);
char trazenoIme[31] = { '\0' };
printf("Unesite ime\n");
scanf("%30s", &trazenoIme);
unsigned int privremeni = 0;
int found = 0;

if (datoteka == "narudzbe.bin")
{
	NARUDZBA* sviKorisnici = NULL;
	sviKorisnici = (NARUDZBA*)calloc(*pBroj, sizeof(NARUDZBA));
	if (sviKorisnici == NULL) {
		perror("Citanje svih narudzbi");
		exit(EXIT_FAILURE);
	}
	fread(sviKorisnici, sizeof(NARUDZBA), *pBroj, fp);
	privremeni = (*pBroj);
	(*pBroj) = 0;
	for (unsigned int i = 0; i < privremeni; i++) {
		printf("\nime:%s, trazenoIme:%s\n", (sviKorisnici + i)->ime, trazenoIme);
		if (strcmp(trazenoIme, (sviKorisnici + i)->ime) == 0) {
			printf("Narudzba pronadena i uspjesno obrisana.\n\n");
			found = 1;
		}
		else {
			(sviKorisnici + i)->id = (*pBroj)++;
			fseek(fp_tmp, sizeof(unsigned int) + ((*pBroj - 1) * sizeof(NARUDZBA)), SEEK_SET);
			fwrite((sviKorisnici + i), sizeof(NARUDZBA), 1, fp_tmp);
			rewind(fp_tmp);
			fwrite(pBroj, sizeof(unsigned int), 1, fp_tmp);
		}
		if (!found) {
			printf("Nijedna narudzba ne odgavara unesenom imenu: %s\n\n", trazenoIme);
	}
}
	fclose(fp_tmp);
	fclose(fp);
	remove(datoteka);
	rename("tmp.bin", datoteka);
	free(sviKorisnici);
}

else if (datoteka == "igre.bin")
{
	IGRA* sviKorisnici = NULL;
	sviKorisnici = (IGRA*)calloc(*pBroj, sizeof(IGRA));
	if (sviKorisnici == NULL) {
		perror("Citanje svih igara");
		exit(EXIT_FAILURE);
	}
	fread(sviKorisnici, sizeof(IGRA), *pBroj, fp);
	privremeni = (*pBroj);
	(*pBroj) = 0;
	for (unsigned int i = 0; i < privremeni; i++) {
		printf("\nime:%s, trazenoIme:%s\n", (sviKorisnici + i)->naziv, trazenoIme);
		if (strcmp(trazenoIme, (sviKorisnici + i)->naziv) == 0) {
			printf("Igra je uspjesno obrisana.\n\n");
			found = 1;
		}
		else {
			(sviKorisnici + i)->id = (*pBroj)++;
			fseek(fp_tmp, sizeof(unsigned int) + ((*pBroj - 1) * sizeof(IGRA)), SEEK_SET);
			fwrite((sviKorisnici + i), sizeof(IGRA), 1, fp_tmp);
			rewind(fp_tmp);
			fwrite(pBroj, sizeof(unsigned int), 1, fp_tmp);
		}
		if (!found) {
			printf("Nije pronadena igra sa unesenim imenom: %s\n\n", trazenoIme);
	}
}
	fclose(fp_tmp);
	fclose(fp);
	remove(datoteka);
	rename("tmp.bin", datoteka);
	free(sviKorisnici);
}
	else {
		perror("Greska!");
	}
}
void fTrazi(char* datoteka, unsigned int* pBroj) {

	NARUDZBA* sviKorisnici = NULL;
	FILE* pDatotekaProcitaj = NULL;
	pDatotekaProcitaj = fopen(datoteka, "rb");

	if (pDatotekaProcitaj == NULL) {
		fprintf(stderr, "Citanje datoteke %s je neuspjesno", datoteka);
		perror("");
		exit(EXIT_FAILURE);
	}
	else {
		fread(pBroj, sizeof(unsigned int), 1, pDatotekaProcitaj);
		if (*pBroj == 0) {
			printf("Popis je prazan!\n");
			fclose(pDatotekaProcitaj);
			return;
		}

		sviKorisnici = (NARUDZBA*)calloc(*pBroj, sizeof(NARUDZBA));
		if (sviKorisnici == NULL) {
			perror("Greska sviKorisnici");
			exit(EXIT_FAILURE);
		}
		else {
			fread(sviKorisnici, sizeof(NARUDZBA), *pBroj, pDatotekaProcitaj);
			fclose(pDatotekaProcitaj);
			printf("Unesite ime korisnika\n");
			char privremenoIme[31] = { '\0' };
			scanf("%30s", privremenoIme);
			unsigned int statusPronalaska = 0;
			unsigned int indeksPronalaska = -1;

			for (unsigned int i = 0; i < *pBroj; i++)
			{
				printf("%u\t", (sviKorisnici + i)->id);
				printf("%s ", (sviKorisnici + i)->ime);
				printf("%s ", (sviKorisnici + i)->prezime);
				printf("%s ", (sviKorisnici + i)->eMail);
				printf("%s\n", (sviKorisnici + i)->brojMobitela);

				if (!strcmp((sviKorisnici + i)->ime, privremenoIme)) {
					statusPronalaska = 1;
					indeksPronalaska = i;
				}
			}

			if (statusPronalaska) {
				printf("Korisnik pronadjen\n");
				printf("%u\t", (sviKorisnici + indeksPronalaska)->id);
				printf("%s ", (sviKorisnici + indeksPronalaska)->ime);
				printf("%s ", (sviKorisnici + indeksPronalaska)->prezime);
				printf("%s ", (sviKorisnici + indeksPronalaska)->eMail);
				printf("%s\n", (sviKorisnici + indeksPronalaska)->brojMobitela);
			}
			else {
				printf("Nepostojeci korisnik\n");
			}
			free(sviKorisnici);
		}
	}
}
void fUredi (char* datoteka, unsigned int* pBroj)
	{
		FILE* fp = NULL;
		fp = fopen(datoteka, "rb");
		if (!fp) {
			fprintf(stderr, "Nemoguce citanje datoteke: %s", datoteka);
			perror("");
			exit(EXIT_FAILURE);
		}
		FILE* fp_tmp = NULL;
		fp_tmp = fopen("tmp.bin", "wb+");
		if (!fp_tmp) {
			perror("Datoteka tmp.bin neuspjesno kreirana.");
			exit(EXIT_FAILURE);
		}
		fread(pBroj, sizeof(unsigned int), 1, fp);
		int searchID = 0;
		printf("Unesite ID igre koju zelite obrisati!\n");
		scanf("%d", &searchID);
		unsigned int privremeni = 0;
		int found = 0;
		IGRA* sviKorisnici = NULL;
		sviKorisnici = (IGRA*)calloc(*pBroj, sizeof(IGRA));
		if (sviKorisnici == NULL) {
			perror("Citanje svih korisnika");
			exit(EXIT_FAILURE);
		}
		else
		{
		fread(sviKorisnici, sizeof(IGRA), *pBroj, fp);
		privremeni = (*pBroj);
		(*pBroj) = 0;

		for (unsigned int i = 0; i < privremeni; i++) {
			printf("\nmyrecord:%d, trazenoIme:%d\n", (sviKorisnici + i)->id, searchID);
			if ((sviKorisnici + i)->id == searchID) {

				unsigned int dan, mjesec, godina;
				char* mjeseci[] = { "sijecnja", "veljace", "ozujka", "travnja", "svibnja", "lipnja",
									"srpnja", "kolovoza","rujna","listopada", "studenoga", "prosinca", };
				printf("A record with requested name found and deleted.\n\n");
				
				IGRA headNode = { 0 };
				printf("Unesite naziv igre!\n");
				scanf(" %30[^\n]s", headNode.naziv);
				printf("Unesite tip edicije!\n");
				scanf(" %30[^\n]s", headNode.edicija);
				printf("Unesite cijenu igre!\n");
				char privremenaCijena[6] = { 0 };
				scanf("%5s", privremenaCijena);
				strcpy(headNode.cijena, privremenaCijena);
				strcat(headNode.cijena, "kn");
				do {
					printf("Enter date(MM/DD/GGGG): \n");
					scanf("%u/%u/%u", &mjesec, &dan, &godina);
				} while (dan > 31 || dan <= 0 || mjesec > 12 || mjesec <= 0 || godina <= 2019 || godina > 9999);
				snprintf(headNode.godina, sizeof(headNode.godina), "%u. %s %u.", dan, mjeseci[mjesec - 1], godina);
				headNode.id = (*pBroj)++;

				fseek(fp_tmp, sizeof(unsigned int) + ((*pBroj - 1) * sizeof(IGRA)), SEEK_SET);
				fwrite(&headNode, sizeof(IGRA), 1, fp_tmp);
				rewind(fp_tmp);
				fwrite(pBroj, sizeof(unsigned int), 1, fp_tmp);

				found = 1;
			}
			else {
				(sviKorisnici + i)->id = (*pBroj)++;
				fseek(fp_tmp, sizeof(unsigned int) + ((*pBroj - 1) * sizeof(IGRA)), SEEK_SET);
				fwrite((sviKorisnici + i), sizeof(IGRA), 1, fp_tmp);
				rewind(fp_tmp);
				fwrite(pBroj, sizeof(unsigned int), 1, fp_tmp);
			}
			if (!found) {
				printf("No record(s) found with the requested name: %d\n\n", searchID);
			}
		}
		fclose(fp_tmp);
		fclose(fp);
		remove(datoteka);
		rename("tmp.bin", datoteka);
		free(sviKorisnici);
	}
}

// BRISANJE SADRZAJA IZ DATOTEKA IGRE.BIN I NARUDZBE.BIN
void fObrisiDatoteku(char* datoteka, unsigned int* pBroj) {

	printf("Da li ste sigurni kako zelite obrisati sadrzaj iz datoteke %s?(unesite 'da/DA' za brisanje)\n",datoteka);
	char izbor[3] = { '\0' };
	scanf(" %2s", izbor);
	if (!strcmp("da", izbor) || !strcmp("DA", izbor))
	{
		FILE* pDatoteka = fopen(datoteka, "wb");
		if (pDatoteka == NULL) {
			fprintf(stderr, "Sadrzaj iz datoteke %s neuspjesno obrisan\n", datoteka);
			perror("");
			exit(EXIT_FAILURE);
		}
		else {
			(*pBroj) = 0;
			fwrite(pBroj, sizeof(unsigned int), 1, pDatoteka);
			fclose(pDatoteka);
			printf("Sadrzaj uspjesno obrisan iz datoteke %s\n\n", datoteka);
		}
	}
	return;
}

//  PROVJERA DATOTEKA I MENI //
void fProvjeraDatoteke(char* datoteka, unsigned int* pBroj) {

	FILE* pDatotekaProvjera = fopen(datoteka, "rb");

	if (pDatotekaProvjera == NULL) {
		printf("Datoteka %s ne postoji, prilikom prvog pokretanja\n", datoteka);
		pDatotekaProvjera = fopen(datoteka, "wb");

		if (pDatotekaProvjera == NULL) {
			fprintf(stderr, "Dadoteka %s se ne može kreirati, prilikom prvog pokretanja\n", datoteka);
			perror("");
			exit(EXIT_FAILURE);
		}
		else {
			fwrite(pBroj, sizeof(unsigned int), 1, pDatotekaProvjera);
			fclose(pDatotekaProvjera);
			printf("Datoteka %s kreirana, prilikom prvog pokretanja\n\n", datoteka);
		}
	}
	else {
		fread(pBroj, sizeof(unsigned int), 1, pDatotekaProvjera);
		fclose(pDatotekaProvjera);
		printf("Datoteka %s postoji, prilikom prvog pokretanja\n\n", datoteka);
	}
}
void fMenu(char* datoteka, unsigned int* pBroj, char* datoteka1, unsigned int* pBroj1) {

	unsigned int izbornik = -1;

	while (1) {
		  printf(" _____________________________________________________\
                \n|    |                                                |\
                \n| BR |                    IZBORNIK                    |\
                \n|____|________________________________________________|\
				\n| 1  | Dodajte novu narudzbu u datoteku narudzbe.bin  |\
				\n| 2  | Citanje iz datoteke narudzbe.bin               |\
				\n| 3  | Trazite narudzbu preko imena                   |\
                \n| 4  | Obrisite narudzbu preko ID-a                   |\
                \n| 5  | Obrisite sve narudzbe iz datoteke narudzbe.bin |\
                \n| 6  | Dodajte novu igru u datoteku igre.bin          |\
                \n| 7  | Citanje iz datoteke igre.bin                   |\
                \n| 8  | Uredite igru preko ID-a                        |\
                \n| 9  | Obrisite igru preko ID-a                       |\
                \n| 10 | Obrisite sve igre iz datoteke igre.bin         |\
                \n| 11 | Zavrsetak programa                             |\
				\n|____|________________________________________________|\n");

		scanf("%u", &izbornik);

		switch (izbornik) {
		case 1:
			fIspis(datoteka1, pBroj1); //Igre
			fUnos(datoteka, pBroj,pBroj1); //Narudzbe
			break;
		case 2:
			fIspis(datoteka, pBroj); //Narudzbe
			break;
		case 3:
			fTrazi(datoteka, pBroj); //Narudzbu
			break;
		case 4:
			fObrisi(datoteka, pBroj); //Narudzbe
			break;
		case 5:
		    fObrisiDatoteku(datoteka, pBroj); //narudzbe.bin
			break;
		case 6:
			fUnos(datoteka1,pBroj1,pBroj); //Igre
			break;
		case 7: 
		    fIspis(datoteka1,pBroj1); //Igre
			break;
		case 8:
			fIspis(datoteka1, pBroj1);
			fUredi(datoteka1, pBroj1); //Igre
			break; 
		case 9:
			fIspis(datoteka1, pBroj1);
			fObrisi(datoteka1, pBroj1); //Igre
			break;
		case 10:
			fObrisiDatoteku(datoteka1, pBroj1); //igre.bin
			break;
		case 11:
			fIzlazakIzPrograma();
			break;
		default:
			printf("Krivo odabrana opcija, pokusajte ponovno\n");
		}
	}
}
void fIzlazakIzPrograma(void) {

	printf("Da li ste sigurni kako zelite zavrsiti program?(unesite 'da/DA' za izlazak)\n");
	char izbor[3] = { '\0' };
	scanf(" %2s", izbor);
	if (!strcmp("da", izbor) || !strcmp("DA", izbor)) {
		exit(EXIT_FAILURE);
	}
	return;
}

