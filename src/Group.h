/*
 * Group.h
 *
 *  Created on: 27/03/2018
 *      Author: Utilizador
 */

#ifndef SRC_GROUP_H_
#define SRC_GROUP_H_

#include "Person.h"


class Group {
private:
	vector<Person *> members;
	double ageDistribution[4];
	double jobDistribution[NUMBER_JOBS];
	double religionDistribution[NUMBER_RELIGIONS];
	double hobbiesDistribution[NUMBER_HOBBIES];

public:
	Group();
	virtual ~Group();
	double * getAgeDistribution();
	double * getJobDistribution();
	double * getReligionDistribution();
	double * getHobbiesDistribution();
	double func_afinity(Group other);//afinidade entre dois grupos
	double eval_age(Group other);
	double eval_jobs(Group other);
	double eval_hobbies(Group other);
	double eval_religions(Group other);

};

#endif /* SRC_GROUP_H_ */
