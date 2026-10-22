#ifndef SET_H
#define SET_H

#include <vector>
#include <iostream>
#include <memory>
#include <string>

/**
 * @file Set.h
 * @brief Заголовочный файл, содержащий классы для работы с множествами (включая вложенные).
 */

/**
 * @brief Базовый абстрактный класс для элемента множества.
 * 
 * Может представлять собой как простую строку, так и вложенное множество.
 */
class SetElement {
public:
    /**
     * @brief Виртуальный деструктор.
     */
    virtual ~SetElement() = default;

    /**
     * @brief Получить строковое представление элемента.
     * @return Строковое представление.
     */
    virtual std::string toString() const = 0;

    /**
     * @brief Оператор сравнения на равенство.
     * @param other Другой элемент для сравнения.
     * @return true, если элементы равны, иначе false.
     */
    virtual bool operator==(const SetElement& other) const = 0;

    /**
     * @brief Оператор сравнения на неравенство.
     * @param other Другой элемент для сравнения.
     * @return true, если элементы не равны, иначе false.
     */
    virtual bool operator!=(const SetElement& other) const { return !(*this == other); }

    /**
     * @brief Создать глубокую копию элемента.
     * @return Умный указатель на копию элемента.
     */
    virtual std::shared_ptr<SetElement> clone() const = 0; 
};

/**
 * @brief Элемент множества, хранящий простую строку.
 */
class StringElement : public SetElement {
private:
    std::string value; ///< Строковое значение элемента
public:
    /**
     * @brief Конструктор строкового элемента.
     * @param val Строковое значение.
     */
    explicit StringElement(const std::string& val); 
    
    /**
     * @brief Получить строковое представление.
     * @return Строка.
     */
    std::string toString() const override;
    
    /**
     * @brief Сравнение на равенство с другим элементом.
     * @param other Другой элемент.
     * @return true, если строки совпадают.
     */
    bool operator==(const SetElement& other) const override;
    
    /**
     * @brief Создать глубокую копию элемента.
     * @return Указатель на копию.
     */
    std::shared_ptr<SetElement> clone() const override;
    
    /**
     * @brief Получить значение строки.
     * @return Значение строки.
     */
    std::string getValue() const { return value; }
};

class Set; // Предварительное объявление

/**
 * @brief Обертка для вложенного множества.
 * 
 * Позволяет использовать множество в качестве элемента другого множества.
 */
class SetElementWrapper : public SetElement {
private:
    std::shared_ptr<Set> setPtr; ///< Указатель на вложенное множество
public:
    /**
     * @brief Конструктор обертки множества.
     * @param s Указатель на множество.
     */
    explicit SetElementWrapper(std::shared_ptr<Set> s);
    
    /**
     * @brief Получить строковое представление вложенного множества.
     * @return Строка.
     */
    std::string toString() const override;
    
    /**
     * @brief Сравнение на равенство с другим элементом.
     * @param other Другой элемент.
     * @return true, если множества равны.
     */
    bool operator==(const SetElement& other) const override;
    
    /**
     * @brief Создать глубокую копию вложенного множества.
     * @return Указатель на копию.
     */
    std::shared_ptr<SetElement> clone() const override;
    
    /**
     * @brief Получить указатель на вложенное множество.
     * @return Указатель на множество.
     */
    std::shared_ptr<Set> getSet() const { return setPtr; }
};

/**
 * @brief Класс, реализующий математическое множество.
 */
class Set {
private:
    std::vector<std::shared_ptr<SetElement>> elements; ///< Вектор элементов множества
    
    /**
     * @brief Удаляет дубликаты элементов из множества.
     */
    void removeDuplicates(); 

public:
    /**
     * @brief Конструктор по умолчанию. Создает пустое множество.
     */
    Set();

    /**
     * @brief Конструктор из вектора элементов.
     * @param elems Вектор умных указателей на элементы.
     */
    explicit Set(const std::vector<std::shared_ptr<SetElement>>& elems);

    /**
     * @brief Конструктор копирования (глубокое копирование).
     * @param other Другое множество.
     */
    Set(const Set& other);
    
    /**
     * @brief Оператор присваивания копированием.
     * @param other Другое множество.
     * @return Ссылка на текущее множество.
     */
    Set& operator=(const Set& other);
    
    /**
     * @brief Деструктор по умолчанию.
     */
    ~Set() = default;

    /**
     * @brief Проверка на пустое множество.
     * @return true, если множество пусто.
     */
    bool isEmpty() const;

    /**
     * @brief Получение мощности (количества элементов) множества.
     * @return Количество элементов.
     */
    size_t getCardinality() const;

    /**
     * @brief Добавление элемента во множество.
     * @param elem Указатель на добавляемый элемент.
     */
    void addElement(const std::shared_ptr<SetElement>& elem);

    /**
     * @brief Удаление элемента из множества.
     * @param elem Указатель на удаляемый элемент.
     */
    void removeElement(const std::shared_ptr<SetElement>& elem);

    /**
     * @brief Проверка принадлежности элемента множеству.
     * @param elem Указатель на элемент.
     * @return true, если элемент содержится во множестве.
     */
    bool operator[](const std::shared_ptr<SetElement>& elem) const;

    /**
     * @brief Объединение двух множеств.
     * @param other Другое множество.
     * @return Новое множество (объединение).
     */
    Set operator+(const Set& other) const;
    
    /**
     * @brief Объединение с присваиванием.
     * @param other Другое множество.
     * @return Ссылка на текущее множество.
     */
    Set& operator+=(const Set& other);

    /**
     * @brief Пересечение двух множеств.
     * @param other Другое множество.
     * @return Новое множество (пересечение).
     */
    Set operator*(const Set& other) const;
    
    /**
     * @brief Пересечение с присваиванием.
     * @param other Другое множество.
     * @return Ссылка на текущее множество.
     */
    Set& operator*=(const Set& other);

    /**
     * @brief Разность двух множеств.
     * @param other Другое множество.
     * @return Новое множество (разность).
     */
    Set operator-(const Set& other) const;
    
    /**
     * @brief Разность с присваиванием.
     * @param other Другое множество.
     * @return Ссылка на текущее множество.
     */
    Set& operator-=(const Set& other);

    /**
     * @brief Сравнение множеств на равенство.
     * @param other Другое множество.
     * @return true, если множества равны.
     */
    bool operator==(const Set& other) const;
    
    /**
     * @brief Сравнение множеств на неравенство.
     * @param other Другое множество.
     * @return true, если множества не равны.
     */
    bool operator!=(const Set& other) const;

    /**
     * @brief Построение булеана (множества всех подмножеств).
     * @return Новое множество, состоящее из подмножеств.
     */
    Set getPowerSet() const;

    /**
     * @brief Формирование множества из строки (например, "{a, b, {c, d}}").
     * @param str Входная строка.
     * @return Новое множество.
     */
    static Set fromString(const std::string& str);

    /**
     * @brief Получение строкового представления множества.
     * @return Строка.
     */
    std::string toString() const;

    /**
     * @brief Оператор вывода множества в поток.
     * @param os Поток вывода.
     * @param s Множество.
     * @return Ссылка на поток вывода.
     */
    friend std::ostream& operator<<(std::ostream& os, const Set& s);

    /**
     * @brief Оператор чтения множества из потока.
     * @param is Поток ввода.
     * @param s Множество.
     * @return Ссылка на поток ввода.
     */
    friend std::istream& operator>>(std::istream& is, Set& s);
};

#endif
