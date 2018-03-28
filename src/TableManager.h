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
	//vector<int> solution;//cada indice equivale a um grupo e o valor do mesmo equivale a mesa onde este se vai ficar

public:
	TableManager();
	int aval_fuct(vector<int> solution);
};




#endif /* SRC_TABLEMANAGER_H_ */
