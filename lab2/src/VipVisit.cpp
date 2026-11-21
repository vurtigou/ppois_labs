#include "VipVisit.h"

#include <memory>
#include <utility>
#include <vector>

#include "AudioGuide.h"
#include "Lounge.h"
#include "Visitor.h"
#include "exceptions.h"   // InvalidActionError

VipVisit::VipVisit(double price) : Favour(price) {}

VipVisit::Bundle* VipVisit::find_bundle(const Visitor& visitor) {
    for (auto& p : bundles) {
        if (p.first == &visitor) return &p.second;
    }
    return nullptr;
}

const VipVisit::Bundle* VipVisit::find_bundle(const Visitor& visitor) const {
    for (const auto& p : bundles) {
        if (p.first == &visitor) return &p.second;
    }
    return nullptr;
}

bool VipVisit::buy(Visitor& visitor) {
    // 0. Защита от повторной покупки тем же посетителем
    if (find_bundle(visitor) != nullptr) {
        throw InvalidActionError(
            "VipVisit: Visitor " + visitor.get_full_name()
            + " already bought this VIP package.");
    }

    // 1. Списываем деньги за сам VIP-визит (добавляет `this` в services)
    if (!purchase(visitor)) {
        return false;
    }

    // 2. Создаём ПЕРСОНАЛЬНЫЙ бандл для этого посетителя
    Bundle b;
    b.audio_guide = std::make_unique<AudioGuide>(0.0);
    for (int i = 0; i < 6; ++i) {
        b.lounges.push_back(std::make_unique<Lounge>(0.0));
    }

    // 3. Раздаём сырые указатели внутрь бандла. Владелец — VipVisit
    visitor.add_service(b.audio_guide.get());
    for (auto& lounge : b.lounges) {
        visitor.add_service(lounge.get());
    }

    // 4. Сохраняем бандл
    bundles.emplace_back(&visitor, std::move(b));
    return true;
}

AudioGuide* VipVisit::get_audio_guide(const Visitor& visitor) const {
    const Bundle* b = find_bundle(visitor);
    return b ? b->audio_guide.get() : nullptr;
}

Lounge* VipVisit::get_lounge(const Visitor& visitor, int index) const {
    const Bundle* b = find_bundle(visitor);
    if (!b) return nullptr;
    if (index < 0 || index >= static_cast<int>(b->lounges.size())) return nullptr;
    return b->lounges[index].get();
}
