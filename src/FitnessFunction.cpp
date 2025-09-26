#include "FitnessFunction.h"
#include <cmath>
#include <stdexcept>
#include <limits>
#include <algorithm>

namespace GA {
    
    // Base FitnessFunction implementation
    FitnessFunction::FitnessFunction(const std::string& name, const std::string& expression, bool maximize)
        : functionName(name), functionExpression(expression), isMaximization(maximize) {
    }
    
    FitnessValue FitnessFunction::evaluateIndividual(const Individual& individual, double minValue, double maxValue) const {
        // Decode the individual's chromosome to a real value
        double x = individual.decodeToValue(minValue, maxValue);
        
        // Evaluate the function at this point
        return evaluate(x);
    }
    
    const std::string& FitnessFunction::getName() const {
        return functionName;
    }
    
    const std::string& FitnessFunction::getExpression() const {
        return functionExpression;
    }
    
    bool FitnessFunction::isMaximizationProblem() const {
        return isMaximization;
    }
    
    double FitnessFunction::getOptimalValue() const {
        return std::numeric_limits<double>::quiet_NaN();  // Unknown by default
    }
    
    double FitnessFunction::getOptimalX() const {
        return std::numeric_limits<double>::quiet_NaN();  // Unknown by default
    }
    
    double FitnessFunction::calculateFitnessPercentage(double fitnessValue, double bestKnownValue, double worstKnownValue) const {
        // If we have theoretical optimal values, use them
        if (!std::isnan(getOptimalValue())) {
            double optimalValue = getOptimalValue();
            
            if (isMaximization) {
                // For maximization: 100% when we reach optimal, 0% when we're far from it
                if (std::abs(optimalValue - fitnessValue) < 1e-10) return 100.0;
                
                // Use range from worst known to optimal
                double range = optimalValue - worstKnownValue;
                if (range > 1e-10) {
                    double percentage = ((fitnessValue - worstKnownValue) / range) * 100.0;
                    return std::max(0.0, std::min(100.0, percentage));
                }
            } else {
                // For minimization: 100% when we reach optimal (minimum), 0% when we're far from it
                if (std::abs(fitnessValue - optimalValue) < 1e-10) return 100.0;
                
                // Use range from worst known to optimal  
                double range = worstKnownValue - optimalValue;
                if (range > 1e-10) {
                    double percentage = ((worstKnownValue - fitnessValue) / range) * 100.0;
                    return std::max(0.0, std::min(100.0, percentage));
                }
            }
        }
        
        // Fallback: use empirical range from population
        double range = std::abs(bestKnownValue - worstKnownValue);
        if (range < 1e-10) return 50.0; // If all values are the same, assign 50%
        
        if (isMaximization) {
            double percentage = ((fitnessValue - worstKnownValue) / range) * 100.0;
            return std::max(0.0, std::min(100.0, percentage));
        } else {
            double percentage = ((worstKnownValue - fitnessValue) / range) * 100.0;
            return std::max(0.0, std::min(100.0, percentage));
        }
    }
    
    std::pair<double, double> FitnessFunction::getTheoreticalRange(double minValue, double maxValue) const {
        // Sample the function over the domain to estimate range
        const int samples = 1000;
        double step = (maxValue - minValue) / samples;
        
        double minFitness = std::numeric_limits<double>::max();
        double maxFitness = std::numeric_limits<double>::lowest();
        
        for (int i = 0; i <= samples; ++i) {
            double x = minValue + i * step;
            double fitness = evaluate(x);
            minFitness = std::min(minFitness, fitness);
            maxFitness = std::max(maxFitness, fitness);
        }
        
        if (isMaximization) {
            return std::make_pair(maxFitness, minFitness); // (best, worst)
        } else {
            return std::make_pair(minFitness, maxFitness); // (best, worst)
        }
    }
    
    // QuadraticFunction implementation (for demonstration only)
    QuadraticFunction::QuadraticFunction(double a_coeff, double b_coeff, double c_coeff, bool maximize)
        : FitnessFunction("Quadratic Function", 
                         "f(x) = " + std::to_string(a_coeff) + "x² + " + 
                         std::to_string(b_coeff) + "x + " + std::to_string(c_coeff), 
                         maximize),
          a(a_coeff), b(b_coeff), c(c_coeff) {
        
        // Note: This function is provided for educational purposes but should not be used
        // for the main assignment as it's specifically excluded (f(x) = x²)
    }
    
    double QuadraticFunction::evaluate(double x) const {
        return a * x * x + b * x + c;
    }
    
    double QuadraticFunction::getOptimalX() const {
        if (std::abs(a) < 1e-10) {
            return std::numeric_limits<double>::quiet_NaN();  // Linear function, no single optimum
        }
        return -b / (2.0 * a);
    }
    
    double QuadraticFunction::getOptimalValue() const {
        double optimalX = getOptimalX();
        if (std::isnan(optimalX)) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return evaluate(optimalX);
    }
    
    std::unique_ptr<FitnessFunction> QuadraticFunction::clone() const {
        return std::make_unique<QuadraticFunction>(a, b, c, isMaximization);
    }
    
    // SinusoidalFunction implementation
    SinusoidalFunction::SinusoidalFunction(double amplitude, double frequency, double phase, double offset, bool maximize)
        : FitnessFunction("Sinusoidal Function", 
                         "f(x) = " + std::to_string(amplitude) + " * sin(" + 
                         std::to_string(frequency) + " * x + " + std::to_string(phase) + 
                         ") + " + std::to_string(offset), 
                         maximize),
          A(amplitude), B(frequency), C(phase), D(offset) {
    }
    
    double SinusoidalFunction::evaluate(double x) const {
        return A * std::sin(B * x + C) + D;
    }
    
    std::unique_ptr<FitnessFunction> SinusoidalFunction::clone() const {
        return std::make_unique<SinusoidalFunction>(A, B, C, D, isMaximization);
    }
    
    // RastriginFunction implementation
    RastriginFunction::RastriginFunction(double A_param, bool maximize)
        : FitnessFunction("Rastrigin Function", 
                         "f(x) = " + std::to_string(A_param) + " + x² - " + 
                         std::to_string(A_param) + " * cos(2π * x)", 
                         maximize),
          A(A_param) {
    }
    
    double RastriginFunction::evaluate(double x) const {
        const double PI = 3.14159265358979323846;
        return A + x * x - A * std::cos(2.0 * PI * x);
    }
    
    double RastriginFunction::getOptimalValue() const {
        return 0.0;  // Global minimum is at x = 0
    }
    
    double RastriginFunction::getOptimalX() const {
        return 0.0;  // Global minimum is at x = 0
    }
    
    std::unique_ptr<FitnessFunction> RastriginFunction::clone() const {
        return std::make_unique<RastriginFunction>(A, isMaximization);
    }
    
    // PolynomialFunction implementation
    PolynomialFunction::PolynomialFunction(double a_coeff, double b_coeff, double c_coeff, double d_coeff, bool maximize)
        : FitnessFunction("Polynomial Function", 
                         "f(x) = " + std::to_string(a_coeff) + "x³ + " + 
                         std::to_string(b_coeff) + "x² + " + 
                         std::to_string(c_coeff) + "x + " + std::to_string(d_coeff), 
                         maximize),
          a(a_coeff), b(b_coeff), c(c_coeff), d(d_coeff) {
    }
    
    double PolynomialFunction::evaluate(double x) const {
        return a * x * x * x + b * x * x + c * x + d;
    }
    
    std::unique_ptr<FitnessFunction> PolynomialFunction::clone() const {
        return std::make_unique<PolynomialFunction>(a, b, c, d, isMaximization);
    }
    
    // ExponentialFunction implementation
    ExponentialFunction::ExponentialFunction(double A_param, double B_param, double C_param, bool maximize)
        : FitnessFunction("Exponential Function", 
                         "f(x) = " + std::to_string(A_param) + " * e^(" + 
                         std::to_string(B_param) + " * x) + " + std::to_string(C_param), 
                         maximize),
          A(A_param), B(B_param), C(C_param) {
    }
    
    double ExponentialFunction::evaluate(double x) const {
        return A * std::exp(B * x) + C;
    }
    
    std::unique_ptr<FitnessFunction> ExponentialFunction::clone() const {
        return std::make_unique<ExponentialFunction>(A, B, C, isMaximization);
    }
    
    // LinearFunction implementation
    LinearFunction::LinearFunction(double a_coeff, double b_coeff, bool maximize)
        : FitnessFunction("Linear Function", 
                         "f(x) = " + std::to_string(a_coeff) + " * x + " + std::to_string(b_coeff), 
                         maximize),
          a(a_coeff), b(b_coeff) {
    }
    
    double LinearFunction::evaluate(double x) const {
        return a * x + b;
    }
    
    std::unique_ptr<FitnessFunction> LinearFunction::clone() const {
        return std::make_unique<LinearFunction>(a, b, isMaximization);
    }
    
    // CosineFunction implementation
    CosineFunction::CosineFunction(double amplitude, double frequency, double phase, double offset, bool maximize)
        : FitnessFunction("Cosine Function", 
                         "f(x) = " + std::to_string(amplitude) + " * cos(" + 
                         std::to_string(frequency) + " * x + " + std::to_string(phase) + 
                         ") + " + std::to_string(offset), 
                         maximize),
          A(amplitude), B(frequency), C(phase), D(offset) {
    }
    
    double CosineFunction::evaluate(double x) const {
        return A * std::cos(B * x + C) + D;
    }
    
    std::unique_ptr<FitnessFunction> CosineFunction::clone() const {
        return std::make_unique<CosineFunction>(A, B, C, D, isMaximization);
    }
}