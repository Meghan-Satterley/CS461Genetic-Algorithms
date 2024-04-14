#ifndef CALCULATIONS_H
#define CALCULATIONS_H

// Function to calculate fitness for a single activity
double calculate_activity_fitness(const Assignment& assignment, const vector<Assignment>& schedule, const vector<Room>& rooms, const vector<Facilitator>& facilitators) {
    double fitness = 0.0;

    // Check if the activity is scheduled at the same time in the same room as another activity
    for (const auto& other_assignment : schedule) {
        if (assignment.activity_name != other_assignment.activity_name && assignment.time == other_assignment.time && assignment.room_name == other_assignment.room_name) {
            fitness -= 0.5;
            break;
        }
    }

    // Check room size
    double room_capacity = 0.0;
    for (const auto& room : rooms) {
        if (room.name == assignment.room_name) {
            room_capacity = room.capacity;
            break;
        }
    }
    double expected_enrollment = 0.0;
    for (const auto& activity : activities) {
        if (activity.name == assignment.activity_name) {
            expected_enrollment = activity.expected_enrollment;
            break;
        }
    }
    if (room_capacity < expected_enrollment) {
        fitness -= 0.5;
    }
    else if (room_capacity > 6 * expected_enrollment) {
        fitness -= 0.4;
    }
    else if (room_capacity > 3 * expected_enrollment) {
        fitness -= 0.2;
    }
    else {
        fitness += 0.3;
    }

    // Check facilitator
    bool preferred_facilitator = false;
    bool other_facilitator = false;
    for (const auto& facilitator : facilitators) {
        if (facilitator.name == assignment.facilitator_name) {
            if (find(facilitator.preferred_activities.begin(), facilitator.preferred_activities.end(), assignment.activity_name) != facilitator.preferred_activities.end()) {
                preferred_facilitator = true;
            }
            else if (find(facilitator.other_activities.begin(), facilitator.other_activities.end(), assignment.activity_name) != facilitator.other_activities.end()) {
                other_facilitator = true;
            }
            else {
                fitness -= 0.1;
            }
            // Check facilitator load
            if (facilitator.total_activities == 1) {
                fitness += 0.2;
            }
            else if (facilitator.total_activities > 1) {
                fitness -= 0.2;
            }
            if (facilitator.total_activities > 4) {
                fitness -= 0.5;
            }
            if ((facilitator.total_activities == 1 || facilitator.total_activities == 2) && facilitator.name != "Tyler") {
                fitness -= 0.4;
            }
            break;
        }
    }
    if (preferred_facilitator) {
        fitness += 0.5;
    }
    else if (other_facilitator) {
        fitness += 0.2;
    }


    // Adjust the activity-specific adjustments using the helper function
    if (assignment.activity_name == "SLA100A" || assignment.activity_name == "SLA100B") {
        // Check if both sections are more than 4 hours apart
        int assignment_index = find_time_slot_index(time_slots, assignment.time);
        for (const auto& other_assignment : schedule) {
            if ((other_assignment.activity_name == "SLA100A" || other_assignment.activity_name == "SLA100B") &&
                other_assignment.time != assignment.time) {
                int other_assignment_index = find_time_slot_index(time_slots, other_assignment.time);
                if (abs(assignment_index - other_assignment_index) > 1) {
                    fitness += 0.5;
                }
                else {
                    fitness -= 0.5;
                }
                break;
            }
        }
    }
    // Assuming you've already implemented the find_time_slot_index function as previously described
    else if (assignment.activity_name == "SLA191A" || assignment.activity_name == "SLA191B") {
        // Check if both sections of SLA191 are more than 4 hours apart
        int assignment_index = find_time_slot_index(time_slots, assignment.time);
        for (const auto& other_assignment : schedule) {
            if ((other_assignment.activity_name == "SLA191A" || other_assignment.activity_name == "SLA191B") &&
                other_assignment.time != assignment.time) {
                int other_assignment_index = find_time_slot_index(time_slots, other_assignment.time);
                if (abs(assignment_index - other_assignment_index) > 1) {
                    fitness += 0.5;
                }
                else {
                    fitness -= 0.5;
                }
                break;
            }
        }
    }
    else if ((assignment.activity_name == "SLA100A" || assignment.activity_name == "SLA100B") ||
        (assignment.activity_name == "SLA191A" || assignment.activity_name == "SLA191B")) {
        // Check if sections of SLA100 and SLA191 are taught in consecutive time slots
        int assignment_index = find_time_slot_index(time_slots, assignment.time);
        for (const auto& other_assignment : schedule) {
            if (((assignment.activity_name == "SLA100A" || assignment.activity_name == "SLA100B") &&
                (other_assignment.activity_name == "SLA191A" || other_assignment.activity_name == "SLA191B")) ||
                ((assignment.activity_name == "SLA191A" || assignment.activity_name == "SLA191B") &&
                    (other_assignment.activity_name == "SLA100A" || other_assignment.activity_name == "SLA100B"))) {
                int other_assignment_index = find_time_slot_index(time_slots, other_assignment.time);
                if (abs(assignment_index - other_assignment_index) == 1) {
                    // Check if one activity is in Roman or Beach and the other isn't
                    bool roman_or_beach = (assignment.room_name.find("Roman") != string::npos) || (assignment.room_name.find("Beach") != string::npos);
                    bool other_roman_or_beach = (other_assignment.room_name.find("Roman") != string::npos) || (other_assignment.room_name.find("Beach") != string::npos);
                    if (roman_or_beach != other_roman_or_beach) {
                        fitness -= 0.4;
                    }
                    fitness += 0.5;
                }
                break;
            }
        }
    }
    else {
        // Check if an SLA100 and SLA191 are taught separated by 1 hour
        int assignment_index = find_time_slot_index(time_slots, assignment.time);
        for (const auto& other_assignment : schedule) {
            if (((assignment.activity_name == "SLA101A" || assignment.activity_name == "SLA101B") &&
                (other_assignment.activity_name == "SLA191A" || other_assignment.activity_name == "SLA191B")) ||
                ((assignment.activity_name == "SLA191A" || assignment.activity_name == "SLA191B") &&
                    (other_assignment.activity_name == "SLA100A" || other_assignment.activity_name == "SLA100B"))) {
                int other_assignment_index = find_time_slot_index(time_slots, other_assignment.time);
                if (abs(assignment_index - other_assignment_index) == 2) {
                    fitness += 0.25;
                }
                else {
                    fitness -= 0.25;
                }
                break;
            }
        }
    }


    return fitness;
}

// Helper function to find the index of a specific time in the time_slots vector
int find_time_slot_index(const vector<TimeSlot>& time_slots, const string& time) {
    auto it = find_if(time_slots.begin(), time_slots.end(), [&](const TimeSlot& slot) { return slot.time == time; });
    if (it != time_slots.end()) {
        return distance(time_slots.begin(), it);
    }
    return -1; // Return -1 if the time is not found
}

// Function to calculate fitness for the entire schedule
double calculate_fitness(const vector<Assignment>& schedule) {
    double total_fitness = 0.0;

    // Iterate over each assignment in the schedule and calculate fitness
    for (const auto& assignment : schedule) {
        total_fitness += calculate_activity_fitness(assignment, schedule, rooms, facilitators);
    }

    return total_fitness;
}

#endif 