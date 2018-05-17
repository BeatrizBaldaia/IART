#include <iostream>
#include <map>
#include <vector>
#include "src/EnumCreation.h"

using namespace std;

struct People {
  string id;
  int age;
  int group_id;
  JobArea job_area;
  Religion religion;
  vector<Hobby> hobbies;
};

void createGroup(vector<People> &people, int group_id, int num_members) {
  string id = "" + people.size();
  int age = rand() % 101;
  
  

  People p(id, age, group_id, job_area, religion, hobbies);
}

/**
 * Argv: max_tables max_table_slots
 */
int main(int argc, const char *argv[]) {
  srand(time(NULL));
  int max_tables = stoi(argv[0]);
  int max_table_slots = stoi(argv[1]);
  map<int, int> tables;
  vector<People> people;
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
}