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
		_sleep(300);
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
	for each (Visita visita in visitas){
		cout << i << ": " << visita.nome << endl;
		i++;
	}
}

Grafo ConsolaMenu:: consolaMain(Grafo grafo) {
	
	int op, opcidade, opvisita;
	
	string username;
	string password;
	cout << "Bem vindo! " << endl;
	cout << "Escolha o tipo de utilizador" << endl;
	cout << "0 : Nao registado" << endl;
	cout << "1 : Registado" << endl;
	scanf("%d", &op);

	//nao registado
	if (op == 0) {
		//seleciona cidade
		cout << endl << "Utilizador Nao Registado--------------" << endl << endl;
		cout << "Cidades Disponiveis:" << endl;
		//FOR ESTATICO POR ENQUANTO
		printCidades(grafo);
		scanf("%d", &opcidade);
		//importamos a info da cidade com id opcidade
		if (opcidade >= 0 && opcidade < grafo.obterCidades().size()) {
			string cidadeEscolhida;
			cidadeEscolhida = grafo.obterCidades().at(opcidade);
			grafo.carregarGrafo(cidadeEscolhida);
			loading();
		}else {
			cout << "ERROR" << endl;
			exit(0);
		}
	}else if (op == 1) {
		cout << "Login" << endl;
		cout << "Username" << endl;
		cin >> username;
		cout << "Password" << endl;
		cin >> password;

		//// Faz ou não a autentificação com username e password
		bool acesso = grafo.obterAcesso("username", "password");

		cout << endl << "Utilizador " << username << endl << endl;
		cout << "Cidades Disponiveis:" << endl;
		
		//// Obtem todas as cidades possiveis ara o usuario
		printCidades(grafo);
		scanf("%d", &opcidade);

	
		if (opcidade >= 0 && opcidade<grafo.obterCidades().size()) {
			string cidadeEscolhida;
			cidadeEscolhida = grafo.obterCidades().at(opcidade);
			//grafo.carregarGrafo(cidadeEscolhida);
			grafo.carregarGrafo(cidadeEscolhida);

			cout << "Visitas Dispoiveis: " << endl;
			printVisitas(grafo, cidadeEscolhida);
			scanf("%d", &opvisita);

			//// Define ou não a visita escolhida é uma opção apenas navegar no mapa
			if (opvisita == 0) {
				loading();
			}else if (opvisita - 1 >= 0 && opvisita <= grafo.obterVisitas(cidadeEscolhida).size()) {
				grafo.definirVisita(grafo.obterVisitas(cidadeEscolhida).at(opvisita-1));
				return grafo;
				loading();
			}
			else {
				cout << "ERROR" << endl;
				exit(0);
			}
		}else {
			cout << "ERROR" << endl;
			exit(0);
		}
	}else {
		cout << "ERROR" << endl;
		exit(0);
	}
	return grafo;
}
