#include "Schedule.h"
#include "Algorithms.h"
#include "Calculations.h"

int main() {
    // Initialize population
    int population_size = 500; // Adjust population size as needed
    vector<vector<Assignment>> population = initialize_population(population_size, activities, rooms, facilitators, time_slots);

    // Main loop
    int num_generations = 100; // Adjust number of generations as needed
    double mutation_rate = 0.01; // Adjust mutation rate as needed
    for (int generation = 0; generation < num_generations; ++generation) {

        // Selection
        vector<vector<Assignment>> selected_parents = tournament_selection(population, 2); // Tournament selection with tournament size 2
        //vector<vector<Assignment>> selected_parents = roulette_wheel_selection(population);
        //vector<vector<Assignment>> selected_parents = rank_based_selection(population);

        // uniform crossover
        vector<vector<Assignment>> offspring_population;
        for (size_t i = 0; i < selected_parents.size(); i += 2) {
            auto offspring_pair = one_point_crossover(selected_parents[i], selected_parents[i + 1]);
            //auto offspring_pair = uniform_crossover(selected_parents[i], selected_parents[i + 1]);
            offspring_population.push_back(offspring_pair.first);
            offspring_population.push_back(offspring_pair.second);
        }

        // Mutation
        for (vector<Assignment>& offspring : offspring_population) {
            mutate(offspring, mutation_rate, rooms, facilitators, time_slots);
        }

        // Calculate fitness for each schedule in the offspring population
        vector<double> offspring_fitness;
        for (const auto& offspring : offspring_population) {
            offspring_fitness.push_back(calculate_fitness(offspring));
        }

        // Replace the old population with the offspring population
        population = offspring_population;

        // Find the best fitness of the current generation
        double best_fitness = *max_element(offspring_fitness.begin(), offspring_fitness.end());

        // Report best fitness of the current generation
        cout << "Generation " << generation + 1 << ": Best Fitness = " << best_fitness << endl;
    }

    // Report final best schedule and its fitness
    // Find the index of the best schedule in the population
    double best_fitness = -1.0;
    size_t best_schedule_index = 0;
    for (size_t i = 0; i < population.size(); ++i) {
        double fitness = calculate_fitness(population[i]);
        if (fitness > best_fitness) {
            best_fitness = fitness;
            best_schedule_index = i;
        }
    }

    // Output the best schedule and its fitness
    cout << "Best Schedule (Final Generation):" << endl;
    for (const auto& assignment : population[best_schedule_index]) {
        cout << "Activity: " << assignment.activity_name << ", Room: " << assignment.room_name << ", Facilitator: " << assignment.facilitator_name << ", Time: " << assignment.time << endl;
    }
    cout << "Fitness of the Best Schedule: " << best_fitness << endl;

    return 0;
}
