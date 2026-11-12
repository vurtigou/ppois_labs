//есть абстрактный класс Employee с дочерними классами janitor, plumber, electrician, cashier. у абстрактного класса должно быть поле "ставка", "стаж". у каждого дочернего класса пусть будет статическое string поле должность (например у всех уборщиков "janitor"), а так же статическое поле с минимальной ставкой. пусть ставка вычисляется для каждого работника путем умножения минимальной ставки на опыт работы
//Есть класс смена: он содержит поле "Employee" (ссылается на реального работника), время начала смены и время конца смены, а так поле "количество происшествий". Методы: 1. метод, генерирующий количество происшествий от 1 до 15 случайным образом; 2. метод, вычисляющий ставку работника  отработавшего смену, за эту смену (пусть этот метод умножает количество происшествий на ставку конкретно этого работника); 3. метод
//класс "график на месяц", содержащий список из объектов класса "смена"

//пусть ставка вычисляется для каждого работника путем умножения минимальной ставки на опыт работы
#pragma once

#include <string>
#include <vector>

// Базовый абстрактный класс
class Employee {
protected:
    int rate;        // ставка 
    int experience;  // стаж в месяцах
    int id; 
    int amount_penalty;
    int amount_bonus;  
    //добавить вектор чисел double "зарплаты"          
    std::vector<double> salaries; 
    int access_level; 

public:
    Employee(int id = 0, int exp = 0, int rate = 0, int access_level = 0);
    virtual ~Employee() = default;

    virtual std::string get_position() const = 0; 

    // Геттеры
    int get_rate() const { return rate; }
    int get_experience() const { return experience; }
    int get_id() const { return id; }
    
    // Геттер для access_level теперь в базовом классе
    int get_access_level() const { return access_level; }

    // Дополнительные геттеры для штрафов и бонусов (понадобятся в Shift)
    int get_amount_penalty() const { return amount_penalty; }
    int get_amount_bonus() const { return amount_bonus; }
    std::vector<double>& get_salaries() { return salaries; }
};

// Класс Уборщик
class Janitor : public Employee {
public:
    static const std::string position;
    static const double min_rate;
    //ставка за штраф 
    static const double pin_rate;
    //ставка за бонус 
    static const double bon_rate;

    Janitor(int id, int exp);
    std::string get_position() const override;
};

// Класс Сантехник
class Plumber : public Employee {
public:
    static const std::string position;
    static const double min_rate;
    //ставка за штраф
    static const double pin_rate;
    //ставка за бонус
    static const double bon_rate;

    Plumber(int id, int exp, int access);
    std::string get_position() const override;
};

// Класс Электрик
class Electrician : public Employee {
public:
    static const std::string position;
    static const double min_rate;
    //ставка за штраф
    static const double pin_rate;
    //ставка за бонус
    static const double bon_rate;

    Electrician(int id, int exp, int access);
    std::string get_position() const override;
};

// Класс Кассир TODO взаимодействие с посетителями и билеты (другой .h файл)
class Cashier : public Employee {
public:
    static const std::string position;
    static const double min_rate;
    //ставка за штраф
    static const double pin_rate;
    //ставка за бонус
    static const double bon_rate;

    Cashier(int id, int exp);
    std::string get_position() const override;
};

// Класс Смена
class Shift {
private:
    Employee* employee; // ссылается на реального работника (non-owning)
    std::string start_time; // время начала смены
    std::string end_time;   // время конца смены
    int incidents;          // количество происшествий
    bool has_penalty;
    bool has_bonus;
    //сколько в итоге заработал за смену
    double shift_rate;

public:
    Shift(Employee* emp = nullptr, std::string start = "", std::string end = "");

    // 1. Генерирует количество происшествий от 1 до 15 случайным образом
    void generate_incidents();
    
    //2. Надо функция, которая генерирует число от 1 до 10. если выпало 1 - has_penalty = true, amount_penalty++ работника, если выпало 10 - has_bonus = true, amount_bonus++ работника
    void generate_events();

    // 3. Вычисляет ставку работника за эту смену (происшествия * ставка - ставка работника за штраф + ставка работника за  бонус)
    double calculate_shift_rate() const;
    
    // Геттеры для удобства
    int get_incidents() const { return incidents; }
    Employee* get_employee() const { return employee; }
    double get_shift_rate() const { return shift_rate; }
};

// Класс График на месяц
class Schedule {
private:
    std::vector<Shift> shifts;
    // добавить поле вектор работников, которые работали этот месяц
    std::vector<Employee*> employees;

public:
    Schedule() = default;

    // Метод для добавления смены в график
    void add_shift(const Shift& shift);
    
    //добавить функцию, которая идет по списку смен, для каждой смены: 1. если этого работника еще нет в векторе: 1.1 добавить его туда, 1.2 добавить в вектор зарплаты этого работника новый элемент - поле смены shift_rate; 2. если этот работник уже есть в векторе: 2.1 ИЗМЕНИТЬ последний элемент вектора его поля с зарплатой путем прибавления к этому элементу поле смены shift_rate;
    void calculate_monthly_salaries();
};
