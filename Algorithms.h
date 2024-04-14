#ifndef ALGORITHMS_H
#define ALGORITHMS_H

// Function to perform tournament selection
vector<vector<Assignment>> tournament_selection(const vector<vector<Assignment>>& population, int tournament_size) {
    vector<vector<Assignment>> selected_parents;

    // Perform tournament selection until enough parents are selected
    while (selected_parents.size() < population.size()) {
        // Randomly select individuals for the tournament
        vector<int> tournament_indices;
        while (tournament_indices.size() < tournament_size) {
            int index = rand() % population.size();
            tournament_indices.push_back(index);
        }

        // Determine the fittest individual from the tournament
        double best_fitness = -1.0;
        vector<Assignment> best_individual;
        for (int index : tournament_indices) {
            double fitness = calculate_fitness(population[index]);
            if (fitness > best_fitness) {
                best_fitness = fitness;
                best_individual = population[index];
            }
        }

        // Add the fittest individual to the selected parents
        selected_parents.push_back(best_individual);
    }

    return selected_parents;
}

// Function to perform roulette wheel selection
std::vector<std::vector<Assignment>> roulette_wheel_selection(const std::vector<std::vector<Assignment>>& population) {
    std::vector<std::vector<Assignment>> selected_parents;

    // Calculate total fitness of the population
    double total_fitness = 0.0;
    for (const auto& individual : population) {
        total_fitness += calculate_fitness(individual);
    }

    // Calculate fitness proportions
    std::vector<double> fitness_proportions;
    for (const auto& individual : population) {
        double fitness = calculate_fitness(individual);
        double proportion = fitness / total_fitness;
        fitness_proportions.push_back(proportion);
    }

    // Create a roulette wheel by cumulatively summing the fitness proportions
    std::vector<double> wheel;
    double sum = 0.0;
    for (double proportion : fitness_proportions) {
        sum += proportion;
        wheel.push_back(sum);
    }

    // Spin the roulette wheel to select individuals
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (size_t i = 0; i < population.size(); ++i) {
        double spin = dis(gen); // Generate a random number between 0 and 1
        // Find the segment of the wheel where the spin falls
        auto it = std::lower_bound(wheel.begin(), wheel.end(), spin);
        size_t index = std::distance(wheel.begin(), it);
        // Add the selected individual to the selected parents
        selected_parents.push_back(population[index]);
    }

    return selected_parents;
}

// Function to perform rank-based selection
std::vector<std::vector<Assignment>> rank_based_selection(const std::vector<std::vector<Assignment>>& population) {
    std::vector<std::vector<Assignment>> selected_parents;

    // Calculate fitness values for each individual
    std::vector<double> fitness_values;
    for (const auto& individual : population) {
        fitness_values.push_back(calculate_fitness(individual));
    }

    // Rank the individuals based on their fitness values
    std::vector<size_t> ranks(population.size());
    std::iota(ranks.begin(), ranks.end(), 0); // Initialize with indices
    std::sort(ranks.begin(), ranks.end(), [&](size_t i, size_t j) {
        return fitness_values[i] > fitness_values[j]; // Sort in descending order of fitness
        });

    // Assign probabilities based on ranks (linear ranking)
    std::vector<double> probabilities(population.size());
    double sum_rank = (population.size() * (population.size() + 1)) / 2.0; // Sum of ranks
    for (size_t i = 0; i < population.size(); ++i) {
        probabilities[ranks[i]] = (i + 1) / sum_rank; // Assign probability based on rank
    }

    // Perform selection
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<size_t> dist(probabilities.begin(), probabilities.end());
    for (size_t i = 0; i < population.size(); ++i) {
        size_t index = dist(gen); // Randomly select an index based on probabilities
        selected_parents.push_back(population[index]);
    }

    return selected_parents;
}

// Function to perform one-point crossover
pair<vector<Assignment>, vector<Assignment>> one_point_crossover(const vector<Assignment>& parent1, const vector<Assignment>& parent2) {
    int crossover_point = rand() % (parent1.size() - 1) + 1; // Random crossover point, ensuring it's not at the beginning

    vector<Assignment> offspring1(parent1.begin(), parent1.begin() + crossover_point);
    offspring1.insert(offspring1.end(), parent2.begin() + crossover_point, parent2.end());

    vector<Assignment> offspring2(parent2.begin(), parent2.begin() + crossover_point);
    offspring2.insert(offspring2.end(), parent1.begin() + crossover_point, parent1.end());

    return make_pair(offspring1, offspring2);
}

// Function to perform uniform crossover
pair<vector<Assignment>, vector<Assignment>> uniform_crossover(const vector<Assignment>& parent1, const vector<Assignment>& parent2) {
    vector<Assignment> offspring1;
    vector<Assignment> offspring2;

    // Perform uniform crossover for each assignment
    for (size_t i = 0; i < parent1.size(); ++i) {
        // Randomly select whether to inherit from parent 1 or parent 2
        if (rand() % 2 == 0) {
            offspring1.push_back(parent1[i]);
            offspring2.push_back(parent2[i]);
        }
        else {
            offspring1.push_back(parent2[i]);
            offspring2.push_back(parent1[i]);
        }
    }

    return make_pair(offspring1, offspring2);
}


// Function to perform mutation
void mutate(vector<Assignment>& schedule, double mutation_rate, const vector<Room>& rooms, const vector<Facilitator>& facilitators, const vector<TimeSlot>& time_slots) {
    // Iterate over each assignment in the schedule
    for (Assignment& assignment : schedule) {
        // Mutate room with a certain probability
        if (rand() / static_cast<double>(RAND_MAX) < mutation_rate) {
            // Randomly select a new room
            int room_index = rand() % rooms.size();
            assignment.room_name = rooms[room_index].name;
        }

        // Mutate facilitator with a certain probability
        if (rand() / static_cast<double>(RAND_MAX) < mutation_rate) {
            // Randomly select a new facilitator
            int facilitator_index = rand() % facilitators.size();
            assignment.facilitator_name = facilitators[facilitator_index].name;
        }

        // Mutate time slot with a certain probability
        if (rand() / static_cast<double>(RAND_MAX) < mutation_rate) {
            // Randomly select a new time slot
            int time_slot_index = rand() % time_slots.size();
            assignment.time = time_slots[time_slot_index].time;
        }
    }
}
#endif 