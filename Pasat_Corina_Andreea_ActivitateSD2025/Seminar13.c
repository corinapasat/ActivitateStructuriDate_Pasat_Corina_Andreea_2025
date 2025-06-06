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
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

typedef struct NodLP NodLP;
typedef struct NodLS NodLS;

struct NodLP {
	Masina info;
	NodLP* next;
	NodLS* vecini;
};

struct NodLS
{
	NodLS* next;
	NodLP* nodInfo;
};

//2.
//functii de inserare in liste
//si in principala si in secundara

void inserareLP(NodLP** cap, Masina masina)
{
	NodLP* nou = (NodLP*)malloc(sizeof(NodLP));
	nou->next = NULL;
	nou->info = masina;
	nou->vecini = NULL;
	if (*cap) {
		NodLP* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}

void inserareLS(NodLS** cap, NodLP* nodInfo) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->next = *cap;
	nou->nodInfo = nodInfo;
	*cap = nou;
}


//3.
//functie de cautarea in lista principala dupa ID
NodLP* cautaNodDupaID(NodLP* cap, int id) {
	while (cap && cap->info.id != id) {
		cap = cap->next;
	}
	return cap;
}


//4.
//inserare muchie
void inserareMuchie(NodLP* cap, int idStart, int idStop) {
	NodLP* nodStart = cautaNodDupaID(cap, idStart);
	NodLP* nodStop = cautaNodDupaID(cap, idStop);
	if (nodStart && nodStop) {
		inserareLS(&(nodStart->vecini), nodStop);
		inserareLS(&(nodStop->vecini), nodStart);
	}
}


NodLP* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodLP* graf = NULL;
	if (f) {
		while (!feof(f)) {
			inserareLP(&graf, citireMasinaDinFisier(f));
		}
	}
	fclose(f);
	return graf;
}

void citireMuchiiDinFisier(const char* numeFisier, NodLP* graf) {
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			int idStart;
			int idStop;
			fscanf(f, "%d %d", &idStart, &idStop);
			inserareMuchie(graf, idStart, idStop);
		}
		fclose(f);
	}
}

void stergereLS(NodLS** cap) {
	while (*cap) {
		NodLS* aux = (*cap)->next;
		free(*cap);
		*cap = aux;
	}
}

void dezalocareNoduriGraf(NodLP** cap) {
	while (*cap) {
		NodLP* aux = (*cap)->next;
		stergereLS(&((*cap)->vecini));
		if ((*cap)->info.numeSofer) {
			free((*cap)->info.numeSofer);
		}
		if ((*cap)->info.model) {
			free((*cap)->info.model);
		}
		free(*cap);
		*cap = aux;
	}
}

NodLP* citireGrafDinFisiere(const char* fisierNoduri, const char* fisierMuchii) {
	NodLP* graf = citireNoduriMasiniDinFisier(fisierNoduri);
	citireMuchiiDinFisier(fisierMuchii, graf);
	return graf;
}

void afisareGraf(NodLP* graf) {
	while (graf) {
		printf("In nodul %d avem masina:", graf->info.id);
		afisareMasina(graf->info);
		printf("aceasta are ca masini nodurile urmatoare: ");

		NodLS* p = graf->vecini;
		while (p) {
			printf("%d; ", p->nodInfo->info.id);
			p = p->next;
		}
		printf("\n\n");
		graf = graf->next;
	}
}

//Parcurgere in adancime
typedef struct NodStiva NodStiva;

struct NodStiva {
	int id;
	NodStiva* next;
};

void push(NodStiva** cap, int id)
{
	NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
	nou->id = id;
	nou->next = *cap;
	*cap = nou;
}

int pop(NodStiva** cap) {
	if (*cap) {
		int id = (*cap)->id;
		NodStiva* aux = *cap;
		*cap = (*cap)->next;
		free(aux);
		return id;
	}
	return -1;
}

int calculeazaNrNoduriGraf(void* listaPrincipala) {
	NodLP* graf = (NodLP*)listaPrincipala;
	int nr = 0;
	while (graf) {
		nr++;
		graf = graf->next;
	}
	return nr;
}

void afisareGrafInAdancime(void* listaPrincipala, int idPlecare) {
	NodLP* graf = (NodLP*)listaPrincipala;
	int nrNoduri = calculeazaNrNoduriGraf(graf);
	int* vizitate = (int*)calloc(nrNoduri + 1, sizeof(int));
	NodStiva* stiva = NULL;

	push(&stiva, idPlecare);
	vizitate[idPlecare] = 1;

	while (stiva) {
		int idCurent = pop(&stiva);
		NodLP* nodCurent = cautaNodDupaID(graf, idCurent);

		if (nodCurent) {
			printf("Nodul %d: ", nodCurent->info.id);
			afisareMasina(nodCurent->info);

			NodLS* vecin = nodCurent->vecini;
			while (vecin) {
				int idVecin = vecin->nodInfo->info.id;
				if (!vizitate[idVecin]) {
					vizitate[idVecin] = 1;
					push(&stiva, idVecin);
				}
				vecin = vecin->next;
			}
		}
	}

	free(vizitate);
}

struct NodCoada {
	int id;
	struct NodCoada* next;
};

typedef struct NodCoada NodCoada;

void enqueue(NodCoada** cap, NodCoada** coada, int id) {
	NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
	nou->id = id;
	nou->next = NULL;

	if (*coada) {
		(*coada)->next = nou;
	}
	else {
		*cap = nou;
	}
	*coada = nou;
}

int dequeue(NodCoada** cap, NodCoada** coada) {
	if (*cap) {
		int id = (*cap)->id;
		NodCoada* aux = *cap;
		*cap = (*cap)->next;
		if (!*cap) {
			*coada = NULL;
		}
		free(aux);
		return id;
	}
	return -1;
}

void afisareGrafInLatime(void* listaPrincipala, int idPlecare) {
	NodLP* graf = (NodLP*)listaPrincipala;
	int nrNoduri = calculeazaNrNoduriGraf(graf);
	int* vizitate = (int*)calloc(nrNoduri + 1, sizeof(int));
	NodCoada* capCoada = NULL;
	NodCoada* coada = NULL;

	enqueue(&capCoada, &coada, idPlecare);
	vizitate[idPlecare] = 1;

	while (capCoada) {
		int idCurent = dequeue(&capCoada, &coada);
		NodLP* nodCurent = cautaNodDupaID(graf, idCurent);

		if (nodCurent) {
			printf("Nodul %d: ", nodCurent->info.id);
			afisareMasina(nodCurent->info);

			NodLS* vecin = nodCurent->vecini;
			while (vecin) {
				int idVecin = vecin->nodInfo->info.id;
				if (!vizitate[idVecin]) {
					vizitate[idVecin] = 1;
					enqueue(&capCoada, &coada, idVecin);
				}
				vecin = vecin->next;
			}
		}
	}

	free(vizitate);
}


int main() {

	NodLP* graf = citireGrafDinFisiere("masini.txt", "muchii.txt");
	afisareGraf(graf);
	afisareGrafInAdancime(graf, 1);

	printf("\n\nLatime\n\n");
	afisareGrafInLatime(graf, 1);

	return 0;
}