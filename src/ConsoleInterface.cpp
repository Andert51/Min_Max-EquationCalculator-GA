#include "ConsoleInterface.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cmath>

namespace GA {
    
    // Constructor
    ConsoleInterface::ConsoleInterface() : useColors(true), consoleWidth(80) {
        #ifdef _WIN32
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        consoleWidth = csbi.dwSize.X;
        #endif
    }
    
    // Destructor
    ConsoleInterface::~ConsoleInterface() {
        resetColor();
    }
    
    // Initialize console interface
    void ConsoleInterface::initialize(bool enableColors) {
        useColors = enableColors;
        setupUnicodeSupport();
        
        #ifdef _WIN32
        if (useColors) {
            // Enable ANSI escape sequences for better color support
            DWORD dwMode = 0;
            GetConsoleMode(hConsole, &dwMode);
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hConsole, dwMode);
        }
        #endif
        
        clearScreen();
    }
    
    // Setup Unicode support
    void ConsoleInterface::setupUnicodeSupport() {
        #ifdef _WIN32
        // Set console code page to UTF-8
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        #endif
    }
    
    // Set console color
    void ConsoleInterface::setColor(Color color) {
        if (!useColors) return;
        
        #ifdef _WIN32
        SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
        #endif
    }
    
    // Reset console color
    void ConsoleInterface::resetColor() {
        if (!useColors) return;
        
        #ifdef _WIN32
        SetConsoleTextAttribute(hConsole, static_cast<WORD>(Color::DEFAULT));
        #endif
    }
    
    // Clear console screen
    void ConsoleInterface::clearScreen() {
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif
    }
    
    // Move cursor to position
    void ConsoleInterface::moveCursor(int x, int y) {
        #ifdef _WIN32
        COORD coord = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
        SetConsoleCursorPosition(hConsole, coord);
        #endif
    }
    
    // Get cursor position
    std::pair<int, int> ConsoleInterface::getCursorPosition() {
        #ifdef _WIN32
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        return std::make_pair(csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);
        #else
        return std::make_pair(0, 0);
        #endif
    }
    
    // Display application header
    void ConsoleInterface::displayHeader() {
        clearScreen();
        setColor(Color::HEADER);
        
        std::string title = "GENETIC ALGORITHM DEMONSTRATION";
        std::string subtitle = "Advanced Optimization Algorithms";
        std::string separator(consoleWidth, '=');
        
        std::cout << separator << std::endl;
        std::cout << centerText(title, consoleWidth) << std::endl;
        std::cout << centerText(subtitle, consoleWidth) << std::endl;
        std::cout << separator << std::endl;
        
        resetColor();
        std::cout << std::endl;
        
        setColor(Color::INFO);
        std::cout << "This application demonstrates the use of genetic algorithms for function optimization." << std::endl;
        std::cout << "It shows detailed step-by-step evolution from binary chromosomes to optimal solutions." << std::endl;
        resetColor();
        std::cout << std::endl;
    }
    
    // Display algorithm configuration
    void ConsoleInterface::displayConfiguration(const GAConfig& config, const FitnessFunction& fitnessFunc) {
        setColor(Color::HEADER);
        std::cout << "ALGORITHM CONFIGURATION" << std::endl;
        displaySeparator(40, '-');
        resetColor();
        
        // GA Parameters
        setColor(Color::EMPHASIS);
        std::cout << "Genetic Algorithm Parameters:" << std::endl;
        resetColor();
        std::cout << "  Population Size:      " << config.populationSize << " individuals" << std::endl;
        std::cout << "  Chromosome Length:    " << config.chromosomeLength << " bits" << std::endl;
        std::cout << "  Maximum Generations:  " << config.maxGenerations << std::endl;
        std::cout << "  Crossover Rate:       " << (config.crossoverRate * 100) << "%" << std::endl;
        std::cout << "  Mutation Rate:        " << (config.mutationRate * 100) << "%" << std::endl;
        std::cout << "  Elitism Rate:         " << (config.elitismRate * 100) << "%" << std::endl;
        
        // Selection Strategy
        std::cout << "  Selection Strategy:   ";
        switch (config.selectionType) {
            case SelectionType::TOURNAMENT:
                std::cout << "Tournament (size: " << config.tournamentSize << ")";
                break;
            case SelectionType::ROULETTE_WHEEL:
                std::cout << "Roulette Wheel";
                break;
            case SelectionType::ELITISM:
                std::cout << "Elitism";
                break;
        }
        std::cout << std::endl;
        
        // Crossover Strategy
        std::cout << "  Crossover Strategy:   ";
        switch (config.crossoverType) {
            case CrossoverType::SINGLE_POINT:
                std::cout << "Single Point";
                break;
            case CrossoverType::TWO_POINT:
                std::cout << "Two Point";
                break;
            case CrossoverType::UNIFORM:
                std::cout << "Uniform";
                break;
        }
        std::cout << std::endl;
        
        std::cout << std::endl;
        
        // Function Information
        setColor(Color::EMPHASIS);
        std::cout << "Optimization Function:" << std::endl;
        resetColor();
        std::cout << "  Function Name:        " << fitnessFunc.getName() << std::endl;
        std::cout << "  Expression:           " << fitnessFunc.getExpression() << std::endl;
        std::cout << "  Optimization Type:    " << (config.isMaximization ? "Maximization" : "Minimization") << std::endl;
        std::cout << "  Domain Range:         [" << config.minValue << ", " << config.maxValue << "]" << std::endl;
        
        // Theoretical optimum if known
        if (!std::isnan(fitnessFunc.getOptimalValue())) {
            std::cout << "  Theoretical Optimum:  f(" << fitnessFunc.getOptimalX() << ") = " << fitnessFunc.getOptimalValue() << std::endl;
        }
        
        std::cout << std::endl;
    }
    
    // Display population information
    void ConsoleInterface::displayPopulation(const Population& population, const GAConfig& config, 
                                           const FitnessFunction& fitnessFunc, bool showAll) {
        setColor(Color::HEADER);
        std::cout << "POPULATION DETAILS" << std::endl;
        displaySeparator(50, '-');
        resetColor();
        
        if (showAll) {
            // Display table header
            std::vector<std::string> headers = {"#", "Chromosome", "Decoded Value", "Fitness", "Fitness %"};
            std::vector<int> widths = {4, config.chromosomeLength + 2, 15, 15, 12};
            displayTableHeader(headers, widths);
            
            // Display each individual
            for (size_t i = 0; i < population.size(); ++i) {
                const Individual& ind = population[i];
                double decodedValue = ind.decodeToValue(config.minValue, config.maxValue);
                
                std::vector<std::string> data = {
                    std::to_string(i + 1),
                    ind.toString(),
                    formatNumber(decodedValue, 6),
                    formatNumber(ind.getFitness(), 6),
                    formatNumber(ind.getFitnessPercentage(), 1) + "%"
                };
                
                // Highlight best individual
                if (i == 0) setColor(Color::SUCCESS);
                displayTableRow(data, widths);
                if (i == 0) resetColor();
            }
        } else {
            // Display summary statistics
            std::cout << "Population Size: " << population.size() << " individuals" << std::endl;
            std::cout << "Chromosome Length: " << config.chromosomeLength << " bits" << std::endl;
            std::cout << "Domain: [" << config.minValue << ", " << config.maxValue << "]" << std::endl;
            
            if (!population.empty()) {
                // Find best and worst
                double bestFitness = population[0].getFitness();
                double bestFitnessPercentage = population[0].getFitnessPercentage();
                double worstFitness = population[0].getFitness();
                double totalFitness = 0.0;
                double totalFitnessPercentage = 0.0;
                
                for (const Individual& ind : population) {
                    double fitness = ind.getFitness();
                    totalFitness += fitness;
                    totalFitnessPercentage += ind.getFitnessPercentage();
                    
                    if (config.isMaximization) {
                        if (fitness > bestFitness) {
                            bestFitness = fitness;
                            bestFitnessPercentage = ind.getFitnessPercentage();
                        }
                        if (fitness < worstFitness) worstFitness = fitness;
                    } else {
                        if (fitness < bestFitness) {
                            bestFitness = fitness;
                            bestFitnessPercentage = ind.getFitnessPercentage();
                        }
                        if (fitness > worstFitness) worstFitness = fitness;
                    }
                }
                
                double avgFitness = totalFitness / population.size();
                double avgFitnessPercentage = totalFitnessPercentage / population.size();
                
                std::cout << "Best Fitness:    " << formatNumber(bestFitness, 6) 
                          << " (" << formatNumber(bestFitnessPercentage, 1) << "%)" << std::endl;
                std::cout << "Average Fitness: " << formatNumber(avgFitness, 6) 
                          << " (" << formatNumber(avgFitnessPercentage, 1) << "%)" << std::endl;
                std::cout << "Worst Fitness:   " << formatNumber(worstFitness, 6) << std::endl;
            }
        }
        
        std::cout << std::endl;
    }
    
    // Display generation statistics
    void ConsoleInterface::displayGenerationStats(const GenerationStats& stats) {
        setColor(Color::INFO);
        std::cout << "Generation " << stats.generation << " - Statistics:" << std::endl;
        resetColor();
        
        std::cout << "  Best Fitness:     ";
        setColor(Color::SUCCESS);
        std::cout << formatNumber(stats.bestFitness, 6);
        resetColor();
        std::cout << " (" << formatNumber(stats.bestFitnessPercentage, 1) << "%)";
        std::cout << "  (x = " << formatNumber(stats.bestValue, 6) << ")" << std::endl;
        
        std::cout << "  Average Fitness:  " << formatNumber(stats.averageFitness, 6);
        std::cout << " (" << formatNumber(stats.averageFitnessPercentage, 1) << "%)" << std::endl;
        std::cout << "  Worst Fitness:    " << formatNumber(stats.worstFitness, 6) << std::endl;
        std::cout << "  Population Diversity: " << formatNumber(stats.convergence * 100, 2) << "%" << std::endl;
    }
    
    // Display progress bar
    void ConsoleInterface::displayProgressBar(int current, int total, int width, const std::string& label) {
        double progress = static_cast<double>(current) / total;
        int filled = static_cast<int>(progress * width);
        
        setColor(Color::INFO);
        std::cout << label << ": [";
        
        setColor(Color::SUCCESS);
        for (int i = 0; i < filled; ++i) {
            std::cout << "█";
        }
        
        setColor(Color::DIM);
        for (int i = filled; i < width; ++i) {
            std::cout << "░";
        }
        
        setColor(Color::INFO);
        std::cout << "] " << std::setw(3) << static_cast<int>(progress * 100) << "%";
        std::cout << " (" << current << "/" << total << ")";
        resetColor();
        std::cout << std::endl;
    }
    
    // Display best individual
    void ConsoleInterface::displayBestIndividual(const Individual& individual, const GAConfig& config, 
                                                const FitnessFunction& fitnessFunc) {
        setColor(Color::SUCCESS);
        std::cout << "BEST INDIVIDUAL FOUND:" << std::endl;
        displaySeparator(30, '-');
        resetColor();
        
        double decodedValue = individual.decodeToValue(config.minValue, config.maxValue);
        
        std::cout << "  Binary Chromosome:  " << individual.toString() << std::endl;
        std::cout << "  Decoded Value (x):  " << formatNumber(decodedValue, 8) << std::endl;
        std::cout << "  Fitness f(x):       " << formatNumber(individual.getFitness(), 8) << std::endl;
        std::cout << "  Fitness Quality:    " << formatNumber(individual.getFitnessPercentage(), 1) << "%" << std::endl;
        
        // Show mathematical evaluation
        setColor(Color::EMPHASIS);
        std::cout << "  Mathematical Check: " << fitnessFunc.getExpression() << std::endl;
        std::cout << "                     f(" << formatNumber(decodedValue, 6) << ") = " 
                  << formatNumber(fitnessFunc.evaluate(decodedValue), 8) << std::endl;
        resetColor();
        
        std::cout << std::endl;
    }
    
    // Display convergence information
    void ConsoleInterface::displayConvergenceInfo(double diversity, int generation, bool hasConverged) {
        setColor(Color::WARNING);
        std::cout << "CONVERGENCE ANALYSIS:" << std::endl;
        displaySeparator(25, '-');
        resetColor();
        
        std::cout << "  Current Diversity:  " << formatNumber(diversity * 100, 2) << "%" << std::endl;
        std::cout << "  Generation:         " << generation << std::endl;
        std::cout << "  Status:             ";
        
        if (hasConverged) {
            setColor(Color::SUCCESS);
            std::cout << "CONVERGED";
        } else {
            setColor(Color::INFO);
            std::cout << "EVOLVING";
        }
        resetColor();
        std::cout << std::endl << std::endl;
    }
    
    // Display final results
    void ConsoleInterface::displayFinalResults(const std::vector<GenerationStats>& stats, 
                                              const Individual& bestIndividual,
                                              const GAConfig& config, const FitnessFunction& fitnessFunc) {
        clearScreen();
        setColor(Color::HEADER);
        std::cout << "GENETIC ALGORITHM - FINAL RESULTS" << std::endl;
        displaySeparator(consoleWidth, '=');
        resetColor();
        std::cout << std::endl;
        
        // Best individual
        displayBestIndividual(bestIndividual, config, fitnessFunc);
        
        // Evolution summary
        setColor(Color::EMPHASIS);
        std::cout << "EVOLUTION SUMMARY:" << std::endl;
        displaySeparator(20, '-');
        resetColor();
        
        std::cout << "  Total Generations:    " << stats.size() - 1 << std::endl;
        std::cout << "  Initial Best Fitness: " << formatNumber(stats[0].bestFitness, 6) 
                  << " (" << formatNumber(stats[0].bestFitnessPercentage, 1) << "%)" << std::endl;
        std::cout << "  Final Best Fitness:   " << formatNumber(stats.back().bestFitness, 6) 
                  << " (" << formatNumber(stats.back().bestFitnessPercentage, 1) << "%)" << std::endl;
        
        double improvement = stats.back().bestFitness - stats[0].bestFitness;
        double percentageImprovement = stats.back().bestFitnessPercentage - stats[0].bestFitnessPercentage;
        std::cout << "  Fitness Improvement:  " << formatNumber(improvement, 6);
        if (improvement > 0) {
            setColor(Color::SUCCESS);
            std::cout << " ↑";
        } else if (improvement < 0) {
            setColor(Color::WARNING);
            std::cout << " ↓";
        }
        resetColor();
        std::cout << " (+" << formatNumber(percentageImprovement, 1) << "% quality)" << std::endl;
        
        std::cout << "  Final Diversity:      " << formatNumber(stats.back().convergence * 100, 2) << "%" << std::endl;
        std::cout << std::endl;
        
        // Generation-by-generation summary (last 10 generations)
        setColor(Color::INFO);
        std::cout << "RECENT EVOLUTION PROGRESS (Last 10 Generations):" << std::endl;
        displaySeparator(60, '-');
        resetColor();
        
        std::vector<std::string> headers = {"Gen", "Best Fitness %", "Avg Fitness %", "Best Value", "Diversity"};
        std::vector<int> widths = {5, 15, 15, 12, 12};
        displayTableHeader(headers, widths);
        
        int startGen = std::max(0, static_cast<int>(stats.size()) - 10);
        for (int i = startGen; i < static_cast<int>(stats.size()); ++i) {
            std::vector<std::string> data = {
                std::to_string(stats[i].generation),
                formatNumber(stats[i].bestFitnessPercentage, 1) + "%",
                formatNumber(stats[i].averageFitnessPercentage, 1) + "%",
                formatNumber(stats[i].bestFitness, 4),
                formatNumber(stats[i].convergence * 100, 1) + "%"
            };
            displayTableRow(data, widths);
        }
        
        std::cout << std::endl;
    }
    
    // Display main menu
    int ConsoleInterface::displayMenu() {
        clearScreen();
        displayHeader();
        
        setColor(Color::HEADER);
        std::cout << "MAIN MENU" << std::endl;
        displaySeparator(20, '-');
        resetColor();
        
        std::cout << "1. Run Genetic Algorithm with Default Settings" << std::endl;
        std::cout << "2. Configure Algorithm Parameters" << std::endl;
        std::cout << "3. Select Fitness Function" << std::endl;
        std::cout << "4. View Genetic Algorithm Explanation" << std::endl;
        std::cout << "5. About This Application" << std::endl;
        std::cout << "6. Exit" << std::endl;
        std::cout << std::endl;
        
        setColor(Color::EMPHASIS);
        std::cout << "Enter your choice (1-6): ";
        resetColor();
        
        int choice;
        std::cin >> choice;
        return choice;
    }
    
    // Get configuration from user
    GAConfig ConsoleInterface::getConfigurationFromUser() {
        GAConfig config;
        
        clearScreen();
        setColor(Color::HEADER);
        std::cout << "ALGORITHM CONFIGURATION" << std::endl;
        displaySeparator(30, '-');
        resetColor();
        
        std::cout << "Enter genetic algorithm parameters:" << std::endl << std::endl;
        
        std::cout << "Population Size (default 50): ";
        if (!(std::cin >> config.populationSize) || config.populationSize <= 0) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            config.populationSize = 50;
            setColor(Color::WARNING);
            std::cout << "Invalid input. Using default value: " << config.populationSize << std::endl;
            resetColor();
        }
        
        std::cout << "Chromosome Length (default 20): ";
        if (!(std::cin >> config.chromosomeLength) || config.chromosomeLength <= 0) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            config.chromosomeLength = 20;
            setColor(Color::WARNING);
            std::cout << "Invalid input. Using default value: " << config.chromosomeLength << std::endl;
            resetColor();
        }
        
        std::cout << "Maximum Generations (default 100): ";
        if (!(std::cin >> config.maxGenerations) || config.maxGenerations <= 0) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            config.maxGenerations = 100;
            setColor(Color::WARNING);
            std::cout << "Invalid input. Using default value: " << config.maxGenerations << std::endl;
            resetColor();
        }
        
        std::cout << "Crossover Rate 0-1 (default 0.8): ";
        if (!(std::cin >> config.crossoverRate) || config.crossoverRate < 0 || config.crossoverRate > 1) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            config.crossoverRate = 0.8;
            setColor(Color::WARNING);
            std::cout << "Invalid input. Using default value: " << config.crossoverRate << std::endl;
            resetColor();
        }
        
        std::cout << "Mutation Rate 0-1 (default 0.01): ";
        if (!(std::cin >> config.mutationRate) || config.mutationRate < 0 || config.mutationRate > 1) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            config.mutationRate = 0.01;
            setColor(Color::WARNING);
            std::cout << "Invalid input. Using default value: " << config.mutationRate << std::endl;
            resetColor();
        }
        
        std::cout << "Elitism Rate 0-1 (default 0.1): ";
        if (!(std::cin >> config.elitismRate) || config.elitismRate < 0 || config.elitismRate > 1) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            config.elitismRate = 0.1;
            setColor(Color::WARNING);
            std::cout << "Invalid input. Using default value: " << config.elitismRate << std::endl;
            resetColor();
        }
        
        std::cout << "Function Domain Min Value (default -10): ";
        if (!(std::cin >> config.minValue)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            config.minValue = -10.0;
            setColor(Color::WARNING);
            std::cout << "Invalid input. Using default value: " << config.minValue << std::endl;
            resetColor();
        }
        
        std::cout << "Function Domain Max Value (default 10): ";
        if (!(std::cin >> config.maxValue)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            config.maxValue = 10.0;
            setColor(Color::WARNING);
            std::cout << "Invalid input. Using default value: " << config.maxValue << std::endl;
            resetColor();
        }
        
        if (config.minValue >= config.maxValue) {
            setColor(Color::WARNING);
            std::cout << "Invalid domain range. Using defaults: [-10, 10]" << std::endl;
            resetColor();
            config.minValue = -10.0;
            config.maxValue = 10.0;
        }
        
        std::cout << "Maximization problem? (1=yes, 0=no, default 1): ";
        int isMax;
        if (!(std::cin >> isMax)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            isMax = 1;
            setColor(Color::WARNING);
            std::cout << "Invalid input. Using default value: " << isMax << std::endl;
            resetColor();
        }
        config.isMaximization = (isMax == 1);
        
        return config;
    }
    
    // Get fitness function from user
    std::unique_ptr<FitnessFunction> ConsoleInterface::getFitnessFunctionFromUser() {
        clearScreen();
        setColor(Color::HEADER);
        std::cout << "SELECT FITNESS FUNCTION" << std::endl;
        displaySeparator(30, '-');
        resetColor();
        
        std::cout << "Available functions:" << std::endl;
        std::cout << "1. Sinusoidal: f(x) = A * sin(B * x + C) + D" << std::endl;
        std::cout << "2. Rastrigin: f(x) = A + x² - A * cos(2π * x)" << std::endl;
        std::cout << "3. Polynomial: f(x) = ax³ + bx² + cx + d" << std::endl;
        std::cout << "4. Exponential: f(x) = A * e^(B * x) + C" << std::endl;
        std::cout << "5. Linear: f(x) = a * x + b (simple test function)" << std::endl;
        std::cout << "6. Cosine: f(x) = A * cos(B * x + C) + D (simple test function)" << std::endl;
        std::cout << std::endl;
        
        setColor(Color::EMPHASIS);
        std::cout << "Enter your choice (1-6): ";
        resetColor();
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                double A, B, C, D;
                std::cout << "Enter parameters for f(x) = A * sin(B * x + C) + D:" << std::endl;
                std::cout << "A (amplitude, default 1.0): ";
                std::cin >> A;
                std::cout << "B (frequency, default 1.0): ";
                std::cin >> B;
                std::cout << "C (phase, default 0.0): ";
                std::cin >> C;
                std::cout << "D (offset, default 0.0): ";
                std::cin >> D;
                return std::make_unique<SinusoidalFunction>(A, B, C, D, true);
            }
            
            case 2: {
                double A;
                std::cout << "Enter parameter A for Rastrigin function (default 10.0): ";
                std::cin >> A;
                return std::make_unique<RastriginFunction>(A, false);  // Minimization
            }
            
            case 3: {
                double a, b, c, d;
                std::cout << "Enter coefficients for f(x) = ax³ + bx² + cx + d:" << std::endl;
                std::cout << "a (default 1.0): ";
                std::cin >> a;
                std::cout << "b (default 0.0): ";
                std::cin >> b;
                std::cout << "c (default 0.0): ";
                std::cin >> c;
                std::cout << "d (default 0.0): ";
                std::cin >> d;
                return std::make_unique<PolynomialFunction>(a, b, c, d, true);
            }
            
            case 4: {
                double A, B, C;
                std::cout << "Enter parameters for f(x) = A * e^(B * x) + C:" << std::endl;
                std::cout << "A (default 1.0): ";
                std::cin >> A;
                std::cout << "B (default 0.1): ";
                std::cin >> B;
                std::cout << "C (default 0.0): ";
                std::cin >> C;
                return std::make_unique<ExponentialFunction>(A, B, C, true);
            }
            
            case 5: {
                double a, b;
                std::cout << "Enter parameters for f(x) = a * x + b:" << std::endl;
                std::cout << "a (slope, default 1.0): ";
                std::cin >> a;
                std::cout << "b (y-intercept, default 0.0): ";
                std::cin >> b;
                return std::make_unique<LinearFunction>(a, b, true);
            }
            
            case 6: {
                double A, B, C, D;
                std::cout << "Enter parameters for f(x) = A * cos(B * x + C) + D:" << std::endl;
                std::cout << "A (amplitude, default 1.0): ";
                std::cin >> A;
                std::cout << "B (frequency, default 1.0): ";
                std::cin >> B;
                std::cout << "C (phase, default 0.0): ";
                std::cin >> C;
                std::cout << "D (offset, default 0.0): ";
                std::cin >> D;
                return std::make_unique<CosineFunction>(A, B, C, D, true);
            }
            
            default:
                return std::make_unique<SinusoidalFunction>();
        }
    }
    
    // Wait for key press
    void ConsoleInterface::waitForKeyPress(const std::string& message) {
        setColor(Color::DIM);
        std::cout << message;
        resetColor();
        
        #ifdef _WIN32
        _getch();
        #else
        std::cin.ignore();
        std::cin.get();
        #endif
        
        std::cout << std::endl;
    }
    
    // Get yes/no input
    bool ConsoleInterface::getYesNoInput(const std::string& question) {
        setColor(Color::EMPHASIS);
        std::cout << question << " (y/n): ";
        resetColor();
        
        char response;
        std::cin >> response;
        return (response == 'y' || response == 'Y');
    }
    
    // Display table header
    void ConsoleInterface::displayTableHeader(const std::vector<std::string>& headers, 
                                            const std::vector<int>& columnWidths) {
        setColor(Color::HEADER);
        for (size_t i = 0; i < headers.size(); ++i) {
            std::cout << std::left << std::setw(columnWidths[i]) << headers[i];
        }
        std::cout << std::endl;
        
        // Draw separator line
        for (int width : columnWidths) {
            for (int i = 0; i < width; ++i) {
                std::cout << "-";
            }
        }
        std::cout << std::endl;
        resetColor();
    }
    
    // Display table row
    void ConsoleInterface::displayTableRow(const std::vector<std::string>& data, 
                                         const std::vector<int>& columnWidths) {
        for (size_t i = 0; i < data.size() && i < columnWidths.size(); ++i) {
            std::cout << std::left << std::setw(columnWidths[i]) << data[i];
        }
        std::cout << std::endl;
    }
    
    // Display separator
    void ConsoleInterface::displaySeparator(int width, char character) {
        setColor(Color::DIM);
        for (int i = 0; i < width; ++i) {
            std::cout << character;
        }
        std::cout << std::endl;
        resetColor();
    }
    
    // Center text
    std::string ConsoleInterface::centerText(const std::string& text, int width) {
        int padding = (width - static_cast<int>(text.length())) / 2;
        return std::string(padding, ' ') + text;
    }
    
    // Format number
    std::string ConsoleInterface::formatNumber(double value, int precision) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(precision) << value;
        return ss.str();
    }
    
    // Display genetic algorithm explanation
    void ConsoleInterface::displayGeneticAlgorithmExplanation() {
        clearScreen();
        setColor(Color::HEADER);
        std::cout << "GENETIC ALGORITHM EXPLANATION" << std::endl;
        displaySeparator(consoleWidth, '=');
        resetColor();
        std::cout << std::endl;
        
        setColor(Color::EMPHASIS);
        std::cout << "What is a Genetic Algorithm?" << std::endl;
        resetColor();
        std::cout << "A genetic algorithm (GA) is a metaheuristic inspired by natural selection." << std::endl;
        std::cout << "It evolves a population of candidate solutions toward better solutions." << std::endl << std::endl;
        
        setColor(Color::EMPHASIS);
        std::cout << "Key Components:" << std::endl;
        resetColor();
        std::cout << "• CHROMOSOME: Binary string representing a candidate solution" << std::endl;
        std::cout << "• POPULATION: Collection of individuals (chromosomes)" << std::endl;
        std::cout << "• FITNESS: Measure of how good a solution is" << std::endl;
        std::cout << "• SELECTION: Choose parents for reproduction" << std::endl;
        std::cout << "• CROSSOVER: Combine two parents to create offspring" << std::endl;
        std::cout << "• MUTATION: Random changes to maintain diversity" << std::endl;
        std::cout << "• ELITISM: Preserve best individuals across generations" << std::endl << std::endl;
        
        setColor(Color::EMPHASIS);
        std::cout << "Algorithm Steps:" << std::endl;
        resetColor();
        std::cout << "1. Initialize random population" << std::endl;
        std::cout << "2. Evaluate fitness of each individual" << std::endl;
        std::cout << "3. Select parents for reproduction" << std::endl;
        std::cout << "4. Apply crossover to create offspring" << std::endl;
        std::cout << "5. Apply mutation to offspring" << std::endl;
        std::cout << "6. Replace old population with new generation" << std::endl;
        std::cout << "7. Repeat until termination criteria met" << std::endl << std::endl;
        
        waitForKeyPress();
    }
    
    // Display about information
    void ConsoleInterface::displayAboutInfo() {
        clearScreen();
        setColor(Color::HEADER);
        std::cout << "ABOUT THIS APPLICATION" << std::endl;
        displaySeparator(consoleWidth, '=');
        resetColor();
        std::cout << std::endl;
        
        setColor(Color::EMPHASIS);
        std::cout << "Genetic Algorithm Demonstration" << std::endl;
        resetColor();
        std::cout << "Version 1.0" << std::endl;
        std::cout << "Developed for Artificial Intelligence Course" << std::endl << std::endl;
        
        setColor(Color::EMPHASIS);
        std::cout << "Application Information:" << std::endl;
        resetColor();
        std::cout << "Advanced Genetic Algorithm Implementation" << std::endl;
        std::cout << "Educational Optimization Software" << std::endl << std::endl;
        
        setColor(Color::EMPHASIS);
        std::cout << "Features:" << std::endl;
        resetColor();
        std::cout << "• Multiple fitness functions (avoiding f(x)=x²)" << std::endl;
        std::cout << "• Configurable GA parameters" << std::endl;
        std::cout << "• Multiple selection strategies" << std::endl;
        std::cout << "• Various crossover operators" << std::endl;
        std::cout << "• Detailed evolution tracking" << std::endl;
        std::cout << "• Enhanced Windows console interface" << std::endl;
        std::cout << "• Real-time progress visualization" << std::endl << std::endl;
        
        setColor(Color::EMPHASIS);
        std::cout << "Implementation Details:" << std::endl;
        resetColor();
        std::cout << "• Language: C++17" << std::endl;
        std::cout << "• Platform: Windows" << std::endl;
        std::cout << "• Interface: Enhanced Console (windows.h)" << std::endl;
        std::cout << "• Architecture: Object-oriented design" << std::endl << std::endl;
        
        waitForKeyPress();
    }
}