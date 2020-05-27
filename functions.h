#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct narudzba {
	unsigned int id;
	char ime[31];
	char prezime[31];
	char eMail[31];
	char brojMobitela[14];
	char naziv[31];
	char edicija[31];
	char cijena[8];
	char godina[31];
}NARUDZBA;

typedef struct igra {
	unsigned int id;
	char naziv[31];
	char edicija[31];
	char cijena[8];
	char godina[31];
}IGRA;

void fProvjeraDatoteke(char*, unsigned int*);
void fMenu(char*, unsigned int*, char*, unsigned int*);
void fIzlazakIzPrograma(void);

void fUnos(char*, unsigned int*, unsigned int*);
void fIspis(char*, unsigned int*);
void fObrisi(char*, unsigned int*);
void fTrazi(char*, unsigned int*);
void fUredi(char*, unsigned int*);
void fObrisiDatoteku(char*, unsigned int*);

#endif //FUNCTIONS_H