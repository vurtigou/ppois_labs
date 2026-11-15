#pragma once

// Базовый класс для всего рабочего инвентаря
class EmployeesEquipment {
protected:
    int durability;      // Прочность (от 0 до 100)
    int max_durability;
    bool is_equipped;    // В руках ли у сотрудника (защита от одновременного использования)

public:
    EmployeesEquipment(int max_dur = 100)
        : durability(max_dur), max_durability(max_dur), is_equipped(false) {}

    virtual ~EmployeesEquipment() = default;

    // Общие методы для всех инструментов
    void wear_out(int amount);
    virtual void repair();
    virtual void use() = 0; // Чисто виртуальный метод использования

    int get_durability() const { return durability; }
    bool is_broken() const { return durability <= 0; }
    bool get_is_equipped() const { return is_equipped; }
    void set_equipped(bool status) { is_equipped = status; }
};
