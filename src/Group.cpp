/*
 * Group.cpp
 *
 *  Created on: 27/03/2018
 *      Author: Utilizador
 */

#include "Group.h"

Group::Group() {
	// TODO Auto-generated constructor stub

}

Group::~Group() {
	// TODO Auto-generated destructor stub
}

double Group::func_afinity(Group other) {
	return this->eval_age(other)
			+ this->eval_jobs(other)
			+ this->eval_hobbies(other)
			+ this->eval_religions(other);
}

double Group::eval_jobs(Group other) {
	//TODO: implement eval_jobs
	return 0;
}

double Group::eval_age(Group other) {
	double res = 0;
	res += 100 - (this->age_group[0] - other.age_group[0]);
	res += 100 - (this->age_group[1] - other.age_group[1]);
	res += 100 - (this->age_group[2] - other.age_group[2]);
	res += 100 - (this->age_group[3] - other.age_group[3]);
	return res;
}

double Group::eval_hobbies(Group other) {
	//TODO: implement eval_jobs
	return 0;
}

double Group::eval_religions(Group other) {
	//TODO: implement eval_jobs
	return 0;
}
