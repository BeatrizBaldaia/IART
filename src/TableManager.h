/*
 * TableManager.h
 *
 *  Created on: 27/03/2018
 *      Author: Utilizador
 */

#ifndef SRC_TABLEMANAGER_H_
#define SRC_TABLEMANAGER_H_

#include <vector>
#include "Group.h"
#include "Table.h"

class TableManager {
private:
	vector<Person> people;
	vector<Group> groups;
	vector<Table> tables;
	vector<vector<double> > groupsAffinity;//(groups.size(), vector<double>(groups.size()));//matriz de afinidades
	void getPeopleFromFile(const char* filename);
	void getTablesFromFile(const char* filename);
	void createGroups();

	vector<int> elitismSelection(vector<double> aval, int n_elite = -1) const;
	vector<int> selectParents(const vector<vector<int> > &population, vector<double> aval, int nSelection) const;
	vector<vector<int> > crossParents(const vector<vector<int> > &population, const vector<int> &parentIndexes, double p_cross) const;
	void mutateChildren(vector<vector<int> > &children, double p_mut) const;
	void selectNextGen(vector<vector<int> > &population, const vector<int> &elitedParentsIndexes, const vector<vector<int> > &children) const;
public:
	TableManager(const char * peopleFile, const char * tablesFile);
	void calcGroupsAffinity();
	vector<int> fillTables(const vector<int> &gene) const;
	bool invalidGene(const vector<int> &population) const;
	vector<int> createNeighbour(vector<int> currGene);
	vector<vector<int> > getRandomPopulation(unsigned int popSize);

	/**
	 * Elitism turned off by default.
	 */
	vector<int> geneticAlgorithm(vector<vector<int> > &population, double p_cross, double p_mut, int max_stale_gens, int max_gens, int n_gene, int n_elite = 0) const;

	double fitnessFunction (const vector<int> &solution) const;
	Group * getGroup(int id);
	vector<vector<unsigned int> > vizinho_func(vector<unsigned int> &solution);
	vector<double> evaluatePopulation(vector<vector<int> > &pop) const;

	/**
	 * calcula o valor de uma distribuicao de grupos por mesas,
	 * tendo em conta a afinidade entre os grupos que partilham uma mesa
	 * e penalizando (eliminar) violacoes as restricoes
	 */
	double calculateStateValue(vector<int>);

	/**
	 * retorna um novo estado (melhor ou pior que o atual).
	 * para a escolha de um estado pior, calcular a probabilidade de este ser escolhido e gerar um numero aleatorio;
	 * se esse numero for menor que a probabilidade calculada, ficar nesse novo estado.
	 */
	vector<int> getNeighbor(vector<int> currState);

	vector<int> simulatedAnnealingAlgorithm(int iterationsMax, double tempMax, int triesMax, const vector<int> &gene, CoolingSchedule schedule);
};




#endif /* SRC_TABLEMANAGER_H_ */
