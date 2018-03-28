/*
 * Main.cpp
 *
 *  Created on: 27/03/2018
 *      Author: Utilizador
 */

#include <iostream>
#include "TableManager.h"
using namespace std;


/**
 * argv[1] = nome do ficheiro de pessoas
 * argv[2] = nome do ficheiro de de mesas
 * @return 1 if arguments are wrong
 */
int main(int argc, const char * argv[]) {
	if(argc != 3) {
		cout << "Invalid arguments: <people_file> <tables_file>\n";
		return 1;
	}
	TableManager tableManager(argv[1], argv[2]);
	cerr<<"FIM\n";
	return 0;
}

