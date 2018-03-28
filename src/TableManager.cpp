#include "TableManager.h"
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

TableManager::TableManager(const char * peopleFile, const char * tablesFile) {
	this->getPeopleFromFile(peopleFile);
	this->getTablesFromFile(tablesFile);
	this->creatGroups();
	for (unsigned int i=0; i <this->groups.size(); i++){
		cout<<"Group: "<<this->groups[i].getId()<<"\n";
		this->groups[i].calculate_attributes();
	}
}


vector<int> TableManager::geneticAlgorithm(vector<vector<int> > &population) const {
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
	for (unsigned int row = 0; row < groups.size(); row++) {
		for (unsigned int col = row + 1; col < groups.size(); col++) {
			groupsAffinity[row][col] = groups.at(row).func_afinity(groups.at(col));
		}
	}
}


/**
 * Calculates the fitness value of one possible solution
 * Sum of affinities of group pairs sharing the same table minus penalty
 * TODO: Missing maximization of empty tables?
 */
double TableManager::aval_fuct(const vector<int> &solution) {
	double res = 0, penalty = -DBL_MAX;

	for (unsigned int i = 0; i < solution.size(); i++) {
		for (unsigned int j = i + 1; j < solution.size(); j++) {
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

void TableManager::getPeopleFromFile(const char* filename){
	cout << "getPeopleFromFile\n";
	fstream myfile;
	string line;
	myfile.open (filename);
	if (myfile.is_open()) {
		while ( getline (myfile,line) ) {
			stringstream person;
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
			this->people.push_back(p);
		}
		myfile.close();
	}
	myfile.close();
	return;
}
void TableManager::getTablesFromFile(const char* filename){
	cout << "getTablesFromFile\n";
	fstream myfile;
	string line;
	myfile.open (filename);
	if (myfile.is_open()) {
		while ( getline (myfile,line) ) {
			stringstream table;
			cout << line << '\n'; // TODO: delete
			table << line;
			string lugares;
			string nuTables;
			getline (table,lugares,';');
			getline (table,nuTables,';');
			int numberOfTables = atoi(nuTables.c_str());
			int numberOfSeats = atoi(lugares.c_str());
			while(numberOfTables>0){
				numberOfTables--;
				Table t = Table();
				t.setNumberOfSeats(numberOfSeats);
				this->tables.push_back(t);
			}
		}
		myfile.close();
	}
	myfile.close();
	for(unsigned int i = 0; i < this->tables.size(); i++){
		cout << "Table "<<i<<": "<<this->tables[i].getNumberOfSeats()<<"\n";
	}
	return;
}
void TableManager::creatGroups(){
	for (unsigned int i = 0; i < this->people.size(); i++) {
		this->getGroup(this->people[i].getGroup())->addMember(&(this->people[i]));
	}
}
Group * TableManager::getGroup(int id){
	for( unsigned int i = 0; i < this->groups.size(); i++) {
		if(this->groups[i].getId() == id) {
			return &(this->groups[i]);
		}
	}
	Group g(id);
	this->groups.push_back(g);
	return &(this->groups.back());
}
void vizinho_func() {

	//Trocar um group de mesa

}
