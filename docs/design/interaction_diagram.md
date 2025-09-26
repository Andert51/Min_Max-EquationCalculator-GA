# Diagrama de Interacciones - Operadores Genéticos

Este diagrama muestra las interacciones entre componentes durante la ejecución de los operadores genéticos.

```mermaid
sequenceDiagram
    participant GA as GeneticAlgorithm
    participant Pop as Population
    participant Sel as SelectionOperator
    participant Cross as CrossoverOperator
    participant Mut as MutationOperator
    participant Elite as ElitismManager
    participant I1 as Individual1
    participant I2 as Individual2
    participant C1 as Child1
    participant C2 as Child2
    participant FF as FitnessFunction
    
    Note over GA: Inicio de Generación N
    
    GA->>Pop: getCurrentPopulation()
    Pop->>GA: return population
    
    Note over GA,FF: Fase de Evaluación
    loop Para cada individuo en población
        GA->>I1: getValue()
        I1->>GA: return decodedValue
        GA->>FF: evaluate(decodedValue)
        FF->>GA: return fitness
        GA->>I1: setFitness(fitness)
        GA->>FF: calculateFitnessPercentage(fitness)
        FF->>GA: return percentage
        GA->>I1: setFitnessPercentage(percentage)
    end
    
    Note over GA,Elite: Fase de Selección
    GA->>Sel: selectParents(population, config)
    activate Sel
    
    alt Tournament Selection
        loop Realizar torneos
            Sel->>Pop: getRandomIndividuals(tournamentSize)
            Pop->>Sel: return competitors
            Sel->>Sel: selectBestFromTournament(competitors)
        end
    else Roulette Wheel Selection
        Sel->>Pop: calculateFitnessSum()
        Pop->>Sel: return totalFitness
        Sel->>Sel: generateRandomSelections(totalFitness)
    else Rank Selection
        Sel->>Pop: sortByFitness()
        Pop->>Sel: return sortedPopulation
        Sel->>Sel: selectByRank(sortedPopulation)
    end
    
    Sel->>GA: return selectedParents
    deactivate Sel
    
    Note over GA,C2: Fase de Cruzamiento
    GA->>Cross: applyCrossover(selectedParents, crossoverRate)
    activate Cross
    
    loop Para cada par de padres
        Cross->>I1: getChromosome()
        I1->>Cross: return chromosome1
        Cross->>I2: getChromosome()
        I2->>Cross: return chromosome2
        
        alt Single Point Crossover
            Cross->>Cross: selectRandomCrossoverPoint()
            Cross->>Cross: swapGeneticMaterial(point)
        else Two Point Crossover
            Cross->>Cross: selectTwoCrossoverPoints()
            Cross->>Cross: swapMiddleSegment(point1, point2)
        else Uniform Crossover
            Cross->>Cross: crossoverEachBitIndependently()
        end
        
        Cross->>C1: <<create>> newIndividual(crossedChromosome1)
        Cross->>C2: <<create>> newIndividual(crossedChromosome2)
    end
    
    Cross->>GA: return offspring
    deactivate Cross
    
    Note over GA,C2: Fase de Mutación
    GA->>Mut: applyMutation(offspring, mutationRate)
    activate Mut
    
    loop Para cada hijo
        loop Para cada bit en cromosoma
            Mut->>Mut: generateRandomProbability()
            alt Probabilidad < mutationRate
                Mut->>C1: flipGene(bitIndex)
                C1->>Mut: bitFlipped
            else
                Note over Mut: No mutation applied
            end
        end
    end
    
    Mut->>GA: return mutatedOffspring
    deactivate Mut
    
    Note over GA,C2: Re-evaluación de Descendencia
    loop Para cada hijo mutado
        GA->>C1: calculateValue(minVal, maxVal)
        C1->>GA: valueCalculated
        GA->>C1: getValue()
        C1->>GA: return newValue
        GA->>FF: evaluate(newValue)
        FF->>GA: return newFitness
        GA->>C1: setFitness(newFitness)
        GA->>FF: calculateFitnessPercentage(newFitness)
        FF->>GA: return newPercentage
        GA->>C1: setFitnessPercentage(newPercentage)
    end
    
    Note over GA,Elite: Fase de Elitismo y Reemplazo
    GA->>Elite: preserveElite(currentPopulation, elitismRate)
    activate Elite
    Elite->>Pop: getBestIndividuals(eliteCount)
    Pop->>Elite: return eliteIndividuals
    Elite->>GA: return preservedElite
    deactivate Elite
    
    GA->>Pop: createNewGeneration(mutatedOffspring, preservedElite)
    activate Pop
    Pop->>Pop: combinePopulations()
    Pop->>Pop: sortByFitness()
    Pop->>Pop: selectBestIndividuals(populationSize)
    Pop->>GA: newGenerationReady
    deactivate Pop
    
    GA->>GA: incrementGeneration()
    GA->>GA: calculateGenerationStatistics()
    
    Note over GA: Generación N+1 Lista
```

## Operadores Genéticos Detallados

### Selección
- **Tournament**: Competencia entre k individuos aleatorios
- **Roulette Wheel**: Probabilidad proporcional al fitness
- **Rank**: Selección basada en ranking ordenado

### Cruzamiento
- **Single Point**: Intercambio en un punto aleatorio
- **Two Point**: Intercambio entre dos puntos
- **Uniform**: Intercambio independiente por bit

### Mutación
- **Bit-Flip**: Inversión de bits con probabilidad configurable
- **Rate Control**: Tasa de mutación aplicada por bit

### Elitismo
- **Preservation**: Mantener mejores individuos de generación anterior
- **Elite Count**: Número basado en porcentaje de elitismo

## Flujo de Datos Clave

### Evaluación de Fitness
1. Decodificación de cromosoma a valor real
2. Aplicación de función de fitness
3. Conversión a porcentaje (0-100%)
4. Actualización de estadísticas

### Creación de Nueva Generación
1. Selección de padres basada en fitness
2. Cruzamiento para generar descendencia
3. Mutación de descendencia
4. Re-evaluación de fitness
5. Aplicación de elitismo
6. Reemplazo de población

### Control de Calidad
- Preservación de diversidad genética
- Mantenimiento de mejores soluciones
- Balance entre exploración y explotación