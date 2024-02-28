#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct Motor {
    int cod;
    char* denumire;
    int nrPiese;
    float* preturi;
};

// Functie pentru citirea unui obiect de tipul Motor de la tastatura
struct Motor citeste_motor() {
    struct Motor motor;

    printf("Introduceti codul motorului: ");
    scanf("%d", &motor.cod);

    printf("Introduceti denumirea motorului: ");
    motor.denumire = (char*)malloc(100 * sizeof(char));
    scanf("%s", motor.denumire);

    printf("Introduceti numarul de piese: ");
    scanf("%d", &motor.nrPiese);

    motor.preturi = (float*)malloc(motor.nrPiese * sizeof(float));
    printf("Introduceti preturile pieselor:\n");
    for (int i = 0; i < motor.nrPiese; i++) {
        printf("Pretul piesei %d: ", i + 1);
        scanf("%f", &motor.preturi[i]);
    }

    return motor;
}

// Functie pentru calcularea mediei preturilor pieselor de pe un motor
float calculeaza_medie_preturi(struct Motor motor) {
    float suma = 0;
    for (int i = 0; i < motor.nrPiese; i++) {
        suma += motor.preturi[i];
    }
    return suma / motor.nrPiese;
}

// Functie pentru modificarea numarului de piese de pe un motor
void modifica_nr_piese(struct Motor* motor, int nou_nr_piese) {
    motor->nrPiese = nou_nr_piese;
    motor->preturi = (float*)realloc(motor->preturi, nou_nr_piese * sizeof(float));
}

// Functie pentru afisarea informatiilor despre un motor
void afiseaza_motor(struct Motor motor) {
    printf("Cod: %d\n", motor.cod);
    printf("Denumire: %s\n", motor.denumire);
    printf("Numar piese: %d\n", motor.nrPiese);
    printf("Preturi piese:\n");
    for (int i = 0; i < motor.nrPiese; i++) {
        printf("Piesa %d: %.2f\n", i + 1, motor.preturi[i]);
    }
}

// Functie pentru eliberarea memoriei alocate dinamic pentru un motor
void elibereaza_motor(struct Motor* motor) {
    free(motor->denumire);
    free(motor->preturi);
}

// Functie pentru alocarea unui vector de motoare
struct Motor* aloca_vector_motoare(int dimensiune) {
    return (struct Motor*)malloc(dimensiune * sizeof(struct Motor));
}

// Functie pentru copierea obiectelor care îndeplinesc o anumita conditie intr-un nou vector
// Conditia este ca media preturilor pieselor sa fie mai mica decât o anumita valoare data
int copiaza_motoare_conditie(struct Motor* motoare_sursa, int dimensiune_sursa, struct Motor* motoare_destinatie, float valoare_conditie) {
    int nr_motoare_copiate = 0;
    for (int i = 0; i < dimensiune_sursa; i++) {
        if (calculeaza_medie_preturi(motoare_sursa[i]) < valoare_conditie) {
            motoare_destinatie[nr_motoare_copiate++] = motoare_sursa[i];
        }
    }
    return nr_motoare_copiate;
}

// Functie pentru mutarea obiectelor care îndeplinesc o anumita conditie intr-un nou vector
// Conditia este ca numarul de piese sa fie mai mare decât o anumita valoare data
int muta_motoare_conditie(struct Motor* motoare_sursa, int dimensiune_sursa, struct Motor* motoare_destinatie, int valoare_conditie) {
    int nr_motoare_mutate = 0;
    for (int i = 0; i < dimensiune_sursa; i++) {
        if (motoare_sursa[i].nrPiese > valoare_conditie) {
            motoare_destinatie[nr_motoare_mutate++] = motoare_sursa[i];
        }
    }
    return nr_motoare_mutate;
}

// Functie pentru concatenarea a doi vectori de motoare
void concateneaza_motoare(struct Motor* motoare1, int dimensiune1, struct Motor* motoare2, int dimensiune2, struct Motor* motoare_concatenate) {
    for (int i = 0; i < dimensiune1; i++) {
        motoare_concatenate[i] = motoare1[i];
    }
    for (int i = 0; i < dimensiune2; i++) {
        motoare_concatenate[dimensiune1 + i] = motoare2[i];
    }
}

int main() {
    int dimensiune = 2;
    struct Motor* motoare = aloca_vector_motoare(dimensiune);

    printf("Introduceti detaliile pentru %d motoare:\n", dimensiune);
    for (int i = 0; i < dimensiune; i++) {
        motoare[i] = citeste_motor();
    }

    printf("\nMotoarele introduse sunt:\n");
    for (int i = 0; i < dimensiune; i++) {
        printf("Motor %d:\n", i + 1);
        afiseaza_motor(motoare[i]);
    }

    // Exemplu de copiere a motoarelor care au o medie a preturilor mai mica decât 500 intr-un alt vector
    float valoare_conditie_copiere = 500;
    struct Motor* motoare_copiate = aloca_vector_motoare(dimensiune);
    int nr_motoare_copiate = copiaza_motoare_conditie(motoare, dimensiune, motoare_copiate, valoare_conditie_copiere);

    printf("\nMotoarele cu media preturilor mai mica decat %.2f sunt:\n", valoare_conditie_copiere);
    for (int i = 0; i < nr_motoare_copiate; i++) {
        printf("Motor %d:\n", i + 1);
        afiseaza_motor(motoare_copiate[i]);
    }

    // Exemplu de mutare a motoarelor care au un numar de piese mai mare decât 5 intr-un alt vector
    int valoare_conditie_mutare = 5;
    struct Motor* motoare_mutate = aloca_vector_motoare(dimensiune);
    int nr_motoare_mutate = muta_motoare_conditie(motoare, dimensiune, motoare_mutate, valoare_conditie_mutare);

    printf("\nMotoarele cu un numar de piese mai mare decat %d sunt:\n", valoare_conditie_mutare);
    for (int i = 0; i < nr_motoare_mutate; i++) {
        printf("Motor %d:\n", i + 1);
        afiseaza_motor(motoare_mutate[i]);
    }

    // Exemplu de concatenare a doua vectori de motoare
    int dimensiune_noua = dimensiune + nr_motoare_mutate;
    struct Motor* motoare_concatenate = aloca_vector_motoare(dimensiune_noua);
    concateneaza_motoare(motoare, dimensiune, motoare_mutate, nr_motoare_mutate, motoare_concatenate);

    printf("\nVectorul de motoare concatenat este:\n");
    for (int i = 0; i < dimensiune_noua; i++) {
        printf("Motor %d:\n", i + 1);
        afiseaza_motor(motoare_concatenate[i]);
    }

    // Eliberarea memoriei alocate dinamic
    for (int i = 0; i < dimensiune; i++) {
        elibereaza_motor(&motoare[i]);
    }
    free(motoare);

    free(motoare_copiate);
    free(motoare_mutate);
    free(motoare_concatenate);

    return 0;
}