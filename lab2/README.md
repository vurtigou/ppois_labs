# Lab 2 – Oceanarium

Anastasia Dracheva, group 521702. This C++17 project models the operation of an oceanarium. It covers exhibits, animal placement and transfers, feeding, veterinary care, water treatment, equipment maintenance, staff shifts, ticket sales, group visits, and paid services.

## Build and run

From the directory containing the extracted `lab2` folder, run:

```sh
make -C lab2
./lab2/build/oceanarium
make -C lab2 test
make -C lab2 coverage CXX=g++ GCOV=gcov
```

The build and tests require GNU Make and a C++17 compiler. To run the coverage check, install Python 3.9 or later and matching versions of GNU GCC and gcov. On macOS, pass the names of your installed GNU toolchain to the coverage command, such as `CXX=g++-15 GCOV=gcov-15`. The standard build can use Apple Clang with `CXX=c++`.

`make test` runs the domain tests and four independent CLI sessions. `make coverage` checks executable-line coverage of the domain model and CLI, including instrumented lines in the headers. The required coverage is greater than 90%. A local run with GCC 14.2 measured **97.23%** (1,859 of 1,912 executable lines).

## Doxygen API reference

Install Doxygen and generate the HTML reference:

```sh
make -C lab2 docs
```

Open `lab2/build/doxygen/html/index.html` in a browser. The reference includes the class hierarchy, header and source listings, and API notes on ownership, units, and failure conditions. This configuration runs without Graphviz.

## Console commands

Use `help` for command syntax. Enclose arguments containing spaces in double quotes. A new session includes reef, reserve, and quarantine tanks, a cashier with ID 1, a janitor with ID 2, a plumber with ID 3, an electrician with ID 4, and a visitor with ID 1. The application keeps session data in memory until exit.

| Area | Commands |
|---|---|
| Oceanarium, zones, and exhibits | `open`, `close`, `zone`, `exhibit`, `tank`, `tank-move`, `report`, `log` |
| Animals and veterinary care | `add`, `cub`, `animals`, `compatibility`, `transfer`, `weigh`, `feed`, `stock`, `vet`, `release`, `breed` |
| Water and equipment | `water`, `water-set`, `water-status`, `filter`, `drain`, `system`, `work` |
| Staff | `hire`, `staff`, `shift`, `payroll` |
| Visitors and services | `visitor`, `balance`, `group-add`, `group-entry`, `ticket`, `service`, `rent`, `return-headphones`, `menu`, `order` |
| Utilities | `contact`, `distance`, `help`, `quit` |

### Demonstration session

```text
open
add nemo 10
add coral 3
cub mini nemo 2
feed nemo
feed mini
breed nemo coral 01122026
vet coral "Needs observation" quarantine
animals
release coral reserve
vet coral Healthy
water-set reef 4 42 30 3
filter
system ph assign 4
system ph repair 4 01122026
hire janitor 5 2
shift 5 09:00 17:00
payroll 5
visitor 2 19 100 "Alex Example"
group-add 101 2
ticket 20 2
group-entry 101
service 2 vip
rent 2 1 vip
order 2 juice 1
return-headphones 2 vip
report
quit
```

`feed nemo` calculates a 500 g portion and `feed mini` calculates 60 g. The `animals` command shows `coral` in quarantine before release. The group ticket costs 14 after the discount on its listed price of 20. The VIP package grants an audio guide and access to six lounges. To demonstrate emergency evacuation in a fresh session, add an adult fish, use `water-set` to make the reef water unsuitable, and run `drain`. The reserve tank receives evacuated animals.

`app/main.cpp` starts the console. `app/Console.cpp` lists and dispatches commands, and `app/*Commands.cpp` implements the handlers by domain area. The handlers call the model in `src/` to place and transfer animals, settle payroll, and process purchases.

## Assignment requirements

| Item | Minimum | Implemented |
|---|---:|---:|
| Domain classes | 50 | 50 classes and 2 structs |
| Fields | 150 | 159 instance fields including structs, or 155 without structs |
| Domain behaviors | 100 | 103 listed below |
| Associations | 30 | 46 directed associations through fields |
| Custom exceptions | 12 | 15 including the base exception |

Inherited fields count once in the declaring class. The tables also show 18 static fields. Each association is a unique pair of a class and a type referenced by one of its fields, including collection elements, pointers, and references. The total of 46 covers field associations.

## Classes, fields, methods, and associations

### Core

| Class | Fields | Declared methods | Field associations |
|---|---|---|---|
| `ContactInfo` | 2: `phone`, `email` | `ContactInfo`, `is_valid_contact`, `get_phone`, `get_email` | — |
| `Identifier` | 2: `prefix`, `number` | `Identifier`, `generate_next`, `get_prefix`, `get_next_number` | — |
| `Location` | 3: `city`, `address`, `coordinates` | `Location`, `calculate_distance_to`, `get_city`, `get_address`, `get_coordinates`, `is_valid` | — |
| `LogEntry` | 2: `timestamp`, `action` | — | — |
| `AuditLog` | 2: `entries`, `max_size` | `AuditLog`, `record_action`, `filter_by_period`, `print_logs`, `size` | LogEntry |
| `Oceanarium` | 6: `name`, `is_open`, `location`, `tanks`, `employees`, `audit_log` | `Oceanarium`, `~Oceanarium`, `operator=`, `open_for_day`, `close_for_day`, `add_tank`, `add_employee`, `get_name`, `get_location`, `get_tanks`, `get_employees`, `get_audit_log` | AuditLog, Employee, Location, Tank |

### Animal care and veterinary visits

| Class | Fields | Declared methods | Field associations |
|---|---|---|---|
| `Species` | 7: `latin_name`, `diet_type`, `min_tank_volume`, `ph_range`, `salinity_range`, `temperature_range`, `oxygen_range` | `Species`, `is_compatible_with_species`, `get_latin_name`, `get_min_tank_volume`, `get_diet_type`, `get_ph_range`, `get_salinity_range`, `get_temperature_range`, `get_oxygen_range` | — |
| `VetVisit` | 4: `animal`, `diagnosis`, `treatment`, `is_closed` | `VetVisit`, `diagnostic`, `prescribe_treatment`, `close_visit`, `get_diagnosis`, `get_treatment` | Animal |
| `HealthRecord` | 3: `animal`, `visits`, `current_status` | `HealthRecord`, `operator=`, `status_diagnosis`, `add_visit` | Animal, VetVisit |
| `FeedingPlan` | 3: `animal`, `feed_items`, `portion_grams` | `FeedingPlan`, `adjust_portion`, `add_feed_item`, `check_diet_compliance`, `calculate_portion`, `get_portion_grams`, `set_animal` | Animal, FeedStock |
| `Animal` | 7: `animal_id`, `species`, `habitat`, `current_tank`, `health_record`, `feeding_plan`, `weight_kg` | `Animal`, `~Animal`, `can_be_moved_to_habitat`, `operator=`, `set_id`, `get_current_tank`, `set_current_tank`, `record_weight_change`, `move_to_habitat`, `assign_feeding_plan`, `can_breed`, `on_vet_visit`, `get_feeding_plan`, `get_id`, `get_weight`, `get_habitat`, `get_species`, `get_health_record` | FeedingPlan, Habitat, HealthRecord, Species, Tank |
| `Cub` ← `Animal` | 1: `age` | `Cub`, `record_weight_change`, `move_to_habitat`, `can_be_moved_to_habitat`, `can_breed`, `on_vet_visit`, `get_age` | — |
| `BreedingProgram` | 3: `species`, `pairs`, `status` | `BreedingProgram`, `select_pair`, `schedule_breeding` | Animal, Species |
| `AnimalTransfer` | 2: `animal`, `destination_habitat` | `AnimalTransfer`, `approve_transfer` | Animal, Habitat |

### Tanks and exhibits

| Class | Fields | Declared methods | Field associations |
|---|---|---|---|
| `Habitat` | 4: `habitat_type`, `volume_liters`, `water_quality`, `tanks` | `Habitat`, `check_suitability_for_animal`, `calculate_biomass`, `add_animal`, `contains_animal`, `remove_animal`, `add_tank`, `get_water_quality`, `get_habitat_type` | Tank, WaterQuality |
| `Exhibit` | 4: `title`, `description`, `tanks`, `opened` | `Exhibit`, `open_exhibit`, `close_exhibit`, `get_all_animals`, `add_tank`, `remove_tank`, `is_open`, `get_title`, `get_tanks` | Tank |
| `Tank` | 8: `tank_id`, `parent_habitat`, `filtration_system`, `sensors`, `animals`, `exhibit`, `animal_count`, `is_filtration_on` | `Tank`, `~Tank`, `operator=`, `start_filtration`, `emergency_drain`, `is_water_critical`, `evacuate_all_animals`, `clear_state`, `set_exhibit`, `get_tank_id`, `get_habitat`, `get_animals`, `get_exhibit`, `get_animal_count`, `add_animal`, `add_sensor`, `contains_animal`, `remove_animal` | Animal, ControlSystem, Exhibit, Habitat |
| `Zone` | 3: `zone_name`, `exhibits`, `access_level` | `Zone`, `restrict_access`, `list_tanks`, `add_tank`, `remove_tank`, `evacuate_tank_animals`, `add_exhibit`, `get_zone_name`, `get_access_level`, `get_exhibits` | Exhibit |
| `QuarantineTank` ← `Tank` | 1: `quarantine_reason` | `QuarantineTank`, `set_diagnosis`, `admit_animal`, `release_after_check` (two overloads) | — |
| `HabitatMap` | 2: `zones`, `scale` | `HabitatMap`, `locate_tank`, `add_zone` | Zone |

### Water quality and maintenance

| Class | Fields | Declared methods | Field associations |
|---|---|---|---|
| `WaterQuality` | 4: `ph`, `salinity`, `temperature`, `oxygen_level` | `WaterQuality`, `get_ph`, `get_salinity`, `get_temperature`, `get_oxygen_level`, `set_ph`, `set_salinity`, `set_temperature`, `set_oxygen_level`, `is_within_norm` | — |
| `ControlSystem` | 6: `id`, `employees`, `access_level`, `is_good_working`, `repair_dates`, `quality` | `ControlSystem`, `~ControlSystem`, `checking`, `improving`, `on_repair`, `get_id`, `get_access_level`, `get_is_good_working`, `get_repair_dates`, `get_employees`, `assign_employee`, `release_employee`, `can_work`, `perform_work`, `is_valid_date`, `mark_broken` | Employee, WaterQuality |
| `WaterSensor` ← `ControlSystem` | 4: `vol_co2`, `vol_alkali`, `max_co2`, `max_alkali` | `WaterSensor`, `checking`, `improving`, `on_repair` | — |
| `SalinityController` ← `ControlSystem` | 4: `vol_water`, `vol_brine`, `max_water`, `max_brine` | `SalinityController`, `checking`, `improving`, `on_repair` | — |
| `TemperatureController` ← `ControlSystem` | 2: `amperage`, `initial_amperage` | `TemperatureController`, `checking`, `improving`, `on_repair` | — |
| `Ozonator` ← `ControlSystem` | 4: `availible_vol_ozon`, `vol_coal`, `max_ozon`, `max_coal` | `Ozonator`, `checking`, `improving`, `on_repair` | — |

### Staff and shifts

| Class | Fields | Declared methods | Field associations |
|---|---|---|---|
| `Employee` | 7: `rate`, `experience`, `id`, `amount_penalty`, `amount_bonus`, `salaries`, `access_level` | `Employee`, `~Employee`, `get_position`, `get_pin_rate`, `get_bon_rate`, `get_rate`, `get_experience`, `get_id`, `get_access_level`, `get_amount_penalty`, `get_amount_bonus`, `get_salaries`, `add_salary`, `update_last_salary`, `add_penalty`, `add_bonus` | — |
| `Janitor` ← `Employee` | 5: `mop`, `position`, `min_rate`, `pin_rate`, `bon_rate` | `Janitor`, `get_position`, `get_pin_rate`, `get_bon_rate`, `set_mop`, `clean_area` | Mop |
| `Plumber` ← `Employee` | 5: `equipment`, `position`, `min_rate`, `pin_rate`, `bon_rate` | `Plumber`, `get_position`, `get_pin_rate`, `get_bon_rate`, `set_equipment`, `repair_leak` | PlumberEquipment |
| `Electrician` ← `Employee` | 5: `equipment`, `position`, `min_rate`, `pin_rate`, `bon_rate` | `Electrician`, `get_position`, `get_pin_rate`, `get_bon_rate`, `set_equipment`, `inspect_circuits` | ElectricianEquipment |
| `Cashier` ← `Employee` | 5: `tickets_sold`, `position`, `min_rate`, `pin_rate`, `bon_rate` | `Cashier`, `get_position`, `get_pin_rate`, `get_bon_rate`, `sell_ticket`, `get_tickets_sold` | — |
| `Shift` | 8: `employee`, `start_time`, `end_time`, `incidents`, `has_penalty`, `has_bonus`, `paid`, `shift_rate` | `Shift`, `generate_incidents`, `generate_events`, `calculate_shift_rate`, `get_incidents`, `get_employee`, `get_shift_rate` | Employee |
| `Schedule` | 2: `shifts`, `employees` | `Schedule`, `add_shift`, `calculate_monthly_salaries` | Employee, Shift |

### Equipment and buffet

| Class | Fields | Declared methods | Field associations |
|---|---|---|---|
| `FeedStock` | 2: `name`, `weight_kg` | `FeedStock`, `get_name`, `get_weight_kg`, `set_name`, `set_weight_kg` | — |
| `Headphones` | 1: `visitor` | `Headphones`, `is_assigned`, `set_visitor`, `get_visitor` | Visitor |
| `EmployeesEquipment` | 3: `durability`, `max_durability`, `is_equipped` | `EmployeesEquipment`, `~EmployeesEquipment`, `wear_out`, `repair`, `use`, `get_durability`, `is_broken`, `get_is_equipped`, `set_equipped` | — |
| `Mop` ← `EmployeesEquipment` | 2: `dirt_level`, `water_level` | `Mop`, `clean`, `wash`, `refill_water`, `use`, `get_dirt_level`, `get_water_level` | — |
| `PlumberEquipment` ← `EmployeesEquipment` | 2: `pressure_gauge`, `has_wrench` | `PlumberEquipment`, `check_pressure`, `fix_leak`, `use`, `repair`, `get_pressure_gauge`, `get_has_wrench` | — |
| `ElectricianEquipment` ← `EmployeesEquipment` | 2: `insulation_level`, `battery_level` | `ElectricianEquipment`, `test_circuit`, `replace_insulation`, `recharge_battery`, `use`, `get_insulation_level`, `get_battery_level` | — |
| `Buffet` | 2: `price`, `cash_register` | `Buffet`, `~Buffet`, `get_price`, `get_cash_register`, `add_to_cash_register` | — |
| `Drinks` ← `Buffet` | 1: `vol` | `Drinks`, `get_vol` | — |
| `AlcoholDrinks` ← `Buffet` | 2: `vol`, `degrees` | `AlcoholDrinks`, `get_vol`, `get_degrees` | — |
| `Food` ← `Buffet` | 2: `compound`, `weight` | `Food`, `get_compound`, `get_weight` | — |

### Visitors and services

| Class | Fields | Declared methods | Field associations |
|---|---|---|---|
| `Ticket` | 4: `price`, `bought`, `visitor_id`, `full_name` | `Ticket`, `get_price`, `is_bought`, `get_visitor_id`, `get_full_name`, `set_bought`, `set_visitor_data` | — |
| `Visitor` | 8: `age`, `services`, `visitor_id`, `full_name`, `ticket`, `money_count`, `can_enter`, `in_group` | `Visitor`, `operator=`, `buy_ticket`, `fill_ticket_data`, `add_service`, `get_services`, `remove_service`, `set_in_group`, `set_visitor_id`, `set_money_count`, `is_in_group`, `get_visitor_id`, `get_age`, `get_money_count`, `get_can_enter`, `get_full_name`, `get_ticket` | Favour, Ticket |
| `GroupVisit` | 3: `group_id`, `visitors`, `can_enter` | `GroupVisit`, `operator=`, `add_visitor`, `check_can_enter`, `get_group_id`, `get_can_enter`, `get_visitors` | Visitor |
| `DailyStatistics` | 7: `date`, `tickets_sold`, `visitors_count`, `groups_count`, `group_visitors`, `total_revenue`, `current` | `DailyStatistics`, `set_current`, `get_current`, `add_ticket_sale`, `add_visitors`, `add_group`, `add_revenue`, `get_date`, `get_tickets_sold`, `get_visitors_count`, `get_groups_count`, `get_group_visitors`, `get_total_revenue`, `print_report` | — |
| `Favour` | 1: `price` | `purchase`, `Favour`, `~Favour`, `get_price`, `buy` | — |
| `AudioGuide` ← `Favour` | 1: `headphones` | `AudioGuide`, `buy`, `assign_headphones`, `return_headphones` | Headphones |
| `Lounge` ← `Favour` | 1: `menu` | `Lounge`, `buy`, `add_to_menu`, `get_menu`, `buy_item` | Buffet |
| `VipVisit` ← `Favour` | 1: `bundles` | `find_bundle`, `VipVisit`, `buy`, `get_audio_guide`, `get_lounge` | Bundle, Visitor |
| `VipVisit::Bundle` | 2: `audio_guide`, `lounges` | — | AudioGuide, Lounge |

## Domain behaviors

The list counts rule checks, calculations, lookups, reports, and changes to the model state. Constructors, destructors, trivial accessors, empty hooks, and pure delegation are excluded. Overrides count separately when they apply different rules.

- `ContactInfo` (1) — `is_valid_contact`.
- `Identifier` (1) — `generate_next`.
- `Location` (1) — `calculate_distance_to`.
- `AuditLog` (3) — `record_action`, `filter_by_period`, `print_logs`.
- `Oceanarium` (4) — `open_for_day`, `close_for_day`, `add_tank`, `add_employee`.
- `Species` (1) — `is_compatible_with_species`.
- `VetVisit` (3) — `diagnostic`, `prescribe_treatment`, `close_visit`.
- `HealthRecord` (2) — `add_visit`, `status_diagnosis`.
- `FeedingPlan` (3) — `adjust_portion`, `check_diet_compliance`, `calculate_portion`.
- `Animal` (3) — `set_id`, `record_weight_change`, `assign_feeding_plan`.
- `Cub` (3) — `record_weight_change`, `move_to_habitat`, `on_vet_visit`.
- `BreedingProgram` (1) — `select_pair`.
- `AnimalTransfer` (1) — `approve_transfer`.
- `Habitat` (6) — `check_suitability_for_animal`, `calculate_biomass`, `add_animal`, `contains_animal`, `remove_animal`, `add_tank`.
- `Exhibit` (5) — `open_exhibit`, `close_exhibit`, `get_all_animals`, `add_tank`, `remove_tank`.
- `Tank` (8) — `start_filtration`, `emergency_drain`, `is_water_critical`, `evacuate_all_animals`, `clear_state`, `add_animal`, `contains_animal`, `remove_animal`.
- `Zone` (5) — `restrict_access`, `list_tanks`, `add_tank`, `remove_tank`, `add_exhibit`.
- `QuarantineTank` (2) — `admit_animal`, `release_after_check`.
- `HabitatMap` (2) — `locate_tank`, `add_zone`.
- `Janitor` (1) — `clean_area`.
- `Plumber` (1) — `repair_leak`.
- `Electrician` (1) — `inspect_circuits`.
- `Cashier` (1) — `sell_ticket`.
- `Shift` (3) — `generate_incidents`, `generate_events`, `calculate_shift_rate`.
- `Schedule` (2) — `add_shift`, `calculate_monthly_salaries`.
- `EmployeesEquipment` (2) — `wear_out`, `repair`.
- `Mop` (3) — `clean`, `wash`, `refill_water`.
- `PlumberEquipment` (3) — `check_pressure`, `fix_leak`, `repair`.
- `ElectricianEquipment` (3) — `test_circuit`, `replace_insulation`, `recharge_battery`.
- `WaterQuality` (1) — `is_within_norm`.
- `ControlSystem` (5) — `assign_employee`, `release_employee`, `can_work`, `perform_work`, `is_valid_date`.
- `WaterSensor` (2) — `checking`, `improving`.
- `SalinityController` (2) — `checking`, `improving`.
- `TemperatureController` (2) — `checking`, `improving`.
- `Ozonator` (2) — `checking`, `improving`.
- `Visitor` (3) — `buy_ticket`, `fill_ticket_data`, `remove_service`.
- `GroupVisit` (2) — `add_visitor`, `check_can_enter`.
- `DailyStatistics` (3) — `add_ticket_sale`, `add_group`, `print_report`.
- `Favour` (1) — `purchase`.
- `AudioGuide` (2) — `assign_headphones`, `return_headphones`.
- `Lounge` (2) — `add_to_menu`, `buy_item`.
- `VipVisit` (1) — `buy`.

## Custom exceptions

The exception hierarchy is declared in `include/exceptions.h`.

| Class | Condition |
|---|---|
| `OceanariumError` | Base domain error, also used for general failures such as a tank reaching capacity |
| `NullPointerError` | A required object pointer is null |
| `DuplicateEntityError` | An object, shift, service, or record already exists |
| `UnsuitableHabitatError` | Tank volume or water conditions fail the animal’s requirements |
| `EntityNotFoundError` | The requested object is absent |
| `InvalidActionError` | The current state disallows the operation |
| `SpeciesMismatchError` | The selected breeding pair contains animals of different species |
| `EmployeePermissionDeniedError` | The employee lacks permission to service the equipment |
| `WaterQualityViolationError` | A water parameter lies outside its valid range |
| `InvalidDateError` | The maintenance date is invalid |
| `StaffManagementError` | A staff shift violates a scheduling rule |
| `EquipmentFailureError` | A tool is broken or unfit for use |
| `ResourceDepletedError` | Water, battery charge, or another resource has run out |
| `InsufficientFundsError` | The balance is too low for the purchase |
| `ServiceNotPurchasedError` | A visitor tries to use a service they have yet to buy |
