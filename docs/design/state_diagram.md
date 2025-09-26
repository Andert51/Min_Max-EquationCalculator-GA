# Diagrama de Estados - Ciclo de Vida de un Individuo

Este diagrama muestra los diferentes estados por los que pasa un individuo durante el proceso evolutivo.

```mermaid
stateDiagram-v2
    [*] --> Created : new Individual()
    
    Created --> Initialized : Generar cromosoma aleatorio
    
    state Initialized {
        state "Cromosoma Binario" as Binary
        state "Longitud Definida" as Length
        Binary --> Length
    }
    
    Initialized --> Decoded : calculateValue()
    
    state Decoded {
        state "Valor Real" as RealValue
        state "En Rango [min,max]" as InRange
        RealValue --> InRange
    }
    
    Decoded --> Evaluated : Calcular fitness
    
    state Evaluated {
        state "Fitness Calculado" as FitnessCalc
        state "Porcentaje Asignado" as PercentageAssigned
        FitnessCalc --> PercentageAssigned : calculateFitnessPercentage()
    }
    
    Evaluated --> InPopulation : Agregar a población
    
    state InPopulation {
        state "Parte de Generación N" as Generation
        state "Estadísticas Actualizadas" as StatsUpdated
        Generation --> StatsUpdated
    }
    
    InPopulation --> SelectionPhase : Proceso de selección
    
    state SelectionPhase {
        state "Participar en Torneo" as Tournament
        state "Evaluación de Fitness" as FitnessEval
        Tournament --> FitnessEval
        
        state Selected : Elegido como padre
        state NotSelected : No seleccionado
        
        FitnessEval --> Selected : fitness alto
        FitnessEval --> NotSelected : fitness bajo
    }
    
    Selected --> Reproduction : Cruzamiento
    
    state Reproduction {
        state "Cruzamiento" as Crossover
        state "Intercambio Genético" as GeneExchange
        Crossover --> GeneExchange
        
        state OffspringCreated : Descendencia creada
        GeneExchange --> OffspringCreated
    }
    
    Reproduction --> MutationPhase : Aplicar mutación
    
    state MutationPhase {
        state CheckMutation : ¿Mutar?
        state ApplyMutation : Invertir bits
        state NoMutation : Sin cambios
        
        CheckMutation --> ApplyMutation : probabilidad
        CheckMutation --> NoMutation : no mutar
        
        ApplyMutation --> MutationComplete
        NoMutation --> MutationComplete
        
        state MutationComplete : Mutación completada
    }
    
    MutationPhase --> NewEvaluation : Re-evaluar fitness
    
    NewEvaluation --> NextGeneration : Nueva generación
    
    state NextGeneration {
        state "Generación N+1" as NextGen
        state "Nuevas Estadísticas" as NewStats
        NextGen --> NewStats
    }
    
    NextGeneration --> InPopulation : Continuar evolución
    
    NotSelected --> Eliminated : Eliminado de población
    
    state ElitismCheck {
        state "Verificar Elite" as CheckElite
        state "Preservar Mejor" as PreserveBest
        state "Descartar" as Discard
        
        CheckElite --> PreserveBest : top performers
        CheckElite --> Discard : resto
    }
    
    InPopulation --> ElitismCheck : Aplicar elitismo
    PreserveBest --> NextGeneration : Garantizar supervivencia
    Discard --> Eliminated
    
    state TerminationCheck {
        state "Criterio Cumplido" as CriteriaMet
        state "Continuar Evolución" as ContinueEvolution
        
        CriteriaMet --> FinalResult
        ContinueEvolution --> SelectionPhase
    }
    
    NextGeneration --> TerminationCheck : Verificar terminación
    
    state FinalResult {
        state "Mejor Individuo" as BestIndividual
        state "Resultado Final" as FinalIndividual
        BestIndividual --> FinalIndividual
    }
    
    FinalResult --> [*] : Algoritmo terminado
    Eliminated --> [*] : Fin del ciclo de vida
    
    note right of Reproduction
        Solo individuos seleccionados
        participan en reproducción
    end note
    
    note left of Evaluated
        Cada generación requiere
        nueva evaluación de fitness
    end note
    
    note right of ElitismCheck
        Elitismo garantiza que los
        mejores individuos sobrevivan
    end note
```

## Estados Principales

### Estados de Inicialización
- **Created**: Individuo recién creado en memoria
- **Initialized**: Cromosoma binario generado aleatoriamente
- **Decoded**: Valor real calculado a partir del cromosoma

### Estados de Evaluación
- **Evaluated**: Fitness calculado y porcentaje asignado
- **InPopulation**: Forma parte de la población actual

### Estados de Selección
- **SelectionPhase**: Participación en proceso de selección
- **Selected**: Elegido como padre para reproducción
- **NotSelected**: No seleccionado para reproducción

### Estados de Reproducción
- **Reproduction**: Participación en cruzamiento
- **MutationPhase**: Aplicación de operador de mutación
- **NewEvaluation**: Re-evaluación después de modificaciones

### Estados de Supervivencia
- **NextGeneration**: Pasa a la siguiente generación
- **ElitismCheck**: Verificación para preservación elite
- **Eliminated**: Eliminado de la población

### Estados de Terminación
- **FinalResult**: Individuo en resultado final
- **TerminationCheck**: Verificación de criterios de parada

## Transiciones Importantes

### Flujo Principal
1. **Creación → Inicialización → Decodificación → Evaluación**
2. **Población → Selección → Reproducción → Mutación**
3. **Nueva Evaluación → Siguiente Generación → Ciclo**

### Flujos Alternativos
- **No Seleccionado → Eliminado** (sin reproducción)
- **Elite → Preservado** (sobrevive automáticamente)
- **Criterio Terminación → Resultado Final**

### Probabilidades
- **Selección**: Basada en fitness relativo
- **Mutación**: Tasa configurable por bit
- **Supervivencia**: Elitismo + nueva población