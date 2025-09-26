#include "Individual.h"
#include <random>
#include <algorithm>
#include <sstream>
#include <cmath>

namespace GA {
    
    // Default constructor
    Individual::Individual() : fitness(0.0), fitnessPercentage(0.0), fitnessCalculated(false) {
        // Empty chromosome, will be initialized later
    }
    
    // Constructor with chromosome length
    Individual::Individual(int chromosomeLength) : fitness(0.0), fitnessPercentage(0.0), fitnessCalculated(false) {
        chromosome.resize(chromosomeLength, false);
    }
    
    // Constructor with existing chromosome
    Individual::Individual(const Chromosome& chromosome) 
        : chromosome(chromosome), fitness(0.0), fitnessPercentage(0.0), fitnessCalculated(false) {
    }
    
    // Copy constructor
    Individual::Individual(const Individual& other) 
        : chromosome(other.chromosome), fitness(other.fitness), fitnessPercentage(other.fitnessPercentage), fitnessCalculated(other.fitnessCalculated) {
    }
    
    // Assignment operator
    Individual& Individual::operator=(const Individual& other) {
        if (this != &other) {
            chromosome = other.chromosome;
            fitness = other.fitness;
            fitnessPercentage = other.fitnessPercentage;
            fitnessCalculated = other.fitnessCalculated;
        }
        return *this;
    }
    
    // Single-point crossover implementation
    std::pair<Individual, Individual> Individual::singlePointCrossover(const Individual& other, int crossoverPoint) const {
        // Ensure crossover point is valid
        if (crossoverPoint < 0 || crossoverPoint >= static_cast<int>(chromosome.size())) {
            throw std::invalid_argument("Crossover point is out of bounds");
        }
        
        // Create offspring chromosomes
        Chromosome offspring1 = chromosome;
        Chromosome offspring2 = other.chromosome;
        
        // Perform crossover: swap segments after crossover point
        for (size_t i = crossoverPoint; i < chromosome.size(); ++i) {
            offspring1[i] = other.chromosome[i];
            offspring2[i] = chromosome[i];
        }
        
        // Create and return offspring individuals
        Individual child1(offspring1);
        Individual child2(offspring2);
        
        return std::make_pair(child1, child2);
    }
    
    // Two-point crossover implementation
    std::pair<Individual, Individual> Individual::twoPointCrossover(const Individual& other, int point1, int point2) const {
        // Ensure crossover points are valid and in order
        if (point1 < 0 || point2 >= static_cast<int>(chromosome.size()) || point1 >= point2) {
            throw std::invalid_argument("Invalid crossover points");
        }
        
        // Create offspring chromosomes
        Chromosome offspring1 = chromosome;
        Chromosome offspring2 = other.chromosome;
        
        // Perform crossover: swap segment between the two points
        for (int i = point1; i <= point2; ++i) {
            offspring1[i] = other.chromosome[i];
            offspring2[i] = chromosome[i];
        }
        
        // Create and return offspring individuals
        Individual child1(offspring1);
        Individual child2(offspring2);
        
        return std::make_pair(child1, child2);
    }
    
    // Uniform crossover implementation
    std::pair<Individual, Individual> Individual::uniformCrossover(const Individual& other, const Chromosome& mask) const {
        if (mask.size() != chromosome.size() || mask.size() != other.chromosome.size()) {
            throw std::invalid_argument("Mask size does not match chromosome size");
        }
        
        // Create offspring chromosomes
        Chromosome offspring1(chromosome.size());
        Chromosome offspring2(chromosome.size());
        
        // Perform uniform crossover based on mask
        for (size_t i = 0; i < chromosome.size(); ++i) {
            if (mask[i]) {
                // Take from first parent for offspring1, second parent for offspring2
                offspring1[i] = chromosome[i];
                offspring2[i] = other.chromosome[i];
            } else {
                // Take from second parent for offspring1, first parent for offspring2
                offspring1[i] = other.chromosome[i];
                offspring2[i] = chromosome[i];
            }
        }
        
        // Create and return offspring individuals
        Individual child1(offspring1);
        Individual child2(offspring2);
        
        return std::make_pair(child1, child2);
    }
    
    // Bit-flip mutation implementation
    void Individual::mutate(double mutationRate, std::mt19937& rng) {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        
        // Apply mutation to each bit based on mutation rate
        for (size_t i = 0; i < chromosome.size(); ++i) {
            if (dist(rng) < mutationRate) {
                chromosome[i] = !chromosome[i];  // Flip the bit
            }
        }
        
        // Invalidate fitness since chromosome has changed
        invalidateFitness();
    }
    
    // Fitness management methods
    void Individual::setFitness(FitnessValue fitnessValue) {
        fitness = fitnessValue;
        fitnessCalculated = true;
    }
    
    FitnessValue Individual::getFitness() const {
        if (!fitnessCalculated) {
            throw std::runtime_error("Fitness has not been calculated yet");
        }
        return fitness;
    }
    
    bool Individual::isFitnessValid() const {
        return fitnessCalculated;
    }
    
    void Individual::invalidateFitness() {
        fitnessCalculated = false;
        fitnessPercentage = 0.0; // Reset percentage when fitness is invalidated
    }
    
    void Individual::setFitnessPercentage(double percentage) {
        fitnessPercentage = std::max(0.0, std::min(100.0, percentage)); // Clamp to 0-100%
    }
    
    double Individual::getFitnessPercentage() const {
        return fitnessPercentage;
    }
    
    // Chromosome access methods
    const Chromosome& Individual::getChromosome() const {
        return chromosome;
    }
    
    void Individual::setChromosome(const Chromosome& newChromosome) {
        chromosome = newChromosome;
        invalidateFitness();  // Fitness needs recalculation
    }
    
    size_t Individual::getChromosomeLength() const {
        return chromosome.size();
    }
    
    // Binary to decimal conversion
    double Individual::decodeToValue(double minValue, double maxValue) const {
        if (chromosome.empty()) {
            return minValue;
        }
        
        // Convert binary chromosome to decimal
        uint64_t binaryValue = 0;
        for (size_t i = 0; i < chromosome.size(); ++i) {
            if (chromosome[i]) {
                binaryValue |= (1ULL << (chromosome.size() - 1 - i));
            }
        }
        
        // Calculate maximum possible value for this chromosome length
        uint64_t maxBinaryValue = (1ULL << chromosome.size()) - 1;
        
        // Scale to the desired range [minValue, maxValue]
        double scaledValue = minValue + (static_cast<double>(binaryValue) / maxBinaryValue) * (maxValue - minValue);
        
        return scaledValue;
    }
    
    // Convert chromosome to string representation
    std::string Individual::toString() const {
        std::stringstream ss;
        for (bool bit : chromosome) {
            ss << (bit ? '1' : '0');
        }
        return ss.str();
    }
    
    // Generate random chromosome
    void Individual::randomize(int length, std::mt19937& rng) {
        chromosome.resize(length);
        std::uniform_int_distribution<int> dist(0, 1);
        
        for (int i = 0; i < length; ++i) {
            chromosome[i] = (dist(rng) == 1);
        }
        
        invalidateFitness();  // New random chromosome needs fitness evaluation
    }
    
    // Comparison operators for sorting
    bool Individual::operator<(const Individual& other) const {
        if (!fitnessCalculated || !other.fitnessCalculated) {
            throw std::runtime_error("Cannot compare individuals without calculated fitness");
        }
        return fitness < other.fitness;
    }
    
    bool Individual::operator>(const Individual& other) const {
        if (!fitnessCalculated || !other.fitnessCalculated) {
            throw std::runtime_error("Cannot compare individuals without calculated fitness");
        }
        return fitness > other.fitness;
    }
    
    bool Individual::operator==(const Individual& other) const {
        if (!fitnessCalculated || !other.fitnessCalculated) {
            throw std::runtime_error("Cannot compare individuals without calculated fitness");
        }
        return std::abs(fitness - other.fitness) < 1e-10;  // Use epsilon for floating point comparison
    }
}