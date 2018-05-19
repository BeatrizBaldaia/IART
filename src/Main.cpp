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
#include <chrono>

#include "TableManager.h"
using namespace std;

void printVectorVectorInteger(const vector<vector<int>> &v);

void getOptimalGene(int threadId, TableManager tableManager, int iterationsMax, double tempMax, int triesMax, const vector<int> &gene, CoolingSchedule schedule);

vector<vector<int>> optimalGenes;
ofstream myfile;
/**
 * argv[1] = people filename
 * argv[2] = tables filename
 *
 * @return 1 if arguments are wrong
 */
int main(int argc, const char *argv[])
{
	if (argc != 14)
	{
		cout << "Invalid arguments: <people_file> <tables_file>\n"
				<< " <p_cross> <p_mut> <n_elite> <max_stale_gens> <max_generations>\n"
				<< " <n_gene> <max_iters> <max_temp> <schedule> <max_tries> \n"
				<< " <mut_type>\n\n";

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

		return 1;
	}
	srand(time(NULL));

	  myfile.open ("example.txt",ofstream::app);
	  if(!myfile.is_open()) return 9;

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

	TableManager tableManager(argv[1], argv[2], p_cross, p_mut, max_stale_gens, max_gens, n_gene, n_elite, max_iters, max_temp, max_tries, schedule, mutType);

	cout << "Calculating initial random population.\n";
	auto start = chrono::high_resolution_clock::now();
	vector<vector<int>> population = tableManager.getRandomPopulation(n_gene);
	auto finish = chrono::high_resolution_clock::now();
	myfile << "Time spent for Random Population = " << chrono::duration_cast<chrono::nanoseconds>(finish-start).count() << "ns\n";
	cout << "Time spent for Random Population = " << chrono::duration_cast<chrono::nanoseconds>(finish-start).count() << "ns\n";

	cout << "Initial population:\n";
	printVectorVectorInteger(population);

	vector<thread> threads;
	start = chrono::high_resolution_clock::now();
	for (unsigned int i = 0; i < population.size(); i++) {
		optimalGenes.resize(population.size());
		threads.emplace_back(getOptimalGene, i, tableManager, max_iters, max_temp, max_tries, population[i], schedule);
	}
	for (thread &th : threads)
	{
		th.join();
	}
	finish = chrono::high_resolution_clock::now();
	myfile << "Time spent for simmulated anealing algorithm = " << chrono::duration_cast<chrono::nanoseconds>(finish-start).count() << "ns\n";
	cout << "Time spent for simmulated anealing algorithm = " << chrono::duration_cast<chrono::nanoseconds>(finish-start).count() << "ns\n";

	cout << "Simulated annealing result:\n";
	printVectorVectorInteger(optimalGenes);
	cout << "\n";

	cout << "Starting Genetic Algorithm.\n";
	double score = -DBL_MAX;
	start = chrono::high_resolution_clock::now();
	vector<int> response = tableManager.geneticAlgorithm(population, score);
	finish = chrono::high_resolution_clock::now();
	myfile << "Time spent for genetic algoritm = " << chrono::duration_cast<chrono::nanoseconds>(finish-start).count() << "ns\n";
	cout << "Time spent for genetic algoritm = " << chrono::duration_cast<chrono::nanoseconds>(finish-start).count() << "ns\n";

	myfile << "Fitness: " << score << "\n";
	for (unsigned int i = 0; i < response.size(); i++)
	{
		printf("The group %d is at table %d.\n", i, response.at(i));
	}
	myfile.close();
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
	auto start = chrono::high_resolution_clock::now();
	vector<int> optimalGene = tableManager.simulatedAnnealingAlgorithm(gene);
	auto finish = chrono::high_resolution_clock::now();
	double fitness = tableManager.fitnessFunction(optimalGene);
	myfile << "Time for Simulated Annealing = " << chrono::duration_cast<chrono::nanoseconds>(finish-start).count() << "ns\n"
			<< "Fitness: " << fitness << "\n";

	optimalGenes[threadId] = optimalGene;
}
