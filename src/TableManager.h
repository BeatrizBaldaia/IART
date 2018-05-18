/*
 * TableManager.h
 *
 *  Created on: 27/03/2018
 *      Author: Utilizador
 */

#ifndef SRC_TABLEMANAGER_H_
#define SRC_TABLEMANAGER_H_

#include <vector>
#include <numeric>      // std::accumulate
#include "Group.h"
#include "Table.h"

class TableManager {
private:
	const double FITNESS_COMPENSATION = 0.1;
	vector<Person> people;
	vector<Group> groups;
	vector<Table> tables;
	vector<vector<double> > groupsAffinity;//(groups.size(), vector<double>(groups.size()));//matriz de afinidades

	double p_cross;
	double p_mut;
	int max_stale_gens;
	int max_gens;
	int n_gene;
	int n_elite;
	int max_iters;
	int max_temp;
	int max_tries;

	bool backtrackingInitialGeneration;

	CoolingSchedule schedule;
	MutationType mutType;

public:
	TableManager(const char * peopleFile, const char * tablesFile, double p_cross, double p_mut, int max_stale_gens, int max_gens, int n_gene, int n_elite, int max_iters, int max_temp, int max_tries, CoolingSchedule schedule, MutationType mutType, bool backtrackingInitialGeneration);	
	void getPeopleFromFile(const char* filename);
	void getTablesFromFile(const char* filename);
	void createGroups();

	/**
	 * Calculates the fitness value of one possible solution
	 * Sum of affinities of group pairs sharing the same table minus penalty
	 * Incomplete result for genetic algorithm: Can be negative, must be summed afterwards
	 * TODO: Missing maximization of empty tables? verificar se uma mesa tem pessoas a mais
	 */
	double fitnessFunction (const vector<int> &solution) const;
	vector<int> elitismSelection(const vector<double> &eval) const;
	vector<int> selectParents(const vector<vector<int> > &population, vector<double> eval, int nSelection) const;
	vector<int> getGenesForCrossover(vector<int> &uncrossedParentInds, int nGenes) const;
	vector<vector<int> > crossParents(const vector<vector<int> > &population, const vector<int> &parentIndexes) const;
	void mutateChildren(vector<vector<int> > &children) const;
	void selectNextGen(vector<vector<int> > &population, const vector<vector<int>> &elitedParents, const vector<vector<int> > &children) const;
	void singleMutation(vector<vector<int>> &children) const;
	void swapMutGene(vector<int> &gene) const;
	void swapMutation(vector<vector<int>> &children) const;
	double calculateTemperature(int i);

	/**
	 * Elitism turned off by default.
	 */
	vector<int> geneticAlgorithm(vector<vector<int> > &population, double &maxScore) const;

	/**
	 * retorna um novo estado (melhor ou pior que o atual).
	 * para a escolha de um estado pior, calcular a probabilidade de este ser escolhido e gerar um numero aleatorio;
	 * se esse numero for menor que a probabilidade calculada, ficar nesse novo estado.
	 */
	vector<int> getNeighbor(vector<int> currState);
	vector<int> simulatedAnnealingAlgorithm(const vector<int> &gene);

	void calcGroupsAffinity();
	vector<double> evaluatePopulation(const vector<vector<int> > &pop) const;
	/**
	 * calcula o valor de uma distribuicao de grupos por mesas,
	 * tendo em conta a afinidade entre os grupos que partilham uma mesa
	 * e penalizando (eliminar) violacoes as restricoes
	 */
	double calculateStateValue(vector<int>);
	vector<vector<int> > getRandomPopulation(unsigned int popSize);
	vector<int> createNeighbour(vector<int> currGene);
	bool invalidGene(const vector<int> &population) const;
	vector<int> fillTables(const vector<int> &gene) const;
	Group * getGroup(int id);

	bool invalidTable(int seatsAtTable, int table) const;

	void getGeneBacktracking(int table, vector<int> gene, vector<int> usedTables, vector<vector<int> > &solutions) const;
};




#endif /* SRC_TABLEMANAGER_H_ */
