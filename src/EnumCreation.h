#ifndef SRC_ENUMCREATION_H_
#define SRC_ENUMCREATION_H_

#include <map>
#include <iostream>
#include <vector>

using namespace std;

enum Hobby {Reading,
	WatchingTV,
	FamilyTime,
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
struct HobbyMap : public map<string, Hobby> {
	HobbyMap() {
		this->operator[]("Reading") =  Reading;
		this->operator[]("WatchingTV") = WatchingTV;
		this->operator[]("FamilyTime") = FamilyTime;
		this->operator[]("GoingToMovies") =  GoingToMovies;
		this->operator[]("Fishing") = Fishing;
		this->operator[]("Computer") = Computer;
		this->operator[]("Gardening") =  Gardening;
		this->operator[]("RentingMovies") = RentingMovies;
		this->operator[]("Walking") = Walking;
		this->operator[]("Exercise") =  Exercise;
		this->operator[]("ListeningToMusic") = ListeningToMusic;
		this->operator[]("Entertaining") = Entertaining;
		this->operator[]("Hunting") =  Hunting;
		this->operator[]("TeamSports") = TeamSports;
		this->operator[]("Shopping") = Shopping;
		this->operator[]("Traveling") = Traveling;
	};
	~HobbyMap(){}
};
inline const char* toString(Hobby v) {
	switch (v) {
		case Reading:	return "Reading";
		case WatchingTV:	return "WatchingTV";
		case FamilyTime:	return "FamilyTime";
		case GoingToMovies:	return "GoingToMovies";
		case Fishing:	return "Fishing";
		case Computer:	return "Computer";
		case Gardening:	return "Gardening";
		case RentingMovies:	return "RentingMovies";
		case Walking:	return "Walking";
		case Exercise:	return "Exercise";
		case ListeningToMusic:	return "ListeningToMusic";
		case Entertaining:	return "Entertaining";
		case Hunting:	return "Hunting";
		case TeamSports:	return "TeamSports";
		case Shopping:	return "Shopping";
		case Traveling:	return "Traveling";
		default:	return "[Unknown Hobby]";
	}
}
#define NUMBER_HOBBIES 16

enum JobArea {Science, Education, Politics};
struct JobAreaMap : public map<string, JobArea> {
	JobAreaMap() {
		this->operator[]("Science") =  Science;
		this->operator[]("Education") = Education;
		this->operator[]("Politics") = Politics;
	};
	~JobAreaMap(){}
};
inline const char* toString(JobArea v) {
	switch (v) {
		case Science:	return "Science";
		case Education:	return "Education";
		case Politics:	return "Politics";
		default:	return "[Unknown JobArea]";
	}
}
#define NUMBER_JOBS 3

enum Religion {Christianity, Gnosticism, Islam, Judaism, Buddhism, Hinduism, Atheism};
struct ReligionMap : public map<string, Religion>
{
	ReligionMap() {
		this->operator[]("Christianity") =  Christianity;
		this->operator[]("Gnosticism") = Gnosticism;
		this->operator[]("Islam") = Islam;
		this->operator[]("Judaism") =  Judaism;
		this->operator[]("Buddhism") = Buddhism;
		this->operator[]("Hinduism") = Hinduism;
		this->operator[]("Atheism") = Atheism;
	};
	~ReligionMap(){}
};
inline const char* toString(Religion v)
{
	switch (v) {
	case Christianity:	return "Christianity";
	case Gnosticism:	return "Gnosticism";
	case Islam:	return "Islam";
	case Judaism:	return "Judaism";
	case Buddhism:	return "Buddhism";
	case Hinduism:	return "Hinduism";
	case Atheism:	return "Atheism";
		default:	return "[Unknown Religion]";
	}
}
#define NUMBER_RELIGIONS 7

enum CoolingSchedule {Logarithmic, Geometric, Exponential};



#endif /* SRC_ENUMCREATION_H_ */
