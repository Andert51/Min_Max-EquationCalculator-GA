#ifndef GENETIC_TYPES_H
#define GENETIC_TYPES_H

#include <vector>
#include <string>
#include <functional>

namespace GA {
    // Type definitions for genetic algorithm components
    using Chromosome = std::vector<bool>;  // Binary representation of an individual
    using FitnessValue = double;
    using Population = std::vector<class Individual>;
    
    // Enumeration for selection strategies
    enum class SelectionType {
        ROULETTE_WHEEL,
        TOURNAMENT,
        ELITISM
    };
    
    // Enumeration for crossover strategies  
    enum class CrossoverType {
        SINGLE_POINT,
        TWO_POINT,
        UNIFORM
    };
    
    // Configuration structure for genetic algorithm parameters
    struct GAConfig {
        int populationSize = 50;        // Number of individuals in population
        int chromosomeLength = 20;      // Length of binary chromosome
        int maxGenerations = 100;       // Maximum number of generations
        double crossoverRate = 0.8;     // Probability of crossover (80%)
        double mutationRate = 0.01;     // Probability of mutation per bit (1%)
        double elitismRate = 0.1;       // Percentage of best individuals to preserve (10%)
        bool isMaximization = true;     // True for maximization, false for minimization
        SelectionType selectionType = SelectionType::TOURNAMENT;
        CrossoverType crossoverType = CrossoverType::SINGLE_POINT;
        int tournamentSize = 3;         // Size of tournament for tournament selection
        
        // Function domain parameters
        double minValue = -10.0;        // Minimum value of the function domain
        double maxValue = 10.0;         // Maximum value of the function domain
    };
    
    // Statistics structure to track algorithm performance
    struct GenerationStats {
        int generation;
        FitnessValue bestFitness;
        FitnessValue averageFitness;
        FitnessValue worstFitness;
        double bestValue;               // Decoded value of best individual
        double convergence;             // Measure of population diversity
        double bestFitnessPercentage;   // Best fitness as percentage
        double averageFitnessPercentage; // Average fitness as percentage
    };
}

#endif // GENETIC_TYPES_H