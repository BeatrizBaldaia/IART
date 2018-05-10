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

void printVectorVectorInteger(const vector<vector<int> > &v);

void getOptimalGene(TableManager tableManager, int iterationsMax, double tempMax, int triesMax, const vector<int> & gene, CoolingSchedule schedule);

vector<vector<int> >optimalGenes;

/**
 * argv[1] = nome do ficheiro de pessoas
 * argv[2] = nome do ficheiro de de mesas
 *
 * @return 1 if arguments are wrong
 */
int main(int argc, const char *argv[])
{
	if (argc != 12)
	{
		cout << "Invalid arguments: <people_file> <tables_file>"
			 << " <p_cross> <p_mut> <n_elite> <max_stale_gens> <max_gens> <max_iters> <max_temp> <schedule> <max_tries>\n\n";

		cout << "\t"
			 << "n_elite: Number of most fit individuals chosen directly to the next generation.\n";
		cout << "\t"
			 << "max_stale_gens: Maximum number of successive generations with no improvement.\n";
		cout << "\t"
			 << "max_gens: Maximum number of generations.\n";
		cout << "\t"
			 << "max_iters: Maximum number of iterations for the Simulated Annealing Algorithm.\n";
		cout << "\t"
			 << "max_temp: Maximum temperature for the Simulated Annealing Algorithm.\n";
		cout << "\t"
			 << "schedule: Cooling schedule for the Simulated Annealing Algorithm: Logarithmic, Geometric or Exponential\n";
		cout << "\t"
			 << "max_tries: Maximum number of tries for the Simulated Annealing Algorithm.\n";
		return 1;
	}
	srand(time(NULL));

	TableManager tableManager(argv[1], argv[2]);
	
	double p_cross = atof(argv[3]);
	double p_mut = atof(argv[4]);
	int max_stale_gens = atoi(argv[6]);
	int max_gens = atoi(argv[7]);
	int n_elite = atoi(argv[5]);
	int max_iters = atoi(argv[8]);
	int max_temp = atoi(argv[9]);

	CoolingScheduleMap CoolingScheduleMap;
	CoolingSchedule schedule = CoolingScheduleMap[argv[10]];

	int max_tries = atoi(argv[11]);
	
	vector<vector<int>> population = tableManager.getRandomPopulation(20); //TODO: popSize
	printVectorVectorInteger(population);

	vector<thread> threads;
	for (int i = 0; i < 20; i++)
	{
		printf("criou thread %d!\n", i);
		thread th (getOptimalGene, tableManager, max_iters, max_temp, max_tries, population[i], schedule);
		printf("thread\n");
		
//		threads.push_back(move(th));
		threads.push_back(&th);
	}

//	for(thread& th: threads) {
//		th.join();
//	}
	for (int i = 0; i < 20; i++) {
		threads[i]->join();
	}

	printVectorVectorInteger(optimalGenes);

	vector<int> response = tableManager.geneticAlgorithm(population, p_cross, p_mut, max_stale_gens, max_gens, n_elite);
	for (unsigned int i = 0; i < response.size(); i++)
	{
		printf("O grupo %d está na mesa %d.\n", i, response.at(i));
	}
	cerr << "FIM\n";
	return 0;
}

void printVectorVectorInteger(const vector<vector<int> > &v)
{
	printf("Mostrar Matriz:\n\n");
	for (unsigned int i = 0; i < v.size(); i++)
	{
		for (unsigned int j = 0; j < v[i].size(); j++)
		{
			cout << v[i][j] << " | ";
		}
		cout << "\n";
	}
}

void getOptimalGene(TableManager tableManager, int iterationsMax, double tempMax, int triesMax, const vector<int> & gene, CoolingSchedule schedule)
{
	printf("No corpo da thread\n");
	vector<int> optimalGene = tableManager.simulatedAnnealingAlgorithm(iterationsMax, tempMax, triesMax, gene, schedule);
	
	optimalGenes.push_back(optimalGene);
}
