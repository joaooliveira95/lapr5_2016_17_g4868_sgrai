#pragma once

#include "cpprest/containerstream.h"
#include "cpprest/filestream.h"
#include "cpprest/http_client.h"
#include "cpprest/json.h"
#include "cpprest/producerconsumerstream.h"
#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include <vector>

#define SCALE_GRAPH 10000

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
	string nome, descricao, categoria, abertura, fecho;
	double latitude, longitude, altitude, largura;
	bool visitar;
};

class Ligacao {
public:
	string nome;
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
	string urlAPI = "http://uvm152.dei.isep.ipp.pt/";
	string token = "";
	string tokenTipo = "Bearer";
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
	json::value httpPost(string username, string password);
	json::value httpGetJSON(string url);
	void adicionarPonto(string nome, string descricao, string categoria, double latitude, double longitude, double altitude, string abertura, string fecho);
	void adicionarLigacao(string nome, string origem, string destino, double distancia, double largura);

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
