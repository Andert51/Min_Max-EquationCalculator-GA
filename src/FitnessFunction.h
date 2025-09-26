#ifndef FITNESS_FUNCTION_H
#define FITNESS_FUNCTION_H

#include "GeneticTypes.h"
#include "Individual.h"
#include <functional>
#include <string>
#include <cmath>
#include <memory>
#include <utility>

namespace GA {
    /**
     * @class FitnessFunction
     * @brief Abstract base class for fitness function evaluation
     * 
     * This class provides a framework for implementing different mathematical
     * functions to be optimized by the genetic algorithm. It supports both
     * maximization and minimization problems.
     */
    class FitnessFunction {
    protected:
        std::string functionName;       // Name/description of the function
        std::string functionExpression; // Mathematical expression as string
        bool isMaximization;           // True for maximization, false for minimization
        
    public:
        // Constructor
        FitnessFunction(const std::string& name, const std::string& expression, bool maximize = true);
        
        // Virtual destructor
        virtual ~FitnessFunction() = default;
        
        /**
         * @brief Creates a copy of this fitness function
         * @return A unique pointer to a copy of this function
         */
        virtual std::unique_ptr<FitnessFunction> clone() const = 0;
        
        /**
         * @brief Pure virtual function to evaluate fitness for a given x value
         * @param x The input value to the function
         * @return The fitness value
         */
        virtual double evaluate(double x) const = 0;
        
        /**
         * @brief Evaluates fitness for an individual
         * @param individual The individual to evaluate
         * @param minValue Minimum value of the domain
         * @param maxValue Maximum value of the domain
         * @return The fitness value
         */
        FitnessValue evaluateIndividual(const Individual& individual, double minValue, double maxValue) const;
        
        /**
         * @brief Gets the function name
         * @return The function name
         */
        const std::string& getName() const;
        
        /**
         * @brief Gets the function expression
         * @return The mathematical expression as string
         */
        const std::string& getExpression() const;
        
        /**
         * @brief Checks if this is a maximization problem
         * @return True if maximization, false if minimization
         */
        bool isMaximizationProblem() const;
        
        /**
         * @brief Gets the theoretical optimum value (if known)
         * @return The optimal value, or NaN if unknown
         */
        virtual double getOptimalValue() const;
        
        /**
         * @brief Gets the theoretical optimum x position (if known)
         * @return The optimal x position, or NaN if unknown
         */
        virtual double getOptimalX() const;
        
        /**
         * @brief Calculates fitness as a percentage (0-100%)
         * @param fitnessValue The raw fitness value to convert
         * @param bestKnownValue The best fitness value found so far (for reference)
         * @param worstKnownValue The worst fitness value found so far (for reference)
         * @return Fitness percentage (0-100%)
         */
        virtual double calculateFitnessPercentage(double fitnessValue, double bestKnownValue, double worstKnownValue) const;
        
        /**
         * @brief Gets the theoretical best possible fitness percentage reference
         * @param minValue Domain minimum value
         * @param maxValue Domain maximum value 
         * @return A pair of (theoretical_best, theoretical_worst) values for percentage calculation
         */
        virtual std::pair<double, double> getTheoreticalRange(double minValue, double maxValue) const;
    };
    
    /**
     * @class QuadraticFunction
     * @brief Implementation of f(x) = ax^2 + bx + c (for demonstration purposes)
     * Note: This is provided as an example but should not be used for the main assignment
     */
    class QuadraticFunction : public FitnessFunction {
    private:
        double a, b, c;  // Coefficients
        
    public:
        QuadraticFunction(double a_coeff, double b_coeff, double c_coeff, bool maximize = false);
        double evaluate(double x) const override;
        double getOptimalX() const override;
        double getOptimalValue() const override;
        std::unique_ptr<FitnessFunction> clone() const override;
    };
    
    /**
     * @class SinusoidalFunction  
     * @brief Implementation of f(x) = A * sin(B * x + C) + D
     * This is a good function for genetic algorithm demonstration
     */
    class SinusoidalFunction : public FitnessFunction {
    private:
        double A, B, C, D;  // Parameters
        
    public:
        SinusoidalFunction(double amplitude = 1.0, double frequency = 1.0, 
                          double phase = 0.0, double offset = 0.0, bool maximize = true);
        double evaluate(double x) const override;
        std::unique_ptr<FitnessFunction> clone() const override;
    };
    
    /**
     * @class RastriginFunction
     * @brief Implementation of modified 1D Rastrigin function
     * f(x) = A + x^2 - A * cos(2π * x)
     * This function has many local optima, making it challenging for optimization
     */
    class RastriginFunction : public FitnessFunction {
    private:
        double A;  // Parameter (typically 10)
        
    public:
        RastriginFunction(double A_param = 10.0, bool maximize = false);
        double evaluate(double x) const override;
        double getOptimalValue() const override;
        double getOptimalX() const override;
        std::unique_ptr<FitnessFunction> clone() const override;
    };
    
    /**
     * @class PolynomialFunction
     * @brief Implementation of f(x) = ax^3 + bx^2 + cx + d
     * A cubic polynomial function for optimization
     */
    class PolynomialFunction : public FitnessFunction {
    private:
        double a, b, c, d;  // Coefficients
        
    public:
        PolynomialFunction(double a_coeff, double b_coeff, double c_coeff, double d_coeff, bool maximize = true);
        double evaluate(double x) const override;
        std::unique_ptr<FitnessFunction> clone() const override;
    };
    
    /**
     * @class ExponentialFunction
     * @brief Implementation of f(x) = A * e^(B * x) + C
     * An exponential function for optimization
     */
    class ExponentialFunction : public FitnessFunction {
    private:
        double A, B, C;  // Parameters
        
    public:
        ExponentialFunction(double A_param = 1.0, double B_param = 0.1, double C_param = 0.0, bool maximize = true);
        double evaluate(double x) const override;
        std::unique_ptr<FitnessFunction> clone() const override;
    };
    
    /**
     * @class LinearFunction
     * @brief Implementation of f(x) = a * x + b
     * A simple linear function for basic optimization testing
     */
    class LinearFunction : public FitnessFunction {
    private:
        double a, b;  // Coefficients: slope and y-intercept
        
    public:
        LinearFunction(double a_coeff = 1.0, double b_coeff = 0.0, bool maximize = true);
        double evaluate(double x) const override;
        std::unique_ptr<FitnessFunction> clone() const override;
    };
    
    /**
     * @class CosineFunction
     * @brief Implementation of f(x) = A * cos(B * x + C) + D
     * A simple trigonometric function for optimization testing
     */
    class CosineFunction : public FitnessFunction {
    private:
        double A, B, C, D;  // Parameters: amplitude, frequency, phase, offset
        
    public:
        CosineFunction(double amplitude = 1.0, double frequency = 1.0, 
                      double phase = 0.0, double offset = 0.0, bool maximize = true);
        double evaluate(double x) const override;
        std::unique_ptr<FitnessFunction> clone() const override;
    };
}

#endif // FITNESS_FUNCTION_H