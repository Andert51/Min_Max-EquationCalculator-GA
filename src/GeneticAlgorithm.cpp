#include "GeneticAlgorithm.h"
#include <algorithm>
#include <numeric>
#include <random>
#include <stdexcept>
#include <cmath>

namespace GA {
    
    // Constructor
    GeneticAlgorithm::GeneticAlgorithm(const GAConfig& config, std::unique_ptr<FitnessFunction> fitnessFunc)
        : config(config), fitnessFunction(std::move(fitnessFunc)), currentGeneration(0) {
        
        // Initialize random number generator with a random seed
        std::random_device rd;
        rng.seed(rd());
        
        // Validate configuration
        if (config.populationSize <= 0 || config.chromosomeLength <= 0 || config.maxGenerations <= 0) {
            throw std::invalid_argument("Invalid GA configuration parameters");
        }
        
        if (!this->fitnessFunction) {
            throw std::invalid_argument("Fitness function cannot be null");
        }
    }
    
    // Main algorithm execution
    GenerationStats GeneticAlgorithm::run() {
        // Initialize the algorithm
        initializePopulation();
        evaluatePopulation();
        
        // Store initial statistics
        statistics.clear();
        statistics.push_back(calculateGenerationStats());
        
        // Evolution loop
        for (currentGeneration = 1; currentGeneration <= config.maxGenerations; ++currentGeneration) {
            GenerationStats stats = evolveGeneration();
            statistics.push_back(stats);
            
            // Check for convergence
            if (hasConverged()) {
                break;
            }
        }
        
        return statistics.back();
    }
    
    // Run with progress callback
    GenerationStats GeneticAlgorithm::run(ProgressCallback callback) {
        progressCallback = callback;
        
        // Initialize the algorithm
        initializePopulation();
        evaluatePopulation();
        
        // Store initial statistics
        statistics.clear();
        GenerationStats initialStats = calculateGenerationStats();
        statistics.push_back(initialStats);
        
        // Call progress callback for initial generation
        if (progressCallback) {
            progressCallback(0, initialStats);
        }
        
        // Evolution loop
        for (currentGeneration = 1; currentGeneration <= config.maxGenerations; ++currentGeneration) {
            GenerationStats stats = evolveGeneration();
            statistics.push_back(stats);
            
            // Call progress callback
            if (progressCallback) {
                progressCallback(currentGeneration, stats);
            }
            
            // Check for convergence
            if (hasConverged()) {
                break;
            }
        }
        
        return statistics.back();
    }
    
    // Initialize population with random individuals
    void GeneticAlgorithm::initializePopulation() {
        population.clear();
        population.reserve(config.populationSize);
        
        // Create random individuals
        for (int i = 0; i < config.populationSize; ++i) {
            Individual individual(config.chromosomeLength);
            individual.randomize(config.chromosomeLength, rng);
            population.push_back(individual);
        }
    }
    
    // Evaluate fitness for all individuals
    void GeneticAlgorithm::evaluatePopulation() {
        // First pass: calculate raw fitness values
        for (Individual& individual : population) {
            if (!individual.isFitnessValid()) {
                FitnessValue fitness = fitnessFunction->evaluateIndividual(individual, config.minValue, config.maxValue);
                individual.setFitness(fitness);
            }
        }
        
        // Second pass: calculate fitness percentages
        if (!population.empty()) {
            // Find best and worst fitness values in current population
            double bestFitness = population[0].getFitness();
            double worstFitness = population[0].getFitness();
            
            for (const Individual& individual : population) {
                double fitness = individual.getFitness();
                if (config.isMaximization) {
                    bestFitness = std::max(bestFitness, fitness);
                    worstFitness = std::min(worstFitness, fitness);
                } else {
                    bestFitness = std::min(bestFitness, fitness);
                    worstFitness = std::max(worstFitness, fitness);
                }
            }
            
            // Calculate percentage for each individual
            for (Individual& individual : population) {
                double fitnessValue = individual.getFitness();
                double percentage = fitnessFunction->calculateFitnessPercentage(fitnessValue, bestFitness, worstFitness);
                individual.setFitnessPercentage(percentage);
            }
        }
    }
    
    // Perform one generation of evolution
    GenerationStats GeneticAlgorithm::evolveGeneration() {
        Population newPopulation;
        newPopulation.reserve(config.populationSize);
        
        // Sort population by fitness (best first)
        sortPopulationByFitness();
        
        // Apply elitism - keep best individuals
        int eliteCount = static_cast<int>(config.populationSize * config.elitismRate);
        std::vector<Individual> elites = eliteSelection(eliteCount);
        
        // Add elite individuals to new population
        for (const Individual& elite : elites) {
            newPopulation.push_back(elite);
        }
        
        // Generate offspring to fill the rest of the population
        while (newPopulation.size() < static_cast<size_t>(config.populationSize)) {
            // Select parents based on selection strategy
            Individual parent1, parent2;
            
            switch (config.selectionType) {
                case SelectionType::TOURNAMENT:
                    parent1 = tournamentSelection(config.tournamentSize);
                    parent2 = tournamentSelection(config.tournamentSize);
                    break;
                    
                case SelectionType::ROULETTE_WHEEL:
                    parent1 = rouletteWheelSelection();
                    parent2 = rouletteWheelSelection();
                    break;
                    
                case SelectionType::ELITISM:
                    // For elitism selection, select from top performers
                    {
                        std::uniform_int_distribution<int> dist(0, std::min(eliteCount * 2, config.populationSize - 1));
                        parent1 = population[dist(rng)];
                        parent2 = population[dist(rng)];
                    }
                    break;
            }
            
            // Apply crossover
            std::uniform_real_distribution<double> crossoverDist(0.0, 1.0);
            if (crossoverDist(rng) < config.crossoverRate) {
                auto offspring = performCrossover(parent1, parent2);
                
                // Apply mutation to offspring
                performMutation(offspring.first);
                performMutation(offspring.second);
                
                // Add offspring to new population (if there's space)
                if (newPopulation.size() < static_cast<size_t>(config.populationSize)) {
                    newPopulation.push_back(offspring.first);
                }
                if (newPopulation.size() < static_cast<size_t>(config.populationSize)) {
                    newPopulation.push_back(offspring.second);
                }
            } else {
                // No crossover, just copy parents with potential mutation
                Individual child1 = parent1;
                Individual child2 = parent2;
                
                performMutation(child1);
                performMutation(child2);
                
                if (newPopulation.size() < static_cast<size_t>(config.populationSize)) {
                    newPopulation.push_back(child1);
                }
                if (newPopulation.size() < static_cast<size_t>(config.populationSize)) {
                    newPopulation.push_back(child2);
                }
            }
        }
        
        // Replace old population with new population
        population = std::move(newPopulation);
        
        // Evaluate new population
        evaluatePopulation();
        
        // Calculate and return statistics for this generation
        return calculateGenerationStats();
    }
    
    // Tournament selection implementation
    Individual GeneticAlgorithm::tournamentSelection(int tournamentSize) {
        if (tournamentSize <= 0 || tournamentSize > config.populationSize) {
            tournamentSize = std::min(3, config.populationSize);  // Default tournament size
        }
        
        std::uniform_int_distribution<int> dist(0, config.populationSize - 1);
        
        // Select random individuals for tournament
        Individual best = population[dist(rng)];
        
        for (int i = 1; i < tournamentSize; ++i) {
            Individual competitor = population[dist(rng)];
            
            // Select better individual based on maximization/minimization
            if (config.isMaximization) {
                if (competitor.getFitness() > best.getFitness()) {
                    best = competitor;
                }
            } else {
                if (competitor.getFitness() < best.getFitness()) {
                    best = competitor;
                }
            }
        }
        
        return best;
    }
    
    // Roulette wheel selection implementation
    Individual GeneticAlgorithm::rouletteWheelSelection() {
        // Calculate total fitness
        double totalFitness = 0.0;
        double minFitness = std::numeric_limits<double>::max();
        
        // Find minimum fitness for offset (in case of negative fitness values)
        for (const Individual& individual : population) {
            double fitness = individual.getFitness();
            if (fitness < minFitness) {
                minFitness = fitness;
            }
        }
        
        // Offset fitness values to make them positive (add 1 to avoid zero)
        double offset = (minFitness < 0) ? -minFitness + 1.0 : 0.0;
        
        for (const Individual& individual : population) {
            totalFitness += individual.getFitness() + offset;
        }
        
        // For minimization problems, invert the fitness
        if (!config.isMaximization) {
            double maxAdjustedFitness = 0.0;
            for (const Individual& individual : population) {
                double adjustedFitness = individual.getFitness() + offset;
                if (adjustedFitness > maxAdjustedFitness) {
                    maxAdjustedFitness = adjustedFitness;
                }
            }
            
            // Recalculate total with inverted fitness
            totalFitness = 0.0;
            for (const Individual& individual : population) {
                double invertedFitness = maxAdjustedFitness - (individual.getFitness() + offset);
                totalFitness += invertedFitness;
            }
        }
        
        // Generate random selection point
        std::uniform_real_distribution<double> dist(0.0, totalFitness);
        double selectionPoint = dist(rng);
        
        // Select individual based on cumulative fitness
        double cumulativeFitness = 0.0;
        for (const Individual& individual : population) {
            double fitness;
            if (config.isMaximization) {
                fitness = individual.getFitness() + offset;
            } else {
                double adjustedFitness = individual.getFitness() + offset;
                double maxAdjustedFitness = 0.0;
                for (const Individual& ind : population) {
                    double adjFit = ind.getFitness() + offset;
                    if (adjFit > maxAdjustedFitness) {
                        maxAdjustedFitness = adjFit;
                    }
                }
                fitness = maxAdjustedFitness - adjustedFitness;
            }
            
            cumulativeFitness += fitness;
            if (cumulativeFitness >= selectionPoint) {
                return individual;
            }
        }
        
        // Fallback: return last individual
        return population.back();
    }
    
    // Elite selection implementation
    std::vector<Individual> GeneticAlgorithm::eliteSelection(int eliteCount) {
        if (eliteCount <= 0 || eliteCount > config.populationSize) {
            eliteCount = std::max(1, static_cast<int>(config.populationSize * 0.1));  // Default 10%
        }
        
        // Sort population by fitness
        sortPopulationByFitness();
        
        // Select top individuals
        std::vector<Individual> elites;
        elites.reserve(eliteCount);
        
        for (int i = 0; i < eliteCount && i < config.populationSize; ++i) {
            elites.push_back(population[i]);
        }
        
        return elites;
    }
    
    // Crossover operation
    std::pair<Individual, Individual> GeneticAlgorithm::performCrossover(const Individual& parent1, const Individual& parent2) {
        std::uniform_int_distribution<int> pointDist(1, config.chromosomeLength - 1);
        
        switch (config.crossoverType) {
            case CrossoverType::SINGLE_POINT: {
                int crossoverPoint = pointDist(rng);
                return parent1.singlePointCrossover(parent2, crossoverPoint);
            }
            
            case CrossoverType::TWO_POINT: {
                int point1 = pointDist(rng);
                int point2 = pointDist(rng);
                if (point1 > point2) {
                    std::swap(point1, point2);
                }
                return parent1.twoPointCrossover(parent2, point1, point2);
            }
            
            case CrossoverType::UNIFORM: {
                // Generate random mask
                Chromosome mask(config.chromosomeLength);
                std::uniform_int_distribution<int> maskDist(0, 1);
                for (int i = 0; i < config.chromosomeLength; ++i) {
                    mask[i] = (maskDist(rng) == 1);
                }
                return parent1.uniformCrossover(parent2, mask);
            }
            
            default:
                throw std::invalid_argument("Unknown crossover type");
        }
    }
    
    // Mutation operation
    void GeneticAlgorithm::performMutation(Individual& individual) {
        individual.mutate(config.mutationRate, rng);
    }
    
    // Calculate generation statistics
    GenerationStats GeneticAlgorithm::calculateGenerationStats() {
        if (population.empty()) {
            throw std::runtime_error("Cannot calculate statistics for empty population");
        }
        
        GenerationStats stats;
        stats.generation = currentGeneration;
        
        // Find best and worst fitness
        double totalFitness = 0.0;
        stats.bestFitness = population[0].getFitness();
        stats.worstFitness = population[0].getFitness();
        
        for (const Individual& individual : population) {
            double fitness = individual.getFitness();
            totalFitness += fitness;
            
            if (config.isMaximization) {
                if (fitness > stats.bestFitness) {
                    stats.bestFitness = fitness;
                    stats.bestValue = individual.decodeToValue(config.minValue, config.maxValue);
                }
                if (fitness < stats.worstFitness) {
                    stats.worstFitness = fitness;
                }
            } else {
                if (fitness < stats.bestFitness) {
                    stats.bestFitness = fitness;
                    stats.bestValue = individual.decodeToValue(config.minValue, config.maxValue);
                }
                if (fitness > stats.worstFitness) {
                    stats.worstFitness = fitness;
                }
            }
        }
        
        stats.averageFitness = totalFitness / population.size();
        stats.convergence = calculatePopulationDiversity();
        
        // Calculate fitness percentages
        double totalPercentage = 0.0;
        stats.bestFitnessPercentage = 0.0;
        
        for (const Individual& individual : population) {
            double percentage = individual.getFitnessPercentage();
            totalPercentage += percentage;
            
            // Find best percentage (highest is always best regardless of min/max problem)
            if (percentage > stats.bestFitnessPercentage) {
                stats.bestFitnessPercentage = percentage;
            }
        }
        
        stats.averageFitnessPercentage = totalPercentage / population.size();
        
        return stats;
    }
    
    // Get best individual
    Individual GeneticAlgorithm::getBestIndividual() const {
        if (population.empty()) {
            throw std::runtime_error("Population is empty");
        }
        
        Individual best = population[0];
        for (const Individual& individual : population) {
            if (config.isMaximization) {
                if (individual.getFitness() > best.getFitness()) {
                    best = individual;
                }
            } else {
                if (individual.getFitness() < best.getFitness()) {
                    best = individual;
                }
            }
        }
        
        return best;
    }
    
    // Get worst individual
    Individual GeneticAlgorithm::getWorstIndividual() const {
        if (population.empty()) {
            throw std::runtime_error("Population is empty");
        }
        
        Individual worst = population[0];
        for (const Individual& individual : population) {
            if (config.isMaximization) {
                if (individual.getFitness() < worst.getFitness()) {
                    worst = individual;
                }
            } else {
                if (individual.getFitness() > worst.getFitness()) {
                    worst = individual;
                }
            }
        }
        
        return worst;
    }
    
    // Calculate population diversity
    double GeneticAlgorithm::calculatePopulationDiversity() const {
        if (population.size() < 2) {
            return 0.0;
        }
        
        // Calculate average Hamming distance between all pairs of individuals
        double totalDistance = 0.0;
        int comparisons = 0;
        
        for (size_t i = 0; i < population.size(); ++i) {
            for (size_t j = i + 1; j < population.size(); ++j) {
                const Chromosome& chrom1 = population[i].getChromosome();
                const Chromosome& chrom2 = population[j].getChromosome();
                
                int hammingDistance = 0;
                for (size_t k = 0; k < chrom1.size(); ++k) {
                    if (chrom1[k] != chrom2[k]) {
                        hammingDistance++;
                    }
                }
                
                totalDistance += static_cast<double>(hammingDistance) / chrom1.size();
                comparisons++;
            }
        }
        
        return (comparisons > 0) ? totalDistance / comparisons : 0.0;
    }
    
    // Sort population by fitness
    void GeneticAlgorithm::sortPopulationByFitness() {
        if (config.isMaximization) {
            // Sort in descending order for maximization (best first)
            std::sort(population.begin(), population.end(), 
                     [](const Individual& a, const Individual& b) {
                         return a.getFitness() > b.getFitness();
                     });
        } else {
            // Sort in ascending order for minimization (best first)
            std::sort(population.begin(), population.end(), 
                     [](const Individual& a, const Individual& b) {
                         return a.getFitness() < b.getFitness();
                     });
        }
    }
    
    // Check convergence
    bool GeneticAlgorithm::hasConverged(double threshold) const {
        return calculatePopulationDiversity() < threshold;
    }
    
    // Reset algorithm
    void GeneticAlgorithm::reset() {
        population.clear();
        statistics.clear();
        currentGeneration = 0;
    }
    
    // Getters
    const Population& GeneticAlgorithm::getPopulation() const {
        return population;
    }
    
    const GAConfig& GeneticAlgorithm::getConfig() const {
        return config;
    }
    
    const std::vector<GenerationStats>& GeneticAlgorithm::getStatistics() const {
        return statistics;
    }
    
    int GeneticAlgorithm::getCurrentGeneration() const {
        return currentGeneration;
    }
    
    const FitnessFunction& GeneticAlgorithm::getFitnessFunction() const {
        return *fitnessFunction;
    }
    
    // Setters
    void GeneticAlgorithm::setFitnessFunction(std::unique_ptr<FitnessFunction> newFitnessFunction) {
        if (!newFitnessFunction) {
            throw std::invalid_argument("Fitness function cannot be null");
        }
        fitnessFunction = std::move(newFitnessFunction);
        
        // Invalidate all fitness values in current population
        for (Individual& individual : population) {
            individual.invalidateFitness();
        }
    }
    
    void GeneticAlgorithm::updateConfig(const GAConfig& newConfig) {
        config = newConfig;
        
        // If population size changed, we may need to reinitialize
        if (population.size() != static_cast<size_t>(config.populationSize)) {
            reset();
        }
    }
}