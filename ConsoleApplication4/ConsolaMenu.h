#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <iostream>
#include <string>
#include "Grafo.h"

using namespace std;

class ConsolaMenu
{
public:
	ConsolaMenu();
	~ConsolaMenu();

	void consolaMain(Grafo grafo);
private:
	void printVisitas(Grafo grafo, string cidade);
	void printCidades(Grafo grafo);
	void loading();
};
