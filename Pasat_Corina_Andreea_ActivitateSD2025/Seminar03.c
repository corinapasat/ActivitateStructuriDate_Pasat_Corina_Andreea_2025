#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
    int id;
    int anFabricatie;
    char* sofer;
    float kilometriiParcursi;
    char initialaProducator;
};
typedef struct StructuraMasina Masina;

void afisareMasina(Masina masina) {
    printf("ID: %d | An Fabricatie: %d | Sofer: %s | Kilometrii: %.2f | Initiala Producator: %c\n",
        masina.id, masina.anFabricatie, masina.sofer, masina.kilometriiParcursi, masina.initialaProducator);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
    for (int i = 0; i < nrMasini; i++) {
        afisareMasina(masini[i]);
    }
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
    (*nrMasini)++;
    *masini = (Masina*)realloc(*masini, (*nrMasini) * sizeof(Masina));
    (*masini)[*nrMasini - 1] = masinaNoua;
}

Masina citireMasinaFisier(FILE* file) {
    Masina masina;
    char buffer[100];

    if (fgets(buffer, sizeof(buffer), file)) {
        char sofer[50];
        sscanf(buffer, "%d,%d,%49[^,],%f,%c",
            &masina.id, &masina.anFabricatie, sofer, &masina.kilometriiParcursi, &masina.initialaProducator);

        masina.sofer = (char*)malloc(strlen(sofer) + 1);
        strcpy(masina.sofer, sofer);
    }
    return masina;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
    FILE* file = fopen(numeFisier, "r");
    if (!file) {
        printf("Eroare la deschiderea fisierului!\n");
        return NULL;
    }

    Masina* masini = NULL;
    *nrMasiniCitite = 0;

    while (!feof(file)) {
        Masina masina = citireMasinaFisier(file);
        adaugaMasinaInVector(&masini, nrMasiniCitite, masina);
    }

    fclose(file);
    return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
    for (int i = 0; i < *nrMasini; i++) {
        free((*vector)[i].sofer);
    }
    free(*vector);
    *vector = NULL;
    *nrMasini = 0;
}

int main() {
    int nrMasini = 0;
    Masina* masini = citireVectorMasiniFisier("masini.txt", &nrMasini);

    if (masini) {
        printf("Masinile citite din fisier:\n");
        afisareVectorMasini(masini, nrMasini);
        dezalocareVectorMasini(&masini, &nrMasini);
    }

    return 0;
}
