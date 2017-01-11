#include "Grafo.h"

json::value Grafo::httpGetJSON(string url) {
	url = urlAPI + url;
	try {
		wstring baseUrl(url.begin(), url.end());
		http_client httpClient(baseUrl);
		http_response httpResponse = httpClient.request(methods::GET).get();
		if (httpResponse.status_code() == status_codes::OK)
		{
			return httpResponse.extract_json().get();
		}
	}
	catch (exception& e) {
		cout << "{error : '" << e.what() << "'}";
	}
	return NULL;
}

Ponto Grafo::obterPonto(string nome) {
	for (Ponto ponto : pontos) {
		if (ponto.nome == nome) {
			return ponto;
		}
	}
	Ponto ponto;
	return ponto;
}

Ligacao Grafo::obterLigacao(string origem, string destino) {
	for (Ligacao &ligacao : ligacoes) {
		if (ligacao.origem.nome == origem && ligacao.destino.nome == destino) {
			return ligacao;
		}
	}
	Ligacao ligacao;
	return ligacao;
}

bool Grafo::visitaPonto(string nome) {
	return false;
}

bool Grafo::visitaLigacao(string origem, string destino) {
	return false;
}

int Grafo::quantidadePontos() {
	return pontos.size();
}

int Grafo::quantidadeLigacoes() {
	return ligacoes.size();
}

Ponto Grafo::obterPonto(int indice) {
	return pontos.at(indice);
}

Ligacao Grafo::obterLigacao(int indice) {
	return ligacoes.at(indice);
}

void Grafo::adicionarPonto(string nome, string descricao, double latitude, double longitude, double altitude, string abertura, string fecho) {
	Ponto ponto;
	ponto.nome = nome;
	ponto.descricao = descricao;
	ponto.abertura = abertura;
	ponto.fecho = fecho;
	ponto.latitude = latitude;
	ponto.longitude = longitude;
	ponto.altitude = altitude;
	ponto.visitar = visitaPonto(nome);
	pontos.push_back(ponto);
}

void Grafo::adicionarLigacao(string origem, string destino, double distancia, double largura) {
	Ligacao ligacao;
	ligacao.origem = obterPonto(origem);
	ligacao.destino = obterPonto(destino);
	ligacao.distancia = distancia;
	ligacao.largura = largura;
	ligacao.visitar = visitaLigacao(origem, destino);
	ligacoes.push_back(ligacao);
}

void Grafo::definirVisita(Visita visita) {
	vector<Ponto> pontosVisita;
	for (unsigned int i = 0; i < pontos.size(); i++) {
		pontos[i].visitar = false;
		for (unsigned int j = 0; j < visita.paragens.size(); j++) {
			if (pontos[i].nome == visita.paragens[j]) {
				pontosVisita.push_back(pontos[i]);
				pontos[i].visitar = true;
			}
		}
	}
	visita.pontos = pontosVisita;
	vector<Ligacao> ligacoesVisita;
	for (unsigned int i = 0; i < ligacoes.size(); i++) {
		ligacoes[i].visitar = false;
		for (unsigned int j = 0; j < visita.percurso.size() - 1; j++) {
			if (ligacoes[i].origem.nome == visita.percurso[j] && ligacoes[i].destino.nome == visita.percurso[j+1]) {
				ligacoesVisita.push_back(ligacoes[i]);
				ligacoes[i].visitar = true;
			}
		}
	}
	visita.ligacoes = ligacoesVisita;
	this->visita = visita;
}

string Grafo::normaliza(string str) {
	size_t index;
	while ((index = str.find(' ')) != std::string::npos) {
		str = str.replace(index, 1, "%20");
	}
	return str;
}

bool Grafo::obterAcesso(string username, string password) {
	return true;
}

vector<string> Grafo::obterCidades() {
	vector<string> todasCidades;
	todasCidades.push_back("Porto");
	todasCidades.push_back("Brasilia");
	return todasCidades;
	/*
	vector<string> todasCidades;
	json::value cidades = httpGetJSON("api/cidade");
	for (unsigned int i = 0; i < cidades.size(); i++) {
		string nome = utility::conversions::to_utf8string(cidades[i].at(U("nome")).as_string());
		cout << nome << endl;
		todasCidades.push_back(nome);
	}
	return todasCidades;
	*/
}

void Grafo::carregarGrafo(string cidade) {
	adicionarPonto("Ponto1", "Ponto1", 0, -50, 2.5, "9h00", "18h00");
	adicionarPonto("Ponto2", "Ponto2", 0, -25, 5, "9h00", "18h00");
	adicionarPonto("Ponto3", "Ponto3", -25, -25, 7.5, "9h00", "18h00");
	adicionarPonto("Ponto4", "Ponto4", 25, -25, 15, "9h00", "18h00");
	adicionarPonto("Ponto5", "Ponto5", -25, 0, 0, "9h00", "18h00");
	adicionarPonto("Ponto6", "Ponto6", 25, 0, 20, "9h00", "18h00");
	adicionarPonto("Ponto7", "Ponto7", 25, 25, 10, "9h00", "18h00");
	adicionarLigacao("Ponto1", "Ponto2", 1, 7);
	adicionarLigacao("Ponto2", "Ponto3", 1, 3);
	adicionarLigacao("Ponto2", "Ponto4", 1, 3);
	adicionarLigacao("Ponto5", "Ponto3", 1, 6);
	adicionarLigacao("Ponto4", "Ponto6", 1, 6);
	adicionarLigacao("Ponto5", "Ponto6", 1, 4);
	adicionarLigacao("Ponto6", "Ponto7", 1, 5);
	adicionarLigacao("Ponto1", "Ponto3", 1, 4);
	adicionarLigacao("Ponto1", "Ponto4", 1, 4);
	adicionarLigacao("Ponto3", "Ponto6", 1, 3);
	adicionarLigacao("Ponto4", "Ponto5", 1, 3);
	adicionarLigacao("Ponto5", "Ponto7", 1, 7);
	for (int i = 0; i < quantidadePontos(); i++) {
		for (int j = 0; j < quantidadeLigacoes(); j++) {
			Ligacao ligacao = obterLigacao(j);
			if ((ligacao.origem.nome == pontos.at(i).nome || ligacao.destino.nome == pontos.at(i).nome) && pontos.at(i).largura < ligacao.largura) {
				pontos.at(i).largura = ligacao.largura;
			}
		}
	}
	/*
	cidade = normaliza(cidade);
	json::value pontos = httpGetJSON("api/cidade/nome/" + cidade + "/ponto");
	for (unsigned int i = 0; i < pontos.size(); i++) {
		auto ponto = pontos[i];
		string nome = utility::conversions::to_utf8string(ponto.at(U("nome")).as_string());
		string descricao = utility::conversions::to_utf8string(ponto.at(U("descricao")).as_string());
		double latitude = ponto.at(U("latitude")).as_double();
		double longitude = ponto.at(U("longitude")).as_double();
		double altitude = ponto.at(U("altitude")).as_double();
		adicionarPonto(nome, descricao, latitude, longitude, altitude, "9h00", "18h00");
	}
	json::value ligacoes = httpGetJSON("api/cidade/nome/" + cidade + "/ligacao");
	for (unsigned int i = 0; i < ligacoes.size(); i++) {
		auto ligacao = ligacoes[i];
		string origem = utility::conversions::to_utf8string(ligacao.at(U("origem")).as_string());
		string destino = utility::conversions::to_utf8string(ligacao.at(U("destino")).as_string());
		double distancia = ligacao.at(U("distancia")).as_double();
		adicionarLigacao(origem, destino, distancia, 10);
	}
	for (unsigned int i = 0; i < quantidadePontos(); i++) {
		Ponto ponto = obterPonto(i);
		ponto.largura = 0.0;
		for (unsigned int j = 0; j < quantidadeLigacoes(); j++) {
			Ligacao ligacao = obterLigacao(j);
			if ((ligacao.origem.nome == ponto.nome || ligacao.destino.nome == ponto.nome) && ponto.largura < ligacao.largura) {
				ponto.largura = ligacao.largura;
			}
		}
	}
	*/
}

vector<Visita> Grafo::obterVisitas(string cidade) {
	Visita visita;
	visita.nome = "Visita";
	visita.descricao = "Visita descrição!";
	visita.data = "2017-01-09 21:00:18";
	visita.origem = "Torre dos Clérigos";
	vector<string> paragens;
	paragens.push_back("ISEP");
	paragens.push_back("Trindade");
	paragens.push_back("Camara do Porto");
	vector<string> percurso;
	percurso.push_back("Torre dos Clérigos");
	percurso.push_back("Camara do Porto");
	percurso.push_back("Trindade");
	percurso.push_back("ISEP");
	visita.paragens = paragens;
	visita.percurso = percurso;
	vector<Visita> todasVisitas;
	todasVisitas.push_back(visita);
	return todasVisitas;
	/*
	cidade = normaliza(cidade);
	vector<Visita> todasVisitas;
	json::value visitas = httpGetJSON("api/visita");
	for (unsigned int i = 0; i < visitas.size(); i++) {
		string nome = utility::conversions::to_utf8string(visitas[i].at(U("nome")).as_string());
		string descricao = utility::conversions::to_utf8string(visitas[i].at(U("descricao")).as_string());
		string data = utility::conversions::to_utf8string(visitas[i].at(U("data")).as_string());
		string origem = utility::conversions::to_utf8string(visitas[i].at(U("origem")).as_string());
		Visita visita;
		visita.nome = nome;
		visita.descricao = descricao;
		visita.data = data;
		visita.origem = origem;
		cout << nome << endl;
		nome = normaliza(nome);
		cout << nome << endl;
		json::value pontosParagem = httpGetJSON("api/visita/nome/" + nome + "/ponto");
		vector<string> paragens;
		for (unsigned int j = 0; j < pontosParagem.size(); j++) {
			string nomeParagem = utility::conversions::to_utf8string(pontosParagem[i].at(U("nome")).as_string());
			paragens.push_back(nomeParagem);
		}
		visita.paragens = paragens;
		json::value pontosPercurso = httpGetJSON("api/visita/nome/" + nome + "/percurso");
		vector<string> percurso;
		for (unsigned int j = 0; j < pontosPercurso.size(); j++) {
			string nomePonto = utility::conversions::to_utf8string(pontosPercurso[i].at(U("nome")).as_string());
			percurso.push_back(nomePonto);
		}
		visita.percurso = percurso;
		todasVisitas.push_back(visita);
	}
	return todasVisitas; 
	*/
}

/* Exemplo de uso no ciclo correto

	// Faz ou não a autentificação com username e password
	bool acesso = grafo.obterAcesso("username", "password");

	// Obtem todas as cidades possiveis ara o usuario
	vector<string> cidades = grafo.obterCidades();

	// Cria o grafo com a cidade escolhida
	grafo.carregarGrafo("cidade");

	// Obtem todas as visitas do usuario para aquela cidade
	vector<Visita> visitas = grafo.obterVisitas("cidade");

	// Define ou não a visita escolhida é uma opção apenas navegar no mapa
	grafo.definirVisita(visitas.front());

*/