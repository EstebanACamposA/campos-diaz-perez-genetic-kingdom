#include <iostream>
#include <vector>
#include <cstring>
// #include <cstdint>
// #include <iomanip>

#include <random>
#include <cmath>
#include <algorithm>

#include "genetics.h"

bool Genetics::RandomBool(double probability)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::bernoulli_distribution d(probability);
    return d(gen);
}

// guide:
// float mutation_relative_change = 0.2f;
// float health = 100;
// float speed_multiplier = 1;
// float pierce_armor = 1;
// float magic_armor = 1;
// float siege_armor = 1;

// This constructor is used to create new Individuals manually and the base Individual for the start of the game. 
// The constructor calls CalculateFitness().
Individual::Individual(float max_health, float speed_multiplier, float pierce_armor, float magic_armor, float siege_armor)
{
    this->mutation_relative_change = 0.2f;
    this->max_health = max_health;
    // this->health = max_health;
    this->speed_multiplier = speed_multiplier;
    this->pierce_armor = pierce_armor;
    this->magic_armor = magic_armor;
    this->siege_armor = siege_armor;

    CalculateFitness();
};

// Default constructor gives -1.0f to all attributes.
Individual::Individual()
{
    this->mutation_relative_change = -1.0f;
    this->max_health = -1.0f;
    this->speed_multiplier = -1.0f;
    this->pierce_armor = -1.0f;
    this->magic_armor = -1.0f;
    this->siege_armor = -1.0f;
}

// Constructor for Character to Individual. Takes completed_path and remaining_health.
// Used when a Character is destroyed to comunicate its genetic performance to the genetic manager.
Individual::Individual(float max_health, float speed_multiplier, float pierce_armor, float magic_armor, float siege_armor, float completed_path, float remaining_health)
{
    this->mutation_relative_change = 0.2f;
    this->max_health = max_health;
    // this->health = max_health;
    this->speed_multiplier = speed_multiplier;
    this->pierce_armor = pierce_armor;
    this->magic_armor = magic_armor;
    this->siege_armor = siege_armor;

    CalculateFitness(completed_path, remaining_health);
};

// Currently has testing behaviour. Actual fitness calculation requires completed_path and remaining_health.
// Use this constructor to create Characters from scratch (with no base Individual from best_individual)
void Individual::CalculateFitness()
{
    // std::cout << "Entered Individual::CalculateFitness(). this testing funciton shouldn't be used outside genetics testing." << std::endl;
    // Fitness range: [0,2]
    // Actual fitness function.
    // float fitness = completed_path/(1 - remaining_health/2)  //remaining_health = health/max_health. Same for the completed path.
    
    // Function for testing the class. 
    float fitness = 2 * (1 - expf(-1 * max_health * speed_multiplier * sqrtf(pierce_armor) * sqrtf(magic_armor) * sqrtf(siege_armor) / 1000));
    this->fitness = fitness;
};

void Individual::CalculateFitness(float completed_path, float remaining_health)
{
    // Fitness range: [0,2]
    // Actual fitness function.
    float calc_fitness = completed_path/(1 - remaining_health/2);  //remaining_health = health/max_health. Same for the completed path.
    this->fitness = calc_fitness;
    std::cout << "CalculateFitness(float completed_path, float remaining_health): completed_path = " << completed_path << "; remaining_health = " << remaining_health;    
    std::cout << "--> calc_fitness = " << calc_fitness << std::endl;
};

// Testing only function.
// Increases or decreases each stat by mutation_relative_change and recalculates the fitness.
// e. g.: stat *= 1.2; OR stat *= 0.8;
// This is called by the testing function Genetics.SimulateWave()
void Individual::MutateStats()
{
    this->max_health    *= 1 + this->mutation_relative_change * (1 - 2*Genetics::RandomBool(0.5));
    this->speed_multiplier         *= 1 + this->mutation_relative_change * (1 - 2*Genetics::RandomBool(0.5));
    this->pierce_armor  *= 1 + this->mutation_relative_change * (1 - 2*Genetics::RandomBool(0.5));
    this->magic_armor   *= 1 + this->mutation_relative_change * (1 - 2*Genetics::RandomBool(0.5));
    this->siege_armor   *= 1 + this->mutation_relative_change * (1 - 2*Genetics::RandomBool(0.5));
    CalculateFitness();
}


// Increases or decreases each stat by mutation_relative_change.
// e. g.: stat *= 1.2; OR stat *= 0.8;
// Decides based on mutation_chance for each of the 5 genes.
int Individual::MutateStats(double mutation_chance)
{
    int mutated_stats = 0;
    if (Genetics::RandomBool(mutation_chance))
    {
        this->max_health    *= 1 + this->mutation_relative_change * (1 - 2*Genetics::RandomBool(0.5));
        mutated_stats ++;
    }
    if (Genetics::RandomBool(mutation_chance))
    {
        this->speed_multiplier    *= 1 + this->mutation_relative_change * (1 - 2*Genetics::RandomBool(0.5));
        mutated_stats ++;
    }
    if (Genetics::RandomBool(mutation_chance))
    {
        this->pierce_armor    *= 1 + this->mutation_relative_change * (1 - 2*Genetics::RandomBool(0.5));
        mutated_stats ++;
    }
    if (Genetics::RandomBool(mutation_chance))
    {
        this->magic_armor    *= 1 + this->mutation_relative_change * (1 - 2*Genetics::RandomBool(0.5));
        mutated_stats ++;
    }
    if (Genetics::RandomBool(mutation_chance))
    {
        this->siege_armor    *= 1 + this->mutation_relative_change * (1 - 2*Genetics::RandomBool(0.5));
        mutated_stats ++;
    }
    // // TESTING RandomBool!!!!!
    // for (size_t i = 0; i < 100; i++)
    // {
    //     if (Genetics::RandomBool(0.5))
    //     {
    //         std::cout << "Genetics::RandomBool(0.5) = " << i << "," << true << std::endl;
    //     }
        
    // }
    // std::cout << "TESTS WITH mutation_chance" << std::endl;
    // for (size_t i = 0; i < 100; i++)
    // {
    //     if (Genetics::RandomBool(mutation_chance))
    //     {
    //         std::cout << "Genetics::RandomBool(mutation_chance) = " << i << "," << true << std::endl;
    //     }
        
    // }
    

    std::cout << "At Individual::MutateStats mutated stats = " << mutated_stats << std::endl;
    return mutated_stats;
}



/* Order of species and spawn ratios:
    0 Orcs, 15
    1 Night elves, 10
    2 Harpies, 7
    3 Mercenaries, 7
*/

// Constructor initializes best_individuals with their start value for the game.
// guide:
// float mutation_relative_change = 0.2f;
// float health = 100;
// float speed_multiplier = 1;
// float pierce_armor = 1;
// float magic_armor = 1;
// float siege_armor = 1;
Genetics::Genetics()
{
    // Initializes the species vector with empty vector<Individual> for each species.  
    std::vector<Individual> orcs;
    std::vector<Individual> night_elves;
    std::vector<Individual> harpies;
    std::vector<Individual> mercs;

    this->species.push_back(orcs);
    this->species.push_back(night_elves);
    this->species.push_back(harpies);
    this->species.push_back(mercs);

    // Initializes the species vector with empty vector<Individual> for each species.  
    std::vector<Individual> best_orcs;
    std::vector<Individual> best_night_elves;
    std::vector<Individual> best_harpies;
    std::vector<Individual> best_mercs;

    this->best_individuals_matrix.push_back(best_orcs);
    this->best_individuals_matrix.push_back(best_night_elves);
    this->best_individuals_matrix.push_back(best_harpies);
    this->best_individuals_matrix.push_back(best_mercs);

    //Initializes the best_individuals vector with starter values. All values are 1 since these represent genes, and not the actual stats that interact within the game.
    Individual best_orc = Individual(1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    Individual best_night_elf = Individual(1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    Individual best_harpy = Individual(1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    Individual best_merc = Individual(1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    this->best_individuals.push_back(best_orc);
    this->best_individuals.push_back(best_night_elf);
    this->best_individuals.push_back(best_harpy);
    this->best_individuals.push_back(best_merc);
}

// AddIndividual(int species) is called from the game loop to add the Individuals' performances from the current wave.
// AddIndividual(int species) is called each time an enemy is destroyed (by the player or when it reaches the goal)

void Genetics::SimulateWave(int n)
{
    std::vector<int> spawn_rates = {15, 10, 7, 7}; 
    // The size of spawn_rates vector must match that of best_individuals and of species.
    // i. e. the amount of species: orcs, night elves, harpies, mercs. -> 4.

    for (size_t i = 0; i < spawn_rates.size(); i++)
    {
        // Inserts n*(enemy spawn rate) mutated Individuals created from the current best into this.species[j] (the orcs, NE, harpies, mercs).
        for (size_t j = 0; j < n*spawn_rates[i]; j++)
        {
            Individual enemy = this->best_individuals[i]; // This is a deep copy because Individual class contains only primitive types.
            enemy.MutateStats();    // Manually mutates the enemy (which recalculates its fitness)
            this->species[i].push_back(enemy);
        }
    }
    
    // Clears the wave for the next one.

    // Calculates matrix best_fits_ids with the best 5 Individuals' ids for each species.
    std::vector<std::vector<int>> best_fits_ids = CalculateBest5FitnessIDs();

    // Show species' fits.
    // ShowSpecies();

    // Show best_fits_ids.
    // std::cout << "---Show best_fits_ids.---" << std::endl;
    // for (size_t i = 0; i < best_fits_ids.size(); i++)
    // {
    //     for (size_t j = 0; j < best_fits_ids[i].size(); j++)
    //     {
    //         std::cout << best_fits_ids[i][j] << ' ';
    //     }
    //     std::cout << std::endl;
    // }

    // Updates best_individuals_matrix.
    GetBest5Individuals(best_fits_ids);
    
    // Updates best_individuals based on best_individuals_matrix.
    CalculateNewBestIndividual();
}

// Returns a D2 vector with the best fitnesses' ids per species.
// species matrix must contain current wave's Individuals. 
// Individuals must have their fitness calculated.
std::vector<std::vector<int>> Genetics::CalculateBest5FitnessIDs()
{
    // for (size_t i = 0; i < this->species.size(); i++)
    // {
    //     std::vector<int> found_ids;
    //     for (size_t j = 0; j < this->species[i].size(); j++)
    //     {
    //         // Access each Individual in a wave.
    //     }
        
    // }

    std::vector<std::vector<int>> id_matrix;

    for (size_t i = 0; i < species.size(); i++)   // Each species.
    {
        std::vector<int> found_ids;
        
        // Fills found_ids with the top five fitness values' ids of the current species.
        for (size_t j = 0; j < 5; j++)  // Five times to get five best individuals' id in order.
        {
            float current_max_fitness = -1.0f;
            int current_max_fitness_id = -1;
            for (size_t k = 0; k < species[i].size(); k++)    // Each Individual of a species.
            {
                // Access each Individual's fitness in a wave ignoring those already accessed in the top 5.

                // If in the current species, k hasn't already been added to found_ids.
                if (std::find(found_ids.begin(), found_ids.end(), k) == found_ids.end())
                {
                    if (species[i][k].fitness > current_max_fitness)
                        {
                            current_max_fitness = species[i][k].fitness;
                            current_max_fitness_id = k;
                        }
                    
                }
            }
            // If something was found. Doesn't find a new index to add if the Individuals list from species is shorter than 5.
            if (current_max_fitness_id != -1)
            {
                found_ids.push_back(current_max_fitness_id);
            }
        }
        id_matrix.push_back(found_ids);
    }
    
    return id_matrix;
    
}


// Takes the 2d matrix best_fits_ids and fills a best_individuals_matrix with corresponding data.
// Finds the best 5 Individuals of a species and places them in one of best_individuals_matrix rows.
// best_individuals_matrix must have as many rows as species and they are emptied at the start.
void Genetics::GetBest5Individuals(const std::vector<std::vector<int>>& best_fits_ids)
{
    // Clears best_individuals_matrix. Keeps the rows and empties them.
    for (size_t i = 0; i < best_individuals_matrix.size(); i++)
    {
        best_individuals_matrix[i].clear();
    }

    // Fills best_individuals_matrix with the best Individuals.
    for (size_t i = 0; i < best_fits_ids.size(); i++)
    {
        for (size_t j = 0; j < best_fits_ids[i].size(); j++)
        {
            best_individuals_matrix[i].push_back(species[i][best_fits_ids[i][j]]);
        }
    }
}

// Modifies the best_individuals list with descendants of the top 5 by fitness.
// Picks a gene from each of the top 5 and there are 5 genes.
// If there aren't 5 individuals in the best_individuals_matrix it cycles circularly until 5 genes are taken from the top indiviuals.
void Genetics::CalculateNewBestIndividual()
{   
    for (size_t i = 0; i < species.size(); i++)
    {
        // Each of the 5 genes in an Individual.
        // gene order float max_health, float speed_multiplier, float pierce_armor, float magic_armor, float siege_armor.
        best_individuals[i] = Individual(
            best_individuals_matrix[i][0 % best_individuals_matrix[i].size()].max_health,
            best_individuals_matrix[i][1 % best_individuals_matrix[i].size()].speed_multiplier,
            best_individuals_matrix[i][2 % best_individuals_matrix[i].size()].pierce_armor,
            best_individuals_matrix[i][3 % best_individuals_matrix[i].size()].magic_armor,
            best_individuals_matrix[i][4 % best_individuals_matrix[i].size()].siege_armor);
    } 
}

void Genetics::ShowBestIndividuals()
{
    for (size_t i = 0; i < best_individuals.size(); i++)
    {
        // std::cout << "Name:\t" << name << "\nScore:\t" << score << "\n" << std::endl;
        std::cout << "ShowBestIndividuals(): " << std::endl;
        std::cout << "Race:\t" << i << "\tFitness:\t" << "(average fitness not shown)" << "\n"
        << "Stats:\t"
        << best_individuals[i].max_health << ",\t" 
        << best_individuals[i].speed_multiplier << ",\t" 
        << best_individuals[i].pierce_armor << ",\t" 
        << best_individuals[i].magic_armor << ",\t"  
        << best_individuals[i].siege_armor << std::endl;
        std::cout << std::endl;
    }
    
};

void Genetics::ShowSpecies()
{
    for (size_t i = 0; i < species.size(); i++)
    {
        for (size_t j = 0; j < species[i].size(); j++)
        {
            std::cout << "(" << species[i][j].fitness << "), ";
        }
        std::cout << std::endl;
    }            
};

// {orc, NE, harpy, merc} = {1,2,3,4}
// Adds an Individual to the species 2d vector.
void Genetics::AddIndividual(int species, Individual enemy)
{
    // ClearWave() must be run before starting a new one so that there aren't different wave enemies in species 2d vector.
    this->species[species].push_back(enemy);
    // std::cout << "AddIndividual of species " << species << std::endl;

}

// Runs:
// CalculateBest5FitnessIDs() ->    uses this.species to get the ids of the top 5 fitness individuals.
// GetBest5Individuals() ->         Updates best_individuals_matrix so that it holds the best 5 of each species.
// CalculateNewBestIndividual() ->  Creates a new Individual as a pattern for the next round. Per species.
void Genetics::ClearWave()
{
    // Calculates matrix best_fits_ids with the best 5 Individuals' ids for each species.
    std::vector<std::vector<int>> best_fits_ids = CalculateBest5FitnessIDs();

    // Show species' fits.
    // ShowSpecies();

    // Show best_fits_ids.
    // std::cout << "---Show best_fits_ids.---" << std::endl;
    // for (size_t i = 0; i < best_fits_ids.size(); i++)
    // {
    //     for (size_t j = 0; j < best_fits_ids[i].size(); j++)
    //     {
    //         std::cout << best_fits_ids[i][j] << ' ';
    //     }
    //     std::cout << std::endl;
    // }

    // Updates best_individuals_matrix.
    GetBest5Individuals(best_fits_ids);
    
    // Updates best_individuals based on best_individuals_matrix.
    CalculateNewBestIndividual();

    // Empties species
    for (size_t i = 0; i < species.size(); i++)
    {
        species[i].clear();
    }
}

std::vector<float> Genetics::GetBestFits()
{
    // Calculates matrix best_fits_ids with the best 5 Individuals' ids for each species.
    std::vector<std::vector<int>> best_fits_ids = CalculateBest5FitnessIDs();

    GetBest5Individuals(best_fits_ids);
   
    std::vector<float> best_fits_per_species;
    for (size_t i = 0; i < 4; i++)
    {
        if (best_individuals_matrix[i].size() > 0)
        {
            best_fits_per_species.push_back(best_individuals_matrix[i][1].fitness);
        }
        else
        {
            best_fits_per_species.push_back(-1.f);
        }       
    }
    return best_fits_per_species;
}



















// // main funcion to test the genetic algorithm thorugh SimulateWave()
// int main(int argc, char const *argv[])
// {
//     Genetics ginny = Genetics();
    
//     std::cout << "Show best Individuals:" << std::endl;
//     ginny.ShowBestIndividuals();
//     std::cout << std::endl;

//     for (size_t i = 0; i < 12; i++)
//     {
        
//         std::cout << "Start simulation:" << std::endl;
//         ginny.SimulateWave(1);
//         std::cout << std::endl;

//         std::cout << "Show best Individuals:" << std::endl;
//         ginny.ShowBestIndividuals();
//         std::cout << std::endl;

//     }
    
//     return 0;
// }


