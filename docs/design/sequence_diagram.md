# Diagrama de Secuencia - Ejecución del Algoritmo Genético

Este diagrama muestra el flujo completo de ejecución del algoritmo genético desde la inicialización hasta la obtención de resultados.

```mermaid
sequenceDiagram
    participant U as Usuario
    participant M as Main
    participant CI as ConsoleInterface
    participant GA as GeneticAlgorithm
    participant FF as FitnessFunction
    participant I as Individual
    
    U->>M: Ejecutar programa
    activate M
    
    M->>CI: displayMainMenu()
    activate CI
    CI->>U: Mostrar opciones del menú
    U->>CI: Seleccionar opción (ej: 3 - Función personalizada)
    
    CI->>CI: getFitnessFunction()
    CI->>U: Solicitar parámetros de función
    U->>CI: Proporcionar parámetros (a, b)
    CI->>FF: new LinearFunction(a, b, maximize)
    activate FF
    
    CI->>CI: getConfigurationFromUser()
    CI->>U: Solicitar configuración del AG
    U->>CI: Proporcionar configuración
    CI->>M: Retornar config y función
    deactivate CI
    
    M->>GA: new GeneticAlgorithm(config, fitnessFunction)
    activate GA
    
    M->>GA: run(algorithmProgressCallback)
    
    Note over GA: Inicialización
    GA->>GA: initializePopulation()
    loop Para cada individuo
        GA->>I: new Individual(length, minVal, maxVal)
        activate I
        I->>I: Generar cromosoma aleatorio
        I->>I: calculateValue(minVal, maxVal)
        I->>GA: Retornar individuo inicializado
        deactivate I
    end
    
    Note over GA: Bucle Evolutivo Principal
    loop Mientras no se cumpla criterio de terminación
        Note over GA: Evaluación
        GA->>GA: evaluatePopulation()
        loop Para cada individuo
            GA->>I: getValue()
            I->>GA: Retornar valor decodificado
            GA->>FF: evaluate(valor)
            FF->>GA: Retornar fitness
            GA->>I: setFitness(fitness)
            GA->>FF: calculateFitnessPercentage(fitness)
            FF->>GA: Retornar porcentaje (0-100%)
            GA->>I: setFitnessPercentage(porcentaje)
        end
        
        GA->>GA: calculateGenerationStats()
        GA->>M: algorithmProgressCallback(generation, stats)
        M->>U: Mostrar progreso en tiempo real
        
        alt Criterio de terminación NO cumplido
            Note over GA: Operadores Genéticos
            GA->>GA: performSelection()
            Note right of GA: Selección por torneo
            
            GA->>GA: performCrossover(parents)
            Note right of GA: Cruzamiento de un punto
            
            GA->>GA: performMutation(offspring)
            Note right of GA: Mutación bit-flip
            
            GA->>GA: performElitism(newPopulation)
            Note right of GA: Preservar mejores individuos
            
            GA->>GA: Incrementar generación
        else Criterio de terminación cumplido
            Note over GA: Fin de la evolución
        end
    end
    
    GA->>M: Retornar estadísticas finales
    deactivate GA
    
    M->>CI: displayFinalResults(statistics, bestIndividual, config, fitnessFunction)
    activate CI
    CI->>U: Mostrar resultados finales
    CI->>U: Mostrar mejor individuo encontrado
    CI->>U: Mostrar estadísticas de convergencia
    CI->>U: waitForKeyPress("Presione cualquier tecla...")
    U->>CI: Presionar tecla
    deactivate CI
    
    M->>U: Regresar al menú principal
    deactivate M
    deactivate FF
```

## Aspectos Destacados del Flujo

### Inicialización
- **Población Aleatoria**: Cada individuo se crea con cromosomas binarios aleatorios
- **Decodificación**: Los cromosomas se convierten a valores reales en el rango [minValue, maxValue]

### Evaluación
- **Fitness Calculation**: Se calcula el fitness usando la función objetivo seleccionada
- **Percentage System**: El fitness se convierte a porcentaje (0-100%) para mejor comprensión

### Operadores Genéticos
1. **Selección**: Torneo, ruleta o ranking
2. **Cruzamiento**: Un punto, dos puntos o uniforme
3. **Mutación**: Bit-flip con probabilidad configurable
4. **Elitismo**: Preservación de los mejores individuos

### Criterios de Terminación
- Número máximo de generaciones alcanzado
- Convergencia del fitness
- Tiempo máximo de ejecución
- Fitness objetivo alcanzado

### Visualización en Tiempo Real
- Progreso generación por generación
- Estadísticas de fitness (mejor, promedio, peor)
- Percentajes intuitivos para el usuario
- Información de convergencia