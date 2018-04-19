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
	vector<const Person *> members;
	double ageDistribution[4] = {0};
	double jobDistribution[NUMBER_JOBS] = {0};
	double religionDistribution[NUMBER_RELIGIONS] = {0};
	double hobbiesDistribution[NUMBER_HOBBIES] = {0};

public:
	Group(int id);
	virtual ~Group();
	const double * getAgeDistribution() const;
	const double * getJobDistribution() const;
	const double * getReligionDistribution() const;
	const double * getHobbiesDistribution() const;
	void calculate_attributes();
	double func_afinity(const Group &other) const;//afinidade entre dois grupos
	double eval_age(const Group &other) const;
	double eval_jobs(const Group &other) const;
	double eval_hobbies(const Group &other) const;
	double eval_religions(const Group &other) const;
	void addMember(const Person * newMember);
	int getId() const {
		return id;
	}
	void setId(int id) {
		this->id = id;
	}
	const vector<const Person*>& getMembers() const {
		return members;
	}
	void setMembers(const vector<const Person*>& members) {
		this->members = members;
	}
};

#endif /* SRC_GROUP_H_ */
