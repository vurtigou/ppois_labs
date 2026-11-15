#include "GroupVisit.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include "Visitor.h"
#include "exceptions.h"   // NullPointerError, DuplicateEntityError

GroupVisit::GroupVisit(int id) : group_id(id), can_enter(false) {}

void GroupVisit::add_visitor(Visitor* v) {
    if (!v) {
        throw NullPointerError("GroupVisit: Cannot add a null visitor to the group.");
    }

    if (v->is_in_group()) {
        throw DuplicateEntityError("GroupVisit: The visitor already belongs to a group.");
    }
    visitors.push_back(v);
    v->set_in_group(true);

    std::cout << "Visitor " << v->get_full_name()
              << " added to group " << group_id << std::endl;
}

bool GroupVisit::check_can_enter() {
    // Удаляем из группы всех, кто не может войти (nullptr или нет права входа)
    visitors.erase(std::remove_if(visitors.begin(), visitors.end(),
    [](Visitor* v) {
        if (v == nullptr || !v->get_can_enter()) {
            if (v) v->set_in_group(false);
            return true;
        }
        return false;
    }), visitors.end());

    // Группа может войти, если после фильтрации в ней остался хотя бы один,
    // а все оставшиеся (по построению) могут войти
    can_enter = !visitors.empty();
    return can_enter;
}

int GroupVisit::get_group_id() const { return group_id; }
bool GroupVisit::get_can_enter()  const { return can_enter; }
const std::vector<Visitor*>& GroupVisit::get_visitors() const { return visitors; }
