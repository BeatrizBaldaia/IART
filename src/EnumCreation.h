#ifndef SRC_ENUMCREATION_H_
#define SRC_ENUMCREATION_H_

#include <map>
#include <iostream>
#include <vector>

using namespace std;

/**
 * SimAnneal: Only run simulated annealing.
 * Genetic: Only run genetic algorithm.
 * All: Run simulated annealing followed by genetic algorithm.
 */
enum ProgramConfig {
	SimAnneal,
	Genetic,
	All
};

struct ProgramConfigMap : public map<string, ProgramConfig> {
	ProgramConfigMap() {
		this->operator[]("SimAnneal") = SimAnneal;
		this->operator[]("Genetic") = Genetic;
		this->operator[]("All") = All;
	}
};

inline string toString(ProgramConfig c) {
	switch (c) {
		case SimAnneal: return "SimAnneal";
		case Genetic: return "Genetic";
		case All: return "All";
		default: return "[Unknown ProgramConfig]";
	}
}

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
};
inline string toString(Hobby v) {
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
};
inline string toString(JobArea v) {
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
};
inline string toString(Religion v)
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
struct CoolingScheduleMap : public map<const char*, CoolingSchedule>
{
	CoolingScheduleMap() {
		this->operator[]((const char*)"Logarithmic") =  Logarithmic;
		this->operator[]((const char*)"Geometric") = Geometric;
		this->operator[]((const char*)"Exponential") = Exponential;
	};
};
inline const char* toString(CoolingSchedule v)
{
	switch (v) {
	case Logarithmic:	return "Logarithmic";
	case Geometric:	return "Geometric";
	case Exponential:	return "Exponential";
	default:	return "[Unknown Cooling Schedule]";
	}
}

#define NUMBER_SCHEDULES 3

enum MutationType {Single, Swap};
struct MutationTypeMap : public map<string, MutationType>
{
	MutationTypeMap() {
		this->operator[]("Single") =  Single;
		this->operator[]("Swap") = Swap;
	};
};
inline const char* toString(MutationType v)
{
	switch (v) {
	case Single:	return "Single";
	case Swap:	return "Swap";
	default:	return "[Unknown MutationType]";
	}
}

#endif /* SRC_ENUMCREATION_H_ */
