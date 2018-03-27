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
	double res = 1;

		for(int i = 0; i < NUMBER_JOBS; i++) {
			res -= abs(this->jobDistribution[i] - other.getJobDistribution()[i]);
		}

		return res;
}

double Group::eval_age(Group other) {
	double res = 1;

	for(int i = 0; i < 4; i++) {
		res -= abs(this->ageDistribution[i] - other.getAgeDistribution()[i]);
	}

	return res;
}

double Group::eval_hobbies(Group other) {
	double res = 1;

		for(int i = 0; i < NUMBER_HOBBIES; i++) {
			res -= abs(this->hobbiesDistribution[i] - other.getHobbiesDistribution()[i]);
		}

		return res;
}

double Group::eval_religions(Group other) {
	double res = 1;

		for(int i = 0; i < NUMBER_RELIGIONS; i++) {
			res -= abs(this->religionDistribution[i] - other.getReligionDistribution()[i]);
		}

		return res;
}

double* Group::getAgeDistribution() {
	return this->ageDistribution;
}

double* Group::getJobDistribution() {
	return this->jobDistribution;
}

double* Group::getReligionDistribution() {
	return this->religionDistribution;
}

double* Group::getHobbiesDistribution() {
	return this->hobbiesDistribution;
}
