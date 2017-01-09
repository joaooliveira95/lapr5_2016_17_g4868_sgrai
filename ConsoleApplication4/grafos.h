#ifndef _GRAFO_INCLUDE
#define _GRAFO_INCLUDE

#define _MAX_NOS_GRAFO 100
#define _MAX_ARCOS_GRAFO 200

#define NORTE_SUL	0
#define ESTE_OESTE	1
#define PLANO		2

typedef struct No {
	char* nome = "Nome";
	char* descricao = "Descricao";
	char* abertura = "9h00";
	char* fecho = "18h00";
	double x, y, z, largura;
	bool visitar = false;
	//int id;
} No;

typedef struct Arco{
	int noi,nof;
	float peso,largura;
	bool visitar = false;
}Arco;

extern No nos[];
extern Arco arcos[];
extern int numNos, numArcos;

void addNo(No);
void deleteNo(int);
void imprimeNo(No);
void listNos();
No criaNo(float, float, float);

void addArco(Arco);
void deleteArco(int);
void imprimeArco(Arco);
void listArcos();
Arco criaArco(int, int, float, float);

void gravaGrafo();
void leGrafo();

#endif