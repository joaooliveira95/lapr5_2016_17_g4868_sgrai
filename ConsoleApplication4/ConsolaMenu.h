#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <iostream>
#include <string>

using namespace std;

class ConsolaMenu
{
public:
	ConsolaMenu();
	~ConsolaMenu();

	void consolaMain();
private:
	void printCidades();
	void loading();
};
