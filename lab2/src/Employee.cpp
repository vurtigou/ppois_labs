#include "Employee.h"

Employee::Employee(int id, int exp, int rate, int access_level)
    : rate(rate), experience(exp), id(id),
      amount_penalty(0), amount_bonus(0), access_level(access_level) {}
