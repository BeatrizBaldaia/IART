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

TableManager::TableManager(const char *peopleFile, const char *tablesFile)
{
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
	printf("Created a Table Manager\n");
}


vector<double> TableManager::evaluatePopulation(const vector<vector<int>> &pop) const
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
	}
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
		// printf("ROW\n");
		for (unsigned int col = row + 1; col < groups.size(); col++)
		{
			// printf("\tCOL\n");
			// cout << "Affinity between groups " << groups.at(row).getId() << " and " << groups.at(col).getId() << endl;
			groupsAffinity[row][col] = groups.at(row).func_afinity(groups.at(col));
			// cout << "A afinidade entre este dois e " << groupsAffinity[row][col] << endl;
		}
	}
}

/**
 * Calculates the fitness value of one possible solution
 * Sum of affinities of group pairs sharing the same table minus penalty
 * Incomplete result for genetic algorithm: Can be negative, must be summed afterwards
 * TODO: Missing maximization of empty tables? verificar se uma mesa tem pessoas a mais
 */
double TableManager::fitnessFunction (const vector<int> &solution) const
{
	double res = 0, penalty = -DBL_MAX;
	vector<int> tables = fillTables(solution);
	if (invalidGene(tables))
	{
		return penalty;
	}
	for (unsigned int i = 0; i < solution.size(); i++)
	{
		for (unsigned int j = i + 1; j < solution.size(); j++)
		{
			//			printf("Solution.size() = %d; i = %d; j = %d\n",solution.size(),i,j);
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
	// cout << "getPeopleFromFile\n";
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
			// cout << "Name: " << p.getName() << '\n';
			getline(person, value, ';');
			p.setAge(atoi(value.c_str()));
			// cout << "Age: " << p.getAge() << '\n';
			getline(person, value, ';');
			p.setGroup(atoi(value.c_str()));
			// cout << "Group: " << p.getGroup() << '\n';
			getline(person, value, ';');
			p.setJob(JobAreaMap[value]);
			// cout << "Job: " << toString(p.getJob()) << '\n';
			getline(person, value, ';');
			p.setReligion(ReligionMap[value]);
			// cout << "Religion: " << toString(p.getReligion()) << '\n';
			vector<Hobby> hobbies;
			// cout << "Hobbies:\n";
			while (getline(person, value, ','))
			{
				hobbies.push_back(HobbyMap[value]);
				// cout << "	" << toString(hobbies.back()) << '\n';
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
	// cout << "getTablesFromFile\n";
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
	// for (unsigned int i = 0; i < this->tables.size(); i++)
	// {
	// 	cout << "Table " << i << ": " << this->tables[i].getNumberOfSeats() << "\n";
	// }
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
	// cout << "Creating group!\n";
	this->groups.push_back(g);
	return &(this->groups.back());
}

vector<vector<unsigned int>> TableManager::vizinho_func(vector<unsigned int> &solution)
{ //indice group value table number
	vector<vector<unsigned int>> res;
	for (unsigned int i = 0; i < solution.size(); i++)
	{
		for (unsigned int j = 0; j < this->tables.size(); j++)
		{
			vector<unsigned int> neighbor = solution;
			if (solution[i] != j)
			{
				neighbor[i] = j;
				res.push_back(neighbor);
			}
		}
	}
	return res;
}
vector<int> TableManager::elitismSelection(vector<double> eval, int n_elite) const
{
	vector<int> res;
	vector<double> aux = eval;

	while (n_elite > 0)
	{
		vector<double>::iterator max_iter = max_element(aux.begin(), aux.end());
		int index = distance(aux.begin(), max_iter);
		res.push_back(index);
		*max_iter = -DBL_MAX;
		n_elite--;
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
	double scale_F = accumulate(eval.begin(), eval.end(), 0);
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
		double random = (double)(rand() % 2); //double between 0 and 1
		int index = getGeneFromRange(scale, random);
		selectedGenes.push_back(index);
	}
	
	return selectedGenes;
}

vector<int> getGenesForCrossover(int nGenes, double p_cross) {
	vector<int> result;
	int index = 0;
	while(nGenes > 0) {
		//double random = (double)(rand() % 2);
		if(probable(p_cross)) {
			result.push_back(index);
		}
		index++;
		nGenes--;
	}
	return result;
}

void crossGenes(vector<int> &gene1, vector<int> &gene2) {
	int max = gene1.size() - 2, min = 1;
	int randNum = getRandomBetween(min, max);

	for(int i = randNum; i < gene1.size(); i++) {
		int seatA = gene1[i], seatB = gene2[i];
		gene1[i] = seatB;
		gene2[i] = seatA;
	}
}

vector<vector<int>> TableManager::crossParents(const vector<vector<int>> &population, const vector<int> &parentIndexes, double p_cross) const
{
	vector<vector<int>> result = population;
	vector<int> crossCandidates = getGenesForCrossover(population.size(), p_cross);
	int nCross = (crossCandidates.size() % 2 == 0)? crossCandidates.size() : crossCandidates.size() - 1;
	
	for(int i = 0; i < nCross; i+=2) {
		int parent1 = parentIndexes[i], parent2 = parentIndexes[i + 1];
		crossGenes(result[parent1], result[parent2]);
	}
	
	return result;
}
void TableManager::mutateChildren(vector<vector<int>> &children, double p_mut) const
{
	for (unsigned int i = 0; i < children.size(); i++)
	{
		for (unsigned int j = 0; j < children[i].size(); j++)
		{
			if (probable(p_mut))
			{
				cerr << "Devia ter mutado!! HOW??\n"; //TODO: mutação
													  //MESA ALEATORIA
			}
		}
	}
	return;
}
void TableManager::selectNextGen(vector<vector<int>> &population, const vector<int> &elitedParentsIndexes, const vector<vector<int>> &children) const
{
	vector<vector<int>> elited;
	for (unsigned int i = 0; i < elitedParentsIndexes.size(); i++)
	{
		elited.push_back(population[elitedParentsIndexes[i]]);
	}
	population = children;
	for (unsigned int i = 0; i < elited.size(); i++)
	{
		population.push_back(elited[i]);
	}
	return;
}

/**
 *
 */
vector<int> TableManager::geneticAlgorithm(vector<vector<int>> &population, double p_cross, double p_mut, int max_stale_gens, int max_gens, int n_gene, int n_elite) const
{
	int currentGen = 0;
	int numStaleGens = 0;
	double max_eval = -DBL_MAX;
	vector<int> res;
	while (currentGen < max_gens && numStaleGens < max_stale_gens)
	{

		vector<double> eval = evaluatePopulation(population);
		printf("%d Cycle\n", currentGen);
		vector<int> elitedParentsIndexes = elitismSelection(eval, n_elite);
		vector<vector<int>> elitedParents = getElitedParents(population, elitedParentsIndexes);
		int nRandomSelection = n_gene - n_elite;
		vector<int> parentIndexes = selectParents(population, eval, nRandomSelection);
		vector<vector<int>> children = crossParents(population, parentIndexes, p_cross);
		mutateChildren(children, p_mut);
		//children.insert( children.end(), elitedParents.begin(), elitedParents.end() ); //next generation
		selectNextGen(population, elitedParentsIndexes, children);
		if (max_eval >= *max_element(eval.begin(), eval.end()))
		{
			numStaleGens++;
		}
		else
		{
			max_eval = *max_element(eval.begin(), eval.end());
			numStaleGens = 0;

			res = population.at(distance(eval.begin(), max_element(eval.begin(), eval.end())));
		}
		printf("\t%f\n", max_eval);
		currentGen++;
	}
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
			//printf(">>pessoas na mesa %d: %d\n", i, tables[i]);
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

vector<vector<int>> TableManager::getRandomPopulation(unsigned int popSize)
{
	vector<vector<int>> res;

	for (unsigned int i = 0; i < popSize; i++)
	{
		vector<int> gene;
		vector<int> auxTables(this->tables.size());
		do
		{
			gene.clear();
			fill(auxTables.begin(), auxTables.end(), 0);
			for (unsigned int j = 0; j < this->groups.size(); j++)
			{
				int nMembers = (this->groups[j].getMembers()).size();
				int table = rand() % this->tables.size();

				auxTables[table] += nMembers;
				gene.push_back(table);
			}

		} while (invalidGene(auxTables));
		res.push_back(gene);
	}

	return res;
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

double calculateTemperature(int i, double initialTemp, CoolingSchedule schedule)
{
	switch (schedule)
	{
	case Logarithmic:
		return calcLogTemp(i, initialTemp);
	case Geometric:
		return calcGeomTemp(i, initialTemp);
	case Exponential:
		return calcExpTemp(i, initialTemp);
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

vector<int> TableManager::simulatedAnnealingAlgorithm(int iterationsMax, double tempMax, int triesMax, const vector<int> &gene, CoolingSchedule schedule)
{
	vector<int> bestGene = gene;
	vector<int> currGene = gene;
	int nTries = 0;

	for (int i = 0; i < iterationsMax && nTries < triesMax; i++)
	{
		nTries++;
		//printf("Vai calcular vizinho\n");
		vector<int> neighbourGene = createNeighbour(currGene);
		//printf("Criou vizinho\n");
		double currTemp = calculateTemperature(i, tempMax, schedule);
		//printf("Calculou temperatura\n");

		if (currTemp <= 0)
		{
			printf("temperatura a zero. parar!\n");
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
				//printf("Atualizar o melhor gene\n");
				bestGene = neighbourGene;
				nTries = 0;
			}
		}
		else if (probable(exp((neighbourCost - currCost) / currTemp)))
		{
			//printf("Atualizar o gene atual\n");
			currGene = neighbourGene;
		}
	}
	return bestGene;
}
