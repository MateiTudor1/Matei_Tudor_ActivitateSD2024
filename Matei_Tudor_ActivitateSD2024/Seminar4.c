#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>


struct Aeroport
{
	int id;
	char* nume;
	int nrTerminale;
};

struct Aeroport* adaugaAeroport(struct Aeroport* vector, struct Aeroport aeroport, int* dimensiune)
{
	struct Aeroport* copie = (struct Aeroport*)malloc(sizeof(struct Aeroport) * ((*dimensiune) + 1));

	copie[(*dimensiune)] = aeroport;
	for (int i = 0; i < (*dimensiune); i++)
	{
		copie[i] = vector[i];
	}

	/*for (int i = 0; i < dimensiune; i++)
	{

	}*///faceam daca aveam deepcopy
	if (vector != NULL)
		free(vector);//pentru ca avem shallow copy 

	(*dimensiune)++;

	return copie;
}

struct Aeroport* citireFisier(const char* numeFisier, int* nrAeroporturi)
{
	struct Aeroport* vectorAeroporturi;
	vectorAeroporturi = NULL;

	FILE* f = fopen(numeFisier, "r");

	if (f != NULL)
	{
		(*nrAeroporturi) = 0;

		char buffer[100];
		while (fgets(buffer, 99, f) != NULL)
		{
			struct Aeroport aeroport;
			char delimitator[] = ",\n";

			char* token = strtok(buffer, delimitator);
			aeroport.id = atoi(token);
			token = strtok(NULL, delimitator);

			aeroport.nume = malloc(sizeof(char) * strlen(token) + 1);
			strcpy(aeroport.nume, token);

			token = strtok(NULL, delimitator);
			aeroport.nrTerminale = atoi(token);

			vectorAeroporturi = adaugaAeroport(vectorAeroporturi, aeroport, nrAeroporturi);
		}
		fclose(f);
	}

	return vectorAeroporturi;
}

void citireMatriceFisier(const char* numeFisier, int* nrAeroporturiLinie, int nrClustere, struct Aeroport** aeroporturi)
{
	FILE* f = fopen(numeFisier, "r");

	if (f != NULL)
	{
		char buffer[100];
		while (fgets(buffer, 99, f) != NULL)
		{
			struct Aeroport aeroport;
			char delimitator[] = ",\n";

			char* token = strtok(buffer, delimitator);
			aeroport.id = atoi(token);
			token = strtok(NULL, delimitator);

			aeroport.nume = malloc(sizeof(char) * strlen(token) + 1);
			strcpy(aeroport.nume, token);

			token = strtok(NULL, delimitator);
			aeroport.nrTerminale = atoi(token);

			int poz = aeroport.nrTerminale % 2;

			aeroporturi[poz] = adaugaAeroport(aeroporturi[poz], aeroport, &(nrAeroporturiLinie[poz]));
		}
		fclose(f);
	}
}

void afisareAeroport(struct Aeroport* aeroport)
{
	if (aeroport != NULL)
	{
		printf("%d. %s are %d terminale.\t", aeroport->id, aeroport->nume, aeroport->nrTerminale);
	}
	else
		printf("Nu exista acest aeroport");

}

void afisareVectorAeroporturi(struct Aeroport* aeroporturi, int dim)
{

	printf("Afisare Vector:\n");
	for (int i = 0; i < dim; i++)
	{
		afisareAeroport(&aeroporturi[i]);
	}
}

void afisareMatrice(struct Aeroport** matrice, int nrClustere, int* nrColoane)
{
	for (int i = 0; i < nrClustere; i++)
	{
		for (int j = 0; j < nrColoane[i]; j++)
		{
			afisareAeroport(&matrice[i][j]);
		}
	}
}


struct Aeroport idCautat(struct Aeroport* aeroporturi, int dim, int id)
{
	for (int i = 0; i < dim; i++)
	{
		if (aeroporturi[i].id == idCautat)
		{
			return aeroporturi[i];
		}
	}

	struct Aeroport aeroportNULL = { -1,NULL,-1 };

	return aeroportNULL;
}

float mediaTerminale(struct Aeroport** matrice, int* nrAeroporturiLinie, int paritate)
{
	paritate = paritate % 2;
	int count = 0;
	int suma = 0;
	for (int i = 0; i < nrAeroporturiLinie[paritate]; i++)
	{
		suma += matrice[paritate][i].nrTerminale;
	}

	if (nrAeroporturiLinie[paritate])
	{
		return suma / nrAeroporturiLinie[paritate];
	}
	return 0;
}

struct Aeroport* cautaAeroportID(struct Aeroport** matrice, int* nrColoane, int nrLinii, int Id)
{
	for (int i = 0; i < nrLinii; i++)
	{
		for (int j = 0; j < nrColoane[i]; j++)
		{
			if (matrice[i][j].id == Id)
			{
				return &(matrice[i][j]);
			}
		}
	}
	return NULL;
}

void dezalocareVector(struct Aeroport** aeroporturi, int* dim)
{
	for (int i = 0; i < (*dim); i++)
	{
		free((*aeroporturi)[i].nume);
	}

	free(*aeroporturi);
	(*aeroporturi) = NULL;
	(*dim) = 0;
}

void dezalocareMatirce(struct Aeroport*** matrice, int** nrColoane, int* nrClustere)
{
	for (int i = 0; i < (*nrClustere); i++)
	{
		dezalocareVector(&((*matrice)[i]), &((*nrColoane)[i]));
	}
	free(*matrice);
	(*matrice) = NULL;
	free(*nrColoane);
	(*nrColoane) = NULL;
	(*nrClustere) = 0;
}

void main()
{
	struct Aeroport** aeroporturi;
	int nrLinii = 2;
	int* nrColoane = (int*)malloc(sizeof(int) * nrLinii);

	aeroporturi = (struct Aeroport**)malloc(sizeof(struct Aeroport*) * nrLinii);

	for (int i = 0; i < nrLinii; i++)
	{
		nrColoane[i] = 0;


		aeroporturi[i] = NULL;
	}

	citireMatriceFisier("Aeroport_Seminar4.txt", nrColoane, nrLinii, aeroporturi);
	afisareMatrice(aeroporturi, nrLinii, nrColoane);
	float medie = mediaTerminale(aeroporturi, nrColoane, 12);
	printf("Media partitate para: %.2f", medie);
	printf("\n\n\n");
	afisareAeroport(cautaAeroportID(aeroporturi, nrColoane, nrLinii, 8));
	dezalocareMatirce(&aeroporturi, &nrColoane, &nrLinii);
}
