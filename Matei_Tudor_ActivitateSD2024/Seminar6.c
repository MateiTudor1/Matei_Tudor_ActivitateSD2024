#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include <string.h>

typedef struct Biblioteca Biblioteca;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Biblioteca {
    char* nume;
    int nrCarti;
    int nrCititori;
};

struct Nod {
    Biblioteca info;
    Nod* prev;
    Nod* next;
};

struct ListaDubla {
    Nod* head;
    Nod* tail;
};

Biblioteca initializare(const char* nume, int nrCarti, int nrCititori) {
    Biblioteca b;
    b.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
    strcpy(b.nume, nume);
    b.nrCarti = nrCarti;
    b.nrCititori = nrCititori;
    return b;
}

void insereazaInceput(ListaDubla* lista, Biblioteca b) {
    Nod* nod_adaugat = (Nod*)malloc(sizeof(Nod));
    nod_adaugat->info = b;
    nod_adaugat->prev = NULL;
    nod_adaugat->next = lista->head;

    if (lista->head != NULL) {
        lista->head->prev = nod_adaugat;
    }
    else {
        lista->tail = nod_adaugat;
    }

    lista->head = nod_adaugat;
}

void afiseazaListaInceput(ListaDubla lista) {
    while (lista.head) {
        printf("Nume: %s, nr. carti: %d, nr.cititori: %d\n", lista.head->info.nume, lista.head->info.nrCarti, lista.head->info.nrCititori);
        lista.head = lista.head->next;
    }
}

void stergeNodDupaNume(ListaDubla* lista, const char* nume) {
    Nod* aux = lista->head;

    while (aux && strcmp(nume, aux->info.nume) != 0) {
        aux = aux->next;
    }

    if (aux) {
        if (aux == lista->head) {
            if (aux == lista->head) {
                lista->head = NULL;
                lista->tail = NULL;
            }
            else {
                lista->head = lista->head->next;
                lista->head->prev = NULL;
            }
        }
        else if (aux == lista->tail) {
            aux->prev->next = NULL;
            lista->tail = aux->prev;
        }
        else {
            aux->next->prev = aux->prev;
            aux->prev->next = aux->next;
        }


        free(aux->info.nume);
        free(aux);
    }

}

void stergeLista(ListaDubla* lista) {
    while (lista->head) {
        free(lista->head->info.nume);
        Nod* aux = lista->head;
        lista->head = lista->head->next;
        free(aux);
    }

    lista->head = NULL;
    lista->tail = NULL;
}

int nrCartiTotal(ListaDubla lista) {
    int rezultat = 0;

    while (lista.tail) {
        rezultat += lista.tail->info.nrCarti;
        lista.tail = lista.tail->prev;
    }

    return rezultat;
}

void main() {
    Biblioteca b1 = initializare("Mihai Eminescu", 150, 30);
    Biblioteca b2 = initializare("Ioan Slavici", 200, 30);
    Biblioteca b3 = initializare("Tudor Arghezi", 100, 15);

    ListaDubla listaDubla;
    listaDubla.head = NULL;
    listaDubla.tail = NULL;


    insereazaInceput(&listaDubla, b1);
    insereazaInceput(&listaDubla, b2);
    insereazaInceput(&listaDubla, b3);

    afiseazaListaInceput(listaDubla);

    //stergeNodDupaNume(&listaDubla, "Ioan Slavici");
    //printf("\n\nStergere mijloc:\n");
    //afiseazaListaInceput(listaDubla);

    //stergeNodDupaNume(&listaDubla, "Mihai Eminescu");
    //printf("\n\nStergere inceput:\n");
    //afiseazaListaInceput(listaDubla);

    //stergeNodDupaNume(&listaDubla, "Tudor Arghezi");
    //printf("\n\nStergere final: \n");
    //afiseazaListaInceput(listaDubla);

    printf("%d carti in total", nrCartiTotal(listaDubla));

    stergeLista(&listaDubla);
}