#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "GeneticTypes.h"
#include <vector>
#include <string>
#include <random>

namespace GA {
    /**
     * @class Individual
     * @brief Represents a single individual in the genetic algorithm population
     * 
     * This class encapsulates a chromosome (binary string) and provides methods
     * for genetic operations such as mutation, crossover, and fitness evaluation.
     */
    class Individual {
    private:
        Chromosome chromosome;          // Binary representation of the individual
        FitnessValue fitness;          // Cached fitness value (raw)
        double fitnessPercentage;      // Fitness as percentage (0-100%)
        bool fitnessCalculated;        // Flag to track if fitness needs recalculation
        
    public:
        // Constructors
        Individual();
        Individual(int chromosomeLength);
        Individual(const Chromosome& chromosome);
        
        // Destructor
        ~Individual() = default;
        
        // Copy constructor and assignment operator
        Individual(const Individual& other);
        Individual& operator=(const Individual& other);
        
        // Genetic operations
        /**
         * @brief Performs single-point crossover with another individual
         * @param other The other parent individual
         * @param crossoverPoint The point where crossover occurs
         * @return Pair of offspring individuals
         */
        std::pair<Individual, Individual> singlePointCrossover(const Individual& other, int crossoverPoint) const;
        
        /**
         * @brief Performs two-point crossover with another individual
         * @param other The other parent individual
         * @param point1 First crossover point
         * @param point2 Second crossover point
         * @return Pair of offspring individuals
         */
        std::pair<Individual, Individual> twoPointCrossover(const Individual& other, int point1, int point2) const;
        
        /**
         * @brief Performs uniform crossover with another individual
         * @param other The other parent individual
         * @param mask Binary mask determining which parent contributes each bit
         * @return Pair of offspring individuals
         */
        std::pair<Individual, Individual> uniformCrossover(const Individual& other, const Chromosome& mask) const;
        
        /**
         * @brief Performs bit-flip mutation on the chromosome
         * @param mutationRate Probability of each bit being mutated
         * @param rng Random number generator
         */
        void mutate(double mutationRate, std::mt19937& rng);
        
        // Fitness and evaluation
        /**
         * @brief Sets the fitness value for this individual
         * @param fitnessValue The computed fitness value
         */
        void setFitness(FitnessValue fitnessValue);
        
        /**
         * @brief Gets the fitness value (must be calculated externally first)
         * @return The fitness value
         */
        FitnessValue getFitness() const;
        
        /**
         * @brief Checks if fitness has been calculated
         * @return True if fitness is valid, false otherwise
         */
        bool isFitnessValid() const;
        
        /**
         * @brief Invalidates the fitness (call after genetic operations)
         */
        void invalidateFitness();
        
        /**
         * @brief Sets the fitness percentage for this individual
         * @param percentage The fitness as percentage (0-100%)
         */
        void setFitnessPercentage(double percentage);
        
        /**
         * @brief Gets the fitness percentage
         * @return The fitness as percentage (0-100%)
         */
        double getFitnessPercentage() const;
        
        // Chromosome access and manipulation
        /**
         * @brief Gets the chromosome (read-only)
         * @return Const reference to the chromosome
         */
        const Chromosome& getChromosome() const;
        
        /**
         * @brief Sets the chromosome
         * @param newChromosome The new chromosome to set
         */
        void setChromosome(const Chromosome& newChromosome);
        
        /**
         * @brief Gets the length of the chromosome
         * @return Size of the chromosome
         */
        size_t getChromosomeLength() const;
        
        // Utility methods
        /**
         * @brief Converts the binary chromosome to a decimal value
         * @param minValue Minimum value of the domain
         * @param maxValue Maximum value of the domain
         * @return Decoded decimal value
         */
        double decodeToValue(double minValue, double maxValue) const;
        
        /**
         * @brief Converts the chromosome to a binary string representation
         * @return String representation of the chromosome
         */
        std::string toString() const;
        
        /**
         * @brief Generates a random chromosome
         * @param length Length of the chromosome to generate
         * @param rng Random number generator
         */
        void randomize(int length, std::mt19937& rng);
        
        // Comparison operators for sorting by fitness
        bool operator<(const Individual& other) const;
        bool operator>(const Individual& other) const;
        bool operator==(const Individual& other) const;
    };
}

#endif // INDIVIDUAL_H