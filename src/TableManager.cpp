#include "Person.h"
#include "Table.h"
#include "Group.h"
#include <map>
#include <stdio.h>
#include <float.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>

JobAreaMap JobAreaMap;
ReligionMap ReligionMap;
HobbyMap HobbyMap;

vector<Person> getPeopleFromFile(const char* filename);
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

void calcGroupsAffinity(vector<vector<double> > &groupsAffinity, const vector<Group> &groups) {
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

	vector<Person> people = getPeopleFromFile("people.txt");
	vector<Group> groups;
	vector<Table> tables;

	vector<vector<double> > groupsAffinity(groups.size(), vector<double>(groups.size()));
	calcGroupsAffinity(groupsAffinity, groups);
	vector<int> solution;//cada indice equivale a um group e o valor do mesmo equivale a mesa onde ela se vai sentar
	genetic_algorithm();
	return 0;
}


/**
 * Sum of affinities of group pairs sharing the same table minus penalty
 * TODO: Missing maximization of empty tables?
 */
double aval_fuct(const vector<int> &solution, const vector<vector<double> > &groupsAffinity) {
	double res = 0, penalty = -DBL_MAX;

	for (int i = 0; i < solution.size(); i++) {
		for (int j = i + 1; j < solution.size(); j++) {
			double affinity = groupsAffinity.at(i).at(j);
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

vector<Person> getPeopleFromFile(const char* filename){
	cout << "getPeopleFromFile\n";
	vector<Person> res;
	fstream myfile;
	string line;
	stringstream person;
	myfile.open (filename);
	if (myfile.is_open()) {
		while ( getline (myfile,line) ) {
			cout << line << '\n'; // TODO: delete
			person << line;
			Person p = Person();
			string value;
			getline (person,value,';');
			p.setName(value);
			cout << "Name: "<<p.getName() << '\n';
			getline (person,value,';');
			p.setAge(atoi(value.c_str()));
			cout << "Age: "<<p.getAge() << '\n';
			getline (person,value,';');
			p.setGroup(atoi(value.c_str()));
			cout << "Group: "<<p.getGroup() << '\n';
			//TODO: add to a group
			getline (person,value,';');
			p.setJob(JobAreaMap[value]);
			cout << "Job: "<< toString(p.getJob()) << '\n';
			getline (person,value,';');
			p.setReligion(ReligionMap[value]);
			cout << "Religion: "<< toString(p.getReligion()) << '\n';
			vector<Hobby> hobbies;
			cout << "Hobbies:\n";
			while ( getline (person,value,',') ) {
				hobbies.push_back(HobbyMap[value]);
				cout << "	"<< toString(hobbies.back()) << '\n';
			}
			p.setHobbies(hobbies);
			res.push_back(p);
		}
		myfile.close();
	}
	myfile.close();
	return res;
}

void vizinho_func() {

	//Trocar um group de mesa

}
