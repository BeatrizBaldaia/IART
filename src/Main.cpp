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
vector<int> getSimAnnealResponse(const TableManager &tm, const vector<vector<int> > &optimalGenes, double &score);

vector<vector<int>> optimalGenes;
ofstream myfile;

void printUsage() {
		cout << "Invalid arguments: <people_file> <tables_file>\n"
				<< " <p_cross> <p_mut> <n_elite> <max_stale_gens> <max_generations>\n"
				<< " <n_gene> <max_iters> <max_temp> <schedule> <max_tries> \n"
				<< " <mut_type> <prog_config>\n\n";

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
				<< "prog_config: Algorithms to run. 'SimAnneal' for only simulated annealing,"
				<< "'Genetic' for only genetic, 'All' for simulated annealing followed by genetic.\n";
}

bool validInputs(string coolSchedule, string mutType, string progConfig) {
	return (coolSchedule == "Logarithmic" || coolSchedule == "Geometric" || coolSchedule == "Exponential")
			&& (mutType == "Single" || mutType == "Swap")
			&& (progConfig == "SimAnneal" || progConfig == "Genetic" || progConfig == "All");
}

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
		printUsage();
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

	ProgramConfigMap progConfigMap;
	ProgramConfig progConfig = progConfigMap[argv[14]];

	if (!validInputs(argv[11], argv[13], argv[14])) {
		printUsage();
		exit(1);
	}

	TableManager tableManager(argv[1], argv[2], p_cross, p_mut, max_stale_gens, max_gens, n_gene, n_elite, max_iters, max_temp, max_tries, schedule, mutType);

	cout << "Calculating initial random population.\n";
	auto initialStart = chrono::high_resolution_clock::now();
	vector<vector<int>> population = tableManager.getRandomPopulation(n_gene);
	auto initialFinish = chrono::high_resolution_clock::now();

	cout << "Initial population:\n";
	printVectorVectorInteger(population);

	vector<int> response;
	double score;

	// declared outside if block so they can be used in the end results
	auto simAnnealStart = chrono::high_resolution_clock::now();
	auto simAnnealFinish = chrono::high_resolution_clock::now();
	if (progConfig == SimAnneal || progConfig == All) {
		vector<thread> threads;
		simAnnealStart = chrono::high_resolution_clock::now();
		for (unsigned int i = 0; i < population.size(); i++) {
			optimalGenes.resize(population.size());
			threads.emplace_back(getOptimalGene, i, tableManager, max_iters, max_temp, max_tries, population[i], schedule);
		}
		for (thread &th : threads)
		{
			th.join();
		}
		simAnnealFinish = chrono::high_resolution_clock::now();

		cout << "Simulated annealing result:\n";
		printVectorVectorInteger(optimalGenes);
		cout << "\n";

		response = getSimAnnealResponse(tableManager, optimalGenes, score);
		population = optimalGenes;
	}
	

	// declared outside if block so they can be used in the end results
	auto geneticStart = chrono::high_resolution_clock::now();
	auto geneticFinish = chrono::high_resolution_clock::now();
	if (progConfig == Genetic || progConfig == All) {
		cout << "Starting Genetic Algorithm.\n";
		score = -DBL_MAX;
		geneticStart = chrono::high_resolution_clock::now();
		response = tableManager.geneticAlgorithm(population, score);
		geneticFinish = chrono::high_resolution_clock::now();
		cout << "\n";
	}
	
	cout << "Results\n";
	cout << "Fitness: " << score << "\n";
	myfile << "Fitness: " << score << "\n";
	for (unsigned int i = 0; i < response.size(); i++)
	{
		printf("The group %d is at table %d.\n", i, response.at(i));
	}
	cout << "\n";
	cout << "Fitness: " << score << "\n";
	myfile << "Fitness: " << score << "\n";
	
	cout << "Time Statistics\n";
	myfile << "Time spent for Random Population = " << chrono::duration_cast<chrono::nanoseconds>(initialFinish-initialStart).count() << "ns\n";
	cout << "Time spent for Random Population = " << chrono::duration_cast<chrono::nanoseconds>(initialFinish-initialStart).count() << "ns\n";
	if (progConfig == SimAnneal || progConfig == All) {
		myfile << "Time spent for simmulated anealing algorithm = " << chrono::duration_cast<chrono::nanoseconds>(simAnnealFinish-simAnnealStart).count() << "ns\n";
		cout << "Time spent for simmulated anealing algorithm = " << chrono::duration_cast<chrono::nanoseconds>(simAnnealFinish-simAnnealStart).count() << "ns\n";
	}
	if (progConfig == Genetic || progConfig == All) {
		myfile << "Time spent for genetic algoritm = " << chrono::duration_cast<chrono::nanoseconds>(geneticFinish-geneticStart).count() << "ns\n";
		cout << "Time spent for genetic algoritm = " << chrono::duration_cast<chrono::nanoseconds>(geneticFinish-geneticStart).count() << "ns\n";
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

vector<int> getSimAnnealResponse(const TableManager &tm, const vector<vector<int> > &optimalGenes, double &score) {
	double shift;
	vector<double> eval = tm.evaluatePopulation(optimalGenes, shift);
	vector<double>::iterator largest_eval_it = max_element(eval.begin(), eval.end());
	score = *largest_eval_it - shift;
	return optimalGenes.at(distance(eval.begin(), largest_eval_it));
}