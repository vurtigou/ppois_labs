#ifndef SET_H
#define SET_H

#include <vector>
#include <iostream>
#include <memory>
#include <string>

// Базовый класс для элемента множества (может быть строкой или другим множеством)
class SetElement {
public:
    virtual ~SetElement() = default;
    virtual std::string toString() const = 0;
    virtual bool operator==(const SetElement& other) const = 0;
    virtual bool operator!=(const SetElement& other) const { return !(*this == other); }
    virtual std::shared_ptr<SetElement> clone() const = 0; //для глубокого копирования
};

// Элемент-строка (простой элемент множества)
class StringElement : public SetElement {
private:
    std::string value; //Значение строкового элемента
public:
    explicit StringElement(const std::string& val);
    std::string toString() const override;
    bool operator==(const SetElement& other) const override;
    std::shared_ptr<SetElement> clone() const override;
    std::string getValue() const { return value; }
};

class Set; // Предварительное объявление для SetElementWrapper

// Обертка для вложенного множества (элемент, который сам является множеством)
class SetElementWrapper : public SetElement {
private:
    std::shared_ptr<Set> setPtr; //Указатель на вложенное множество
public:
    explicit SetElementWrapper(std::shared_ptr<Set> s);
    std::string toString() const override;
    bool operator==(const SetElement& other) const override;
    std::shared_ptr<SetElement> clone() const override;
    std::shared_ptr<Set> getSet() const { return setPtr; }
};

class Set {
private:
    std::vector<std::shared_ptr<SetElement>> elements; //Элементы множества
    void removeDuplicates(); //Удаляет дубликаты элементов

public:
    //конструктор по умолчанию создает пустое множество
    Set();

    //Конструктор из вектора элементов
    explicit Set(const std::vector<std::shared_ptr<SetElement>>& elems);

    // оператор присваивания и деструктор (вручную, так как храним указатели)
    Set(const Set& other);
    Set& operator=(const Set& other);
    ~Set() = default;

    //Проверка на пустое множество
    bool isEmpty() const;

    //Определение мощности множества
    size_t getCardinality() const;

    //Добавление элемента
    void addElement(const std::shared_ptr<SetElement>& elem);

    //Удаление элемента
    void removeElement(const std::shared_ptr<SetElement>& elem);

    //Проверка принадлежности элемента множеству ([])
    bool operator[](const std::shared_ptr<SetElement>& elem) const;

    //Объединение двух множеств (+, +=)
    Set operator+(const Set& other) const;
    Set& operator+=(const Set& other);

    //Пересечение двух множеств (*, *=)
    Set operator*(const Set& other) const;
    Set& operator*=(const Set& other);

    //Разность двух множеств (-, -=)
    Set operator-(const Set& other) const;
    Set& operator-=(const Set& other);

    //Сравнение на равенство
    bool operator==(const Set& other) const;
    bool operator!=(const Set& other) const;

    //Построение булеана (множества всех подмножеств)
    Set getPowerSet() const;

    //Формирование множества из строки (например, {a, b, {c, d}})
    static Set fromString(const std::string& str);

    //Получение строкового представления
    std::string toString() const;

    //Оператор вывода в поток
    friend std::ostream& operator<<(std::ostream& os, const Set& s);

    //Оператор чтения из потока
    friend std::istream& operator>>(std::istream& is, Set& s);
};

#endif
