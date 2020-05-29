#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct narudzba {
	unsigned int id;
	char ime[22];
	char prezime[22];
	char eMail[31];
	char brojMobitela[14];
	char naziv[31];
	char zanr[22];
	char edicija[22];
	char cijena[8];
	char godina[22];
	char kupljeno[30];
}NARUDZBA;

typedef struct igra {
	unsigned int id;
	char naziv[31];
	char zanr[22];
	char edicija[22];
	char cijena[8];
	char godina[22];
}IGRA;

void fProvjeraDatoteke(char*, unsigned int*);
void fMenu(char*, unsigned int*, char*, unsigned int*);
void fIzlazakIzPrograma(void);

void fUnos(char*, unsigned int*, char*, unsigned int*);
void fIspis(char*, unsigned int*);
void fObrisi(char*, unsigned int*);
void fTrazi(char*, unsigned int*);
void fUredi(char*, unsigned int*);
void fObrisiDatoteku(char*, unsigned int*);
void fSortiranje(char*, unsigned int*);
void fKupljeno(char*, unsigned int*);

#endif //FUNCTIONS_H