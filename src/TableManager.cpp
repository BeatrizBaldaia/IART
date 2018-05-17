#include "TableManager.h"
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
#include <math.h>

#include "Utils/utils.h"

JobAreaMap JobAreaMap;
ReligionMap ReligionMap;
HobbyMap HobbyMap;

TableManager::TableManager(const char * peopleFile, const char * tablesFile, double p_cross, double p_mut, int max_stale_gens, int max_gens, int n_gene, int n_elite, int max_iters, int max_temp, int max_tries, CoolingSchedule schedule, MutationType mutType, bool backtrackingInitialGeneration)
{
	this->p_cross = p_cross;
	this->p_mut = p_mut;
	this->max_stale_gens = max_stale_gens;
	this->max_gens = max_gens;
	this->n_gene = n_gene;
	this->n_elite = n_elite;
	this->max_iters = max_iters;
	this->max_temp = max_temp;
	this->max_tries = max_tries;
	this->schedule = schedule;
	this->mutType = mutType;

	this->getPeopleFromFile(peopleFile);
	this->getTablesFromFile(tablesFile);
	this->createGroups();
	for (unsigned int i = 0; i < this->groups.size(); i++)
	{
		// cout << "Group: " << this->groups[i].getId() << "\n";
		// cout << "	p: " << this->groups[i].getMembers()[0]->getName() << "\n";

		this->groups[i].calculate_attributes();
	}
	// printf("Calling: calcGroupsAffinity\n");
	for (unsigned int i = 0; i < this->groups.size(); i++)
	{
		this->groupsAffinity.push_back(vector<double>(this->groups.size()));
	}
	this->calcGroupsAffinity();
}


vector<double> TableManager::evaluatePopulation(const vector<vector<int> > &pop) const
{
	vector<double> res;
	double minFitness = DBL_MAX;
	for (unsigned int i = 0; i < pop.size(); i++)
	{
		double fitness = fitnessFunction(pop[i]);
		if (fitness < minFitness) {
			minFitness = fitness;
		}
		res.push_back(fitness);
		cout << fitness << " | ";
	}
	cout << "\n";
	if (minFitness < 0) {
		for (double &elem : res) {
			elem += -minFitness + FITNESS_COMPENSATION; // summed so the worst element won't have 0 chance of being picked
		}
	}

	return res;
}
/**
 * Generate the affinity matrix (with all affinities between groups)
 */
void TableManager::calcGroupsAffinity()
{
	for (unsigned int row = 0; row < groups.size(); row++)
	{
		for (unsigned int col = row + 1; col < groups.size(); col++)
		{
			groupsAffinity[row][col] = groups.at(row).func_afinity(groups.at(col));
		}
	}
}

double TableManager::fitnessFunction (const vector<int> &solution) const
{
	double res = 0, penalty = -pow(10, 4);
	vector<int> tables = fillTables(solution);
	if (invalidGene(tables))
	{
		return penalty;
	}
	for (unsigned int i = 0; i < solution.size(); i++)
	{
		for (unsigned int j = i + 1; j < solution.size(); j++)
		{
			double affinity = this->groupsAffinity.at(i).at(j);
			if (solution.at(i) == solution.at(j))
			{ // same table
				res += affinity;
			}
			else if (affinity > penalty)
			{ // if new max affinity in separated tables
				penalty = affinity;
			}
		}
	}

	res -= penalty;
	return res;
}

void TableManager::getPeopleFromFile(const char *filename)
{
	fstream myfile;
	string line;
	myfile.open(filename);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			stringstream person;
			person << line;
			Person p = Person();
			string value;
			getline(person, value, ';');
			p.setName(value);
			getline(person, value, ';');
			p.setAge(atoi(value.c_str()));
			getline(person, value, ';');
			p.setGroup(atoi(value.c_str()));
			getline(person, value, ';');
			p.setJob(JobAreaMap[value]);
			getline(person, value, ';');
			p.setReligion(ReligionMap[value]);
			vector<Hobby> hobbies;
			while (getline(person, value, ','))
			{
				hobbies.push_back(HobbyMap[value]);
			}
			p.setHobbies(hobbies);
			this->people.push_back(p);
		}
		myfile.close();
	}
	myfile.close();
	return;
}
void TableManager::getTablesFromFile(const char *filename)
{
	fstream myfile;
	string line;
	myfile.open(filename);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			stringstream table;
			table << line;
			string lugares;
			string nuTables;
			getline(table, lugares, ';');
			getline(table, nuTables, ';');
			int numberOfTables = atoi(nuTables.c_str());
			int numberOfSeats = atoi(lugares.c_str());
			while (numberOfTables > 0)
			{
				numberOfTables--;
				Table t = Table();
				t.setNumberOfSeats(numberOfSeats);
				this->tables.push_back(t);
			}
		}
		myfile.close();
	}
	myfile.close();
	return;
}
void TableManager::createGroups()
{
	for (unsigned int i = 0; i < this->people.size(); i++)
	{
		this->getGroup(this->people[i].getGroup())->addMember(&(this->people[i]));
	}
}
Group *TableManager::getGroup(int id)
{
	for (unsigned int i = 0; i < this->groups.size(); i++)
	{
		if (this->groups[i].getId() == id)
		{
			return &(this->groups[i]);
		}
	}
	Group g(id);
	this->groups.push_back(g);
	return &(this->groups.back());
}

vector<int> TableManager::elitismSelection(const vector<double> &eval) const
{
	vector<int> res;
	vector<double> aux = eval;
	int n = this->n_elite;

	while (n > 0)
	{
		vector<double>::iterator max_iter = max_element(aux.begin(), aux.end());
		int index = distance(aux.begin(), max_iter);
		res.push_back(index);
		*max_iter = -DBL_MAX;
		n--;
	}
	return res;
}

vector<vector<int>> getElitedParents(const vector<vector<int>> &population, vector<int> indexes) {
	vector<vector<int>> result;
	for(int i = 0; i < indexes.size(); i++) {
		result.push_back(population[indexes[i]]);
	}

	return result;
}

vector<double> scaleFunction(vector<double> eval) {
	double scale_F = accumulate(eval.begin(), eval.end(), 0.0);
	for(double &e: eval) {
		e /= scale_F;
	}
	
	vector<double> range;
	range.push_back(eval[0]);
	for(int i = 1; i < eval.size(); i++) {
		range.push_back(range[i - 1] + eval[i]);
	}

	return range;
}

int getGeneFromRange(const vector<double> &range, double n) {
	for(int i = 0; i < range.size(); i++) {
		if(range[i] >= n) {
			return i;
		}
	}

	cerr << "Error in getGeneFromRange(): Number out of range.\n";
	return -1;
}

vector<int> TableManager::selectParents(const vector<vector<int>> &population, vector<double> eval, int nSelection) const
{
	vector<double> scale = scaleFunction(eval);
	vector<int> selectedGenes;

	for (unsigned int i = 0; i < nSelection; i++)
	{
		double random = getRandomBetween(0, 1);
		int index = getGeneFromRange(scale, random);
		selectedGenes.push_back(index);
	}
	
	return selectedGenes;
}

/**
 * @param uncrossedParentInds Indexes of parents who by probability do not cross, mapped in a vector of parent indexes calculated elsewhere.
 */
vector<int> TableManager::getGenesForCrossover(vector<int> &uncrossedParentInds, int nGenes) const {
	vector<int> result;
	int index = 0;
	while(nGenes > 0) {
		if(probable(this->p_cross)) {
			result.push_back(index);
		} else {
			uncrossedParentInds.push_back(index);
		}
		index++;
		nGenes--;
	}
	if (result.size() % 2 != 0) {
		uncrossedParentInds.push_back(result.back());
		result.pop_back();
	}
	return result;
}

void crossGenes(const vector<int> &gene1, const vector<int> &gene2, vector<int> &child1, vector<int> &child2) {
	int max = gene1.size() - 2, min = 1;
	int randNum = getRandomBetween(min, max);

	for(int i = randNum; i < gene1.size(); i++) {
		int seatA = gene1[i], seatB = gene2[i];
		child1[i] = seatB;
		child2[i] = seatA;
	}
}

vector<vector<int>> TableManager::crossParents(const vector<vector<int>> &population, const vector<int> &parentIndexes) const
{
	vector<vector<int>> children;
	vector<int> uncrossedParentInds;
	vector<int> crossCandidates = getGenesForCrossover(uncrossedParentInds, parentIndexes.size());
	
	for(int i = 0; i < crossCandidates.size(); i+=2) {
		int parent1n = crossCandidates[i], parent2n = crossCandidates[i + 1];
		vector<int> parent1 = population[parentIndexes[parent1n]], parent2 = population[parentIndexes[parent2n]];
		vector<int> child1 = parent1, child2 = parent2;
		crossGenes(parent1, parent2, child1, child2);
		children.push_back(child1);
		children.push_back(child2);
	}
	// add parents who don't cross by probability
	for (int &parentInd : uncrossedParentInds) {
		children.push_back(population[parentIndexes[parentInd]]);
	}
	
	return children;
}

void TableManager::singleMutation(vector<vector<int>> &children) const {
	if (children.empty()) {
		return;
	}
	int mutTable = getRandomBetween(0, children.size() * children[0].size() - 1);
	int mutTableRow = mutTable / children.size();
	int mutTableCol = mutTable % children[0].size();
	
	children[mutTableRow][mutTableCol] = getRandomBetween(0, tables.size() - 1);
}

void TableManager::swapMutGene(vector<int> &gene) const {
	if (gene.size() < 2) {
		return;
	}

	int mutTable1 = getRandomBetween(0, gene.size() - 1);
	int mutTable2 = getRandomBetween(0, gene.size() - 1);
	swap(gene[mutTable1], gene[mutTable2]);
}

void TableManager::swapMutation(vector<vector<int>> &children) const {
	if (children.empty()) {
		return;
	}

	int mutGene = getRandomBetween(0, children.size() - 1);
	swapMutGene(children[mutGene]);

	int mutTable1 = getRandomBetween(0, children.size() * children[0].size() - 1);
	int mutTable2 = getRandomBetween(0, children.size() * children[0].size() - 1);
}

void TableManager::mutateChildren(vector<vector<int> > &children) const
{
	if (!probable(this->p_mut)) {
		return;
	}
	
	switch (this->mutType) {
		case Single:
			singleMutation(children);
			break;
		case Swap:
			swapMutation(children);
			break;
		default:
			cerr << "Invalid mutation type " << mutType << ".\n";
			break;
	}
}

void TableManager::selectNextGen(vector<vector<int>> &population, const vector<vector<int>> &elitedParents, const vector<vector<int>> &children) const
{
	population = children;
	population.insert(population.end(), elitedParents.begin(), elitedParents.end());
}

vector<int> TableManager::geneticAlgorithm(vector<vector<int> > &population, double &maxScore) const
{
	int currentGen = 0;
	int numStaleGens = 0;
	double max_eval = -DBL_MAX;
	vector<int> res;
	vector<double> eval = evaluatePopulation(population);
		
	while (currentGen < this->max_gens && numStaleGens < this->max_stale_gens)
	{
		printf("%d Cycle\n", currentGen);
		vector<int> elitedParentsIndexes = elitismSelection(eval);
		vector<vector<int>> elitedParents = getElitedParents(population, elitedParentsIndexes);
		int nRandomSelection = this->n_gene - this->n_elite;
		vector<int> parentIndexes = selectParents(population, eval, nRandomSelection);
		vector<vector<int>> children = crossParents(population, parentIndexes);
		mutateChildren(children);
		selectNextGen(population, elitedParents, children);
		eval = evaluatePopulation(population);
		vector<double>::iterator largest_eval_it = max_element(eval.begin(), eval.end());
		if (max_eval >= *largest_eval_it)
		{
			numStaleGens++;
		}
		else
		{
			max_eval = *largest_eval_it;
			numStaleGens = 0;

			res = population.at(distance(eval.begin(), largest_eval_it));
		}
		currentGen++;
	}
	maxScore = max_eval;
	return res;
}

vector<int> TableManager::fillTables(const vector<int> &gene) const
{
	vector<int> tables(this->tables.size());
	for (unsigned int i = 0; i < gene.size(); i++)
	{
		tables[gene[i]] += this->groups[i].getMembers().size();
	}
	return tables;
}

bool TableManager::invalidGene(const vector<int> &tables) const
{

	for (unsigned int i = 0; i < tables.size(); i++)
	{
		if (this->tables[i].getNumberOfSeats() < tables[i])
		{
			//cerr << "Invalid values: The number of people is higher than the number of seats.\n";
			//cerr << "\tPeople: " << tables[i] << " ; Seats: " << this->tables[i].getNumberOfSeats() << endl;
			return true;
		}
	}
	return false;
}

void printRow(const vector<int> &v)
{
	for (int i = 0; i < v.size(); i++)
	{
		printf("%d| ", v[i]);
	}
	printf("\n");
}

bool TableManager::invalidTable(int seatsAtTable, int tableInd) const {
	return this->tables[tableInd].getNumberOfSeats() < seatsAtTable;
}

void TableManager::getGeneBacktracking(int table, vector<int> gene, vector<int> usedTables, vector<vector<int> > &solutions) const {
	int currGroup = gene.size();
	cout << currGroup << " | " << table << " | ";
	for (int i = 0; i < gene.size(); i++) {
		cout << gene[i] << " ";
	}
	cout << "\n";
	int nMembers = this->groups[currGroup].getMembers().size();
	usedTables[table] += nMembers;
	if (usedTables[table] > this->tables[table].getNumberOfSeats()) {
		return;
	}
	gene.push_back(table);
	if (gene.size() == this->groups.size()) {
		solutions.push_back(gene);
		cout << "valid gene\n";
		return;
	}
	for (int i = 0; i < this->tables.size(); i++) {
		getGeneBacktracking(i, gene, usedTables, solutions);
	}
}

vector<vector<int> > TableManager::getRandomPopulation(unsigned int popSize)
{
	vector<vector<int> > solutions;
	if (backtrackingInitialGeneration) {
		vector<int> gene;
		vector<int> usedTables(this->tables.size(), 0);
		for (int i = 0; i < this->tables.size(); i++) {
			getGeneBacktracking(i, gene, usedTables, solutions);
		}

		if (solutions.size() < popSize) {
			cout << "The number of possible solutions, " << solutions.size() << ", is lower than the desired population number, " << popSize << ".\n";
		}

		while (solutions.size() > popSize) {
			int ind = rand() % solutions.size();
			cout << solutions.size() << " | ";
			solutions.erase(solutions.begin() + ind);
			cout << "erased\n";
		}
	} else {
		for (int i = 0; i < popSize; i++) {
			vector<int> gene;
			for (int i = 0; i < this->groups.size(); i++) {
				int table = rand() % this->tables.size();
				gene.push_back(table);
			}
			solutions.push_back(gene);
		}
	}
	
	return solutions;

	// vector<vector<int>> res;

	// for (unsigned int i = 0; i < popSize; i++)
	// {
	// 	vector<int> gene;
		
	// 	getGeneBacktracking(gene);
	// 	// vector<int> auxTables(this->tables.size());
	// 	// bool invalidGene = false;
	// 	// do
	// 	// {
	// 	// 	invalidGene = false;
	// 	// 	gene.clear();
	// 	// 	fill(auxTables.begin(), auxTables.end(), 0);
	// 	// 	for (unsigned int j = 0; j < this->groups.size(); j++)
	// 	// 	{
	// 	// 		int nMembers = (this->groups[j].getMembers()).size();
	// 	// 		int table = rand() % this->tables.size();

	// 	// 		auxTables[table] += nMembers;

	// 	// 		if (invalidTable(auxTables[table], table)) {
	// 	// 			invalidGene = true;
	// 	// 			break;
	// 	// 		}

	// 	// 		gene.push_back(table);
	// 	// 	}

	// 	// } while (invalidGene);
	// 	res.push_back(gene);
	// 	cout << "pushed back gene\n";
	// }

	// return res;
}

double calcLogTemp(int iteration, double initialTemp, double alpha = 1.0)
{
	return alpha * initialTemp / log(1 + iteration);
}

double calcGeomTemp(int iteration, double initialTemp, double alpha = 0.85)
{
	return pow(alpha, iteration) * initialTemp;
}

double calcExpTemp(int iteration, double initialTemp, double alpha = 1.0)
{
	return initialTemp * pow(alpha, iteration);
}

double TableManager::calculateTemperature(int i)
{
	switch (this->schedule)
	{
	case Logarithmic:
		return calcLogTemp(i, this->max_temp);
	case Geometric:
		return calcGeomTemp(i, this->max_temp);
	case Exponential:
		return calcExpTemp(i, this->max_temp);
	default:
		return 0;
	}
}

vector<int> TableManager::createNeighbour(vector<int> currGene)
{
	vector<int> neighbourGene = currGene;
	int nGroups = currGene.size();
	int nTables = this->tables.size();
	int randomGroup = rand() % nGroups;
	int randomTable = rand() % nTables;

	neighbourGene[randomGroup] = randomTable;
	return neighbourGene;
}

vector<int> TableManager::simulatedAnnealingAlgorithm(const vector<int> &gene)
{
	vector<int> bestGene = gene;
	vector<int> currGene = gene;
	int nTries = 0;

	for (int i = 0; i < this->max_iters && nTries < this->max_tries; i++)
	{
		nTries++;
		vector<int> neighbourGene = createNeighbour(currGene);
		double currTemp = calculateTemperature(i);

		if (currTemp <= 0)
		{
			printf("The temperature reached its limit value (zero).\n");
			break;
		}

		double neighbourCost = fitnessFunction (neighbourGene);
		double currCost = fitnessFunction (currGene);

		if (neighbourCost >= currCost)
		{
			currGene = neighbourGene;
			double bestCost = fitnessFunction (bestGene);
			if (neighbourCost >= bestCost)
			{
				bestGene = neighbourGene;
				nTries = 0;
			}
		}
		else if (probable(exp((neighbourCost - currCost) / currTemp)))
		{
			currGene = neighbourGene;
		}
	}
	return bestGene;
}
