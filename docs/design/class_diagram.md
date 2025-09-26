# Diagrama de Clases - Algoritmos Genéticos

Este diagrama muestra la estructura principal de clases del proyecto de algoritmos genéticos.

```mermaid
classDiagram
    class GeneticAlgorithm {
        -GAConfig config
        -Population population
        -unique_ptr~FitnessFunction~ fitnessFunction
        -mt19937 rng
        -vector~GenerationStats~ statistics
        -int currentGeneration
        -ProgressCallback progressCallback
        
        +GeneticAlgorithm(config, func)
        +run() GenerationStats
        +run(callback) GenerationStats
        +getBestIndividual() Individual
        +getCurrentGeneration() int
        +getStatistics() vector~GenerationStats~
        +getConfig() GAConfig
        +getFitnessFunction() FitnessFunction&
        
        -initializePopulation() void
        -evaluatePopulation() void
        -performSelection() Population
        -performCrossover(parents) Population
        -performMutation(offspring) void
        -performElitism(newPop) void
        -calculateGenerationStats() GenerationStats
        -shouldTerminate() bool
    }
    
    class Individual {
        -vector~bool~ chromosome
        -double value
        -double fitness
        -double fitnessPercentage
        
        +Individual()
        +Individual(length, minVal, maxVal)
        +calculateValue(minVal, maxVal) void
        +getChromosome() vector~bool~
        +getValue() double
        +getFitness() double
        +getFitnessPercentage() double
        +setFitness(fitness) void
        +setFitnessPercentage(percentage) void
        +getGene(index) bool
        +setGene(index, value) void
        +flipGene(index) void
        +getChromosomeLength() int
        +toString() string
    }
    
    class FitnessFunction {
        <<abstract>>
        #string name
        #string expression
        #bool isMaximization
        #double bestKnownValue
        #double worstKnownValue
        
        +FitnessFunction(name, expr, maximize)
        +evaluate(x)* double
        +clone()* unique_ptr~FitnessFunction~
        +getName() string
        +getExpression() string
        +isMaximization() bool
        +calculateFitnessPercentage(fitness) double
        +getTheoreticalRange(minVal, maxVal) pair~double,double~
        +getOptimalValue() double
        +getOptimalX() double
        +updateKnownRange(fitness) void
    }
    
    class LinearFunction {
        -double a
        -double b
        
        +LinearFunction(a, b, maximize)
        +evaluate(x) double
        +clone() unique_ptr~FitnessFunction~
    }
    
    class QuadraticFunction {
        -double a
        -double b
        -double c
        
        +QuadraticFunction(a, b, c, maximize)
        +evaluate(x) double
        +getOptimalX() double
        +getOptimalValue() double
        +clone() unique_ptr~FitnessFunction~
    }
    
    class SinusoidalFunction {
        -double A
        -double B
        -double C
        -double D
        
        +SinusoidalFunction(A, B, C, D, maximize)
        +evaluate(x) double
        +clone() unique_ptr~FitnessFunction~
    }
    
    class RastriginFunction {
        -double A
        
        +RastriginFunction(A, maximize)
        +evaluate(x) double
        +getOptimalValue() double
        +getOptimalX() double
        +clone() unique_ptr~FitnessFunction~
    }
    
    class ConsoleInterface {
        +clearScreen() void
        +setColor(color) void
        +resetColor() void
        +displaySeparator(length, char) void
        +displayMainMenu() void
        +getMenuChoice() int
        +displayConfiguration(config, func) void
        +displayPopulation(pop, config, func, showAll) void
        +displayGenerationStats(stats) void
        +displayFinalResults(stats, best, config, func) void
        +getFitnessFunction() unique_ptr~FitnessFunction~
        +getConfigurationFromUser() GAConfig
        +getYesNoInput(prompt) bool
        +waitForKeyPress(message) void
    }
    
    class GAConfig {
        +int populationSize
        +int chromosomeLength
        +int maxGenerations
        +double crossoverRate
        +double mutationRate
        +double elitismRate
        +bool isMaximization
        +SelectionType selectionType
        +CrossoverType crossoverType
        +int tournamentSize
        +double minValue
        +double maxValue
    }
    
    class GenerationStats {
        +int generation
        +double bestFitness
        +double averageFitness
        +double worstFitness
        +double bestFitnessPercentage
        +double averageFitnessPercentage
        +double bestValue
        +double convergence
    }
    
    class SelectionType {
        <<enumeration>>
        TOURNAMENT
        ROULETTE_WHEEL
        RANK_BASED
    }
    
    class CrossoverType {
        <<enumeration>>
        SINGLE_POINT
        TWO_POINT
        UNIFORM
    }
    
    class Color {
        <<enumeration>>
        DEFAULT
        INFO
        SUCCESS
        WARNING
        ERR
        HEADER
    }
    
    %% Relaciones
    GeneticAlgorithm *-- Individual : contains
    GeneticAlgorithm *-- FitnessFunction : uses
    GeneticAlgorithm *-- GAConfig : has
    GeneticAlgorithm *-- GenerationStats : generates
    
    FitnessFunction <|-- LinearFunction : extends
    FitnessFunction <|-- QuadraticFunction : extends
    FitnessFunction <|-- SinusoidalFunction : extends
    FitnessFunction <|-- RastriginFunction : extends
    
    ConsoleInterface ..> GeneticAlgorithm : uses
    ConsoleInterface ..> Individual : displays
    ConsoleInterface ..> GAConfig : configures
    ConsoleInterface ..> FitnessFunction : selects
    
    GAConfig ..> SelectionType : uses
    GAConfig ..> CrossoverType : uses
    ConsoleInterface ..> Color : uses
```

## Notas del Diagrama

- **GeneticAlgorithm**: Clase principal que coordina todo el proceso evolutivo
- **Individual**: Representa un individuo en la población con su cromosoma binario
- **FitnessFunction**: Jerarquía de funciones para diferentes problemas de optimización
- **ConsoleInterface**: Maneja toda la interacción con el usuario
- **GAConfig**: Estructura de configuración con todos los parámetros del algoritmo
- **GenerationStats**: Almacena estadísticas de cada generación

## Patrones de Diseño Implementados

1. **Strategy Pattern**: Jerarquía de FitnessFunction
2. **Observer Pattern**: Callback de progreso
3. **Template Method**: Estructura del algoritmo genético
4. **Factory Pattern**: Creación de funciones de fitness