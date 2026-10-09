#include "../include/Polynomial.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace {
    const double EPSILON = 1e-9; // Точность для сравнения вещественных чисел
}

Polynomial::Polynomial() : coefficients{0.0} {}

Polynomial::Polynomial(const std::vector<double>& coeffs) : coefficients(coeffs) { //конст чтобы коефф нельзя было менять в этом методе; coefficients(coeffs) список инициализации, coefficients=coeffs, по факту конструктор копировани вектора 
    removeLeadingZeros(); //удаляет нули в старших коэффициентах
}

void Polynomial::removeLeadingZeros() {
    while (coefficients.size() > 1 && std::abs(coefficients.back()) < EPSILON) {
        coefficients.pop_back();
    }
    if (coefficients.empty()) {  //empty метод вектора
        coefficients.push_back(0.0);
    }
}
//перегрузка оператора []
double Polynomial::operator[](int degree) const { //не меняет объект, можно вызывать у константных объектов
    if (degree < 0) {
        throw std::out_of_range("Degree cannot be negative"); //выход за пределы диапазона
    }
    if (degree >= static_cast<int>(coefficients.size())) { //преобразование size_t в инт
        return 0.0;
    }
    return coefficients[degree];
}

double Polynomial::operator()(double x) const {
    double result = 0.0;
    for (int i = static_cast<int>(coefficients.size()) - 1; i >= 0; --i) {
        result = result * x + coefficients[i]; //по схеме горнера
    }
    return result;
}

Polynomial Polynomial::operator+(const Polynomial& other) const { //other второй многочлен который мы не меняем методом; текущий многочлен мы тоже не меняем
    size_t maxSize = std::max(coefficients.size(), other.coefficients.size());
    std::vector<double> result(maxSize, 0.0);
    for (size_t i = 0; i < maxSize; ++i) {
        result[i] = (*this)[i] + other[i]; //разыменование указателя
    }
    return Polynomial(result);
}

Polynomial& Polynomial::operator+=(const Polynomial& other) {
    *this = *this + other;
    return *this;
}

Polynomial Polynomial::operator-(const Polynomial& other) const {
    size_t maxSize = std::max(coefficients.size(), other.coefficients.size());
    std::vector<double> result(maxSize, 0.0);
    for (size_t i = 0; i < maxSize; ++i) {
        result[i] = (*this)[i] - other[i]; 
    }
    return Polynomial(result);
}

Polynomial& Polynomial::operator-=(const Polynomial& other) {
    *this = *this - other;
    return *this;
}

Polynomial Polynomial::operator*(const Polynomial& other) const {
    if (isZero() || other.isZero()) {
        return Polynomial();
    }
    std::vector<double> result(coefficients.size() + other.coefficients.size() - 1, 0.0);
    for (size_t i = 0; i < coefficients.size(); ++i) {
        for (size_t j = 0; j < other.coefficients.size(); ++j) {
            result[i + j] += coefficients[i] * other.coefficients[j];
        }
    }
    return Polynomial(result);
}

Polynomial& Polynomial::operator*=(const Polynomial& other) {
    *this = *this * other;
    return *this;
}

Polynomial Polynomial::operator/(const Polynomial& other) const {
    if (other.isZero()) {
        throw std::invalid_argument("Division by zero polynomial");
    }
    if (degree() < other.degree()) {
        return Polynomial();   // частное = 0
    }
    
    std::vector<double> dividend = coefficients;   // копия делимого (= конструктор копированияя вектора)
    std::vector<double> quotient(degree() - other.degree() + 1, 0.0); //частное (пустой вектор)
    int dOther = other.degree();
    double leadOther = other.coefficients[dOther]; //старший коэфф делителя
    
    for (int i = degree() - dOther; i >= 0; --i) {  //от старшей степени частного
        double coef = dividend[i + dOther] / leadOther;
        quotient[i] = coef;
        for (int j = 0; j <= dOther; ++j) {
            dividend[i + j] -= coef * other.coefficients[j];
        }
    }
    return Polynomial(quotient);
}

Polynomial& Polynomial::operator/=(const Polynomial& other) {
    *this = *this / other;
    return *this;
}

bool Polynomial::operator==(const Polynomial& other) const { //сравнивание 
    if (coefficients.size() != other.coefficients.size()) {
        return false;
    }
    for (size_t i = 0; i < coefficients.size(); ++i) {
        if (std::abs(coefficients[i] - other.coefficients[i]) > EPSILON) {
            return false;
        }
    }
    return true;
}

bool Polynomial::operator!=(const Polynomial& other) const {
    return !(*this == other);
}

int Polynomial::degree() const {
    return static_cast<int>(coefficients.size()) - 1;
}

bool Polynomial::isZero() const {
    return coefficients.size() == 1 && std::abs(coefficients[0]) < EPSILON;
}

std::ostream& operator<<(std::ostream& os, const Polynomial& p) { //поток ос (пишем в него->меняем состояние), выводимый объект, ссылка для цепочки выводов (тк << бинарный оператор
    bool first = true;
    for (int i = p.degree(); i >= 0; --i) {
        double coeff = p[i];
        if (std::abs(coeff) < EPSILON) continue;
        
        if (!first && coeff > 0) os << " + ";
        if (coeff < 0) os << " - ";
        
        double absCoeff = std::abs(coeff);
        if (i == 0 || absCoeff != 1.0) {
            os << absCoeff;
        }
        if (i > 0) {
            os << "x";
            if (i > 1) os << "^" << i;
        }
        first = false;
    }
    if (first) os << "0";
    return os;
}

std::istream& operator>>(std::istream& is, Polynomial& p) { 
    int degree;
    std::cout << "Enter degree: ";
    is >> degree;
    if (degree < 0) {
    	throw std::invalid_argument("Degree cannot be negative");
    }
    std::vector<double> coeffs(degree + 1);
    std::cout << "Enter coefficients from lowest to highest: ";
    for (int i = 0; i <= degree; ++i) {
        is >> coeffs[i];
    }
    p = Polynomial(coeffs);
    return is;
}
