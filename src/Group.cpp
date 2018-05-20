#include <cmath>
#include <map>
#include <stdio.h>
#include <float.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <stdlib.h>

#include "Group.h"

void Group::addMember(const Person * newMember){
	this->members.push_back(newMember);
}

AgeGroup getAgeStage(int age) {
	if (age < 10) {
		return Child;
	} else if (age < 20) {
		return Teen;
	} else if (age < 65) {
		return Adult;
	} else {
		return Elder;
	}
}

Group::Group(int id) : id(id) {
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
	for (unsigned int i = 0; i < members.size(); i++) {
		//age
		ageDistribution[getAgeStage(members[i]->getAge())]++;
		//job
		jobDistribution[members[i]->getJob()]++;
		//religion
		religionDistribution[members[i]->getReligion()]++;
		//hobbies
		for (unsigned int j = 0; j < members[i]->getHobbies().size(); j++) {
			hobbiesDistribution[members[i]->getHobbies()[j]]++;
		}
	}

	int nMembers = members.size();

	for (unsigned int i = 0; i < NUMBER_AGES; i++) {
		ageDistribution[i] /= nMembers;
	}
	for (unsigned int i = 0; i < NUMBER_JOBS; i++) {
		jobDistribution[i] /= nMembers;
	}
	for (unsigned int i = 0; i < NUMBER_RELIGIONS; i++) {
		religionDistribution[i] /= nMembers;
	}
	for (unsigned int i = 0; i < NUMBER_HOBBIES; i++) {
		hobbiesDistribution[i] /= members.size();
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
	int notNullFields = 0;
	vector<double> diffs;
	bool common = false;

	for(int i = 0; i < 4; i++) {
		if(!(this->ageDistribution[i] == 0 && other.getAgeDistribution()[i] == 0)) {
			notNullFields++;
			diffs.push_back(abs(this->ageDistribution[i] - other.getAgeDistribution()[i]));
		}
		if(!common && (this->ageDistribution[i] != 0 && other.getAgeDistribution()[i] != 0)) {
			common = true;
		}
	}
	if(!common) {
		return 0;
	}

	for(unsigned int j = 0; j < diffs.size(); j++) {
		res -= diffs[j] / notNullFields;
	}
	return res;
}

double Group::eval_jobs(const Group &other) const {
	double res = 1;
	int notNullFields = 0;
	vector<double> diffs;
	bool common = false;

	for(int i = 0; i < NUMBER_JOBS; i++) {
		if(!(this->jobDistribution[i] == 0 && other.getJobDistribution()[i] == 0)) {
			notNullFields++;
			diffs.push_back(abs(this->jobDistribution[i] - other.getJobDistribution()[i]));
		}
		if(!common && (this->jobDistribution[i] != 0 && other.getJobDistribution()[i] != 0)) {
			common = true;
		}
	}
	if(!common) {
		return 0;
	}

	for(unsigned int j = 0; j < diffs.size(); j++) {
		res -= diffs[j] / notNullFields;
	}
	return res;
}

double Group::eval_hobbies(const Group &other) const {
	double res = 1;
	int notNullFields = 0;
	vector<double> diffs;
	bool common = false;

	for(int i = 0; i < NUMBER_HOBBIES; i++) {
		if(!(this->hobbiesDistribution[i] == 0 && other.getHobbiesDistribution()[i] == 0)) {
			notNullFields++;
			diffs.push_back(abs(this->hobbiesDistribution[i] - other.getHobbiesDistribution()[i]));
		}
		if(!common && (this->hobbiesDistribution[i] != 0 && other.getHobbiesDistribution()[i] != 0)) {
			common = true;
		}
	}
	if(!common) {
		return 0;
	}

	for(unsigned int j = 0; j < diffs.size(); j++) {
		res -= diffs[j] / notNullFields;
	}
	return res;

}

double Group::eval_religions(const Group &other) const {
	double res = 1;
	int notNullFields = 0;
	vector<double> diffs;
	bool common = false;

	for(int i = 0; i < NUMBER_RELIGIONS; i++) {
		if(!(this->religionDistribution[i] == 0 && other.getReligionDistribution()[i] == 0)) {
			notNullFields++;
			diffs.push_back(abs(this->religionDistribution[i] - other.getReligionDistribution()[i]));
		}
		if(!common && (this->religionDistribution[i] != 0 && other.getReligionDistribution()[i] != 0)) {
			common = true;
		}
	}
	if(!common) {
		return 0;
	}

	for(unsigned int j = 0; j < diffs.size(); j++) {
		res -= diffs[j] / notNullFields;
	}
	return res;
}