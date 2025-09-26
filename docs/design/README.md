# Índice de Diagramas UML - Proyecto Algoritmos Genéticos

Este documento proporciona una guía completa de todos los diagramas UML disponibles para el proyecto de algoritmos genéticos, implementado en C++17.

---

## 📁 Estructura de Diagramas

```
docs/design/
├── class_diagram.md           # Diagrama de Clases Principal
├── sequence_diagram.md        # Diagrama de Secuencia - Ejecución
├── flowchart_diagram.md       # Diagrama de Flujo - Proceso Evolutivo
├── state_diagram.md           # Diagrama de Estados - Ciclo de Vida
├── architecture_diagram.md    # Diagrama de Arquitectura - Componentes
├── interaction_diagram.md     # Diagrama de Interacciones - Operadores
├── use_case_diagram.md        # Diagrama de Casos de Uso
└── README.md                  # Este archivo de índice
```

---

## 🎯 Guía de Diagramas por Propósito

### Para Entender la Estructura del Código
1. **[Diagrama de Clases](./class_diagram.md)** 🏗️
   - **Propósito**: Visualizar todas las clases, sus relaciones y jerarquías
   - **Cuándo usar**: Cuando necesites entender la estructura orientada a objetos
   - **Incluye**: Atributos, métodos, herencia, composición, agregación

2. **[Diagrama de Arquitectura](./architecture_diagram.md)** 🏛️
   - **Propósito**: Vista de alto nivel de componentes y subsistemas
   - **Cuándo usar**: Para entender la organización general del sistema
   - **Incluye**: Módulos, dependencias, patrones arquitectónicos

### Para Entender el Flujo de Ejecución
3. **[Diagrama de Secuencia](./sequence_diagram.md)** ⏯️
   - **Propósito**: Flujo temporal de ejecución del algoritmo completo
   - **Cuándo usar**: Para seguir el proceso desde inicio hasta fin
   - **Incluye**: Interacciones usuario-sistema, llamadas de métodos, tiempo

4. **[Diagrama de Flujo](./flowchart_diagram.md)** 🔄
   - **Propósito**: Proceso evolutivo paso a paso con decisiones
   - **Cuándo usar**: Para entender la lógica del algoritmo genético
   - **Incluye**: Operadores genéticos, criterios de terminación, bucles

5. **[Diagrama de Interacciones](./interaction_diagram.md)** 🤝
   - **Propósito**: Comunicación detallada entre operadores genéticos
   - **Cuándo usar**: Para entender cómo funcionan selección, cruzamiento, mutación
   - **Incluye**: Secuencias de operadores, flujo de datos, evaluaciones

### Para Entender Estados y Comportamientos
6. **[Diagrama de Estados](./state_diagram.md)** 🔄
   - **Propósito**: Ciclo de vida de un individuo en la población
   - **Cuándo usar**: Para entender las transiciones de estado
   - **Incluye**: Estados de vida, transiciones, condiciones

### Para Entender Funcionalidades
7. **[Diagrama de Casos de Uso](./use_case_diagram.md)** 👥
   - **Propósito**: Funcionalidades desde perspectiva del usuario
   - **Cuándo usar**: Para entender qué puede hacer cada tipo de usuario
   - **Incluye**: Actores, casos de uso, relaciones, extensiones

---

## 🎨 Tecnologías de Diagramas Utilizadas

### Mermaid
- **Formato**: Texto plano en markdown
- **Ventajas**: Fácil edición, versionado con Git, visualización en GitHub/VS Code
- **Uso**: Todos los diagramas están en formato Mermaid

### Herramientas de Visualización
1. **GitHub/GitLab**: Renderizado automático de diagramas Mermaid
2. **VS Code**: Extensión "Markdown Preview Mermaid Support"
3. **Mermaid Live Editor**: https://mermaid.live/
4. **Obsidian**: Plugin de Mermaid
5. **Notion**: Soporte nativo de Mermaid

---

## 🚀 Cómo Usar los Diagramas

### 1. Visualización Directa
```bash
# En VS Code con extensión Mermaid
Ctrl+Shift+V  # Preview de markdown con diagramas

# En GitHub
# Los diagramas se renderizan automáticamente
```

### 2. Exportar como Imágenes
```bash
# Usando Mermaid CLI
npm install -g @mermaid-js/mermaid-cli
mmdc -i diagram.md -o diagram.png

# Usando VS Code
# Click derecho en diagram → Export as PNG/SVG
```

### 3. Edición en Línea
- Abrir https://mermaid.live/
- Copiar código del diagrama
- Editar en tiempo real
- Descargar resultado

---

## 📋 Checklist de Comprensión

### Antes de Modificar el Código
- [ ] Revisar **Diagrama de Clases** para entender estructura
- [ ] Consultar **Diagrama de Arquitectura** para ver dependencias
- [ ] Verificar **Diagrama de Secuencia** para impacto en flujo

### Para Agregar Nuevas Funcionalidades
- [ ] Consultar **Diagrama de Casos de Uso** para identificar actores
- [ ] Revisar **Diagrama de Estados** para entender ciclo de vida
- [ ] Verificar **Diagrama de Interacciones** para integración

### Para Debugging y Mantenimiento
- [ ] Usar **Diagrama de Flujo** para seguir lógica de ejecución
- [ ] Consultar **Diagrama de Secuencia** para orden de llamadas
- [ ] Revisar **Diagrama de Estados** para transiciones válidas

---

## 🔧 Mantenimiento de Diagramas

### Cuándo Actualizar
- ✅ **Agregar nuevas clases**: Actualizar Diagrama de Clases
- ✅ **Modificar flujo**: Actualizar Diagramas de Secuencia/Flujo
- ✅ **Nuevas funcionalidades**: Actualizar Casos de Uso
- ✅ **Cambios arquitectónicos**: Actualizar Diagrama de Arquitectura

### Convenciones de Nomenclatura
```
class_diagram.md           # Clases y relaciones
sequence_diagram.md        # Flujos temporales
flowchart_diagram.md       # Procesos y decisiones
state_diagram.md           # Estados y transiciones
architecture_diagram.md    # Componentes y módulos
interaction_diagram.md     # Comunicaciones detalladas
use_case_diagram.md        # Funcionalidades de usuario
```

---

## 📊 Estadísticas del Proyecto

### Cobertura de Documentación
- ✅ **Estructura de Clases**: 100% documentada
- ✅ **Flujo de Ejecución**: 100% documentado
- ✅ **Operadores Genéticos**: 100% documentados
- ✅ **Interface de Usuario**: 100% documentada
- ✅ **Casos de Uso**: 100% cubiertos

### Patrones de Diseño Identificados
- 🎯 **Strategy Pattern**: Jerarquía de FitnessFunction
- 👁️ **Observer Pattern**: Sistema de callbacks de progreso
- 🏭 **Factory Pattern**: Creación de funciones de fitness
- 📋 **Template Method**: Estructura del algoritmo genético
- 🛡️ **RAII**: Gestión automática de recursos

---

## 🔗 Enlaces Útiles

### Documentación del Proyecto
- [Documentación Completa](../Documentacion_Completa.md)
- [Reporte IMRAD](../Reporte_IMRAD_Algoritmos_Geneticos.md)
- [Diagramas UML (PlantUML)](../Diagramas_UML.md)

### Recursos Externos
- [Mermaid Documentation](https://mermaid-js.github.io/mermaid/)
- [UML Diagrams Guide](https://www.uml-diagrams.org/)
- [C++17 Reference](https://en.cppreference.com/w/cpp/17)

---

## 🎯 Próximos Pasos

### Diagramas Futuros
- [ ] **Diagrama de Deployment**: Para distribución del sistema
- [ ] **Diagrama de Paquetes**: Para organización de módulos
- [ ] **Diagrama de Comunicación**: Para colaboraciones entre objetos
- [ ] **Diagrama de Timing**: Para comportamiento temporal crítico

### Mejoras Sugeridas
- [ ] Agregar diagramas interactivos
- [ ] Incluir métricas de rendimiento
- [ ] Documentar patrones de optimización
- [ ] Crear guías de extensión del sistema

---

*Última actualización: Septiembre 22, 2025*