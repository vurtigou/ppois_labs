#pragma once

#include <vector>

class Visitor;

/** @brief Visitor group with membership and eligibility checks.
 * @ingroup visitors
 */
class GroupVisit {
private:
    int group_id;
    std::vector<Visitor*> visitors;   // non-owning
    bool can_enter;

public:
    GroupVisit(int id = 0);

    GroupVisit(const GroupVisit&)            = delete;
    GroupVisit& operator=(const GroupVisit&) = delete;
    GroupVisit(GroupVisit&&)                 = default;
    GroupVisit& operator=(GroupVisit&&)      = default;

    void add_visitor(Visitor* v);
    /**
     * @brief Removes visitors without valid admission and checks the remaining group.
     * @return True if at least one eligible visitor remains.
     */
    bool check_can_enter();

    int  get_group_id() const;
    bool get_can_enter() const;
    const std::vector<Visitor*>& get_visitors() const;
};
