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
#include "Hobby.h"

using namespace std;

enum Hobby {Reading, WatchingTV, FamilyTime,
	GoingToMovies,
	Fishing,
	Computer,
	Gardening,
	RentingMovies,
	Walking,
	Exercise,
	ListeningToMusic,
	Entertaining,
	Hunting,
	TeamSports,
	Shopping,
	Traveling};
enum JobArea {Science, Education, Politics};
enum Religion {Christianity, Gnosticism, Islam,	Judaism, Buddhism, Hinduism};

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
};

#endif /* SRC_PERSON_H_ */
