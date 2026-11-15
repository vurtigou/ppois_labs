#include "Console.h"

#include "Ticket.h"

#include <iostream>

Visitor& Console::visitor(const std::string& id) {
    return require(visitors, number<int>(id), "visitor");
}

AudioGuide& Console::guide_for(Visitor& guest, const std::string& kind) {
    if (kind == "standard") return audio;
    if (kind == "vip") {
        if (auto* guide = vip.get_audio_guide(guest)) return *guide;
        throw ServiceNotPurchasedError("Visitor has no VIP package.");
    }
    throw InvalidActionError("Choose standard or vip audio guide.");
}

Buffet& Console::product(const std::string& name) {
    if (name == "bread") return bread;
    if (name == "juice") return juice;
    if (name == "beer") return beer;
    throw EntityNotFoundError("Unknown buffet item: " + name);
}

void Console::stock_menu(Lounge& room) {
    room.add_to_menu(&bread);
    room.add_to_menu(&juice);
    room.add_to_menu(&beer);
}

void Console::register_visitor(const Args& args) {
    const auto id = number<int>(args[0]);
    if (id < 1 || args[3].empty()) throw InvalidActionError("Visitor needs a positive ID and a name.");
    if (visitors.count(id) != 0) throw DuplicateEntityError("Visitor ID already exists.");
    auto guest = std::make_unique<Visitor>(id, args[3], number<int>(args[1]), number<double>(args[2]));
    visitors.emplace(id, std::move(guest));
}

void Console::visitor_status(const Args& args) {
    const auto& guest = visitor(args[0]);
    std::cout << guest.get_full_name() << " | balance: " << guest.get_money_count()
              << " | ticket: " << (guest.get_can_enter() ? "yes" : "no")
              << " | services: " << guest.get_services().size() << '\n';
}

void Console::group_add(const Args& args) {
    const auto id = number<int>(args[0]);
    auto& guest = visitor(args[1]);
    if (id < 1) throw InvalidActionError("Group ID must be positive.");
    if (admitted_groups.count(id)) throw InvalidActionError("This group is already admitted.");
    auto found = groups.find(id);
    if (found == groups.end()) found = groups.emplace(id, std::make_unique<GroupVisit>(id)).first;
    found->second->add_visitor(&guest);
}

void Console::group_entry(const Args& args) {
    const auto id = number<int>(args[0]);
    auto& group = require(groups, id, "group");
    if (admitted_groups.count(id)) throw InvalidActionError("This group is already admitted.");
    if (!group.check_can_enter()) {
        std::cout << "Group " << id << ": no valid tickets\n";
        return;
    }

    admitted_groups.insert(id);
    const auto count = group.get_visitors().size();
    statistics.add_group(static_cast<int>(count));
    std::cout << "Group " << id << " admitted: " << count << '\n';
}

void Console::ticket(const Args& args) {
    auto& guest = visitor(args.size() >= 2 ? args[1] : "1");
    auto& employee = require(employees, args.size() == 3 ? number<int>(args[2]) : 1, "employee");
    auto* cashier = dynamic_cast<Cashier*>(&employee);
    if (!cashier) throw InvalidActionError("Only a cashier can sell tickets.");
    const auto result = cashier->sell_ticket(guest, std::make_unique<Ticket>(number<int>(args[0])));
    std::cout << (result.first ? "Ticket sold" : "Ticket not sold") << " to " << guest.get_full_name() << '\n';
}

void Console::buy_service(const Args& args) {
    auto& guest = visitor(args[0]);
    const auto& name = args[1];
    if (name == "audio") {
        audio.buy(guest);
    } else if (name == "lounge") {
        lounge.buy(guest);
    } else if (name == "vip") {
        vip.buy(guest);
        constexpr int lounge_count = 6;
        for (int index = 0; index < lounge_count; ++index) stock_menu(*vip.get_lounge(guest, index));
    } else {
        throw EntityNotFoundError("Unknown service: " + name);
    }
    std::cout << "Service purchased: " << name << '\n';
}

void Console::rent(const Args& args) {
    auto& guest = visitor(args[0]);
    auto headset = headsets.find(args[1]);
    if (headset == headsets.end()) throw EntityNotFoundError("Unknown headset: " + args[1]);
    guide_for(guest, args.size() == 3 ? args[2] : "standard").assign_headphones(&headset->second, guest);
}

void Console::return_headphones(const Args& args) {
    auto& guest = visitor(args[0]);
    guide_for(guest, args.size() == 2 ? args[1] : "standard").return_headphones(guest);
    std::cout << "Headphones returned by " << guest.get_full_name() << '\n';
}

void Console::menu(const Args&) {
    std::cout << "bread: " << bread.get_price() << " (" << bread.get_weight() << " g)"
              << " | juice: " << juice.get_price() << " (" << juice.get_vol() << " ml)"
              << " | beer: " << beer.get_price() << " (" << beer.get_degrees() << "%, 18+)\n";
}

void Console::order(const Args& args) {
    auto& guest = visitor(args[0]);
    auto& item = product(args[1]);
    Lounge* room = &lounge;
    if (args.size() == 3) {
        const int index = number<int>(args[2]);
        constexpr int lounge_count = 6;
        if (index < 1 || index > lounge_count) throw InvalidActionError("VIP lounge number must be from 1 to 6.");
        room = vip.get_lounge(guest, index - 1);
        if (!room) throw ServiceNotPurchasedError("Visitor has no VIP package.");
    }
    room->buy_item(guest, &item);
}
