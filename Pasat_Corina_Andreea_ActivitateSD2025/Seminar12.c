#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct NodListaPrincipala NodListaPrincipala;

typedef struct NodListaSecundara {
	NodListaPrincipala* vecin;
	struct NodListaSecundara* next;
} NodListaSecundara;

struct NodListaPrincipala {
	Masina info;
	NodListaSecundara* listaVecini;
	struct NodListaPrincipala* next;
};

// ----- CITIRE MASINA -----
Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	if (!fgets(buffer, 100, file)) {
		Masina gol = { 0 };
		gol.model = NULL;
		gol.numeSofer = NULL;
		return gol;
	}
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

// ----- AFISARE MASINA -----
void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

// ----- INSERARE IN LISTA PRINCIPALA -----
NodListaPrincipala* inserareInListaPrincipala(NodListaPrincipala* cap, Masina m) {
	NodListaPrincipala* nou = (NodListaPrincipala*)malloc(sizeof(NodListaPrincipala));
	nou->info = m;
	nou->listaVecini = NULL;
	nou->next = NULL;

	if (!cap) return nou;

	NodListaPrincipala* temp = cap;
	while (temp->next)
		temp = temp->next;
	temp->next = nou;
	return cap;
}

// ----- CAUTARE NOD DUPA ID -----
NodListaPrincipala* cautaNodDupaID(NodListaPrincipala* cap, int id) {
	while (cap) {
		if (cap->info.id == id)
			return cap;
		cap = cap->next;
	}
	return NULL;
}

// ----- INSERARE MUCHIE -----
void inserareMuchie(NodListaPrincipala* cap, int idStart, int idStop) {
	NodListaPrincipala* nodStart = cautaNodDupaID(cap, idStart);
	NodListaPrincipala* nodStop = cautaNodDupaID(cap, idStop);

	if (nodStart && nodStop) {
		NodListaSecundara* muchie = (NodListaSecundara*)malloc(sizeof(NodListaSecundara));
		muchie->vecin = nodStop;
		muchie->next = nodStart->listaVecini;
		nodStart->listaVecini = muchie;
	}
}

// ----- CITIRE NODURI -----
NodListaPrincipala* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		printf("Fisierul nu poate fi deschis.\n");
		return NULL;
	}

	NodListaPrincipala* lista = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.model)
			lista = inserareInListaPrincipala(lista, m);
	}
	fclose(f);
	return lista;
}

// ----- CITIRE MUCHII -----
void citireMuchiiDinFisier(NodListaPrincipala* cap, const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		printf("Fisierul de muchii nu poate fi deschis.\n");
		return;
	}
	int start, stop;
	while (fscanf(f, "%d,%d", &start, &stop) == 2) {
		inserareMuchie(cap, start, stop);
	}
	fclose(f);
}

// ----- DEALOCARE -----
void dezalocareNoduriGraf(NodListaPrincipala* cap) {
	while (cap) {
		NodListaPrincipala* tmp = cap;
		cap = cap->next;

		free(tmp->info.model);
		free(tmp->info.numeSofer);

		NodListaSecundara* vecin = tmp->listaVecini;
		while (vecin) {
			NodListaSecundara* aux = vecin;
			vecin = vecin->next;
			free(aux);
		}
		free(tmp);
	}
}

// ----- NUMAR NODURI -----
int calculeazaNrNoduriGraf(NodListaPrincipala* cap) {
	int count = 0;
	while (cap) {
		count++;
		cap = cap->next;
	}
	return count;
}

// ----- DFS -----
void afisareGrafInAdancime(NodListaPrincipala* cap, int idPlecare) {
	int n = calculeazaNrNoduriGraf(cap);
	int* vizitat = (int*)calloc(n, sizeof(int));
	NodListaPrincipala** stiva = (NodListaPrincipala**)malloc(n * sizeof(NodListaPrincipala*));
	int top = -1;

	NodListaPrincipala* start = cautaNodDupaID(cap, idPlecare);
	if (!start) return;

	stiva[++top] = start;
	vizitat[start->info.id] = 1;

	while (top >= 0) {
		NodListaPrincipala* nod = stiva[top--];
		afisareMasina(nod->info);

		NodListaSecundara* vecin = nod->listaVecini;
		while (vecin) {
			if (!vizitat[vecin->vecin->info.id]) {
				stiva[++top] = vecin->vecin;
				vizitat[vecin->vecin->info.id] = 1;
			}
			vecin = vecin->next;
		}
	}
	free(stiva);
	free(vizitat);
}

// ----- BFS -----
void afisareGrafInLatime(NodListaPrincipala* cap, int idPlecare) {
	int n = calculeazaNrNoduriGraf(cap);
	int* vizitat = (int*)calloc(n, sizeof(int));
	NodListaPrincipala** coada = (NodListaPrincipala**)malloc(n * sizeof(NodListaPrincipala*));
	int front = 0, rear = 0;

	NodListaPrincipala* start = cautaNodDupaID(cap, idPlecare);
	if (!start) return;

	coada[rear++] = start;
	vizitat[start->info.id] = 1;

	while (front < rear) {
		NodListaPrincipala* nod = coada[front++];
		afisareMasina(nod->info);

		NodListaSecundara* vecin = nod->listaVecini;
		while (vecin) {
			if (!vizitat[vecin->vecin->info.id]) {
				coada[rear++] = vecin->vecin;
				vizitat[vecin->vecin->info.id] = 1;
			}
			vecin = vecin->next;
		}
	}
	free(coada);
	free(vizitat);
}

// ----- MAIN -----
int main() {
	NodListaPrincipala* graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier(graf, "muchii.txt");

	printf("\n--- Parcurgere in adancime ---\n");
	afisareGrafInAdancime(graf, 0); // ID-ul de start poate fi schimbat

	printf("\n--- Parcurgere in latime ---\n");
	afisareGrafInLatime(graf, 0);

	dezalocareNoduriGraf(graf);
	return 0;
}
