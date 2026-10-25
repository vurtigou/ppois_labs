#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <iostream>

/**
 * @file Polynomial.h
 * @brief Заголовочный файл для класса, реализующего многочлен (полином).
 */

/**
 * @brief Класс, представляющий многочлен.
 */
class Polynomial {
private:
    std::vector<double> coefficients; ///< Коэффициенты многочлена (индекс = степень)
    
    /**
     * @brief Удаляет ведущие нули из вектора коэффициентов.
     */
    void removeLeadingZeros(); 

public:
    /**
     * @brief Конструктор по умолчанию. Создает нулевой многочлен 0.0.
     */
    Polynomial();

    /**
     * @brief Конструктор из вектора коэффициентов.
     * @param coeffs Вектор коэффициентов (индекс = степень).
     */
    explicit Polynomial(const std::vector<double>& coeffs);

    /**
     * @brief Конструктор копирования по умолчанию.
     */
    Polynomial(const Polynomial& other) = default;
    
    /**
     * @brief Оператор присваивания копированием по умолчанию.
     * @param other Другой многочлен.
     * @return Ссылка на текущий многочлен.
     */
    Polynomial& operator=(const Polynomial& other) = default;
    
    /**
     * @brief Деструктор по умолчанию.
     */
    ~Polynomial() = default;

    /**
     * @brief Получение коэффициента при заданной степени.
     * @param degree Степень.
     * @return Коэффициент (0.0, если степень больше текущей).
     */
    double operator[](int degree) const;

    /**
     * @brief Вычисление значения многочлена для заданного аргумента.
     * @param x Значение переменной.
     * @return Значение многочлена.
     */
    double operator()(double x) const;

    /**
     * @brief Сложение двух многочленов.
     * @param other Другой многочлен.
     * @return Новый многочлен (сумма).
     */
    Polynomial operator+(const Polynomial& other) const;
    
    /**
     * @brief Сложение с присваиванием.
     * @param other Другой многочлен.
     * @return Ссылка на текущий многочлен.
     */
    Polynomial& operator+=(const Polynomial& other);

    /**
     * @brief Вычитание двух многочленов.
     * @param other Другой многочлен.
     * @return Новый многочлен (разность).
     */
    Polynomial operator-(const Polynomial& other) const;
    
    /**
     * @brief Вычитание с присваиванием.
     * @param other Другой многочлен.
     * @return Ссылка на текущий многочлен.
     */
    Polynomial& operator-=(const Polynomial& other);

    /**
     * @brief Произведение двух многочленов.
     * @param other Другой многочлен.
     * @return Новый многочлен (произведение).
     */
    Polynomial operator*(const Polynomial& other) const;
    
    /**
     * @brief Произведение с присваиванием.
     * @param other Другой многочлен.
     * @return Ссылка на текущий многочлен.
     */
    Polynomial& operator*=(const Polynomial& other);

    /**
     * @brief Деление двух многочленов (упрощенное, возвращает частное).
     * @param other Другой многочлен.
     * @return Новый многочлен (частное).
     */
    Polynomial operator/(const Polynomial& other) const;
    
    /**
     * @brief Деление с присваиванием.
     * @param other Другой многочлен.
     * @return Ссылка на текущий многочлен.
     */
    Polynomial& operator/=(const Polynomial& other);

    /**
     * @brief Сравнение многочленов на равенство.
     * @param other Другой многочлен.
     * @return true, если многочлены равны.
     */
    bool operator==(const Polynomial& other) const;
    
    /**
     * @brief Сравнение многочленов на неравенство.
     * @param other Другой многочлен.
     * @return true, если многочлены не равны.
     */
    bool operator!=(const Polynomial& other) const;

    /**
     * @brief Получение степени многочлена.
     * @return Степень многочлена.
     */
    int degree() const;

    /**
     * @brief Проверка, является ли многочлен нулевым.
     * @return true, если многочлен нулевой.
     */
    bool isZero() const;

    /**
     * @brief Оператор вывода многочлена в поток.
     * @param os Поток вывода.
     * @param p Многочлен.
     * @return Ссылка на поток вывода.
     */
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p);

    /**
     * @brief Оператор чтения многочлена из потока.
     * @param is Поток ввода.
     * @param p Многочлен.
     * @return Ссылка на поток ввода.
     */
    friend std::istream& operator>>(std::istream& is, Polynomial& p);
};

#endif
