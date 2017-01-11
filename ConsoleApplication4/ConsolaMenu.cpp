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
void ConsolaMenu::printCidades() {
	char *cidades[2] = { "Porto","Brasília" };
	for (int i = 0; i < 2; i++) {
		cout << i << " : " << cidades[i] << endl;
	}
}

void ConsolaMenu:: consolaMain() {
	int op, opcidade;
	
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
		cout << endl << "Utilizador Nao Registado--------------" << endl;
		cout << "Escolha a cidade." << endl;
		//FOR ESTATICO POR ENQUANTO
		printCidades();
		scanf("%d", &opcidade);
		//importamos a info da cidade com id opcidade
		loading();
	}else if (op == 1) {
		cout << "Login" << endl;
		cout << "Username" << endl;
		cin >> username;
		cout << "Password" << endl;
		cin >> password;

		//// Faz ou não a autentificação com username e password
		//bool acesso = grafo.obterAcesso("username", "password");

		cout << endl << "Utilizador " << username << endl;
		
		//// Obtem todas as cidades possiveis ara o usuario
		//vector<string> cidades = grafo.obterCidades();

		cout << "Escolha a cidade." << endl;

		//// Define ou não a visita escolhida é uma opção apenas navegar no mapa
		//grafo.definirVisita(visitas.front());

		printCidades();
		scanf("%d", &opcidade);
		loading();
	}
}
