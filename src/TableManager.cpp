#include "TableManager.h"
#include <map>
#include <stdio.h>
#include <float.h>


vector<int> TableManager::geneticAlgorithm(vector<vector<int>> population){
	printf("I am the genetic algorithm");
	//Create random pool of solutions
	//REPEAT ate ...
		//SEE aval of each solution
		//Calcular % of selection
		//Random Selection
		//Random crussover
			//FOR ALL crossover
			//Random point of crossover
			//Cross
		//Mutation
		//Second gen
}

/**
 * Generate the affinity matrix (with all affinities between groups)
 */
void TableManager::calcGroupsAffinity() {
	for (int row = 0; row < groups.size(); row++) {
		for (int col = row + 1; col < groups.size(); col++) {
			groupsAffinity[row][col] = groups.at(row).func_afinity(groups.at(col));
		}
	}
}


/**
 * Calculates the fitness value of one possible solution
 * Sum of affinities of group pairs sharing the same table minus penalty
 *
 */
double TableManager::aval_fuct(const vector<int> &solution) {
	double res = 0, penalty = -DBL_MAX;

	for (int i = 0; i < solution.size(); i++) {
		for (int j = i + 1; j < solution.size(); j++) {
			double affinity = this->groupsAffinity.at(i).at(j);
			if (solution.at(i) == solution.at(j)) { // same table
				res += affinity;
			} else if (affinity > penalty) { // if new max affinity in separated tables
				penalty = affinity;
			}
		}
	}

	res -= penalty;

    //Funcao de afinidade entra cada par de groups na mesma mesa
	//funcao de penalizacao, valor dos grupos com maior afinidade que nao estao na mesma mesa
	//AB
	//AC
	//BC
	return res;
}

void vizinho_func() {

	//Trocar um group de mesa

}
