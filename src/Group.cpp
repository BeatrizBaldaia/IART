/*
 * Group.cpp
 *
 *  Created on: 27/03/2018
 *      Author: Utilizador
 */

#include "Group.h"
#include <cmath>

//TODO: Correct ages.
AgeGroup getGroup(int age) {
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

void Group::calculate_attributes() {
	for (const Person &person : members) {
		//age
		age_groups[getGroup(person.age)]++;
		//job
		jobs[person.job]++;
		//religion
		religions[person.religion]++;
		//hobbies
		for (const Hobby &hobby : person.hobbies) {
			hobbies[hobby]++;
		}
	}
	for (double &age_group : age_groups) {
		age_group /= members.size();
	}
	for (double &job : jobs) {
		job /= members.size();
	}
	for (double &religion : religions) {
		religion /= members.size();
	}
	for (double &hobby : hobbies) {
		hobby /= members.size();
	}
}

double Group::func_afinity(const Group &other) const {
	return this->eval_age(other)
			+ this->eval_jobs(other)
			+ this->eval_hobbies(other)
			+ this->eval_religions(other);
}

double Group::eval_jobs(const Group &other) const {
	double res = 0;
	for (int i = 0; i < NUMBER_JOBS; i++) {
		res += 1 - abs(this->jobs[i] - other.jobs[i]);
	}
	return res;
}

double Group::eval_age(const Group &other) const {
	double res = 0;
	res += 1 - abs(this->age_groups[0] - other.age_groups[0]);
	res += 1 - abs(this->age_groups[1] - other.age_groups[1]);
	res += 1 - abs(this->age_groups[2] - other.age_groups[2]);
	res += 1 - abs(this->age_groups[3] - other.age_groups[3]);
	return res;
}

double Group::eval_hobbies(const Group &other) const {
	double res = 0;
	for (int i = 0; i < NUMBER_HOBBIES; i++) {
		res += 1 - abs(this->hobbies[i] - other.hobbies[i]);
	}
	return res;
}

double Group::eval_religions(const Group &other) const {
	double res = 0;
	for (int i = 0; i < NUMBER_RELIGIONS; i++) {
		res += 1 - abs(this->religions[i] - other.religions[i]);
	}
	return res;
}
