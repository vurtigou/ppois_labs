#include "../include/Set.h"
#include <algorithm>
#include <sstream>
#include <cctype>

// Реализация StringElement
StringElement::StringElement(const std::string& val) : value(val) {}

std::string StringElement::toString() const { return value; }

bool StringElement::operator==(const SetElement& other) const {
    const StringElement* strElem = dynamic_cast<const StringElement*>(&other); //приведение к типу строки
    return strElem && value == strElem->value;
}

std::shared_ptr<SetElement> StringElement::clone() const {
    return std::make_shared<StringElement>(value);
}

// Реализация SetElementWrapper
SetElementWrapper::SetElementWrapper(std::shared_ptr<Set> s) : setPtr(s) {}

std::string SetElementWrapper::toString() const { return setPtr->toString(); }

bool SetElementWrapper::operator==(const SetElement& other) const {
    const SetElementWrapper* setElem = dynamic_cast<const SetElementWrapper*>(&other); //приведение к типу множества
    return setElem && *setPtr == *setElem->setPtr;
}

std::shared_ptr<SetElement> SetElementWrapper::clone() const {
    return std::make_shared<SetElementWrapper>(std::make_shared<Set>(*setPtr));
}

// Реализация Set
Set::Set() : elements{} {} // Пустое множество

Set::Set(const std::vector<std::shared_ptr<SetElement>>& elems) : elements(elems) {
    removeDuplicates(); //удаляет дубликаты
}

Set::Set(const Set& other) { //конст копирования (глубокое копирование)
    for (const auto& elem : other.elements) {
        elements.push_back(elem->clone());
    }
}

Set& Set::operator=(const Set& other) { //оператор присваивания
    if (this != &other) {
        Set temp(other);
        std::swap(elements, temp.elements);
    }
    return *this;
}

void Set::removeDuplicates() {
    std::vector<std::shared_ptr<SetElement>> uniqueElements;
    for (const auto& elem : elements) { //перебор всех элементов
        bool found = false;
        for (const auto& uElem : uniqueElements) { //сравнение с уже добавленными
            if (*elem == *uElem) {
                found = true;
                break;
            }
        }
        if (!found) {
            uniqueElements.push_back(elem);
        }
    }
    elements = uniqueElements;
}

bool Set::isEmpty() const {
    return elements.empty();
}

size_t Set::getCardinality() const {
    return elements.size();
}

void Set::addElement(const std::shared_ptr<SetElement>& elem) {
    if (!(*this)[elem]) { //добавляем только если такого элемента нет
        elements.push_back(elem->clone());
    }
}

void Set::removeElement(const std::shared_ptr<SetElement>& elem) {
    elements.erase(
        std::remove_if(elements.begin(), elements.end(),
            [&elem](const std::shared_ptr<SetElement>& e) { return *e == *elem; }),
        elements.end()
    );
}

bool Set::operator[](const std::shared_ptr<SetElement>& elem) const { //проверка принадлежности
    for (const auto& e : elements) {
        if (*e == *elem) {
            return true;
        }
    }
    return false;
}

Set Set::operator+(const Set& other) const { //объединение
    Set result(*this);
    result += other;
    return result;
}

Set& Set::operator+=(const Set& other) {
    for (const auto& elem : other.elements) {
        addElement(elem);
    }
    return *this;
}

Set Set::operator*(const Set& other) const { //пересечение
    Set result;
    for (const auto& elem : elements) {
        if (other[elem]) {
            result.addElement(elem);
        }
    }
    return result;
}

Set& Set::operator*=(const Set& other) {
    *this = *this * other;
    return *this;
}

Set Set::operator-(const Set& other) const { //разность
    Set result;
    for (const auto& elem : elements) {
        if (!other[elem]) {
            result.addElement(elem);
        }
    }
    return result;
}

Set& Set::operator-=(const Set& other) {
    *this = *this - other;
    return *this;
}

bool Set::operator==(const Set& other) const { //сравнение
    if (elements.size() != other.elements.size()) {
        return false;
    }
    for (const auto& elem : elements) {
        if (!other[elem]) {
            return false;
        }
    }
    return true;
}

bool Set::operator!=(const Set& other) const {
    return !(*this == other);
}

Set Set::getPowerSet() const { //булеан
    Set powerSet;
    powerSet.addElement(std::make_shared<SetElementWrapper>(std::make_shared<Set>()));
    
    for (const auto& elem : elements) {
        Set newSubsets;
        for (const auto& subset : powerSet.elements) {
            std::shared_ptr<SetElementWrapper> wrapper = std::dynamic_pointer_cast<SetElementWrapper>(subset);
            if (wrapper) { //если элемент является множеством
                std::shared_ptr<Set> newSet = std::make_shared<Set>(*wrapper->getSet());
                newSet->addElement(elem);
                newSubsets.addElement(std::make_shared<SetElementWrapper>(newSet));
            }
        }
        powerSet += newSubsets;
    }
    return powerSet;
}

std::string Set::toString() const { //вывод в строку
    std::string result = "{";
    for (size_t i = 0; i < elements.size(); ++i) {
        result += elements[i]->toString();
        if (i < elements.size() - 1) {
            result += ", ";
        }
    }
    result += "}";
    return result;
}

// Вспомогательные функции для парсинга строки
namespace {
    std::string trim(const std::string& str) { //удаляет пробелы по краям
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, (last - first + 1));
    }
    
    std::vector<std::string> tokenize(const std::string& str) { //разбивает строку на токены верхнего уровня
        std::vector<std::string> tokens;
        int depth = 0;
        std::string token;
        for (char c : str) {
            if (c == '{') depth++;
            if (c == '}') depth--;
            if (c == ',' && depth == 0) { //запятая на верхнем уровне
                tokens.push_back(trim(token));
                token.clear();
            } else {
                token += c;
            }
        }
        if (!trim(token).empty()) {
            tokens.push_back(trim(token));
        }
        return tokens;
    }
}

Set Set::fromString(const std::string& str) { //парсинг строки в множество
    std::string cleaned = trim(str);
    if (cleaned.size() < 2 || cleaned.front() != '{' || cleaned.back() != '}') {
        throw std::invalid_argument("Invalid set format");
    }
    
    Set result;
    std::string inner = cleaned.substr(1, cleaned.size() - 2);
    std::vector<std::string> tokens = tokenize(inner);
    
    for (const auto& token : tokens) { //перебор токенов
        if (token.front() == '{' && token.back() == '}') { //если это вложенное множество
            result.addElement(std::make_shared<SetElementWrapper>(std::make_shared<Set>(fromString(token))));
        } else { //если это строка
            result.addElement(std::make_shared<StringElement>(token));
        }
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Set& s) { //вывод в поток
    os << s.toString();
    return os;
}

std::istream& operator>>(std::istream& is, Set& s) { //чтение из потока
    std::string input;
    std::cout << "Enter set string (e.g., {a, b, {c, d}}): ";
    std::getline(is, input);
    s = Set::fromString(input);
    return is;
}
