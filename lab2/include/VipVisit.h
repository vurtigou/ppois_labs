#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "AudioGuide.h"   // unique_ptr<AudioGuide> внутри Bundle
#include "Favour.h"
#include "Lounge.h"       // unique_ptr<Lounge> внутри Bundle

class Visitor;

/** @brief VIP package with a personal audio guide and lounges.
 * @ingroup visitors
 */
class VipVisit : public Favour {
private:
    // Один персональный набор услуг на посетителя.
    /// Personal services owned by the package for one visitor.
    struct Bundle {
        std::unique_ptr<AudioGuide> audio_guide;
        std::vector<std::unique_ptr<Lounge>> lounges;
    };

    // Visitor* — non-owning; Bundle — owned.
    std::vector<std::pair<Visitor*, Bundle>> bundles;

    Bundle*       find_bundle(const Visitor& visitor);
    const Bundle* find_bundle(const Visitor& visitor) const;

public:
    VipVisit(double price = 0.0);

    // Создаёт персональный бандл и раздаёт указатели в services.
    // Бросает InvalidActionError при повторной покупке тем же посетителем.
    /**
     * @brief Purchases a VIP package and creates the visitor’s audio guide and six lounges.
     */
    bool buy(Visitor& visitor) override;

    // Возвращают nullptr, если visitor не покупал.
    AudioGuide* get_audio_guide(const Visitor& visitor) const;
    /**
     * @brief Returns the personal lounge at the requested zero-based index.
     * @return A non-owning pointer, or nullptr if the package or index is unavailable.
     */
    Lounge*     get_lounge(const Visitor& visitor, int index) const;
};
