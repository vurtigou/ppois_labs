#pragma once

#include <iosfwd>
#include <string>

namespace model {

class Student {
public:
    Student();
    Student(int id, std::string fullName, std::string group, double gpa, int age);

    Student(const Student&) = default;
    Student& operator=(const Student&) = default;
    Student(Student&&) noexcept = default;
    Student& operator=(Student&&) noexcept = default;
    ~Student() = default;

    int getId() const noexcept;
    const std::string& getFullName() const noexcept;
    const std::string& getGroup() const noexcept;
    double getGpa() const noexcept;
    int getAge() const noexcept;

    void setId(int id) noexcept;
    void setFullName(std::string fullName);
    void setGroup(std::string group);
    void setGpa(double gpa) noexcept;
    void setAge(int age) noexcept;

    // Основной порядок сравнения (по умолчанию по среднему баллу, затем по ID и ФИО)
    bool operator<(const Student& other) const noexcept;
    bool operator==(const Student& other) const noexcept;
    bool operator!=(const Student& other) const noexcept;
    bool operator<=(const Student& other) const noexcept;
    bool operator>(const Student& other) const noexcept;
    bool operator>=(const Student& other) const noexcept;

    // Компараторы для разных полей при сортировке
    static bool compareById(const Student& a, const Student& b) noexcept;
    static bool compareByName(const Student& a, const Student& b) noexcept;
    static bool compareByGroup(const Student& a, const Student& b) noexcept;
    static bool compareByGpa(const Student& a, const Student& b) noexcept;
    static bool compareByAge(const Student& a, const Student& b) noexcept;

private:
    int id_{0};
    std::string fullName_;
    std::string group_;
    double gpa_{0.0};
    int age_{18};
};

std::ostream& operator<<(std::ostream& os, const Student& student);

} // namespace model
