#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include "GeneticTypes.h"
#include "Individual.h"
#include "FitnessFunction.h"
#include <vector>
#include <random>
#include <memory>
#include <functional>

namespace GA {
    /**
     * @class GeneticAlgorithm
     * @brief Main genetic algorithm implementation
     * 
     * This class implements a complete genetic algorithm with support for
     * various selection, crossover, and mutation strategies. It provides
     * detailed tracking of generations and statistics.
     */
    class GeneticAlgorithm {
    private:
        GAConfig config;                           // Algorithm configuration
        Population population;                     // Current population
        std::unique_ptr<FitnessFunction> fitnessFunction;  // Fitness evaluation function
        std::mt19937 rng;                         // Random number generator
        std::vector<GenerationStats> statistics;  // Statistics for each generation
        int currentGeneration;                    // Current generation number
        
        // Progress callback function type
        using ProgressCallback = std::function<void(int, const GenerationStats&)>;
        ProgressCallback progressCallback;
        
    public:
        // Constructors
        GeneticAlgorithm(const GAConfig& config, std::unique_ptr<FitnessFunction> fitnessFunc);
        
        // Destructor
        ~GeneticAlgorithm() = default;
        
        // Main algorithm execution
        /**
         * @brief Runs the genetic algorithm
         * @return Statistics of the final generation
         */
        GenerationStats run();
        
        /**
         * @brief Runs the genetic algorithm with progress callback
         * @param callback Function called after each generation
         * @return Statistics of the final generation
         */
        GenerationStats run(ProgressCallback callback);
        
        // Population management
        /**
         * @brief Initializes the population with random individuals
         */
        void initializePopulation();
        
        /**
         * @brief Evaluates fitness for all individuals in the population
         */
        void evaluatePopulation();
        
        /**
         * @brief Performs one generation of the genetic algorithm
         * @return Statistics for the current generation
         */
        GenerationStats evolveGeneration();
        
        // Selection methods
        /**
         * @brief Performs tournament selection
         * @param tournamentSize Number of individuals in tournament
         * @return Selected individual
         */
        Individual tournamentSelection(int tournamentSize);
        
        /**
         * @brief Performs roulette wheel selection
         * @return Selected individual
         */
        Individual rouletteWheelSelection();
        
        /**
         * @brief Selects elite individuals from population
         * @param eliteCount Number of elite individuals to select
         * @return Vector of elite individuals
         */
        std::vector<Individual> eliteSelection(int eliteCount);
        
        // Crossover methods
        /**
         * @brief Performs crossover between two parents
         * @param parent1 First parent
         * @param parent2 Second parent
         * @return Pair of offspring
         */
        std::pair<Individual, Individual> performCrossover(const Individual& parent1, const Individual& parent2);
        
        // Mutation methods
        /**
         * @brief Performs mutation on an individual
         * @param individual Individual to mutate
         */
        void performMutation(Individual& individual);
        
        // Statistics and analysis
        /**
         * @brief Calculates statistics for the current generation
         * @return GenerationStats object with current statistics
         */
        GenerationStats calculateGenerationStats();
        
        /**
         * @brief Gets the best individual from current population
         * @return The individual with best fitness
         */
        Individual getBestIndividual() const;
        
        /**
         * @brief Gets the worst individual from current population
         * @return The individual with worst fitness
         */
        Individual getWorstIndividual() const;
        
        /**
         * @brief Calculates population diversity (convergence measure)
         * @return Diversity value (0 = fully converged, 1 = maximum diversity)
         */
        double calculatePopulationDiversity() const;
        
        // Getters and setters
        /**
         * @brief Gets the current population
         * @return Const reference to the population
         */
        const Population& getPopulation() const;
        
        /**
         * @brief Gets the algorithm configuration
         * @return Const reference to the configuration
         */
        const GAConfig& getConfig() const;
        
        /**
         * @brief Gets all generation statistics
         * @return Vector of generation statistics
         */
        const std::vector<GenerationStats>& getStatistics() const;
        
        /**
         * @brief Gets the current generation number
         * @return Current generation number
         */
        int getCurrentGeneration() const;
        
        /**
         * @brief Gets the fitness function
         * @return Reference to the fitness function
         */
        const FitnessFunction& getFitnessFunction() const;
        
        // Utility methods
        /**
         * @brief Sorts the population by fitness (best first)
         */
        void sortPopulationByFitness();
        
        /**
         * @brief Checks if the algorithm has converged
         * @param threshold Convergence threshold (diversity below this value)
         * @return True if converged, false otherwise
         */
        bool hasConverged(double threshold = 0.01) const;
        
        /**
         * @brief Resets the algorithm to initial state
         */
        void reset();
        
        /**
         * @brief Sets a new fitness function
         * @param newFitnessFunction New fitness function to use
         */
        void setFitnessFunction(std::unique_ptr<FitnessFunction> newFitnessFunction);
        
        /**
         * @brief Updates algorithm configuration
         * @param newConfig New configuration to use
         */
        void updateConfig(const GAConfig& newConfig);
    };
}

#endif // GENETIC_ALGORITHM_H