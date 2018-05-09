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
		cout << "Group: " << this->groups[i].getId() << "\n";
		cout << "	p: " << this->groups[i].getMembers()[0]->getName() << "\n";

		this->groups[i].calculate_attributes();
	}
	printf("Calling: calcGroupsAffinity\n");
	for (unsigned int i = 0; i < this->groups.size(); i++)
	{
		this->groupsAffinity.push_back(vector<double>(this->groups.size()));
	}
	this->calcGroupsAffinity();
	printf("I created a Table Manager!\n");
}

/**
 *
 */
vector<int> TableManager::geneticAlgorithm(vector<vector<int>> &population, double p_cross, double p_mut, int max_stale_gens, int max_gens, int n_elite) const
{
	printf("I am the genetic algorithm\n");
	int currentGen = 0;
	int numStaleGens = 0;
	double max_aval = -DBL_MAX;
	vector<int> res;
	while (currentGen < max_gens && numStaleGens < max_stale_gens)
	{

		vector<double> aval = this->evaluatePopulation(population);
		printf("%d Cycle\n", currentGen);
		vector<int> elitedParentsIndexes = elitismSelection(population, aval, n_elite);
		vector<int> parentIndexes = selectParents(population, aval, n_elite);
		vector<vector<int>> children = crossParents(population, parentIndexes, p_cross);
		mutateChildren(children, p_mut);
		selectNextGen(population, elitedParentsIndexes, children);
		if (max_aval >= *max_element(aval.begin(), aval.end()))
		{
			numStaleGens++;
		}
		else
		{
			max_aval = *max_element(aval.begin(), aval.end());
			numStaleGens = 0;

			res = population.at(distance(aval.begin(), max_element(aval.begin(), aval.end())));
		}
		printf("\t%f\n", max_aval);
		currentGen++;
	}
	return res;
}
vector<double> TableManager::evaluatePopulation(vector<vector<int>> &pop) const
{
	vector<double> res;
	for (unsigned int i = 0; i < pop.size(); i++)
	{
		res.push_back(this->aval_funct(pop[i]));
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
		printf("ROW\n");
		for (unsigned int col = row + 1; col < groups.size(); col++)
		{
			printf("\tCOL\n");
			cout << "Affinity between groups " << groups.at(row).getId() << " and " << groups.at(col).getId() << endl;
			groupsAffinity[row][col] = groups.at(row).func_afinity(groups.at(col));
			cout << "A afinidade entre este dois e " << groupsAffinity[row][col] << endl;
		}
	}
}

/**
 * Calculates the fitness value of one possible solution
 * Sum of affinities of group pairs sharing the same table minus penalty
 * TODO: Missing maximization of empty tables? verificar se uma mesa tem pessoas a mais
 */
double TableManager::aval_funct(const vector<int> &solution) const
{
	double res = 0, penalty = -DBL_MAX;
	vector<int> tables = fillTables(solution);
	if (invalidGene(tables))
	{
		return -1;
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
	cout << "getPeopleFromFile\n";
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
			cout << "Name: " << p.getName() << '\n';
			getline(person, value, ';');
			p.setAge(atoi(value.c_str()));
			cout << "Age: " << p.getAge() << '\n';
			getline(person, value, ';');
			p.setGroup(atoi(value.c_str()));
			cout << "Group: " << p.getGroup() << '\n';
			getline(person, value, ';');
			p.setJob(JobAreaMap[value]);
			cout << "Job: " << toString(p.getJob()) << '\n';
			getline(person, value, ';');
			p.setReligion(ReligionMap[value]);
			cout << "Religion: " << toString(p.getReligion()) << '\n';
			vector<Hobby> hobbies;
			cout << "Hobbies:\n";
			while (getline(person, value, ','))
			{
				hobbies.push_back(HobbyMap[value]);
				cout << "	" << toString(hobbies.back()) << '\n';
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
	cout << "getTablesFromFile\n";
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
	for (unsigned int i = 0; i < this->tables.size(); i++)
	{
		cout << "Table " << i << ": " << this->tables[i].getNumberOfSeats() << "\n";
	}
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
	cout << "Creating group!\n";
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
vector<int> TableManager::elitismSelection(const vector<vector<int>> &population, vector<double> aval, int n_elite /*=-1*/) const
{
	vector<int> res;
	if (n_elite > 0)
	{
		cerr << "n_elite > 0, ainda não foi implementado!!!\n";
	}
	return res;
}
vector<int> TableManager::selectParents(const vector<vector<int>> &population, vector<double> aval, int n_elite) const
{
	vector<int> res;
	double scale_F = 0;
	//TODO: incorporate n_elite
	vector<double> randomNumber;
	for (unsigned int i = 0; i < aval.size(); i++)
	{
		scale_F += aval.at(i);
		double random = (double)rand() / RAND_MAX; //double between 0 and 1
		randomNumber.push_back(random);
	}
	aval.at(0) = aval.at(0) / scale_F;
	for (unsigned int i = 1; i < randomNumber.size(); i++)
	{
		aval.at(i) = aval.at(i) / scale_F;
		aval.at(i) = aval.at(i) + aval.at(i - 1);
	}
	for (unsigned int i = 0; i < randomNumber.size(); i++)
	{
		unsigned int j = 0;
		while (j < (aval.size() - 1) && randomNumber.at(i) > aval.at(j))
		{
			j++;
		}
		res.push_back(j);
	}
	//	for(unsigned int i = 0; i < randomNumber.size(); i++) {
	//		cout << i << " - "<<res.at(i)<<"- "<<randomNumber.at(i)<<"- "<<aval.at(i)/scale_F<<"\n";
	//	}
	return res;
}
vector<vector<int>> TableManager::crossParents(const vector<vector<int>> &population, const vector<int> &parentIndexes, double p_cross) const
{
	vector<vector<int>> res;
	vector<int> isCrossing;
	for (unsigned int i = 0; i < parentIndexes.size(); i++)
	{
		res.push_back(population.at(parentIndexes.at(i)));
		if (((double)rand() / RAND_MAX) < p_cross)
		{
			isCrossing.push_back(i);
		}
	}
	for (unsigned int i = 0; i < isCrossing.size() - 1; i++)
	{
		int j = i;
		i++;

		unsigned int cross_point = rand() % population[0].size();

		vector<int> aux_i;
		vector<int> aux_j;

		unsigned int k = 0;
		while (k < cross_point)
		{
			aux_i.push_back(res[i][k]);
			aux_j.push_back(res[j][k]);
			k++;
		}
		while (k < res[0].size())
		{
			aux_i.push_back(res[j][k]);
			aux_j.push_back(res[i][k]);
			k++;
		}

		res.at(i) = aux_i;
		res.at(j) = aux_j;
	}
	return res;
}
void TableManager::mutateChildren(vector<vector<int>> &children, double p_mut) const
{
	for (unsigned int i = 0; i < children.size(); i++)
	{
		for (unsigned int j = 0; j < children[i].size(); j++)
		{
			if (((double)rand() / RAND_MAX) < p_mut)
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
			return true;
		}
	}
	return false;
}

void printRow(const vector<int> &v) {
	for(int i = 0; i < v.size(); i++) {
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
		printf("Vai calcular vizinho\n");
		vector<int> neighbourGene = createNeighbour(currGene);
		printf("Criou vizinho\n");
		double currTemp = calculateTemperature(i, tempMax, schedule);
		printf("Calculou temperatura\n");

		if (currTemp <= 0)
		{
			printf("temperatura a zero. parar!\n");
			break;
		}

		double neighbourCost = aval_funct(neighbourGene);
		printf("Avaliou o vizinho: %f\n", neighbourCost);
		double currCost = aval_funct(currGene);
		printf("Avaliou o gene atual: %f\n", currCost);

		if (neighbourCost <= currCost)
		{
			currGene = neighbourGene;
			if (neighbourCost <= aval_funct(bestGene))
			{
				printf("Atualizar o melhor gene\n");
				bestGene = neighbourGene;
				nTries = 0;
			}
		}
		else if (exp((currCost - neighbourCost) / currTemp) > ((double)rand() / RAND_MAX))
		{
			printf("Atualizar o gene atual\n");
			currGene = neighbourGene;
		}
	}
	return bestGene;
}
