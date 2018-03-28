/*
 * Group.cpp
 *
 *  Created on: 27/03/2018
 *      Author: Utilizador
 */

#include "Group.h"
#include <cmath>

//TODO: Correct ages.
AgeGroup getAgeStage(int age) {
	if (age < 10) {
		return Child;
	} else if (age < 20) {
		return Teen;
	} else if (age < 50) {
		return Adult;
	} else {
		return Elder;
	}
}

Group::Group(int id) : id(id) {
	// TODO Auto-generated constructor stub

}

Group::~Group() {
	// TODO Auto-generated destructor stub
}

const double* Group::getAgeDistribution() const {
	return this->ageDistribution;
}

const double* Group::getJobDistribution() const {
	return this->jobDistribution;
}

const double* Group::getReligionDistribution() const {
	return this->religionDistribution;
}

const double* Group::getHobbiesDistribution() const{
	return this->hobbiesDistribution;
}

void Group::calculate_attributes() {
	for (const Person &person : members) {
		//age
		ageDistribution[getAgeStage(person.getAge())]++;
		//job
		jobDistribution[person.getJob()]++;
		//religion
		religionDistribution[person.getReligion()]++;
		//hobbies
		for (const Hobby &hobby : person.getHobbies()) {
			hobbiesDistribution[hobby]++;
		}
	}
	for (double &age_group : ageDistribution) {
		age_group /= members.size();
	}
	for (double &job : jobDistribution) {
		job /= members.size();
	}
	for (double &religion : religionDistribution) {
		religion /= members.size();
	}
	for (double &hobby : hobbiesDistribution) {
		hobby /= members.size();
	}
}

double Group::func_afinity(const Group &other) const {
	return this->eval_age(other)
			+ this->eval_jobs(other)
			+ this->eval_hobbies(other)
			+ this->eval_religions(other);
}

double Group::eval_age(const Group &other) const {
	double res = 1;

	for(int i = 0; i < 4; i++) {
		res -= abs(this->ageDistribution[i] - other.getAgeDistribution()[i]);
	}
}

double Group::eval_jobs(const Group &other) const {
	double res = 1;

	for(int i = 0; i < NUMBER_JOBS; i++) {
		res -= abs(this->jobDistribution[i] - other.getJobDistribution()[i]);
	}

	return res;

}


double Group::eval_hobbies(const Group &other) const {
	double res = 1;

		for(int i = 0; i < NUMBER_HOBBIES; i++) {
			res -= abs(this->hobbiesDistribution[i] - other.getHobbiesDistribution()[i]);
		}

		return res;

}


double Group::eval_religions(const Group &other) const {
	double res = 1;

		for(int i = 0; i < NUMBER_RELIGIONS; i++) {
			res -= abs(this->religionDistribution[i] - other.getReligionDistribution()[i]);
		}

		return res;
}


