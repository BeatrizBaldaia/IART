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
		cout<<"	p: "<<this->groups[i].getMembers()[0]->getName()<<"\n";

		this->groups[i].calculate_attributes();
	}
}

vector<int> TableManager::geneticAlgorithm(vector<vector<int> > &population, double p_cross, double p_mut, int max_stale_gens, int max_gens, int n_elite) const {
	printf("I am the genetic algorithm");
	int currentGen = 0;
	int numStaleGens = 0;
	while (currentGen < max_gens && numStaleGens < max_stale_gens) {
		vector<double> aval = this->avaliete(population);
		vector<int> elitedParentsIndexes = elitismSelection(population, aval, n_elite);
		vector<int> parentIndexes = selectParents(population, aval, n_elite);
		vector<vector<int> > children = crossParents(population, parentIndexes, p_cross);
		mutateChildren(children, p_mut);
		selectNextGen(population, elitedParentsIndexes, children);
		currentGen++;
	}
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

	return vector<int>();
}
vector<double> TableManager::avaliete(vector<vector<int> > &pop) const {
	vector<double> res;
	for(unsigned int i = 0; i < pop.size(); i++) {
		res.push_back(this->aval_fuct(pop[i]));
	}
	return res;
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
double TableManager::aval_fuct(const vector<int> &solution) const {
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
	cout << "Creating group!\n";
	this->groups.push_back(g);
	return &(this->groups.back());
}

vector<vector<unsigned int> > TableManager::vizinho_func(vector<unsigned int> &solution) { //indice group value table number
	vector<vector<unsigned int> > res;
	for (unsigned int i = 0; i < solution.size(); i++){
		for (unsigned int j = 0; j < this->tables.size(); j++){
			vector<unsigned int> neighbor = solution;
			if(solution[i] != j){
				neighbor[i] = j;
				res.push_back(neighbor);
			}
		}
	}
	return res;
}
vector<int> TableManager::elitismSelection(const vector<vector<int> > &population, vector<double> aval, int n_elite /*=-1*/) const{
	vector<int> res;
	if(n_elite > 0){
		cerr << "n_elite > 0, ainda não foi implementado!!!\n";
	}
	return res;
}
vector<int> TableManager::selectParents(const vector<vector<int> > &population, vector<double> aval, int n_elite) const{
	vector<int> res;
	//TODO: incorporate n_elite
	double scale_F = 0;
	for(unsigned int i = 0; i < population.size(); i++) {
		scale_F += aval[i];
	}
	for(unsigned int i = 0; i < population.size(); i++) {
		unsigned int j = 0;
		double random = (double)rand() / RAND_MAX; //double between 0 and 1
		while((aval[j]/scale_F) < random){
			j++;
		}
		res.push_back(j);
	}
	return res;
}
vector<vector<int> > TableManager::crossParents(const vector<vector<int> > &population, const vector<int> &parentIndexes, double p_cross) const{
	//TODO: implement
	return vector<vector<int> >();
}
void TableManager::mutateChildren(vector<vector<int> > &children, double p_mut) const{
	//TODO: implement
	return;
}
void TableManager::selectNextGen(vector<vector<int> > &population, const vector<int> &elitedParentsIndexes, const vector<vector<int> > &children) const{
	//TODO: implement
	return;
}

vector<vector<int> > TableManager::getRandomPopulation(unsigned int popSize) const{
	vector<vector <int> > res;
	srand (time(NULL));
	for(unsigned int i = 0; i < popSize; i++) {
		vector<int> gene;
		for(unsigned int j = 0; j < this->groups.size(); j++) {
			int table = rand() % this->tables.size();
			gene.push_back(table);
		}
		res.push_back(gene);
	}
	return res;
}

