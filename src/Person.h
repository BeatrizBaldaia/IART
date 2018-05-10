/*
 * Person.h
 *
 *  Created on: 27/03/2018
 *      Author: anabela
 */

#ifndef SRC_PERSON_H_
#define SRC_PERSON_H_

#include <iostream>
#include <vector>
#include <map>
#include"EnumCreation.h"

using namespace std;

#define CHILDHOOD 0
#define ADOLESCENCE 1
#define ADULTHOOD 2
#define OLDAGE 3
#define NUMBER_AGES 4


class Person {
private:
	string name;
	int group;
	int age;
	vector<Hobby> hobbies;
	JobArea job;
	Religion religion;
public:
	Person();
	virtual ~Person();
	double eval_func(Person person); //afinidade entre duas pessoas

	int getAge() const {
		return age;
	}

	void setAge(int age) {
		this->age = age;
	}

	int getGroup() const {
		return group;
	}

	void setGroup(int group) {
		this->group = group;
	}

	JobArea getJob() const {
		return job;
	}

	void setJob(JobArea job) {
		this->job = job;
	}

	const string& getName() const {
		return name;
	}

	void setName(const string& name) {
		this->name = name;
	}

	Religion getReligion() const {
		return religion;
	}

	void setReligion(Religion religion) {
		this->religion = religion;
	}

	const vector<Hobby>& getHobbies() const {
		return hobbies;
	}

	void setHobbies(const vector<Hobby>& hobbies) {
		this->hobbies = hobbies;
	}
};

#endif /* SRC_PERSON_H_ */
