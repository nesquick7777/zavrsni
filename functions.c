#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include <string.h>
#include <time.h>

// UNOS, ISPIS,BRISANJE I UREDIVANJE NARUDZBA I IGRA //
void fUnos(char* datoteka, unsigned int* pBroj, char* datoteka1, unsigned int* pBroj1) {

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	FILE* pDatotekaDodajNaKraj = NULL;
	pDatotekaDodajNaKraj = fopen(datoteka, "rb+");

	if (pDatotekaDodajNaKraj == NULL) {
		perror("Greska");
		exit(EXIT_FAILURE);
	}
	else if (datoteka == "narudzbe.bin") {
		FILE* pDatotekaProcitaj = NULL;
		pDatotekaProcitaj = fopen(datoteka1, "rb");

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
						strcat(privremeniClan.zanr, (sviKorisnici + i)->zanr);
						strcat(privremeniClan.cijena, (sviKorisnici + i)->cijena);
						strcat(privremeniClan.edicija, (sviKorisnici + i)->edicija);
						strcat(privremeniClan.godina, (sviKorisnici + i)->godina);
						found = 1;
					}
				}
				if (found == 0) {
					printf("\n\nUnijeli ste ID koji ne odgovara ni jednoj igri\n\n");
					return;
				}
				printf("Unesite ime korisnika\n");
				scanf("%21s", privremeniClan.ime);
				printf("Unesite prezime korisnika\n");
				scanf("%21s", privremeniClan.prezime);

				unsigned int pronadenM = 0;
				char privremeniMail[31] = { '\0' };
				char privremeniMail1[31] = { '\0' };
				do {
					printf("Unesite e-mail korisnika\n");
					scanf("%30s", privremeniClan.eMail);
					if (strchr(privremeniClan.eMail, '@') != NULL) {
						strcpy(privremeniMail, strchr(privremeniClan.eMail, '@') + 1);
					}

					if (strchr(privremeniMail, '@') != NULL)
					{
						pronadenM = 0;
					}
					else if (strchr(privremeniMail, '.') != NULL)
					{
						strcpy(privremeniMail1, strchr(privremeniMail, '.') + 1);
						if (strchr(privremeniMail1, '.') != NULL)
						{
							pronadenM = 0;
						}
						else
							pronadenM = 1;
					}
				} while (pronadenM != 1);

				printf("Unesite broj mobitela bez +385\n");
				char privremeniBroj[10] = { '\0' };
				scanf("%9s", privremeniBroj);
				strcpy(privremeniClan.brojMobitela, "+385");
				strcat(privremeniClan.brojMobitela, privremeniBroj);
				snprintf(privremeniClan.kupljeno, sizeof(privremeniClan.kupljeno), "%d-%02d-%02d %02d:%02d:%02d\n\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
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
		IGRA privremenaIgra = { 0 };
		printf("Unesite naziv igre!\n");
		scanf(" %30[^\n]s", privremenaIgra.naziv);
		printf("Unesite zanr igre!\n");
		scanf(" %21[^\n]s", privremenaIgra.zanr);
		printf("Unesite tip edicije!\n");
		scanf(" %21[^\n]s", privremenaIgra.edicija);
		printf("Unesite cijenu igre!\n");
		char privremenaCijena[6] = { 0 };
		scanf("%5s", privremenaCijena);
		strcpy(privremenaIgra.cijena, privremenaCijena);
		strcat(privremenaIgra.cijena, "kn");

		do {
			printf("Unesite datum(MM/DD/GGGG): \n");
			scanf("%u/%u/%u", &mjesec, &dan, &godina);
		} while (dan > 31 || dan <= 0 || mjesec > 12 || mjesec <= 0 || godina <= 2019 || godina > 9999);
		snprintf(privremenaIgra.godina, sizeof(privremenaIgra.godina), "%u. %s %u.", dan, mjeseci[mjesec - 1], godina);
		privremenaIgra.id = (*pBroj)++;

		fseek(pDatotekaDodajNaKraj, sizeof(unsigned int) + ((*pBroj - 1) * sizeof(IGRA)), SEEK_SET);
		fwrite(&privremenaIgra, sizeof(IGRA), 1, pDatotekaDodajNaKraj);
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
			printf("Popis je prazan!\n");
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
				printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
				printf("| %-2s | %-21s| %-21s| %-30s| %-13s | %-30s| %-21s| %-21s| %-7s| %-21s|\n", "ID", "IME", "PREZIME", "E-MAIL", "MOBITEL", "NAZIV", "ZANR", "EDICIJA", "CIJENA", "DATUM IZLASKA");
				printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

				for (unsigned int i = 0; i < *pBroj; i++)
				{
					printf("| %-2d | %-21s| %-21s| %-30s| %-13s | %-30s| %-21s| %-21s| %-7s| %-21s|\n", (sviKorisnici + i)->id, (sviKorisnici + i)->ime, (sviKorisnici + i)->prezime, (sviKorisnici + i)->eMail, (sviKorisnici + i)->brojMobitela, (sviKorisnici + i)->naziv,
						(sviKorisnici + i)->zanr, (sviKorisnici + i)->edicija,
						(sviKorisnici + i)->cijena, (sviKorisnici + i)->godina);
				}
				printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
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

				printf("-------------------------------------------------------------------------------------------------------------------------\n");
				printf("| %-2s | %-30s | %-21s | %-21s | %-7s | %-21s |\n", "ID", "NAZIV", "ZANR", "EDICIJA", "CIJENA", "DATUM IZLASKA");
				printf("-------------------------------------------------------------------------------------------------------------------------\n");

				for (unsigned int i = 0; i < *pBroj; i++)
				{
					printf("| %-2d | %-30s | %-21s | %-21s | %-7s | %-21s |\n", (sviKorisnici + i)->id, (sviKorisnici + i)->naziv,
						(sviKorisnici + i)->zanr, (sviKorisnici + i)->edicija,
						(sviKorisnici + i)->cijena, (sviKorisnici + i)->godina);

				}
				printf("-------------------------------------------------------------------------------------------------------------------------\n");
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

	if (*pBroj == 0) {
		printf("\nPopis je prazan!\n");
		return;
	}
	FILE* fp = NULL;
	fp = fopen(datoteka, "rb");
	if (!fp) {
		fprintf(stderr, "Neuspjesno otvorena datoteka %s", datoteka);
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
	unsigned int trazeniID = 0;
	printf("Unesite ID\n");
	scanf("%u", &trazeniID);
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
			printf("\nID:%u, trazeniID:%u\n", (sviKorisnici + i)->id, trazeniID);
			if (trazeniID == (sviKorisnici + i)->id) {
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
				printf("Nijedna narudzba ne odgavara unesenom ID-u: %u\n\n", trazeniID);
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
			printf("\nID:%u, trazeniID:%u\n", (sviKorisnici + i)->id, trazeniID);
			if (trazeniID == (sviKorisnici + i)->id) {
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
				printf("Nije pronadena igra sa unesenim ID-om: %u\n\n", trazeniID);
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
		else if (datoteka == "narudzbe.bin") {
			NARUDZBA* sviKorisnici = NULL;
			sviKorisnici = (NARUDZBA*)calloc(*pBroj, sizeof(NARUDZBA));
			if (sviKorisnici == NULL) {
				perror("Greska sviKorisnici");
				exit(EXIT_FAILURE);
			}
			else {
				fread(sviKorisnici, sizeof(NARUDZBA), *pBroj, pDatotekaProcitaj);
				fclose(pDatotekaProcitaj);

				printf("Unesite ime korisnika\n");
				char privremenoIme[22] = { '\0' };
				scanf("%21s", privremenoIme);
				unsigned int found = 0;

				printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
				printf("| %-2s | %-21s| %-21s| %-30s| %-13s | %-30s| %-21s| %-21s| %-7s| %-21s|\n", "ID", "IME", "PREZIME", "E-MAIL", "MOBITEL", "NAZIV", "ZANR", "EDICIJA", "CIJENA", "DATUM IZLASKA");
				printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

				for (unsigned int i = 0; i < *pBroj; i++) {
					if (strcmp((sviKorisnici + i)->ime, privremenoIme) == 0) {
						printf("| %-2d | %-21s| %-21s| %-30s| %-13s | %-30s| %-21s| %-21s| %-7s| %-21s|\n", (sviKorisnici + i)->id, (sviKorisnici + i)->ime, (sviKorisnici + i)->prezime, (sviKorisnici + i)->eMail, (sviKorisnici + i)->brojMobitela, (sviKorisnici + i)->naziv,
							(sviKorisnici + i)->zanr, (sviKorisnici + i)->edicija,
							(sviKorisnici + i)->cijena, (sviKorisnici + i)->godina);
						found = 1;
					}
				}
				printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
				free(sviKorisnici);
				if (!found) {
					printf("Nijedna narudzba ne odgavara unesenom imenu korisnika: %s\n\n", privremenoIme);
				}
			}
		}
		else if (datoteka == "igre.bin") {
			IGRA* sviKorisnici = NULL;
			sviKorisnici = (IGRA*)calloc(*pBroj, sizeof(IGRA));
			if (sviKorisnici == NULL) {
				perror("Greska sviKorisnici");
				exit(EXIT_FAILURE);
			}
			fread(sviKorisnici, sizeof(IGRA), *pBroj, pDatotekaProcitaj);
			fclose(pDatotekaProcitaj);

			printf("Unesite zanr igre:\n");
			char privremenoIme[22] = { '\0' };
			scanf("%21s", privremenoIme);
			unsigned int found = 0;

			printf("-------------------------------------------------------------------------------------------------------------------------\n");
			printf("| %-2s | %-30s | %-21s | %-21s | %-7s | %-21s |\n", "ID", "NAZIV", "ZANR", "EDICIJA", "CIJENA", "DATUM IZLASKA");
			printf("-------------------------------------------------------------------------------------------------------------------------\n");

			for (unsigned int i = 0; i < *pBroj; i++) {
				if (strcmp((sviKorisnici + i)->zanr, privremenoIme) == 0) {
					printf("| %-2d | %-30s | %-21s | %-21s | %-7s | %-21s |\n", (sviKorisnici + i)->id, (sviKorisnici + i)->naziv,
						(sviKorisnici + i)->zanr, (sviKorisnici + i)->edicija,
						(sviKorisnici + i)->cijena, (sviKorisnici + i)->godina);
					found = 1;
				}
			}
			printf("-------------------------------------------------------------------------------------------------------------------------\n");
			free(sviKorisnici);
			if (!found) {
				printf("Nijedna igra ne odgavara unesenom zanru: %s\n\n", privremenoIme);
			}
		}
		else {
			printf("Greska");
			return;

		}
	}
}
void fUredi(char* datoteka, unsigned int* pBroj)
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
	if ((*pBroj) == 0) {
		printf("Morate prvo unijeti igru!\n");
		return;
	}
	int searchID = 0;
	printf("Unesite ID igre koju zelite urediti!\n");
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
			printf("\nID:%d, trazeniID:%d\n", (sviKorisnici + i)->id, searchID);
			if ((sviKorisnici + i)->id == searchID) {

				unsigned int dan, mjesec, godina;
				char* mjeseci[] = { "sijecnja", "veljace", "ozujka", "travnja", "svibnja", "lipnja",
									"srpnja", "kolovoza","rujna","listopada", "studenoga", "prosinca", };
				printf("Igra sa trazenim ID-om pronadena!\n\n");

				IGRA privremenaIgra = { 0 };
				printf("Unesite naziv igre!\n");
				scanf(" %30[^\n]s", privremenaIgra.naziv);
				printf("Unesite zanr igre!\n");
				scanf(" %21[^\n]s", privremenaIgra.zanr);
				printf("Unesite tip edicije!\n");
				scanf(" %21[^\n]s", privremenaIgra.edicija);
				printf("Unesite cijenu igre!\n");
				char privremenaCijena[6] = { 0 };
				scanf("%5s", privremenaCijena);
				strcpy(privremenaIgra.cijena, privremenaCijena);
				strcat(privremenaIgra.cijena, "kn");
				do {
					printf("Unesite datum(MM/DD/GGGG): \n");
					scanf("%u/%u/%u", &mjesec, &dan, &godina);
				} while (dan > 31 || dan <= 0 || mjesec > 12 || mjesec <= 0 || godina <= 2019 || godina > 9999);
				snprintf(privremenaIgra.godina, sizeof(privremenaIgra.godina), "%u. %s %u.", dan, mjeseci[mjesec - 1], godina);
				privremenaIgra.id = (*pBroj)++;

				fseek(fp_tmp, sizeof(unsigned int) + ((*pBroj - 1) * sizeof(IGRA)), SEEK_SET);
				fwrite(&privremenaIgra, sizeof(IGRA), 1, fp_tmp);
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
				printf("Igra sa ID-om: %d ne postoji!\n\n", searchID);
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

	printf("Da li ste sigurni kako zelite obrisati sadrzaj iz datoteke %s?(unesite 'da/Da/DA' za brisanje)\n", datoteka);
	char izbor[3] = { '\0' };
	scanf(" %2s", izbor);
	if (!strcmp("da", izbor) || !strcmp("DA", izbor) || !strcmp("Da", izbor))
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
		printf(" _________________________________________________________\
                \n|    |                                                    |\
                \n| BR |                      IZBORNIK                      |\
                \n|____|____________________________________________________|\
				\n| 1  | Dodajte novu \033[1;31mnarudzbu\033[0m u datoteku \033[1;31mnarudzbe\033[0m.bin      |\
				\n| 2  | Citanje iz datoteke \033[1;31mnarudzbe\033[0m.bin                   |\
				\n| 3  | Trazite \033[1;31mnarudzbu\033[0m preko imena korisnika             |\
                \n| 4  | Pogledajte kada je \033[1;31mnarudzba\033[0m kupljena preko ID-a    |\
                \n| 5  | Obrisite \033[1;31mnarudzbu\033[0m preko ID-a                       |\
                \n| 6  | Obrisite sve \033[1;31mnarudzbe\033[0m iz datoteke \033[1;31mnarudzbe\033[0m.bin     |\
                \n| 7  | Dodajte novu \033[1;34migru\033[0m u datoteku \033[1;34migre\033[0m.bin              |\
                \n| 8  | Citanje iz datoteke \033[1;34migre\033[0m.bin                       |\
                \n| 9  | Trazite \033[1;34migru\033[0m preko zanra                           |\
                \n| 10 | Uredite \033[1;34migru\033[0m preko ID-a                            |\
                \n| 11 | Sortiranje \033[1;34migara\033[0m po zeljenom kriteriju             |\
                \n| 12 | Obrisite \033[1;34migru\033[0m preko ID-a                           |\
                \n| 13 | Obrisite sve \033[1;34migre\033[0m iz datoteke \033[1;34migre\033[0m.bin             |\
                \n| 14 | Zavrsetak programa                                 |\
				\n|____|____________________________________________________|\n");
		printf("Unesite broj zeljene naredbe:\n");
		
		scanf("%u", &izbornik);
		switch (izbornik) {
		case 1:
			printf("\033[1;36m");
			fIspis(datoteka1, pBroj1); //Igre
			printf("\033[0m");
			fUnos(datoteka, pBroj, datoteka1, pBroj1); //Narudzbe
			break;
		case 2:
			printf("\033[1;36m");
			fIspis(datoteka, pBroj); //Narudzbe
			printf("\033[0m");
			break;
		case 3:
			printf("\033[1;36m");
			fTrazi(datoteka, pBroj); //Narudzbu
			printf("\033[0m");
			break;
		case 4:
			printf("\033[1;36m");
			fIspis(datoteka, pBroj); //Narudzbe
			fKupljeno(datoteka, pBroj); //Narudzbe
			printf("\033[0m");
			break;
		case 5:
			printf("\033[1;36m");
			fIspis(datoteka, pBroj); //Narudzbe
			printf("\033[0m");
			fObrisi(datoteka, pBroj); //Narudzbe
			break;
		case 6:
			fObrisiDatoteku(datoteka, pBroj); //narudzbe.bin
			break;
		case 7:
			fUnos(datoteka1, pBroj1, datoteka, pBroj); //Igre
			break;
		case 8:
			printf("\033[1;36m");
			fIspis(datoteka1, pBroj1); //Igre
			printf("\033[0m");
			break;
		case 9:
			printf("\033[1;36m");
			fTrazi(datoteka1, pBroj1); //Igre
			printf("\033[0m");
			break;
		case 10:
			printf("\033[1;36m");
			fIspis(datoteka1, pBroj1);//Igre
			printf("\033[0m");
			fUredi(datoteka1, pBroj1); //Igre
			break;
		case 11:
			printf("\033[1;36m");
			fSortiranje(datoteka1, pBroj1); //Igre
			printf("\033[0m");
			break;
		case 12:
			printf("\033[1;36m");
			fIspis(datoteka1, pBroj1);//Igre
			printf("\033[0m");
			fObrisi(datoteka1, pBroj1); //Igre
			break;
		case 13:
			fObrisiDatoteku(datoteka1, pBroj1); //igre.bin
			break;
		case 14:
			fIzlazakIzPrograma();
			break;
		default:
			printf("Krivo odabrana opcija, pokusajte ponovno\n");
		}
	}
}
void fIzlazakIzPrograma(void) {

	printf("Da li ste sigurni kako zelite zavrsiti program?(unesite 'da/Da/DA' za izlazak)\n");
	char izbor[3] = { '\0' };
	scanf(" %2s", izbor);
	if (!strcmp("da", izbor) || !strcmp("DA", izbor) || !strcmp("Da", izbor)) {
		exit(EXIT_FAILURE);
	}
	return;
}

// SORTIRANJE IGARA
void fSortiranje(char* datoteka, unsigned int* pBroj) {
	FILE* pDatotekaProcitaj = NULL;
	pDatotekaProcitaj = fopen(datoteka, "rb");

	if (pDatotekaProcitaj == NULL) {
		printf("Citanje datoteke %s je neuspjesno", datoteka);
		return;
	}
	else {
		fread(pBroj, sizeof(unsigned int), 1, pDatotekaProcitaj);

		if (*pBroj == 0) {
			printf("Popis je prazan!\n");
			fclose(pDatotekaProcitaj);
			return;
		}

		IGRA* sviKorisnici = NULL;
		IGRA* temp = NULL;
		sviKorisnici = (IGRA*)calloc(*pBroj, sizeof(IGRA));
		temp = (IGRA*)calloc(*pBroj, sizeof(IGRA));

		if (sviKorisnici == NULL || temp == NULL) {
			perror("Greska igre");
			exit(EXIT_FAILURE);
		}
		else {
			fread(sviKorisnici, sizeof(IGRA), *pBroj, pDatotekaProcitaj);
			fclose(pDatotekaProcitaj);
			char izborGlavni[8] = { '\0' };
			printf("Zelite li sortirati po alfabetu, cijeni,ediciji ili zanru?(alfabet/Alfabet/ALFABET)(cijena/Cijena/CIJENA)(zanr/Zanr/ZANR)(edicija/Edicija/EDICIJA)\n");
			scanf("%7s", izborGlavni);
			if (!strcmp("alfabet", izborGlavni) || !strcmp("Alfabet", izborGlavni) || !strcmp("ALFABET", izborGlavni)) {
				char izbor[8] = { '\0' };
				printf("Zelite li sortirati uzlazno ili silazno?\n");
				scanf("%7s", izbor);
				if (!strcmp("uzlazno", izbor) || !strcmp("Uzlazno", izbor) || !strcmp("UZLAZNO", izbor)) {
					for (unsigned int i = 0; i <= *pBroj - 1; i++)
						for (unsigned int j = i + 1; j <= (*pBroj) - 1; j++) {
							if (strcmp((sviKorisnici + i)->naziv, (sviKorisnici + j)->naziv) > 0) {
								memcpy(temp, (sviKorisnici + i), sizeof(IGRA));
								memcpy((sviKorisnici + i), (sviKorisnici + j), sizeof(IGRA));
								memcpy((sviKorisnici + j), temp, sizeof(IGRA));
							}
						}
				}
				else if (!strcmp("silazno", izbor) || !strcmp("Silazno", izbor) || !strcmp("SILAZNO", izbor)) {
					for (unsigned int i = 0; i <= *pBroj - 1; i++)
						for (unsigned int j = i + 1; j <= (*pBroj) - 1; j++) {
							if (strcmp((sviKorisnici + i)->naziv, (sviKorisnici + j)->naziv) < 0) {
								memcpy(temp, (sviKorisnici + i), sizeof(IGRA));
								memcpy((sviKorisnici + i), (sviKorisnici + j), sizeof(IGRA));
								memcpy((sviKorisnici + j), temp, sizeof(IGRA));
							}
						}

				}
				else {
					printf("Krivo unesen nacin sortiranja!\n");
					return;
				}
			}
			else if (!strcmp("cijena", izborGlavni) || !strcmp("Cijena", izborGlavni) || !strcmp("CIJENA", izborGlavni)) {
				char izbor[8] = { '\0' };
				printf("Zelite li sortirati uzlazno ili silazno?\n");
				scanf("%7s", izbor);
				if (!strcmp("uzlazno", izbor) || !strcmp("Uzlazno", izbor) || !strcmp("UZLAZNO", izbor)) {

					for (unsigned int i = 0; i <= *pBroj - 1; i++)
						for (unsigned int j = i + 1; j <= (*pBroj) - 1; j++) {
							int num = atoi((sviKorisnici + i)->cijena);
							int num1 = atoi((sviKorisnici + j)->cijena);
							if (num > num1) {
								memcpy(temp, (sviKorisnici + i), sizeof(IGRA));
								memcpy((sviKorisnici + i), (sviKorisnici + j), sizeof(IGRA));
								memcpy((sviKorisnici + j), temp, sizeof(IGRA));
							}
						}
				}
				else if (!strcmp("silazno", izbor) || !strcmp("Silazno", izbor) || !strcmp("SILAZNO", izbor)) {
					for (unsigned int i = 0; i <= *pBroj - 1; i++)
						for (unsigned int j = i + 1; j <= (*pBroj) - 1; j++) {
							int num = atoi((sviKorisnici + i)->cijena);
							int num1 = atoi((sviKorisnici + j)->cijena);
							if (num < num1) {
								memcpy(temp, (sviKorisnici + i), sizeof(IGRA));
								memcpy((sviKorisnici + i), (sviKorisnici + j), sizeof(IGRA));
								memcpy((sviKorisnici + j), temp, sizeof(IGRA));
							}
						}
				}
				else {
					printf("Krivo unesen nacin sortiranja!\n");
					return;
				}
			}
			else if (!strcmp("zanr", izborGlavni) || !strcmp("Zanr", izborGlavni) || !strcmp("ZANR", izborGlavni)) {
				char izbor[8] = { '\0' };
				printf("Zelite li sortirati uzlazno ili silazno?\n");
				scanf("%7s", izbor);
				if (!strcmp("uzlazno", izbor) || !strcmp("Uzlazno", izbor) || !strcmp("UZLAZNO", izbor)) {
					for (unsigned int i = 0; i <= *pBroj - 1; i++)
						for (unsigned int j = i + 1; j <= (*pBroj) - 1; j++) {
							if (strcmp((sviKorisnici + i)->zanr, (sviKorisnici + j)->zanr) > 0) {
								memcpy(temp, (sviKorisnici + i), sizeof(IGRA));
								memcpy((sviKorisnici + i), (sviKorisnici + j), sizeof(IGRA));
								memcpy((sviKorisnici + j), temp, sizeof(IGRA));
							}
						}
				}
				else if (!strcmp("silazno", izbor) || !strcmp("Silazno", izbor) || !strcmp("SILAZNO", izbor)) {
					for (unsigned int i = 0; i <= *pBroj - 1; i++)
						for (unsigned int j = i + 1; j <= (*pBroj) - 1; j++) {
							if (strcmp((sviKorisnici + i)->zanr, (sviKorisnici + j)->zanr) < 0) {
								memcpy(temp, (sviKorisnici + i), sizeof(IGRA));
								memcpy((sviKorisnici + i), (sviKorisnici + j), sizeof(IGRA));
								memcpy((sviKorisnici + j), temp, sizeof(IGRA));
							}
						}
				}
				else {
					printf("Krivo unesen nacin sortiranja!\n");
					return;
				}
			}
			else if (!strcmp("edicija", izborGlavni) || !strcmp("Edicija", izborGlavni) || !strcmp("EDICIJA", izborGlavni)) {
				char izbor[8] = { '\0' };
				printf("Zelite li sortirati uzlazno ili silazno?\n");
				scanf("%7s", izbor);
				if (!strcmp("uzlazno", izbor) || !strcmp("Uzlazno", izbor) || !strcmp("UZLAZNO", izbor)) {
					for (unsigned int i = 0; i <= *pBroj - 1; i++)
						for (unsigned int j = i + 1; j <= (*pBroj) - 1; j++) {
							if (strcmp((sviKorisnici + i)->edicija, (sviKorisnici + j)->edicija) > 0) {
								memcpy(temp, (sviKorisnici + i), sizeof(IGRA));
								memcpy((sviKorisnici + i), (sviKorisnici + j), sizeof(IGRA));
								memcpy((sviKorisnici + j), temp, sizeof(IGRA));
							}
						}
				}
				else if (!strcmp("silazno", izbor) || !strcmp("Silazno", izbor) || !strcmp("SILAZNO", izbor)) {
					for (unsigned int i = 0; i <= *pBroj - 1; i++)
						for (unsigned int j = i + 1; j <= (*pBroj) - 1; j++) {
							if (strcmp((sviKorisnici + i)->edicija, (sviKorisnici + j)->edicija) < 0) {
								memcpy(temp, (sviKorisnici + i), sizeof(IGRA));
								memcpy((sviKorisnici + i), (sviKorisnici + j), sizeof(IGRA));
								memcpy((sviKorisnici + j), temp, sizeof(IGRA));
							}
						}
				}
			}
			else {
				printf("\nKrivo unesen nacin sortiranja!\n\n");
				return;
			}
			printf("-------------------------------------------------------------------------------------------------------------------------\n");
			printf("| %-2s | %-30s | %-21s | %-21s | %-7s | %-21s |\n", "ID", "NAZIV", "ZANR", "EDICIJA", "CIJENA", "DATUM IZLASKA");
			printf("-------------------------------------------------------------------------------------------------------------------------\n");

			for (unsigned int i = 0; i <= *pBroj - 1; i++)
			{
				printf("| %-2d | %-30s | %-21s | %-21s | %-7s | %-21s |\n", (sviKorisnici + i)->id, (sviKorisnici + i)->naziv,
					(sviKorisnici + i)->zanr, (sviKorisnici + i)->edicija,
					(sviKorisnici + i)->cijena, (sviKorisnici + i)->godina);

			}
			printf("-------------------------------------------------------------------------------------------------------------------------\n");
			free(sviKorisnici);
			free(temp);
		}
	}
}

void fKupljeno(char* datoteka, unsigned int* pBroj) {
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
		else {
			NARUDZBA* sviKorisnici = NULL;
			sviKorisnici = (NARUDZBA*)calloc(*pBroj, sizeof(NARUDZBA));
			if (sviKorisnici == NULL) {
				perror("Greska sviKorisnici");
				exit(EXIT_FAILURE);
			}
			else {
				fread(sviKorisnici, sizeof(NARUDZBA), *pBroj, pDatotekaProcitaj);
				fclose(pDatotekaProcitaj);

				printf("Unesite ID narudzbe\n");
				unsigned int privremenoIme = 0;
				scanf("%u", &privremenoIme);
				unsigned int found = 0;

				printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
				printf("| %-2s | %-21s| %-21s| %-30s| %-13s | %-30s| %-21s| %-21s| %-7s| %-21s|\n", "ID", "IME", "PREZIME", "E-MAIL", "MOBITEL", "NAZIV", "ZANR", "EDICIJA", "CIJENA", "DATUM IZLASKA");
				printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

				for (unsigned int i = 0; i < *pBroj; i++) {
					if ((sviKorisnici + i)->id == privremenoIme) {
						printf("| %-2d | %-21s| %-21s| %-30s| %-13s | %-30s| %-21s| %-21s| %-7s| %-21s|\n", (sviKorisnici + i)->id, (sviKorisnici + i)->ime, (sviKorisnici + i)->prezime, (sviKorisnici + i)->eMail, (sviKorisnici + i)->brojMobitela, (sviKorisnici + i)->naziv,
							(sviKorisnici + i)->zanr, (sviKorisnici + i)->edicija,
							(sviKorisnici + i)->cijena, (sviKorisnici + i)->godina);
						found = 1;
						printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

						printf("Narudzba je kupljena: %s\n\n", (sviKorisnici + i)->kupljeno);
					}
				}
				free(sviKorisnici);
				if (!found) {
					printf("Nijedna narudzba ne odgavara unesenom ID-u: %u\n\n", privremenoIme);
				}
			}
		}

	}
}