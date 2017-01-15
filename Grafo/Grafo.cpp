#include "stdafx.h"
#include "Grafo.h"

json::value Grafo::httpPost(string username, string password) {
	try {
		string info = "username=" + username + "&password=" + password;
		wstring parametros(info.begin(), info.end());
		http_client httpClient(L"http://uvm152.dei.isep.ipp.pt/api/seguranca/entrar");
		http_request request(methods::POST);
		request.headers().add(L"Content-Type", L"application/x-www-form-urlencoded; charset=UTF-8");
		request.headers().add(L"Content-Length", L"100");
		request.headers().add(L"X-Requested-With", L"XMLHttpRequest");
		request.set_body(parametros);
		http_response httpResponse = httpClient.request(request).get();
		if (httpResponse.status_code() == status_codes::OK) {
			return httpResponse.extract_json().get();
		}
	}
	catch (exception& e) {
		return NULL;
	}
}

json::value Grafo::httpGetJSON(string url) {
	url = urlAPI + url;
	cout << url << endl;
	try {
		wstring baseUrl(url.begin(), url.end());
		http_client httpClient(baseUrl);
		http_request request(methods::GET);
		wstring authorization(this->token.begin(), this->token.end());
		request.headers().add(L"Authorization", authorization);
		http_response httpResponse = httpClient.request(request).get();
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

void Grafo::adicionarPonto(string nome, string descricao, string categoria, double latitude, double longitude, double altitude, string abertura, string fecho) {
	Ponto ponto;
	ponto.nome = nome;
	ponto.categoria = categoria;
	ponto.descricao = descricao;
	ponto.abertura = abertura;
	ponto.fecho = fecho;
	ponto.latitude = latitude;
	ponto.longitude = longitude;
	ponto.altitude = altitude;
	ponto.largura = 1;
	ponto.visitar = false;
	pontos.push_back(ponto);
}

void Grafo::adicionarLigacao(string nome, string origem, string destino, double distancia, double largura) {
	for (unsigned int i = 0; i < ligacoes.size(); i++) {
		auto ligacao = ligacoes[i];
		if (ligacao.origem.nome == destino && ligacao.destino.nome == origem) {
			ligacao.bidireccional = true;
			return;
		}
	}
	Ligacao ligacao;
	ligacao.nome = nome;
	ligacao.origem = obterPonto(origem);
	ligacao.destino = obterPonto(destino);
	ligacao.distancia = distancia;
	ligacao.largura = largura;
	ligacao.visitar = false;
	if (ligacao.origem.largura < largura) {
		ligacao.origem.largura = largura;
	}
	if (ligacao.destino.largura < largura) {
		ligacao.destino.largura = largura;
	}
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
	json::value token = httpPost(username, password);
	if (token == NULL) {
		return false;
	}
	string access_token = utility::conversions::to_utf8string(token.at(U("access_token")).as_string());
	string token_type = utility::conversions::to_utf8string(token.at(U("token_type")).as_string());
	this->token = token_type + " " + access_token;
	return true;
}

vector<string> Grafo::obterCidades() {
	vector<string> todasCidades;
	json::value cidades = httpGetJSON("api/cidade");
	cout << utility::conversions::to_utf8string(cidades.serialize()) << endl;
	for (unsigned int i = 0; i < cidades.size(); i++) {
		string nome = utility::conversions::to_utf8string(cidades[i].at(U("nome")).as_string());
		cout << nome << endl;
		todasCidades.push_back(nome);
	}
	return todasCidades;
}

void Grafo::carregarGrafo(string cidade) {
	cidade = normaliza(cidade);
	json::value pontos = httpGetJSON("api/cidade/" + cidade + "/ponto");
	for (unsigned int i = 0; i < pontos.size(); i++) {
		auto ponto = pontos[i];
		string nome = utility::conversions::to_utf8string(ponto.at(U("nome")).as_string());
		string categoria = utility::conversions::to_utf8string(ponto.at(U("categoria")).as_string());
		string descricao = utility::conversions::to_utf8string(ponto.at(U("descricao")).as_string());
		double latitude = ponto.at(U("latitude")).as_double()*SCALE_GRAPH;
		double longitude = ponto.at(U("longitude")).as_double()*SCALE_GRAPH;
		double altitude = ponto.at(U("altitude")).as_double();
		adicionarPonto(nome, descricao, categoria, latitude, longitude, altitude, "9h00", "18h00");
	}
	json::value ligacoes = httpGetJSON("api/cidade/" + cidade + "/ligacao");
	for (unsigned int i = 0; i < ligacoes.size(); i++) {
		auto ligacao = ligacoes[i];
		string nome = utility::conversions::to_utf8string(ligacao.at(U("nome")).as_string());
		string origem = utility::conversions::to_utf8string(ligacao.at(U("origem")).as_string());
		string destino = utility::conversions::to_utf8string(ligacao.at(U("destino")).as_string());
		double distancia = ligacao.at(U("distancia")).as_double()*SCALE_GRAPH;
		double largura = ligacao.at(U("largura")).as_double();
		adicionarLigacao(nome, origem, destino, distancia, largura);
	}
}

vector<Visita> Grafo::obterVisitas(string cidade) {
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
		nome = normaliza(nome);
		json::value pontosParagem = httpGetJSON("api/visita/" + nome + "/ponto");
		vector<string> paragens;
		for (unsigned int j = 0; j < pontosParagem.size(); j++) {
			string nomeParagem = utility::conversions::to_utf8string(pontosParagem[j].at(U("nome")).as_string());
			paragens.push_back(nomeParagem);
		}
		visita.paragens = paragens;
		/*
		json::value pontosPercurso = httpGetJSON("api/visita/" + nome + "/paragens");
		vector<string> percurso;
		for (unsigned int j = 0; j < pontosPercurso.size(); j++) {
			string nomePonto = utility::conversions::to_utf8string(pontosPercurso[j].at(U("nome")).as_string());
			percurso.push_back(nomePonto);
		}
		visita.percurso = percurso;
		*/
		todasVisitas.push_back(visita);
	}
	return todasVisitas; 
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

Ponto Grafo::getOrigem() {
	return origem;
}

void Grafo::setOrigem(Ponto p) {
	origem = p;
}