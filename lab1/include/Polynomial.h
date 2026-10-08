#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <iostream>

/*Класс, описывающий многочлен от одной переменной.
Многочлен задается степенью и массивом коэффициентов. 
Коэффициент с индексом i соответствует степени i.
 */
class Polynomial {
private:
    std::vector<double> coefficients; //Коэффициенты многочлена (индекс = степень)
    void removeLeadingZeros(); //Удаляет ведущие нули из вектора коэффициентов.Нужно для корректного сравнения и определения степени.

public:
    //Конструктор по умолчанию. Создает нулевой многочлен.
    Polynomial();

    //Конструктор из вектора коэффициентов. param coeffs Вектор коэффициентов.
    explicit Polynomial(const std::vector<double>& coeffs);

    // Правило "нуля": компилятор сам сгенерирует корректные конструктор копирования,
    // оператор присваивания и деструктор, так как мы используем std::vector.
    Polynomial(const Polynomial& other) = default;
    Polynomial& operator=(const Polynomial& other) = default;
    ~Polynomial() = default;

    //Получение коэффициента при заданной степени. param degree Степень. return Коэффициент (0.0, если степень больше текущей).
    double operator[](int degree) const;

    //Вычисление значения многочлена для заданного аргумента. x Значение переменной. return Значение многочлена.
    double operator()(double x) const;

//Сложение двух многочленов.
    Polynomial operator+(const Polynomial& other) const;
    Polynomial& operator+=(const Polynomial& other);

//Вычитание двух многочленов.
    Polynomial operator-(const Polynomial& other) const;
    Polynomial& operator-=(const Polynomial& other);

//Произведение двух многочленов.
    Polynomial operator*(const Polynomial& other) const;
    Polynomial& operator*=(const Polynomial& other);

//Деление двух многочленов (упрощенное, возвращает частное).
    Polynomial operator/(const Polynomial& other) const;
    Polynomial& operator/=(const Polynomial& other);

//Сравнение на равенство.
    bool operator==(const Polynomial& other) const;
    bool operator!=(const Polynomial& other) const;

//Получение степени многочлена.
    int degree() const;

//Проверка, является ли многочлен нулевым.
    bool isZero() const;

//Оператор вывода в поток.
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p);

    //Оператор чтения из потока.
    friend std::istream& operator>>(std::istream& is, Polynomial& p);
};

#endif
