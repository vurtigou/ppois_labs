#include "model/Student.h"

#include <cmath>
#include <iomanip>
#include <ostream>
#include <tuple>
#include <utility>

namespace model {

Student::Student() = default;

Student::Student(int id, std::string fullName, std::string group, double gpa, int age)
    : id_(id),
      fullName_(std::move(fullName)),
      group_(std::move(group)),
      gpa_(gpa),
      age_(age) {}

int Student::getId() const noexcept {
    return id_;
}

const std::string& Student::getFullName() const noexcept {
    return fullName_;
}

const std::string& Student::getGroup() const noexcept {
    return group_;
}

double Student::getGpa() const noexcept {
    return gpa_;
}

int Student::getAge() const noexcept {
    return age_;
}

void Student::setId(int id) noexcept {
    id_ = id;
}

void Student::setFullName(std::string fullName) {
    fullName_ = std::move(fullName);
}

void Student::setGroup(std::string group) {
    group_ = std::move(group);
}

void Student::setGpa(double gpa) noexcept {
    gpa_ = gpa;
}

void Student::setAge(int age) noexcept {
    age_ = age;
}

bool Student::operator<(const Student& other) const noexcept {
    return std::tie(gpa_, id_, fullName_, group_, age_) <
           std::tie(other.gpa_, other.id_, other.fullName_, other.group_, other.age_);
}

bool Student::operator==(const Student& other) const noexcept {
    constexpr double kEps = 1e-9;
    return id_ == other.id_ &&
           age_ == other.age_ &&
           fullName_ == other.fullName_ &&
           group_ == other.group_ &&
           std::fabs(gpa_ - other.gpa_) < kEps;
}

bool Student::operator!=(const Student& other) const noexcept {
    return !(*this == other);
}

bool Student::operator<=(const Student& other) const noexcept {
    return (*this < other) || (*this == other);
}

bool Student::operator>(const Student& other) const noexcept {
    return other < *this;
}

bool Student::operator>=(const Student& other) const noexcept {
    return !(*this < other);
}

bool Student::compareById(const Student& a, const Student& b) noexcept {
    return a.id_ < b.id_;
}

bool Student::compareByName(const Student& a, const Student& b) noexcept {
    return std::tie(a.fullName_, a.id_) < std::tie(b.fullName_, b.id_);
}

bool Student::compareByGroup(const Student& a, const Student& b) noexcept {
    return std::tie(a.group_, a.fullName_, a.id_) <
           std::tie(b.group_, b.fullName_, b.id_);
}

bool Student::compareByGpa(const Student& a, const Student& b) noexcept {
    return std::tie(a.gpa_, a.id_) < std::tie(b.gpa_, b.id_);
}

bool Student::compareByAge(const Student& a, const Student& b) noexcept {
    return std::tie(a.age_, a.id_) < std::tie(b.age_, b.id_);
}

std::ostream& operator<<(std::ostream& os, const Student& student) {
    os << "Student{id=" << student.getId()
       << ", name=\"" << student.getFullName() << "\""
       << ", group=\"" << student.getGroup() << "\""
       << ", gpa=" << std::fixed << std::setprecision(2) << student.getGpa()
       << ", age=" << student.getAge() << "}";
    return os;
}

} // namespace model
