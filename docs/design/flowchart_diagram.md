# Diagrama de Flujo - Proceso Evolutivo

Este diagrama muestra el flujo detallado del proceso evolutivo del algoritmo genético.

```mermaid
flowchart TD
    Start([Inicio del Algoritmo]) --> Init[Inicializar población aleatoria]
    Init --> SetGen[Generación = 0]
    
    SetGen --> EvalPop[Evaluar población]
    EvalPop --> CalcFit[Calcular fitness de cada individuo]
    CalcFit --> CalcPerc[Convertir fitness a porcentaje]
    CalcPerc --> UpdateStats[Actualizar estadísticas de generación]
    UpdateStats --> ShowProgress[Mostrar progreso en tiempo real]
    
    ShowProgress --> CheckTerm{¿Criterio de terminación cumplido?}
    
    CheckTerm -->|Máx generaciones| TermMax[Terminación por generaciones]
    CheckTerm -->|Convergencia| TermConv[Terminación por convergencia]
    CheckTerm -->|Fitness objetivo| TermFit[Terminación por fitness]
    CheckTerm -->|Tiempo límite| TermTime[Terminación por tiempo]
    
    TermMax --> Final[Generar reporte final]
    TermConv --> Final
    TermFit --> Final
    TermTime --> Final
    
    CheckTerm -->|No| Selection[Proceso de Selección]
    
    Selection --> TournamentSel[Selección por Torneo]
    Selection --> RouletteSel[Selección por Ruleta]
    Selection --> RankSel[Selección por Ranking]
    
    TournamentSel --> Crossover[Operador de Cruzamiento]
    RouletteSel --> Crossover
    RankSel --> Crossover
    
    Crossover --> SinglePoint[Cruzamiento de un punto]
    Crossover --> TwoPoint[Cruzamiento de dos puntos]
    Crossover --> Uniform[Cruzamiento uniforme]
    
    SinglePoint --> Mutation[Operador de Mutación]
    TwoPoint --> Mutation
    Uniform --> Mutation
    
    Mutation --> BitFlip[Mutación bit-flip]
    BitFlip --> CheckMutRate{¿Aplicar mutación?}
    CheckMutRate -->|Sí| FlipBit[Invertir bit aleatorio]
    CheckMutRate -->|No| SkipMut[Sin mutación]
    
    FlipBit --> Elitism
    SkipMut --> Elitism[Aplicar Elitismo]
    
    Elitism --> PreserveBest[Preservar mejores individuos]
    PreserveBest --> ReplacePopulation[Reemplazar población]
    ReplacePopulation --> IncGen[Incrementar generación]
    
    IncGen --> EvalPop
    
    Final --> DisplayResults[Mostrar resultados finales]
    DisplayResults --> ShowBest[Mostrar mejor individuo]
    ShowBest --> ShowStats[Mostrar estadísticas]
    ShowStats --> End([Fin])
    
    %% Estilos
    classDef processClass fill:#e1f5fe,stroke:#0277bd,stroke-width:2px
    classDef decisionClass fill:#fff3e0,stroke:#f57c00,stroke-width:2px
    classDef terminationClass fill:#f3e5f5,stroke:#7b1fa2,stroke-width:2px
    classDef operatorClass fill:#e8f5e8,stroke:#388e3c,stroke-width:2px
    
    class Init,SetGen,EvalPop,CalcFit,CalcPerc,UpdateStats,ShowProgress processClass
    class CheckTerm,CheckMutRate decisionClass
    class TermMax,TermConv,TermFit,TermTime,Final terminationClass
    class Selection,TournamentSel,RouletteSel,RankSel,Crossover,SinglePoint,TwoPoint,Uniform,Mutation,BitFlip,Elitism operatorClass
```

## Detalles del Proceso

### Inicialización
- **Población Aleatoria**: Se generan individuos con cromosomas binarios aleatorios
- **Tamaño Configurable**: El tamaño de población es definido por el usuario
- **Rango de Valores**: Los individuos representan valores en el rango [minValue, maxValue]

### Evaluación
- **Decodificación**: Conversión de binario a decimal
- **Función Objetivo**: Aplicación de la función de fitness seleccionada
- **Sistema de Porcentajes**: Conversión a escala 0-100% para mejor comprensión

### Criterios de Terminación
1. **Máximo de Generaciones**: Límite superior de iteraciones
2. **Convergencia**: Estabilización del fitness promedio
3. **Fitness Objetivo**: Alcanzar un valor específico
4. **Tiempo Límite**: Duración máxima de ejecución

### Operadores Genéticos

#### Selección
- **Torneo**: Competencia entre k individuos aleatorios
- **Ruleta**: Probabilidad proporcional al fitness
- **Ranking**: Basado en posición ordenada

#### Cruzamiento
- **Un Punto**: División en punto aleatorio
- **Dos Puntos**: División en dos puntos aleatorios
- **Uniforme**: Intercambio bit por bit

#### Mutación
- **Bit-Flip**: Inversión aleatoria de bits
- **Tasa Configurable**: Probabilidad de mutación por bit

#### Elitismo
- **Preservación**: Mantener mejores individuos
- **Porcentaje Configurable**: Fracción de elite a preservar