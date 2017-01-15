#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "ConsolaMenu.h"

ConsolaMenu::ConsolaMenu()
{
}


ConsolaMenu::~ConsolaMenu()
{
}

void ConsolaMenu::loading() {
	cout << "Loading";
	for (int i = 0; i < 3; i++) {
//		_sleep(300);
		cout << ".";
	}
}

//ESTATICO
void ConsolaMenu::printCidades(Grafo grafo) {
	//char *cidades[2] = { "Porto","Brasilia" };
	//for (int i = 0; i < 2; i++) {
	//	cout << i << " : " << cidades[i] << endl;
	//}

	vector<string> cidades = grafo.obterCidades();
	int i = 0;
	for each (string cidade in cidades) {
		cout << i << ": " << cidade << endl;
		i++;
	}
}

void ConsolaMenu::printVisitas(Grafo grafo, string cidade) {
	vector<Visita> visitas = grafo.obterVisitas(cidade);
	cout << "0: Visita Livre" << endl;
	int i = 1;
	for each (Visita visita in visitas) {
		cout << i << ": " << visita.nome << endl;
		i++;
	}
}

Grafo ConsolaMenu::consolaMain(Grafo grafo) {

	int op, opcidade, opvisita;

	string username;
	string password;
	cout << "Bem vindo! " << endl;
	cout << "Escolha o tipo de utilizador" << endl;
	cout << "0 : Nao registado" << endl;
	cout << "1 : Registado" << endl;
	scanf_s("%d", &op);

	//nao registado
	if (op == 0) {
		//seleciona cidade
		cout << endl << "Utilizador Nao Registado--------------" << endl << endl;
		cout << "Cidades Disponiveis:" << endl;
		//FOR ESTATICO POR ENQUANTO
		printCidades(grafo);
		scanf_s("%d", &opcidade);
		//importamos a info da cidade com id opcidade
		if (opcidade >= 0 && opcidade < grafo.obterCidades().size()) {
			string cidadeEscolhida;
			cidadeEscolhida = grafo.obterCidades().at(opcidade);
			grafo.carregarGrafo(cidadeEscolhida);
			grafo.setOrigem(grafo.obterPonto(0));
			loading();
		}
		else {
			cout << "ERROR" << endl;
			exit(0);
		}
	}
	else if (op == 1) {
		cout << "Login" << endl;
		cout << "Username" << endl;
		cin >> username;
		cout << "Password" << endl;
		cin >> password;

		//// Faz ou não a autentificação com username e password
		bool acesso = grafo.obterAcesso(username, password);

		cout << endl << "Utilizador " << username << endl << endl;
		cout << "Cidades Disponiveis:" << endl;

		//// Obtem todas as cidades possiveis ara o usuario
		printCidades(grafo);
		scanf_s("%d", &opcidade);


		if (opcidade >= 0 && opcidade<grafo.obterCidades().size()) {
			string cidadeEscolhida;
			cidadeEscolhida = grafo.obterCidades().at(opcidade);
			//grafo.carregarGrafo(cidadeEscolhida);
			grafo.carregarGrafo(cidadeEscolhida);

			cout << "Visitas Dispoiveis: " << endl;
			printVisitas(grafo, cidadeEscolhida);
			scanf_s("%d", &opvisita);

			//// Define ou não a visita escolhida é uma opção apenas navegar no mapa
			if (opvisita == 0) {
				grafo.setOrigem(grafo.obterPonto(0));
				loading();
			}
			else if (opvisita - 1 >= 0 && opvisita <= grafo.obterVisitas(cidadeEscolhida).size()) {
				vector<Visita> v = grafo.obterVisitas(cidadeEscolhida);
				grafo.definirVisita(v[1]);
				Ponto orig = grafo.obterPonto(grafo.obterVisitas(cidadeEscolhida).at(opvisita - 1).origem);
				grafo.setOrigem(orig);
				return grafo;
				loading();
			}
			else {
				cout << "ERROR" << endl;
				exit(0);
			}
		}
		else {
			cout << "ERROR" << endl;
			exit(0);
		}
	}
	else {
		cout << "ERROR" << endl;
		exit(0);
	}
	return grafo;
}

void ConsolaMenu::imprime_ajuda() {
	printf("\n\nDesenho de um labirinto a partir de um grafo\n");
	printf("h,H - Ajuda \n");
	printf("i,I - Reset dos Valores \n");
	printf("******* Diversos ******* \n");
	printf("l,L - Alterna o calculo luz entre Z e eye (GL_LIGHT_MODEL_LOCAL_VIEWER)\n");
	printf("k,K - Alerna luz de camera com luz global \n");
	printf("s,S - PolygonMode Fill \n");
	printf("w,W - PolygonMode Wireframe \n");
	printf("p,P - PolygonMode Point \n");
	printf("c,C - Liga/Desliga Cull Face \n");
	printf("n,N - Liga/Desliga apresentação das normais \n");
	printf("******* grafos ******* \n");
	printf("F1  - Grava grafo do ficheiro \n");
	printf("F2  - Lê grafo para ficheiro \n");
	printf("F6  - Cria novo grafo\n");
	printf("******* Camera ******* \n");
	printf("Botão esquerdo - Arrastar os eixos (centro da camera)\n");
	printf("Botão direito  - Rodar camera\n");
	printf("Botão direito com CTRL - Zoom-in/out\n");
	printf("PAGE_UP, PAGE_DOWN - Altera distância da camara \n");
	printf("ESC - Sair\n");
}