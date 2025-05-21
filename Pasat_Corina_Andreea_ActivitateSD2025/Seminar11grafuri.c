#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structura mașină
struct StructuraMasina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};
typedef struct StructuraMasina Masina;

// Structuri pentru graf
typedef struct NodListaPrincipala NodListaPrincipala;

typedef struct NodListaSecundara {
    NodListaPrincipala* nod;
    struct NodListaSecundara* next;
} NodListaSecundara;

struct NodListaPrincipala {
    Masina info;
    NodListaPrincipala* next;
    NodListaSecundara* vecini;
};

// Citirea unei mașini din fișier
Masina citireMasinaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    fgets(buffer, 100, file);
    char* aux;
    Masina m1;

    aux = strtok(buffer, sep);
    m1.id = atoi(aux);
    m1.nrUsi = atoi(strtok(NULL, sep));
    m1.pret = atof(strtok(NULL, sep));

    aux = strtok(NULL, sep);
    m1.model = malloc(strlen(aux) + 1);
    strcpy(m1.model, aux);

    aux = strtok(NULL, sep);
    m1.numeSofer = malloc(strlen(aux) + 1);
    strcpy(m1.numeSofer, aux);

    m1.serie = *strtok(NULL, sep);
    return m1;
}

// Afișarea unei mașini
void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi : %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

// Inserare nod principal
NodListaPrincipala* inserareNodPrincipal(NodListaPrincipala* lista, Masina m) {
    NodListaPrincipala* nou = (NodListaPrincipala*)malloc(sizeof(NodListaPrincipala));
    nou->info = m;
    nou->next = lista;
    nou->vecini = NULL;
    return nou;
}

// Inserare nod secundar (muchie)
void inserareNodSecundar(NodListaPrincipala* nodStart, NodListaPrincipala* nodDest) {
    NodListaSecundara* nou = (NodListaSecundara*)malloc(sizeof(NodListaSecundara));
    nou->nod = nodDest;
    nou->next = nodStart->vecini;
    nodStart->vecini = nou;
}

// Căutare nod principal după ID
NodListaPrincipala* cautaNodDupaID(NodListaPrincipala* lista, int id) {
    while (lista) {
        if (lista->info.id == id) return lista;
        lista = lista->next;
    }
    return NULL;
}

// Inserare muchie între două noduri (mașini) după ID
void inserareMuchie(NodListaPrincipala* lista, int idStart, int idStop) {
    NodListaPrincipala* start = cautaNodDupaID(lista, idStart);
    NodListaPrincipala* stop = cautaNodDupaID(lista, idStop);
    if (start && stop) {
        inserareNodSecundar(start, stop);
    }
}

// Citire noduri (mașini) din fișier
NodListaPrincipala* citireNoduriMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        printf("Nu s-a putut deschide fisierul %s\n", numeFisier);
        return NULL;
    }

    NodListaPrincipala* lista = NULL;
    while (!feof(f)) {
        Masina m = citireMasinaDinFisier(f);
        lista = inserareNodPrincipal(lista, m);
    }

    fclose(f);
    return lista;
}

// Citire muchii din fișier (ex: muchii.txt cu linii de forma: 1,2)
void citireMuchiiDinFisier(NodListaPrincipala* lista, const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        printf("Nu s-a putut deschide fisierul %s\n", numeFisier);
        return;
    }

    int idStart, idStop;
    while (fscanf(f, "%d,%d", &idStart, &idStop) == 2) {
        inserareMuchie(lista, idStart, idStop);
    }

    fclose(f);
}

// Afișare graf
void afisareGraf(NodListaPrincipala* lista) {
    while (lista) {
        afisareMasina(lista->info);
        printf("Vecini: ");
        NodListaSecundara* vecin = lista->vecini;
        while (vecin) {
            printf("%d ", vecin->nod->info.id);
            vecin = vecin->next;
        }
        printf("\n------------------------\n");
        lista = lista->next;
    }
}

// Dezalocare memorie graf
void dezalocareNoduriGraf(NodListaPrincipala* lista) {
    while (lista) {
        NodListaSecundara* vecin = lista->vecini;
        while (vecin) {
            NodListaSecundara* temp = vecin;
            vecin = vecin->next;
            free(temp);
        }

        free(lista->info.model);
        free(lista->info.numeSofer);

        NodListaPrincipala* temp = lista;
        lista = lista->next;
        free(temp);
    }
}

// Main
int main() {
    NodListaPrincipala* graf = citireNoduriMasiniDinFisier("masini.txt");
    citireMuchiiDinFisier(graf, "muchii.txt");

    printf("AFISARE GRAF:\n");
    afisareGraf(graf);

    dezalocareNoduriGraf(graf);

    return 0;
}
