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
	void creatGroups();
public:
	TableManager(const char * peopleFile, const char * tablesFile);
	void calcGroupsAffinity();
	vector<int> getRandomSolution();
	vector<int> geneticAlgorithm(vector<vector<int> > population);

	double aval_fuct(const vector<int> &solution);
	Group * getGroup(int id);
};




#endif /* SRC_TABLEMANAGER_H_ */
