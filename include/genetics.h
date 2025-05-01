#ifndef GENETICS_H
#define GENETICS_H

// #include <vector>
#include <string>
#include <cstring>
// #include <cstdint>



bool RandomBool(double probability);

















class Individual
{
    public:
        // stats:
        float mutation_relative_change; // This is used only for the testing function.
        
        float fitness;

        float max_health;
        // float health;
        float speed;
        float pierce_armor;
        float magic_armor;
        float siege_armor;

        // guide:
        // float mutation_relative_change = 0.2f;
        // float health = 100;
        // float speed = 10;
        // float pierce_armor = 1;
        // float magic_armor = 1;
        // float siege_armor = 1;

        // Test Constructor. Actual constructor requires CalculateFitness(float completed_path, float remaining_health).
        // Modify this constructor so that it receives an enemy unit object. Individual(Enemy enemy){}
        // The constructor calls CalculateFitness().
        Individual(float max_health, float speed, float pierce_armor, float magic_armor, float siege_armor);

        // Default constructor gives -1.0f to all attributes.
        Individual();

        void CalculateFitness();
        
        // Testing only function.
        // Increases or decreases each stat by mutation_relative_change and recalculates the fitness.
        // e. g.: stat *= 1.2; OR stat *= 0.8;
        // This is called by the testing function Genetics.SimulateWave()
        void MutateStats();
        

        
        // void Show();


};

























class Genetics
{
    public:
        // Vector of vectors of Individuals. Each vector<Individual> holds the Individual's performance of the current round.
        // The whole vector<vector> holds the performances from a whole wave.
        std::vector<std::vector<Individual>> species; 
        
        // The current best genome of each species. 
        std::vector<Individual> best_individuals;

        // A list of the top 5 Individuals from the current wave.
        // Managed by GetBest5Individuals(const std::vector<std::vector<int>>& best_fits_ids)
        std::vector<std::vector<Individual>> best_individuals_matrix;

        /* Order of species and spawn ratios:
            0 Orcs, 10
            1 Night elves, 15
            2 Harpies, 7
            3 Mercenaries, 7
        */

        // Constructor initializes best_individuals with their start value for the game.
        // guide:
        // float mutation_relative_change = 0.2f;
        // float health = 100;
        // float speed = 10;
        // float pierce_armor = 1;
        // float magic_armor = 1;
        // float siege_armor = 1;
        Genetics();

        void SimulateWave(int n);

        // Returns a D2 vector with the best fitnesses' ids per species.
        // species matrix must contain current wave's Individuals. 
        std::vector<std::vector<int>> CalculateBest5FitnessIDs();


        // Takes the 2d matrix best_fits_ids and fills a best_individuals_matrix with corresponding data.
        // best_individuals_matrix must have as many rows as species and they are emptied at the start.
        void GetBest5Individuals(const std::vector<std::vector<int>>& best_fits_ids);

        // Modifies the best_individuals list with descendants of the top 5.
        // Picks a gene from each of the top five and there are 5 genes.
        // If there aren't 5 individuals in the best_individuals_matrix it cycles circularly until 5 genes are taken from the top 5.
        void CalculateNewBestIndividual();

        void ShowBestIndividuals();

        void ShowSpecies();
};






















#endif