#include "AnimalTransfer.h"

#include <iostream>
#include <string>

#include "Animal.h"
#include "exceptions.h"   // NullPointerError, InvalidActionError, OceanariumError,
                          // UnsuitableHabitatError, EntityNotFoundError
#include "Habitat.h"     // Habitat

AnimalTransfer::AnimalTransfer(Animal* a, Habitat* dest)
    : animal(a), destination_habitat(dest) {}

void AnimalTransfer::approve_transfer() {
    if (!animal || !destination_habitat) {
        throw NullPointerError("AnimalTransfer: Animal or destination habitat is null.");
    }

    if (!animal->can_be_moved_to_habitat()) {
        throw InvalidActionError("AnimalTransfer: Animal " + animal->get_id() + " cannot be moved to another habitat.");
    }

    Habitat* old_habitat = animal->get_habitat();
    if (!old_habitat) {
        throw OceanariumError("AnimalTransfer: Animal has no current habitat.");
    }

    if (old_habitat == destination_habitat) {
        throw InvalidActionError("AnimalTransfer: Animal is already in this habitat.");
    }

    if (!destination_habitat->check_suitability_for_animal(*animal)) {
        throw UnsuitableHabitatError("AnimalTransfer: Destination habitat is not suitable for animal " + animal->get_id());
    }

    if (!old_habitat->contains_animal(animal->get_id())) {
        throw EntityNotFoundError("AnimalTransfer: Animal not found in its current habitat.");
    }

    destination_habitat->add_animal(animal);

    std::cout << "Transfer approved. Animal " << animal->get_id()
              << " successfully moved to new habitat." << std::endl;
}
