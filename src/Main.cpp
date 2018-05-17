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
#include <thread>

#include "TableManager.h"
using namespace std;

void printVectorVectorInteger(const vector<vector<int>> &v);

void getOptimalGene(int threadId, TableManager tableManager, int iterationsMax, double tempMax, int triesMax, const vector<int> &gene, CoolingSchedule schedule);

vector<vector<int>> optimalGenes;

/**
 * argv[1] = people filename
 * argv[2] = tables filename
 *
 * @return 1 if arguments are wrong
 */
int main(int argc, const char *argv[])
{
	if (argc != 15)
	{
		cout << "Invalid arguments: <people_file> <tables_file>"
			 << " <p_cross> <p_mut> <n_elite> <max_stale_gens> <max_generations> <n_gene> <max_iters> <max_temp> <schedule> <max_tries> <mut_type> <valid_initial>\n\n";

		cout << "\t"
			 << "n_elite: Number of most fit individuals chosen directly to the next generation.\n";
		cout << "\t"
			 << "max_stale_gens: Maximum number of successive generations with no improvement.\n";
		cout << "\t"
			 << "max_generations: Maximum number of generations.\n";
			 cout << "\t"
			 << "n_gene: Number of genes in each generation.\n";
		cout << "\t"
			 << "max_iters: Maximum number of iterations for the Simulated Annealing Algorithm.\n";
		cout << "\t"
			 << "max_temp: Maximum temperature for the Simulated Annealing Algorithm.\n";
		cout << "\t"
			 << "schedule: Cooling schedule for the Simulated Annealing Algorithm: Logarithmic, Geometric or Exponential\n";
		cout << "\t"
			 << "max_tries: Maximum number of tries for the Simulated Annealing Algorithm.\n";
		cout << "\t"
			 << "mut_type: Mutation type.\n";
		cout << "\t"
			 << "valid_initial: 1 if generates valid initial population by backtracking or 0 if random.\n";
		return 1;
	}
	srand(time(NULL));

	double p_cross = atof(argv[3]);
	double p_mut = atof(argv[4]);
	int max_stale_gens = atoi(argv[6]);
	int max_gens = atoi(argv[7]);
	int n_gene = atoi(argv[8]);
	int n_elite = atoi(argv[5]);
	int max_iters = atoi(argv[9]);
	int max_temp = atoi(argv[10]);

	CoolingScheduleMap coolingScheduleMap;
	CoolingSchedule schedule = coolingScheduleMap[argv[11]];

	int max_tries = atoi(argv[12]);

	MutationTypeMap mutTypeMap;
	MutationType mutType = mutTypeMap[argv[13]];

	bool valid_initial = atoi(argv[14]);

	TableManager tableManager(argv[1], argv[2], p_cross, p_mut, max_stale_gens, max_gens, n_gene, n_elite, max_iters, max_temp, max_tries, schedule, mutType, valid_initial);

	cout << "Calculating initial random population.\n";
	vector<vector<int>> population = tableManager.getRandomPopulation(n_gene);
	cout << "Initial population:\n";
	printVectorVectorInteger(population);

	vector<thread> threads;
	for (int i = 0; i < n_gene; i++)
	{
		optimalGenes.resize(n_gene);
		threads.emplace_back(getOptimalGene, i, tableManager, max_iters, max_temp, max_tries, population[i], schedule);
	}

	for (thread &th : threads)
	{
		th.join();
	}
	cout << "Simulated annealing result:\n";
	printVectorVectorInteger(optimalGenes);
	cout << "\n";

	cout << "Starting Genetic Algorithm.\n";
	double score;
	vector<int> response = tableManager.geneticAlgorithm(population, score);
	cout << "Fitness: " << score << "\n";
	for (unsigned int i = 0; i < response.size(); i++)
	{
		printf("The group %d is at table %d.\n", i, response.at(i));
	}
	cerr << "FIM\n";
	return 0;
}

void printVectorVectorInteger(const vector<vector<int>> &v)
{
	for (unsigned int i = 0; i < v.size(); i++)
	{
		for (unsigned int j = 0; j < v[i].size(); j++)
		{
			cout << v[i][j] << " | ";
		}
		cout << "\n";
	}
}

void getOptimalGene(int threadId, TableManager tableManager, int iterationsMax, double tempMax, int triesMax, const vector<int> &gene, CoolingSchedule schedule)
{
	printf("No corpo da thread\n");
	vector<int> optimalGene = tableManager.simulatedAnnealingAlgorithm(gene);
	optimalGenes[threadId] = optimalGene;
}
