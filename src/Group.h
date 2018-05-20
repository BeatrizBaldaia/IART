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
	const double * getAgeDistribution() const;
	const double * getJobDistribution() const;
	const double * getReligionDistribution() const;
	const double * getHobbiesDistribution() const;
	void calculate_attributes();

	/**
	 * Calculates a group's affinity with another.
	 * Takes into account their age, job, hobby and religious distributions.
	 * @param other Group to compare this one with.
	 * @return Affinity.
	 */
	double func_afinity(const Group &other) const;//afinidade entre dois grupos

	/**
	 * Calculates a group's age affinity with another.
	 * @param other Group to compare this one with.
	 * @return Age affinity.
	 */
	double eval_age(const Group &other) const;

	/**
	 * Calculates a group's job affinity with another.
	 * @param other Group to compare this one with.
	 * @return Job affinity.
	 */
	double eval_jobs(const Group &other) const;

	/**
	 * Calculates a group's hobby affinity with another.
	 * @param other Group to compare this one with.
	 * @return Hobby affinity.
	 */
	double eval_hobbies(const Group &other) const;

	/**
	 * Calculates a group's religious affinity with another.
	 * @param other Group to compare this one with.
	 * @return Religious affinity.
	 */
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
