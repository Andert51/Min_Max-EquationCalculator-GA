#include "GeneticAlgorithm.h"
#include "ConsoleInterface.h"
#include "FitnessFunction.h"
#include <iostream>
#include <memory>
#include <stdexcept> 
#include <iomanip> 
#include <chrono>
#include <thread> 

/**
 * @file main.cpp
 * @brief Main application for Genetic Algorithm Demonstration
 * @author Andres Torres Ceja
 * @institution Universidad de Guanajuato - DICIS
 * @course Artificial Intelligence
 * 
 * This application demonstrates the implementation and usage of genetic algorithms
 * for function optimization. It showcases all the essential components of a GA:
 * - Binary chromosome representation
 * - Population management
 * - Selection strategies (Tournament, Roulette Wheel, Elitism)
 * - Crossover operations (Single-point, Two-point, Uniform)
 * - Mutation with configurable rates
 * - Fitness evaluation and optimization
 * - Real-time evolution tracking
 */

using namespace GA;

/**
 * @brief Progress callback function for real-time algorithm monitoring
 * @param generation Current generation number
 * @param stats Statistics for the current generation
 */
void algorithmProgressCallback(int generation, const GenerationStats& stats) {
    static ConsoleInterface ui;  // Static to avoid repeated initialization
    
    // Clear previous line and display current progress
    std::cout << "\r";  // Return to beginning of line
    
    // Display generation progress with colored output
    ui.setColor(ConsoleInterface::Color::INFO);
    std::cout << "Generation " << std::setw(3) << generation << " | ";
    
    ui.setColor(ConsoleInterface::Color::SUCCESS);
    std::cout << "Best: " << std::setw(12) << std::fixed << std::setprecision(6) << stats.bestFitness;
    
    ui.setColor(ConsoleInterface::Color::DEFAULT);
    std::cout << " | Avg: " << std::setw(12) << std::fixed << std::setprecision(6) << stats.averageFitness;
    
    ui.setColor(ConsoleInterface::Color::WARNING);
    std::cout << " | Diversity: " << std::setw(6) << std::fixed << std::setprecision(2) << (stats.convergence * 100) << "%";
    
    ui.resetColor();
    std::cout << std::flush;
    
    // Add a small delay to make the evolution visible
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

/**
 * @brief Demonstrates the genetic algorithm with detailed step-by-step explanation
 * @param ga The genetic algorithm instance
 * @param ui Console interface for display
 */
void demonstrateDetailedEvolution(GeneticAlgorithm& ga, ConsoleInterface& ui) {
    ui.clearScreen();
    ui.setColor(ConsoleInterface::Color::HEADER);
    std::cout << "DETAILED GENETIC ALGORITHM DEMONSTRATION" << std::endl;
    ui.displaySeparator(50, '=');
    ui.resetColor();
    std::cout << std::endl;
    
    // Show initial configuration
    ui.displayConfiguration(ga.getConfig(), ga.getFitnessFunction());
    ui.waitForKeyPress("Press any key to start evolution...");
    
    // Initialize and show initial population
    ui.setColor(ConsoleInterface::Color::EMPHASIS);
    std::cout << "STEP 1: INITIALIZATION" << std::endl;
    ui.displaySeparator(25, '-');
    ui.resetColor();
    std::cout << "Creating random initial population..." << std::endl;
    
    ga.initializePopulation();
    ga.evaluatePopulation();
    
    std::cout << "Initial population created with " << ga.getPopulation().size() << " individuals." << std::endl;
    
    // Show population details if requested
    if (ui.getYesNoInput("Display initial population details?")) {
        ui.displayPopulation(ga.getPopulation(), ga.getConfig(), ga.getFitnessFunction(), true);
    }
    
    // Show initial statistics
    GenerationStats initialStats = ga.calculateGenerationStats();
    ui.displayGenerationStats(initialStats);
    ui.waitForKeyPress();
    
    // Evolution process
    ui.clearScreen();
    ui.setColor(ConsoleInterface::Color::EMPHASIS);
    std::cout << "STEP 2: EVOLUTION PROCESS" << std::endl;
    ui.displaySeparator(30, '-');
    ui.resetColor();
    std::cout << std::endl;
    
    std::cout << "Starting evolution with the following genetic operators:" << std::endl;
    std::cout << "• Selection: ";
    switch (ga.getConfig().selectionType) {
        case SelectionType::TOURNAMENT:
            std::cout << "Tournament Selection (size: " << ga.getConfig().tournamentSize << ")" << std::endl;
            break;
        case SelectionType::ROULETTE_WHEEL:
            std::cout << "Roulette Wheel Selection" << std::endl;
            break;
        case SelectionType::ELITISM:
            std::cout << "Elitism Selection" << std::endl;
            break;
    }
    
    std::cout << "• Crossover: ";
    switch (ga.getConfig().crossoverType) {
        case CrossoverType::SINGLE_POINT:
            std::cout << "Single-Point Crossover";
            break;
        case CrossoverType::TWO_POINT:
            std::cout << "Two-Point Crossover";
            break;
        case CrossoverType::UNIFORM:
            std::cout << "Uniform Crossover";
            break;
    }
    std::cout << " (Rate: " << (ga.getConfig().crossoverRate * 100) << "%)" << std::endl;
    
    std::cout << "• Mutation: Bit-flip Mutation (Rate: " << (ga.getConfig().mutationRate * 100) << "%)" << std::endl;
    std::cout << "• Elitism: Preserving top " << (ga.getConfig().elitismRate * 100) << "% of population" << std::endl;
    std::cout << std::endl;
    
    ui.waitForKeyPress("Press any key to start evolution monitoring...");
    
    // Run algorithm with progress monitoring
    std::cout << std::endl;
    ui.setColor(ConsoleInterface::Color::INFO);
    std::cout << "Evolution Progress:" << std::endl;
    ui.resetColor();
    
    auto startTime = std::chrono::high_resolution_clock::now();
    GenerationStats finalStats = ga.run(algorithmProgressCallback);
    auto endTime = std::chrono::high_resolution_clock::now();
    
    std::cout << std::endl << std::endl;
    
    // Calculate execution time
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    ui.setColor(ConsoleInterface::Color::SUCCESS);
    std::cout << "Evolution completed!" << std::endl;
    ui.resetColor();
    std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
    std::cout << "Final generation: " << ga.getCurrentGeneration() << std::endl;
    std::cout << std::endl;
    
    // Show final results
    ui.displayFinalResults(ga.getStatistics(), ga.getBestIndividual(), ga.getConfig(), ga.getFitnessFunction());
    
    // Wait for user to review results
    ui.waitForKeyPress("Press any key to return to the main menu...");
}

/**
 * @brief Runs a quick genetic algorithm demonstration
 * @param ga The genetic algorithm instance
 * @param ui Console interface for display
 */
void runQuickDemo(GeneticAlgorithm& ga, ConsoleInterface& ui) {
    ui.clearScreen();
    ui.setColor(ConsoleInterface::Color::HEADER);
    std::cout << "GENETIC ALGORITHM - QUICK DEMONSTRATION" << std::endl;
    ui.displaySeparator(45, '=');
    ui.resetColor();
    std::cout << std::endl;
    
    // Show configuration summary
    ui.displayConfiguration(ga.getConfig(), ga.getFitnessFunction());
    
    std::cout << "Running genetic algorithm..." << std::endl;
    std::cout << std::endl;
    
    // Run algorithm with progress display
    auto startTime = std::chrono::high_resolution_clock::now();
    GenerationStats finalStats = ga.run(algorithmProgressCallback);
    auto endTime = std::chrono::high_resolution_clock::now();
    
    std::cout << std::endl << std::endl;
    
    // Show results
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    ui.setColor(ConsoleInterface::Color::SUCCESS);
    std::cout << "Algorithm completed in " << duration.count() << " ms" << std::endl;
    ui.resetColor();
    std::cout << std::endl;
    
    ui.displayFinalResults(ga.getStatistics(), ga.getBestIndividual(), ga.getConfig(), ga.getFitnessFunction());
    ui.waitForKeyPress();
}

/**
 * @brief Creates a default genetic algorithm configuration
 * @return Default GAConfig with reasonable parameters
 */
GAConfig createDefaultConfig() {
    GAConfig config;
    config.populationSize = 50;
    config.chromosomeLength = 20;
    config.maxGenerations = 100;
    config.crossoverRate = 0.8;
    config.mutationRate = 0.01;
    config.elitismRate = 0.1;
    config.isMaximization = true;
    config.selectionType = SelectionType::TOURNAMENT;
    config.crossoverType = CrossoverType::SINGLE_POINT;
    config.tournamentSize = 3;
    config.minValue = -10.0;
    config.maxValue = 10.0;
    return config;
}

/**
 * @brief Creates a default fitness function (Sinusoidal)
 * @return Unique pointer to a sinusoidal fitness function
 */
std::unique_ptr<FitnessFunction> createDefaultFitnessFunction() {
    // Use a sinusoidal function: f(x) = 2 * sin(0.5 * x) + 1
    return std::make_unique<SinusoidalFunction>(2.0, 0.5, 0.0, 1.0, true);
}

/**
 * @brief Main application entry point
 * @return Exit code (0 for success)
 */
int main() {
    try {
        // Initialize console interface
        ConsoleInterface ui;
        ui.initialize(true);  // Enable colors
        
        // Initialize default configuration and fitness function
        GAConfig config = createDefaultConfig();
        std::unique_ptr<FitnessFunction> fitnessFunction = createDefaultFitnessFunction();
        
        // Main application loop
        bool running = true;
        while (running) {
            int choice = ui.displayMenu();
            
            switch (choice) {
                case 1: {
                    // Run with default settings
                    try {
                        GeneticAlgorithm ga(config, std::make_unique<SinusoidalFunction>(2.0, 0.5, 0.0, 1.0, true));
                        runQuickDemo(ga, ui);
                    } catch (const std::exception& e) {
                        ui.setColor(ConsoleInterface::Color::ERR);
                        std::cout << "Error running genetic algorithm: " << e.what() << std::endl;
                        ui.resetColor();
                        ui.waitForKeyPress();
                    }
                    break;
                }
                
                case 2: {
                    // Configure parameters
                    config = ui.getConfigurationFromUser();
                    
                    ui.clearScreen();
                    ui.setColor(ConsoleInterface::Color::SUCCESS);
                    std::cout << "Configuration updated successfully!" << std::endl;
                    std::cout << std::endl;
                    std::cout << "Current settings:" << std::endl;
                    std::cout << "Population Size: " << config.populationSize << std::endl;
                    std::cout << "Chromosome Length: " << config.chromosomeLength << std::endl;
                    std::cout << "Max Generations: " << config.maxGenerations << std::endl;
                    std::cout << "Crossover Rate: " << config.crossoverRate << std::endl;
                    std::cout << "Mutation Rate: " << config.mutationRate << std::endl;
                    std::cout << "Elitism Rate: " << config.elitismRate << std::endl;
                    std::cout << "Domain: [" << config.minValue << ", " << config.maxValue << "]" << std::endl;
                    std::cout << "Problem Type: " << (config.isMaximization ? "Maximization" : "Minimization") << std::endl;
                    ui.resetColor();
                    ui.waitForKeyPress("Press any key to return to main menu...");
                    break;
                }
                
                case 3: {
                    // Select fitness function
                    fitnessFunction = ui.getFitnessFunctionFromUser();
                    config.isMaximization = fitnessFunction->isMaximizationProblem();
                    
                    ui.clearScreen();
                    ui.setColor(ConsoleInterface::Color::SUCCESS);
                    std::cout << "Fitness function configured successfully!" << std::endl;
                    std::cout << "Function: " << fitnessFunction->getName() << std::endl;
                    std::cout << "Optimization: " << (config.isMaximization ? "Maximization" : "Minimization") << std::endl;
                    ui.resetColor();
                    ui.waitForKeyPress("Press any key to continue...");
                    
                    // Option to run detailed demonstration
                    if (ui.getYesNoInput("Run detailed demonstration with this function?")) {
                        try {
                            // Use the actual configured fitness function by creating a proper clone
                            std::unique_ptr<FitnessFunction> gaFunction = fitnessFunction->clone();
                            
                            GeneticAlgorithm ga(config, std::move(gaFunction));
                            demonstrateDetailedEvolution(ga, ui);
                        } catch (const std::exception& e) {
                            ui.setColor(ConsoleInterface::Color::ERR);
                            std::cout << "Error in detailed demonstration: " << e.what() << std::endl;
                            ui.resetColor();
                            ui.waitForKeyPress("Press any key to return to menu...");
                        }
                    }
                    break;
                }
                
                case 4: {
                    // View explanation
                    ui.displayGeneticAlgorithmExplanation();
                    break;
                }
                
                case 5: {
                    // About
                    ui.displayAboutInfo();
                    break;
                }
                
                case 6: {
                    // Exit
                    running = false;
                    break;
                }
                
                default: {
                    ui.setColor(ConsoleInterface::Color::WARNING);
                    std::cout << "Invalid choice. Please select 1-6." << std::endl;
                    ui.resetColor();
                    ui.waitForKeyPress();
                    break;
                }
            }
        }
        
        // Exit message
        ui.clearScreen();
        ui.setColor(ConsoleInterface::Color::HEADER);
        std::cout << "Thank you for using the Genetic Algorithm Demonstration!" << std::endl;
        ui.resetColor();
        std::cout << std::endl;
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        std::cerr << "Press any key to exit..." << std::endl;
        #ifdef _WIN32
        _getch();
        #else
        std::cin.get();
        #endif
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred." << std::endl;
        std::cerr << "Press any key to exit..." << std::endl;
        #ifdef _WIN32
        _getch();
        #else
        std::cin.get();
        #endif
        return 1;
    }
}