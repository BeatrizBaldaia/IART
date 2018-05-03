#include <iostream>
#include <stdlib.h>
#include <map>
#include <stdio.h>
#include <float.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <algorithm>
#include "TableManager.h"
using namespace std;

void printVectorVectorInteger(vector<vector<int> > v);
/**
 * argv[1] = nome do ficheiro de pessoas
 * argv[2] = nome do ficheiro de de mesas
 *
 * @return 1 if arguments are wrong
 */
int main(int argc, const char * argv[]) {
	if(argc != 8) {
		cout << "Invalid arguments: <people_file> <tables_file>"
				<< "<p_cross> <p_mut> <n_elite> <max_stale_gens> <max_gens>\n\n";

		cout << "\t" << "n_elite: Number of most fit individuals chosen directly to the next generation.\n";
		cout << "\t" << "max_stale_gens: Maximum number of successive generations with no improvement.\n";
		cout << "\t" << "max_gens: Maximum number of generations.\n";
		return 1;
	}
	TableManager tableManager(argv[1], argv[2]);
	double p_cross = atof(argv[3]);
	double p_mut = atof(argv[4]);
	int max_stale_gens = atoi(argv[6]);
	int max_gens = atoi(argv[7]);
	int n_elite = atoi(argv[5]);
	vector<vector<int> > population = tableManager.getRandomPopulation(20);//TODO: popSize
	printVectorVectorInteger(population);
	vector<int> response = tableManager.geneticAlgorithm(population, p_cross, p_mut, max_stale_gens, max_gens, n_elite);
	for(int i = 0; i < response.size(); i++) {
		printf("O grupo %d está na mesa %d.\n",i,response.at(i));
	}
	cerr<<"FIM\n";
	return 0;
}

void printVectorVectorInteger(vector<vector<int> > v) {
	for(unsigned int i = 0; i < v.size(); i++) {
		for(unsigned int j = 0; j < v[i].size(); j++) {
			cout << v[i][j] << " | ";
		}
		cout << "\n";
	}
}
