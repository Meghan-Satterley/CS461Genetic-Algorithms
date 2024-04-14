#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <cmath>
#include <string>
#include <iterator>
#include <numeric>

using namespace std;

struct Facilitator
{
    string name;
    vector<string> preferred_activities;
    vector<string> other_activities;
    int total_activities = 0;
};

struct Room
{
    string name;
    int capacity;
};

struct Activity
{
    string name;
    int expected_enrollment;
    vector<string> preferred_activities;
    vector<string> other_activities;
};

struct TimeSlot
{
    string time;
};

struct Assignment
{
    string activity_name;
    string room_name;
    string facilitator_name;
    string time;
};

double calculate_fitness(const vector<Assignment>& schedule);
int find_time_slot_index(const vector<TimeSlot>& time_slots, const string& time);
//fix facilitator 
vector<Facilitator> facilitators = {
    {"Lock", {"SLA100A", "SLA100B", "SLA191A", "SLA191B", "SLA291"}, {"SLA201", "SLA303", "SLA304", "SLA394", "SLA449", "SLA451"}},
    {"Glen", {"SLA100A", "SLA100B", "SLA191A", "SLA191B", "SLA201", "SLA303"}, {"SLA291", "SLA304", "SLA394", "SLA449", "SLA451"}},
    {"Banks", {"SLA100A", "SLA100B", "SLA191A", "SLA191B", "SLA201", "SLA291", "SLA303"}, {"SLA304", "SLA394", "SLA449", "SLA451"}},
    {"Richards", {"SLA100A", "SLA100B", "SLA191A", "SLA191B"}, {"SLA201", "SLA291", "SLA303", "SLA304", "SLA394", "SLA449", "SLA451"}},
    {"Shaw", {"SLA201", "SLA291", "SLA303"}, {"SLA100A", "SLA100B", "SLA191A", "SLA191B", "SLA304", "SLA394", "SLA449", "SLA451"}},
    {"Singer", {"SLA201", "SLA291", "SLA303", "SLA304", "SLA394", "SLA449", "SLA451"}, {"SLA100A", "SLA100B", "SLA191A", "SLA191B"}},
    {"Uther", {"SLA304", "SLA394", "SLA449", "SLA451"}, {"SLA100A", "SLA100B", "SLA191A", "SLA191B", "SLA201", "SLA291", "SLA303"}},
    {"Tyler", {"SLA304", "SLA394", "SLA449", "SLA451"}, {"SLA100A", "SLA100B", "SLA191A", "SLA191B", "SLA201", "SLA291", "SLA303"}},
    {"Numen", {"SLA100A", "SLA100B", "SLA191A", "SLA191B", "SLA201", "SLA291", "SLA303", "SLA304", "SLA394", "SLA449", "SLA451"}, {}},
    {"Zeldin", {"SLA100A", "SLA100B", "SLA191A", "SLA191B", "SLA201", "SLA291", "SLA303", "SLA304", "SLA394", "SLA449", "SLA451"}, {}}
};

vector<Room> rooms = {
     {"Slater 003", 45},
     {"Roman 216", 30},
     {"Loft 206", 75},
     {"Roman 201", 50},
     {"Loft 310", 108},
     {"Beach 201", 60},
     {"Beach 301", 75},
     {"Logos 325", 450},
     {"Frank 119", 60}

};

vector<TimeSlot> time_slots = {
     {"10 AM"},
     {"11 AM"},
     {"12 PM"},
     {"1 PM"},
     {"2 PM"},
     {"3 PM"}
};

vector<Activity> activities = {
    // goes as room, enrollment, preferred, other
    {"SLA100A", 50, {"Glen", "Lock", "Banks", "Zeldin"}, {"Numen", "Richards"}},
    {"SLA100B", 50, {"Glen", "Lock", "Banks", "Zeldin"}, {"Numen", "Richards"}},
    {"SLA191A", 50, {"Glen", "Lock", "Banks", "Zeldin"}, {"Numen", "Richards"}},
    {"SLA191B", 50, {"Glen", "Lock", "Banks", "Zeldin"}, {"Numen", "Richards"}},
    {"SLA201", 50, {"Glen", "Banks", "Zeldin", "Shaw"}, {"Numen", "Richards", "Singer"}},
    {"SLA291", 50, {"Lock", "Banks", "Zeldin", "Singer"}, {"Numen", "Richards", "Shaw", "Tyler"}},
    {"SLA303", 60, {"Glen", "Zeldin", "Banks"}, {"Numen", "Singer", "Shaw"}},
    {"SLA304", 25, {"Glen", "Banks", "Tyler"}, {"Numen", "Singer", "Shaw", "Richards", "Uther", "Zeldin"}},
    {"SLA394", 20, {"Tyler", "Singer"}, {"Richards", "Zeldin"}},
    {"SLA449", 60, {"Tyler", "Singer", "Shaw"}, {"Zeldin", "Uther"}},
    {"SLA451", 100, {"Tyler", "Singer", "Shaw"}, {"Zeldin", "Uther", "Richards", "Banks"}}
};

// Function to initialize the population
vector<vector<Assignment>> initialize_population(int population_size, const vector<Activity>& activities, const vector<Room>& rooms, const vector<Facilitator>& facilitators, const vector<TimeSlot>& time_slots) {
    vector<vector<Assignment>> population(population_size);

    // Validate time slots
    for (const auto& activity : activities) {
        for (const auto& preferred_time : time_slots) {
            bool time_found = false;
            for (const auto& assigned_time : time_slots) {
                if (preferred_time.time == assigned_time.time) {
                    time_found = true;
                    break;
                }
            }
            if (!time_found) {
                cerr << "Error: Time slot " << preferred_time.time << " is missing." << endl;
                // Handle the error (e.g., choose a default time slot)
            }
        }
    }

    // Initialize population with valid time slots
    // Iterate over the population size
    for (int i = 0; i < population_size; ++i) {
        // Create a new schedule
        vector<Assignment> schedule;

        // Randomly assign activities to rooms, facilitators, and time slots
        for (const auto& activity : activities) {
            Assignment assignment;
            assignment.activity_name = activity.name;

            // Randomly select room
            int room_index = rand() % rooms.size();
            assignment.room_name = rooms[room_index].name;

            // Randomly select facilitator
            vector<string> possible_facilitators = activity.preferred_activities;
            possible_facilitators.insert(possible_facilitators.end(), activity.other_activities.begin(), activity.other_activities.end());
            int facilitator_index = rand() % possible_facilitators.size();
            assignment.facilitator_name = possible_facilitators[facilitator_index];

            // Randomly select time slot (ensure it exists in time_slots vector)
            int time_slot_index = rand() % time_slots.size();
            assignment.time = time_slots[time_slot_index].time;

            // Add assignment to schedule
            schedule.push_back(assignment);
        }

        // Add the generated schedule to the population
        population[i] = schedule;
    }

    return population;
}
#endif 