#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structura Masina
typedef struct StructuraMasina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
} Masina;

// Nod pentru stiva și coadă
typedef struct Nod {
    Masina info;
    struct Nod* next;
} Nod;

// Citire Masina din fișier
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

// Afișare Masina
void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi : %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

// ===================== STACK =====================

void pushStack(Nod** varf, Masina masina) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masina;
    nou->next = *varf;
    *varf = nou;
}

Masina popStack(Nod** varf) {
    Masina m;
    if (*varf) {
        Nod* temp = *varf;
        *varf = (*varf)->next;
        m = temp->info;
        free(temp);
    }
    else {
        m.id = -1; // semnalizare
    }
    return m;
}

int emptyStack(Nod* varf) {
    return varf == NULL;
}

Nod* citireStackMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        printf("Eroare deschidere fisier!\n");
        return NULL;
    }

    Nod* stiva = NULL;
    while (!feof(f)) {
        Masina m = citireMasinaDinFisier(f);
        pushStack(&stiva, m);
    }

    fclose(f);
    return stiva;
}

void dezalocareStivaDeMasini(Nod** stiva) {
    while (!emptyStack(*stiva)) {
        Masina m = popStack(stiva);
        free(m.model);
        free(m.numeSofer);
    }
}

int sizeStack(Nod* stiva) {
    int count = 0;
    while (stiva) {
        count++;
        stiva = stiva->next;
    }
    return count;
}

// ===================== QUEUE =====================

typedef struct {
    Nod* front;
    Nod* rear;
} Queue;

void enqueue(Queue* q, Masina masina) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masina;
    nou->next = NULL;
    if (q->rear) {
        q->rear->next = nou;
    }
    else {
        q->front = nou;
    }
    q->rear = nou;
}

Masina dequeue(Queue* q) {
    Masina m;
    if (q->front) {
        Nod* temp = q->front;
        m = temp->info;
        q->front = q->front->next;
        if (!q->front) q->rear = NULL;
        free(temp);
    }
    else {
        m.id = -1; // semnalizare
    }
    return m;
}

Queue citireCoadaDeMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Queue q = { NULL, NULL };
    if (!f) {
        printf("Eroare deschidere fisier!\n");
        return q;
    }

    while (!feof(f)) {
        Masina m = citireMasinaDinFisier(f);
        enqueue(&q, m);
    }

    fclose(f);
    return q;
}

void dezalocareCoadaDeMasini(Queue* q) {
    while (q->front) {
        Masina m = dequeue(q);
        free(m.model);
        free(m.numeSofer);
    }
}

// ===================== PROCESARE =====================

Masina getMasinaByID(Nod* lista, int id) {
    while (lista) {
        if (lista->info.id == id) return lista->info;
        lista = lista->next;
    }
    Masina m;
    m.id = -1; // nu a fost gasit
    return m;
}

float calculeazaPretTotal(Nod* lista) {
    float total = 0;
    while (lista) {
        total += lista->info.pret;
        lista = lista->next;
    }
    return total;
}

float calculeazaPretTotalQueue(Queue q) {
    float total = 0;
    Nod* temp = q.front;
    while (temp) {
        total += temp->info.pret;
        temp = temp->next;
    }
    return total;
}

// ===================== MAIN =====================

int main() {
    printf("--- STACK ---\n");
    Nod* stiva = citireStackMasiniDinFisier("masini.txt");
    printf("Numar masini in stiva: %d\n", sizeStack(stiva));
    printf("Pret total: %.2f\n", calculeazaPretTotal(stiva));
    Masina cautata = getMasinaByID(stiva, 2);
    if (cautata.id != -1) {
        printf("Masina cu ID 2:\n");
        afisareMasina(cautata);
    }

    printf("\n--- QUEUE ---\n");
    Queue coada = citireCoadaDeMasiniDinFisier("masini.txt");
    printf("Pret total in coada: %.2f\n", calculeazaPretTotalQueue(coada));

    // Dezalocari
    dezalocareStivaDeMasini(&stiva);
    dezalocareCoadaDeMasini(&coada);

    return 0;
}
