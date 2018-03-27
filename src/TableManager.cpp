#include "Person.h"
#include "Table.h"
#include "Group.h"
#include <map>
#include <stdio.h>


void genetic_algorithm(){
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

void calcGroupsAffinity(vector<vector<double>> &groupsAffinity, const vector<Group> &groups) {
	for (int row = 0; row < groups.size(); row++) {
		for (int col = row + 1; col < groups.size(); col++) {
			groupsAffinity[row][col] = groups.at(row).func_afinity(groups.at(col));
		}
	}
}

/**
 * argv[1] = nome do ficheiro de pessoas
 * argv[2] = nome do ficheiro de de mesas
 */
int main(int argc, const char * argv[]) {

	vector<Person> people;
	vector<Group> groups;
	vector<Table> tables;
	vector<vector<double>> groupsAffinity(groups.size(), vector<double>(groups.size()));
	calcGroupsAffinity(groupsAffinity, groups);
	vector<int> solution;//cada indice equivale a um group e o valor do mesmo equivale a mesa onde ela se vai sentar
	genetic_algorithm();
	return 0;
}



int aval_fuct(vector<int> solution) {
	int res = 0;

    //Funcao de afinidade entra cada par de groups na mesma mesa
	//funcao de pemnalizacao, valor dos gruopos com maior afinidade que nao estao na mesma mesa
	//AB
	//AC
	//BC
	return res;
}

void vizinho_func() {

	//Trocar um group de mesa

}
