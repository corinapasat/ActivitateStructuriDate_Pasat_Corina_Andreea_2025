#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume Sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n", masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++) {
		afisareMasina(masini[i]);
		printf("\n");
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	Masina* aux;
	aux = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));
	for (int i = 0; i < (*nrMasini); i++) {
		aux[i] = (*masini)[i];
	}
	aux[(*nrMasini)] = masinaNoua;
	free(*masini);
	(*masini) = aux;
	(*nrMasini) = (*nrMasini) + 1;

}

Masina citireMasinaFisier(FILE* file) {
	Masina m;
	char buffer[100];
	char separator[4] = ",;\n";
	fgets(buffer, 100, file);
	char* aux;
	aux = strtok(buffer, separator);
	m.id = atoi(aux);
	m.nrUsi = atoi(strtok(NULL, separator));
	m.pret = atof(strtok(NULL, separator));
	aux = strtok(NULL, separator);
	m.model = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m.model, strlen(aux) + 1, aux);
	aux = strtok(NULL, separator);
	m.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m.numeSofer, strlen(aux) + 1, aux);
	aux = strtok(NULL, separator);
	m.serie = aux[0];
	return m;

	//functia citeste o masina dintr-un strceam deja deschis
	//masina citita este returnata;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE* f = fopen(numeFisier, "r");
	Masina* masini = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaFisier(f);
		adaugaMasinaInVector(&masini, nrMasiniCitite, m);
	}
	fclose(f);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	for (int i = 0; i < *nrMasini; i++) {
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
	free(*vector);
	*vector = NULL;
	*nrMasini = 0;
}

int main() {
	Masina* masini = NULL;
	int nrMasini = 0;
	masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(masini, nrMasini);
	dezalocareVectorMasini(&masini, &nrMasini);
	return 0;
}