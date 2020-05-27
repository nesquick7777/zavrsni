#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "functions.h"

int main(void) {

	char* igre = "igre.bin";
	char* narudzbe = "narudzbe.bin";
	unsigned int brojNarudzbi = 0, brojIgara = 0;

	fProvjeraDatoteke(narudzbe, &brojNarudzbi);
	fProvjeraDatoteke(igre, &brojIgara);
	fMenu(narudzbe, &brojNarudzbi, igre,&brojIgara);

	return 0;
}