/*
 * Group.h
 *
 *  Created on: 27/03/2018
 *      Author: Utilizador
 */

#ifndef SRC_GROUP_H_
#define SRC_GROUP_H_

enum AgeGroup {
	Child,
	Teen,
	Adult,
	Elder
};

AgeGroup getGroup(int age);


#include "Person.h"

class Group {
private:
	int id;
	vector<const Person &> members;
	double age_groups[4];//Child, Teen, Adult, Elder
	double jobs[NUMBER_JOBS];//
	double religions[NUMBER_RELIGIONS];//
	double hobbies[NUMBER_HOBBIES];

public:
	Group(int id);
	virtual ~Group();
	void calculate_attributes();
	double func_afinity(Group other) const;//afinidade entre dois grupos
	double eval_age(Group other) const;
	double eval_jobs(Group other) const;
	double eval_hobbies(Group other) const;
	double eval_religions(Group other) const;

};

#endif /* SRC_GROUP_H_ */
