#ifndef CONSOLE_INTERFACE_H
#define CONSOLE_INTERFACE_H

#include "GeneticTypes.h"
#include "Individual.h"
#include "FitnessFunction.h"
#include "GeneticAlgorithm.h"
#include <string>
#include <vector>
#include <memory>

// Windows-specific includes for enhanced console interface
#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
    #include <io.h>
    #include <fcntl.h>
#endif

namespace GA {
    /**
     * @class ConsoleInterface
     * @brief Enhanced console interface for genetic algorithm demonstration
     * 
     * This class provides a rich console interface using Windows API features
     * for better visualization of the genetic algorithm process, including
     * colored output, progress bars, and formatted tables.
     */
    class ConsoleInterface {
    public:
        // Color constants for different types of output
        enum class Color {
            DEFAULT = 7,    // Light gray
            HEADER = 11,    // Bright cyan
            SUCCESS = 10,   // Bright green
            WARNING = 14,   // Yellow
            ERR = 12,       // Bright red
            INFO = 9,       // Bright blue
            EMPHASIS = 13,  // Bright magenta
            DIM = 8        // Dark gray
        };
        
    private:
        
    private:
        #ifdef _WIN32
        HANDLE hConsole;                // Console handle for Windows API
        CONSOLE_SCREEN_BUFFER_INFO csbi; // Console screen buffer info
        #endif
        
        bool useColors;                 // Whether to use colored output
        int consoleWidth;              // Console width for formatting
        
    public:
        // Constructor
        ConsoleInterface();
        
        // Destructor
        ~ConsoleInterface();
        
        // Initialization and setup
        /**
         * @brief Initializes the console interface
         * @param enableColors Whether to enable colored output
         */
        void initialize(bool enableColors = true);
        
        /**
         * @brief Sets up the console for Unicode support
         */
        void setupUnicodeSupport();
        
        // Output formatting methods
        /**
         * @brief Sets console text color
         * @param color Color to set
         */
        void setColor(Color color);
        
        /**
         * @brief Resets console color to default
         */
        void resetColor();
        
        /**
         * @brief Clears the console screen
         */
        void clearScreen();
        
        /**
         * @brief Moves cursor to specific position
         * @param x X coordinate
         * @param y Y coordinate
         */
        void moveCursor(int x, int y);
        
        /**
         * @brief Gets current cursor position
         * @return Pair of (x, y) coordinates
         */
        std::pair<int, int> getCursorPosition();
        
        // Display methods for genetic algorithm
        /**
         * @brief Displays the application header and title
         */
        void displayHeader();
        
        /**
         * @brief Displays algorithm configuration
         * @param config The GA configuration to display
         * @param fitnessFunc The fitness function being used
         */
        void displayConfiguration(const GAConfig& config, const FitnessFunction& fitnessFunc);
        
        /**
         * @brief Displays current population information
         * @param population The population to display
         * @param config Algorithm configuration
         * @param fitnessFunc Fitness function for value decoding
         * @param showAll Whether to show all individuals or just summary
         */
        void displayPopulation(const Population& population, const GAConfig& config, 
                             const FitnessFunction& fitnessFunc, bool showAll = false);
        
        /**
         * @brief Displays generation statistics
         * @param stats Generation statistics to display
         */
        void displayGenerationStats(const GenerationStats& stats);
        
        /**
         * @brief Displays a progress bar
         * @param current Current progress value
         * @param total Total progress value
         * @param width Width of the progress bar
         * @param label Label for the progress bar
         */
        void displayProgressBar(int current, int total, int width = 50, const std::string& label = "Progress");
        
        /**
         * @brief Displays the best individual information
         * @param individual The best individual
         * @param config Algorithm configuration
         * @param fitnessFunc Fitness function for value decoding
         */
        void displayBestIndividual(const Individual& individual, const GAConfig& config, 
                                 const FitnessFunction& fitnessFunc);
        
        /**
         * @brief Displays algorithm convergence information
         * @param diversity Current population diversity
         * @param generation Current generation
         * @param hasConverged Whether algorithm has converged
         */
        void displayConvergenceInfo(double diversity, int generation, bool hasConverged);
        
        /**
         * @brief Displays final results and summary
         * @param stats Vector of all generation statistics
         * @param bestIndividual Final best individual
         * @param config Algorithm configuration
         * @param fitnessFunc Fitness function
         */
        void displayFinalResults(const std::vector<GenerationStats>& stats, 
                               const Individual& bestIndividual,
                               const GAConfig& config, const FitnessFunction& fitnessFunc);
        
        // User interaction methods
        /**
         * @brief Displays main menu and gets user choice
         * @return Selected menu option
         */
        int displayMenu();
        
        /**
         * @brief Gets GA configuration from user input
         * @return User-configured GAConfig
         */
        GAConfig getConfigurationFromUser();
        
        /**
         * @brief Gets fitness function choice from user
         * @return Unique pointer to selected fitness function
         */
        std::unique_ptr<FitnessFunction> getFitnessFunctionFromUser();
        
        /**
         * @brief Waits for user to press a key
         * @param message Message to display
         */
        void waitForKeyPress(const std::string& message = "Press any key to continue...");
        
        /**
         * @brief Gets yes/no input from user
         * @param question Question to ask
         * @return True for yes, false for no
         */
        bool getYesNoInput(const std::string& question);
        
        // Utility methods
        /**
         * @brief Displays a formatted table header
         * @param headers Vector of header strings
         * @param columnWidths Vector of column widths
         */
        void displayTableHeader(const std::vector<std::string>& headers, 
                              const std::vector<int>& columnWidths);
        
        /**
         * @brief Displays a formatted table row
         * @param data Vector of data strings
         * @param columnWidths Vector of column widths
         */
        void displayTableRow(const std::vector<std::string>& data, 
                           const std::vector<int>& columnWidths);
        
        /**
         * @brief Displays a horizontal line separator
         * @param width Width of the line
         * @param character Character to use for the line
         */
        void displaySeparator(int width = 80, char character = '-');
        
        /**
         * @brief Centers text within a given width
         * @param text Text to center
         * @param width Total width
         * @return Centered text string
         */
        std::string centerText(const std::string& text, int width);
        
        /**
         * @brief Formats a number with specified precision
         * @param value Number to format
         * @param precision Number of decimal places
         * @return Formatted string
         */
        std::string formatNumber(double value, int precision = 6);
        
        /**
         * @brief Displays genetic algorithm explanation
         */
        void displayGeneticAlgorithmExplanation();
        
        /**
         * @brief Displays about information
         */
        void displayAboutInfo();
    };
}

#endif // CONSOLE_INTERFACE_H