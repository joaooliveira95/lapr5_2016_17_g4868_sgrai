#pragma once

#include "cpprest/containerstream.h"
#include "cpprest/filestream.h"
#include "cpprest/http_client.h"
#include "cpprest/json.h"
#include "cpprest/producerconsumerstream.h"
#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include <vector>

using namespace ::pplx;
using namespace utility;
using namespace concurrency::streams;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::json;
using namespace std;

class Ponto {
public:
	string nome, descricao, abertura, fecho;
	double latitude, longitude, altitude, largura;
	bool visitar;
};

class Ligacao {
public:
	string nome = "Nome";
	Ponto origem, destino;
	double distancia, largura;
	bool visitar;
	bool bidireccional;
};

class Visita {
public:
	string nome;
	string descricao;
	string data;
	string origem;
	vector<string> paragens;
	vector<string> percurso;
	vector<Ponto> pontos;
	vector<Ligacao> ligacoes;
};

class Grafo {
private:
	string urlAPI = "http://127.0.0.1:8000/";
	vector<Ponto> pontos;
	vector<Ligacao> ligacoes;
	Visita visita;
	Ponto origem;

public:
	Ponto getOrigem();
	void setOrigem(Ponto p);
	Ponto obterPonto(string nome);
	Ligacao obterLigacao(string origem, string destino);
	string normaliza(string str);
	json::value httpGetJSON(string url);
	bool visitaPonto(string nome);
	bool visitaLigacao(string origem, string destino);
	void adicionarPonto(string nome, string descricao, double latitude, double longitude, double altitude, string abertura, string fecho);
	void adicionarLigacao(string origem, string destino, double distancia, double largura);

	bool obterAcesso(string username, string password);
	vector<string> obterCidades();
	void carregarGrafo(string cidade);
	vector<Visita> obterVisitas(string cidade);
	void definirVisita(Visita visita);

	int quantidadePontos();
	int quantidadeLigacoes();
	Ponto obterPonto(int indice);
	Ligacao obterLigacao(int indice);
};
