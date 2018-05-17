/**
 * Age between 0 and 100.
 */

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include "../EnumCreation.h"

using namespace std;

struct Person {
  string id;
  int age;
  int group_id;
  JobArea job_area;
  Religion religion;
  vector<Hobby> hobbies;

  Person(string id, int age, int group_id, JobArea job_area, Religion religion, const vector<Hobby> &hobbies) : id(id), age(age), group_id(group_id), job_area(job_area), religion(religion), hobbies(hobbies) {
  }

  string toString() const {
    string res =  id + ";" + to_string(age) + ";" + to_string(group_id) + ";" + ::toString(job_area) + ";" + ::toString(religion) + ";";
    for (const Hobby &h : hobbies) {
      res += ::toString(h) + ",";
    }
    if (!hobbies.empty()) {
      res.pop_back();
    }
    return res;
  }
};

void createGroup(vector<Person> &people, int group_id, int num_members) {
  for (int i = 0; i < num_members; i++) {
    string id = to_string(people.size());
    int age = rand() % 101;
    JobArea jobArea = (JobArea) (rand() % NUMBER_JOBS);
    Religion religion = (Religion) (rand() % NUMBER_RELIGIONS);
    int numHobbies = rand() % (NUMBER_HOBBIES + 1);
    vector<Hobby> hobbies;
    for (int i = 0; i < numHobbies; i++) {
      hobbies.push_back((Hobby) (rand() % NUMBER_HOBBIES));
    }

    Person p(id, age, group_id, jobArea, religion, hobbies);
    people.push_back(p);
  }
  
}

/**
 * File format:
 *  lugares;numTables
 */
void writeTables(string file_id, const map<int, int> &tables) {
  ofstream file("input/tables_" + file_id + ".txt");
  for (auto it = tables.begin(); it != tables.end(); it++) {
    file << it->first << ";" << it->second << "\r\n";
  }
  file.close();
}

/**
 * File format:
 *  id;age;group_id;job_area;religion;hobby1,hobby2
 */
void writePeople(string file_id, const vector<Person> &people) {
  ofstream file("input/people_" + file_id + ".txt");
  for (const Person &p : people) {
    file << p.toString() << "\r\n";

    //cout << p.id << "\r\n";
  }
  file.close();
}

/**
 * Args: file_id max_tables max_table_slots
 */
int main(int argc, const char *argv[]) {
  if (argc != 4) {
    cerr << argv[0] << " file_id max_tables max_table_slots\n";
    exit(1);
  }
  srand(time(NULL));
  int max_tables = stoi(argv[2]);
  int max_table_slots = stoi(argv[3]);
  map<int, int> tables;
  vector<Person> people;
  int num_tables = rand() % max_tables + 1;
  int group_id = 0;
  for (int i = 0; i < num_tables; i++) {
    int lugares = rand() % max_table_slots + 1;
    tables[lugares]++;
    group_id++;
    if (rand() % 2) {
      createGroup(people, group_id, lugares);
      group_id++;
    }
  }
  writeTables(argv[1], tables);
  writePeople(argv[1], people);
}