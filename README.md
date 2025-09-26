# Genetic Algorithm Demonstration

**Author:** Andres Torres Ceja  
**Institution:** Universidad de Guanajuato - DICIS  
**Course:** Artificial Intelligence  

## Project Overview

This project demonstrates the implementation and application of genetic algorithms for function optimization. It provides a comprehensive, educational implementation that showcases all essential components of genetic algorithms while avoiding the excluded function f(x) = x².

## Key Features

### Core Genetic Algorithm Components
- **Binary Chromosome Representation**: Each individual encoded as a binary string
- **Population Management**: Configurable population sizes with generation tracking
- **Multiple Selection Strategies**:
  - Tournament Selection (configurable tournament size)
  - Roulette Wheel Selection (fitness-proportionate)
  - Elitism Selection (preserving best individuals)
- **Crossover Operations**:
  - Single-Point Crossover
  - Two-Point Crossover  
  - Uniform Crossover
- **Mutation**: Bit-flip mutation with configurable rates
- **Elitism**: Configurable percentage of elite individuals preserved

### Fitness Functions (Excluding f(x)=x²)
1. **Sinusoidal Function**: f(x) = A × sin(B × x + C) + D
2. **Rastrigin Function**: f(x) = A + x² - A × cos(2π × x)
3. **Polynomial Function**: f(x) = ax³ + bx² + cx + d
4. **Exponential Function**: f(x) = A × e^(B × x) + C

### Enhanced Windows Console Interface
- Colored output using Windows API
- Real-time evolution monitoring
- Progress bars and statistical displays
- Detailed generation-by-generation tracking
- Interactive configuration menus

## Algorithm Parameters

| Parameter | Default Value | Description |
|-----------|---------------|-------------|
| Population Size | 50 | Number of individuals per generation |
| Chromosome Length | 20 | Number of bits per individual |
| Max Generations | 100 | Maximum evolution iterations |
| Crossover Rate | 80% | Probability of crossover operation |
| Mutation Rate | 1% | Probability of bit mutation |
| Elitism Rate | 10% | Percentage of elite individuals preserved |
| Tournament Size | 3 | Size for tournament selection |

## Build Instructions

### Prerequisites
- Windows operating system
- MinGW-w64 or similar GCC compiler with C++17 support
- Command prompt or PowerShell

### Building the Application
1. Open command prompt in the project directory
2. Run the build script:
   ```batch
   .\build.bat
   ```
3. The executable will be created in the `bin` directory as `GA_Demo.exe`

### Build Configuration
The build script uses the following compiler flags:
- `-static-libgcc -static-libstdc++ -static`: Static linking for portability
- `-std=c++17`: C++17 standard compliance
- `-Wall -Wextra`: Enable comprehensive warnings
- `-m64`: 64-bit compilation
- `-O2`: Optimization level 2
- `-DUNICODE -D_UNICODE`: Unicode support for Windows

## Usage

### Running the Application
```batch
.\bin\GA_Demo.exe
```

### Main Menu Options
1. **Run with Default Settings**: Quick demonstration with sinusoidal function
2. **Configure Parameters**: Customize all genetic algorithm parameters
3. **Select Fitness Function**: Choose from available mathematical functions
4. **View GA Explanation**: Educational overview of genetic algorithms
5. **About**: Application and author information
6. **Exit**: Close the application

### Detailed Demonstration Mode
When selecting a fitness function, the application offers a detailed demonstration that shows:
- Step-by-step algorithm initialization
- Population creation and evaluation
- Real-time evolution monitoring
- Detailed statistics and convergence analysis
- Mathematical verification of results

## Project Structure

```
GA_Cpp/
├── src/
│   ├── GeneticTypes.h          # Type definitions and configuration
│   ├── Individual.h/cpp        # Individual chromosome implementation
│   ├── FitnessFunction.h/cpp   # Fitness function hierarchy
│   ├── GeneticAlgorithm.h/cpp  # Main GA implementation
│   ├── ConsoleInterface.h/cpp  # Windows console interface
│   └── main.cpp                # Main application logic
├── bin/                        # Compiled executable output
├── docs/                       # Documentation (reserved)
├── build.bat                   # Build script
└── README.md                   # This file
```

## Educational Objectives

This implementation demonstrates:

### 1. Genetic Algorithm Fundamentals
- **Chromosome Encoding**: Binary representation of solution candidates
- **Population Dynamics**: How populations evolve over generations
- **Fitness Evaluation**: Mathematical function optimization
- **Selection Pressure**: How better solutions are favored

### 2. Genetic Operators
- **Selection Methods**: Different strategies for parent selection
- **Crossover Mechanisms**: How genetic material is exchanged
- **Mutation Effects**: Maintaining population diversity
- **Elitism Benefits**: Preserving good solutions

### 3. Algorithm Analysis
- **Convergence Tracking**: Population diversity measurement
- **Performance Metrics**: Fitness statistics across generations
- **Parameter Sensitivity**: Impact of different configuration values
- **Real-time Monitoring**: Evolution visualization

### 4. Mathematical Optimization
- **Function Landscapes**: Different optimization challenges
- **Local vs Global Optima**: How GAs handle complex search spaces
- **Maximization vs Minimization**: Bidirectional optimization
- **Domain Constraints**: Working within defined value ranges

## Technical Specifications

- **Language**: C++17
- **Platform**: Windows (utilizing windows.h for enhanced console features)
- **Compiler**: GCC with MinGW-w64
- **Dependencies**: Standard C++ library only
- **Architecture**: Object-oriented with clear component separation

## License and Usage

This project is developed for educational purposes as part of the Artificial Intelligence course at Universidad de Guanajuato - DICIS. The code demonstrates genetic algorithm concepts and is intended for learning and academic use.

## Author Information

**Andres Torres Ceja**  
Student at Universidad de Guanajuato  
Sede DICIS (División de Ingenierías)  
Artificial Intelligence Course  

## Roadmap
- Implement additional fitness functions
- Add GUI version using a cross-platform library
- Explore parallelization for performance improvement
- Completely update all the UI to be more user friendly and esthetic and modern