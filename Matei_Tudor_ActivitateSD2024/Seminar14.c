#include <stdio.h>
#include <malloc.h>
#include <string.h>




typedef struct Metrou Metrou;
typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;
struct Metrou
{
	int serie;
	int nrStatie;
	char* magistrala;
};


struct NodPrincipal
{
	Metrou info;
	NodPrincipal* next;
	NodSecundar* vecini;
};


struct NodSecundar
{
	NodPrincipal* nod;
	NodSecundar* next;
};




//functie initializare Metrou
Metrou creareMetrou(int serie, int nrStatii, const char* magistrala)
{
	Metrou m;


	m.serie = serie;
	m.nrStatie = nrStatii;
	m.magistrala = (char*)malloc(sizeof(char) * (strlen(magistrala) + 1));
	strcpy_s(m.magistrala, strlen(magistrala) + 1, magistrala);


	return m;
}


//inserare la inceput in lista principala
void inserarePrincipala(NodPrincipal** cap, Metrou m)
{
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nou->info = m;
	nou->next = *cap;
	nou->vecini = NULL;


	*cap = nou;
}






//functie de cautare Nod dupa serie
NodPrincipal* cautaNodDupaSerie(NodPrincipal* graf, int serie)
{
	while (graf && graf->info.serie != serie)
	{
		graf = graf->next;
	}


	return graf;
}




//inserare la final in lista secundara
NodSecundar* inserareSecundara(NodSecundar* cap, NodPrincipal* nod)
{
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
	nou->next = NULL;
	nou->nod = nod;


	if (cap)
	{
		NodSecundar* p = cap;


		while (p->next)
		{
			p = p->next;
		}


		p->next = nou;


		return cap;
	}
	else
	{
		return nou;
	}


}


//functie de inserare muchie
void inserareMuchie(NodPrincipal* graf, int serieStart, int serieStop)
{
	NodPrincipal* nodStart = cautaNodDupaSerie(graf, serieStart);


	NodPrincipal* nodStop = cautaNodDupaSerie(graf, serieStop);


	if (nodStart && nodStop)
	{
		nodStart->vecini = inserareSecundara(nodStart->vecini, nodStop);


		nodStop->vecini = inserareSecundara(nodStop->vecini, nodStart);
	}
}




void afisareMetrou(Metrou m)
{
	printf("sierie : %d, nr statii: %d, magistrala: %s \n", m.serie, m.nrStatie, m.magistrala);
}




//fac pragma ca sa imi restranga codul (e strict estetic)
///regiune pt coada
#pragma region Coada


typedef struct NodCoada NodCoada;


struct NodCoada
{
	int serie;
	NodCoada* next;
};


//la capete diferite daca e coada (inserarea si extragerea)
// la stiva la aelasi capat




//inserare
void inserareCoada(NodCoada** cap, int serie) //serie - info de inserare
{
	NodCoada* nodNou = (NodCoada*)malloc(sizeof(NodCoada));
	nodNou->serie = serie;
	nodNou->next = NULL;


	if (*cap)
	{
		NodCoada* aux = *cap;
		while (aux->next != NULL)
		{
			aux = aux->next;
		}
		aux->next = nodNou;
	}
	else
	{
		*cap = nodNou;
	}
}




//extragere
int extragereCoada(NodCoada** cap)
{
	//extragem cap daca exista coada
	if (*cap)
	{
		int rezultat = (*cap)->serie;
		NodCoada* aux = (*cap);
		*cap = aux->next;
		free(aux);
		return rezultat;


	}
	else
	{
		return -1;
	}
}




#pragma endregion  




int getNumarNoduri(NodPrincipal* graf)
{
	int nr = 0;


	while (graf)
	{
		nr++;
		graf = graf->next;
	}


	return nr;
}




//afisare prin parcurgere prin latime (avem si adancime dar e cu stiva aia )
void afisarePrinLatime(NodPrincipal** graf, int serie)//seria primului nod
{
	//if(graf)
	NodCoada* coada = NULL;
	int nrNoduri = getNumarNoduri(graf);


	char* vizitate = (char*)malloc(nrNoduri);//nu e bool in C (nu int ca e cu 4 octeti)


	//initializam cu 0
	for (int i = 0; i < nrNoduri; i++)
	{
		vizitate[i] = 0;
	}




	//punem seria de pornire in coada(primul nod)
	inserareCoada(&coada, serie);


	//marcam nodul vizitat
	vizitate[serie] = 1;




	while (coada)
	{
		int serieNoua = extragereCoada(&coada);


		NodPrincipal* nou = cautaNodDupaSerie(graf, serieNoua);


		afisareMetrou(nou->info);


		//aflam vecinii nodului nou 
		NodSecundar* temp = nou->vecini;
		while (temp)
		{
			if (vizitate[temp->nod->info.serie] == 0)
			{
				vizitate[temp->nod->info.serie] = 1;
				inserareCoada(&coada, temp->nod->info.serie);
			}


			temp = temp->next;
		}
	}


	if (vizitate)
	{
		free(vizitate);
	}
}








void stergeVecini(NodSecundar** vecini)
{
	while (*vecini)
	{
		NodSecundar* temp = *vecini;


		*vecini = temp->next;


		free(temp);
	}
}




//functie de dezalocare
void dezalocareGraf(NodPrincipal** graf)//daca se modifica **
{
	while (*graf)
	{
		NodPrincipal* aux = *graf;
		free(aux->info.magistrala);




		//stergem lista de vecini
		while (aux->vecini)
		{
			NodSecundar* temp = aux->vecini;


			aux->vecini = temp->next;


			free(temp);
		}




		//stergeVecini(&(aux->vecini));


		*graf = aux->next;
		free(aux);


	}
}






void main()
{
	NodPrincipal* graf = NULL;


	inserarePrincipala(&graf, creareMetrou(4, 6, "M2")); //primul metrou


	inserarePrincipala(&graf, creareMetrou(3, 7, "M1"));


	inserarePrincipala(&graf, creareMetrou(2, 8, "M4"));


	inserarePrincipala(&graf, creareMetrou(1, 12, "M5"));


	inserarePrincipala(&graf, creareMetrou(0, 4, "M6"));




	inserareMuchie(graf, 0, 1);
	inserareMuchie(graf, 1, 2);
	inserareMuchie(graf, 1, 3);
	inserareMuchie(graf, 1, 4);
	inserareMuchie(graf, 2, 3);






	afisarePrinLatime(graf, 0);






}