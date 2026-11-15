# Лабораторная работа №2 — Океанариум

Крупный ООП-проект на C++17, моделирующий работу океанариума: экспозиции, заселение и переселение животных, кормление и ветеринарные осмотры, контроль воды и обслуживание оборудования, сотрудников и смены, билеты, групповые визиты и платные услуги.

## Сборка и запуск

Из каталога с распакованной папкой `lab2`:

```sh
make -C lab2
./lab2/build/oceanarium
make -C lab2 test
make -C lab2 coverage CXX=g++ GCOV=gcov
```

Для сборки и запуска тестов подходят GNU Make и компилятор с поддержкой C++17. Цель `coverage` дополнительно требует Python 3.9+ и парные версии GNU GCC и gcov. На macOS с установленным GNU GCC передайте его имена, например `CXX=g++-15 GCOV=gcov-15`. Для обычной сборки на macOS подходит системный Apple Clang через `CXX=c++`.

`make test` запускает тесты модели и четыре независимых консольных сеанса. `make coverage` измеряет покрытие исполняемых строк модели и CLI, включая использованные методы в заголовках. Порог строго выше 90%. При локальной проверке на GCC 14.2 получено **97,23%** (1859 из 1912 исполняемых строк).

## Консольное меню

`help` покажет синтаксис команд. Слова с пробелами заключайте в двойные кавычки. Демонстрационный набор содержит рифовый, резервный и карантинный аквариумы, кассира с ID 1, уборщика 2, сантехника 3, электрика 4 и посетителя с ID 1. Созданные за сеанс объекты хранятся в памяти до выхода.

| Раздел | Команды |
|---|---|
| Океанариум, зоны и экспозиции | `open`, `close`, `zone`, `exhibit`, `tank`, `tank-move`, `report`, `log` |
| Животные и ветеринария | `add`, `cub`, `animals`, `compatibility`, `transfer`, `weigh`, `feed`, `stock`, `vet`, `release`, `breed` |
| Вода и оборудование | `water`, `water-set`, `water-status`, `filter`, `drain`, `system`, `work` |
| Сотрудники | `hire`, `staff`, `shift`, `payroll` |
| Посетители и услуги | `visitor`, `balance`, `group-add`, `group-entry`, `ticket`, `service`, `rent`, `return-headphones`, `menu`, `order` |
| Справочные операции | `contact`, `distance`, `help`, `quit` |

### Пример для проверки работы

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

В примере `feed nemo` рассчитывает 500 г, а `feed mini` — 60 г. Первая команда `animals` показывает животное с ID `coral` в карантине. Групповой билет за 20 стоит 14 после скидки, а VIP-пакет даёт доступ к аудиогиду и шести залам. Для аварийного сценария в новом сеансе добавьте взрослую рыбу, задайте рифу параметры вне нормы командой `water-set` и выполните `drain`. Эвакуация использует резервный аквариум.

`app/main.cpp` запускает консоль. В `app/Console.cpp` находятся справка и диспетчер команд, а обработчики сгруппированы по предметным областям в `app/*Commands.cpp`. Заселение, переселение, начисления и покупки выполняет доменная модель из `src/`.

## Количественные требования

| Показатель | Минимум | Реализовано |
|---|---:|---:|
| Классы предметной области | 50 | 50 классов и 2 структуры |
| Поля | 150 | 159 нестатических, включая поля структур, или 155 без них |
| Содержательные поведения | 100 | 103 по перечню ниже |
| Ассоциации | 30 | 46 направленных связей через поля |
| Собственные исключения | 12 | 15, включая базовое исключение |

Наследуемые поля учитываются один раз в объявляющем классе. В таблицах ниже также показаны 18 статических полей. Ассоциации считаются как уникальные пары «класс, тип его поля», в том числе поля-коллекции, указатели и ссылки. Наследование и связи только через параметры методов в число 46 не входят.

## Классы, поля, методы и ассоциации

### Ядро

| Класс | Поля | Объявленные методы | Связи через поля |
|---|---|---|---|
| `ContactInfo` | 2: `phone`, `email` | `ContactInfo`, `is_valid_contact`, `get_phone`, `get_email` | — |
| `Identifier` | 2: `prefix`, `number` | `Identifier`, `generate_next`, `get_prefix`, `get_next_number` | — |
| `Location` | 3: `city`, `address`, `coordinates` | `Location`, `calculate_distance_to`, `get_city`, `get_address`, `get_coordinates`, `is_valid` | — |
| `LogEntry` | 2: `timestamp`, `action` | — | — |
| `AuditLog` | 2: `entries`, `max_size` | `AuditLog`, `record_action`, `filter_by_period`, `print_logs`, `size` | LogEntry |
| `Oceanarium` | 6: `name`, `is_open`, `location`, `tanks`, `employees`, `audit_log` | `Oceanarium`, `~Oceanarium`, `operator=`, `open_for_day`, `close_for_day`, `add_tank`, `add_employee`, `get_name`, `get_location`, `get_tanks`, `get_employees`, `get_audit_log` | AuditLog, Employee, Location, Tank |

### Животные и ветеринария

| Класс | Поля | Объявленные методы | Связи через поля |
|---|---|---|---|
| `Species` | 7: `latin_name`, `diet_type`, `min_tank_volume`, `ph_range`, `salinity_range`, `temperature_range`, `oxygen_range` | `Species`, `is_compatible_with_species`, `get_latin_name`, `get_min_tank_volume`, `get_diet_type`, `get_ph_range`, `get_salinity_range`, `get_temperature_range`, `get_oxygen_range` | — |
| `VetVisit` | 4: `animal`, `diagnosis`, `treatment`, `is_closed` | `VetVisit`, `diagnostic`, `prescribe_treatment`, `close_visit`, `get_diagnosis`, `get_treatment` | Animal |
| `HealthRecord` | 3: `animal`, `visits`, `current_status` | `HealthRecord`, `operator=`, `status_diagnosis`, `add_visit` | Animal, VetVisit |
| `FeedingPlan` | 3: `animal`, `feed_items`, `portion_grams` | `FeedingPlan`, `adjust_portion`, `add_feed_item`, `check_diet_compliance`, `calculate_portion`, `get_portion_grams`, `set_animal` | Animal, FeedStock |
| `Animal` | 7: `animal_id`, `species`, `habitat`, `current_tank`, `health_record`, `feeding_plan`, `weight_kg` | `Animal`, `~Animal`, `can_be_moved_to_habitat`, `operator=`, `set_id`, `get_current_tank`, `set_current_tank`, `record_weight_change`, `move_to_habitat`, `assign_feeding_plan`, `can_breed`, `on_vet_visit`, `get_feeding_plan`, `get_id`, `get_weight`, `get_habitat`, `get_species`, `get_health_record` | FeedingPlan, Habitat, HealthRecord, Species, Tank |
| `Cub` ← `Animal` | 1: `age` | `Cub`, `record_weight_change`, `move_to_habitat`, `can_be_moved_to_habitat`, `can_breed`, `on_vet_visit`, `get_age` | — |
| `BreedingProgram` | 3: `species`, `pairs`, `status` | `BreedingProgram`, `select_pair`, `schedule_breeding` | Animal, Species |
| `AnimalTransfer` | 2: `animal`, `destination_habitat` | `AnimalTransfer`, `approve_transfer` | Animal, Habitat |

### Аквариумы и экспозиции

| Класс | Поля | Объявленные методы | Связи через поля |
|---|---|---|---|
| `Habitat` | 4: `habitat_type`, `volume_liters`, `water_quality`, `tanks` | `Habitat`, `check_suitability_for_animal`, `calculate_biomass`, `add_animal`, `contains_animal`, `remove_animal`, `add_tank`, `get_water_quality`, `get_habitat_type` | Tank, WaterQuality |
| `Exhibit` | 4: `title`, `description`, `tanks`, `opened` | `Exhibit`, `open_exhibit`, `close_exhibit`, `get_all_animals`, `add_tank`, `remove_tank`, `is_open`, `get_title`, `get_tanks` | Tank |
| `Tank` | 8: `tank_id`, `parent_habitat`, `filtration_system`, `sensors`, `animals`, `exhibit`, `animal_count`, `is_filtration_on` | `Tank`, `~Tank`, `operator=`, `start_filtration`, `emergency_drain`, `is_water_critical`, `evacuate_all_animals`, `clear_state`, `set_exhibit`, `get_tank_id`, `get_habitat`, `get_animals`, `get_exhibit`, `get_animal_count`, `add_animal`, `add_sensor`, `contains_animal`, `remove_animal` | Animal, ControlSystem, Exhibit, Habitat |
| `Zone` | 3: `zone_name`, `exhibits`, `access_level` | `Zone`, `restrict_access`, `list_tanks`, `add_tank`, `remove_tank`, `evacuate_tank_animals`, `add_exhibit`, `get_zone_name`, `get_exhibits` | Exhibit |
| `QuarantineTank` ← `Tank` | 1: `quarantine_reason` | `QuarantineTank`, `set_diagnosis`, `admit_animal`, `release_after_check` | — |
| `HabitatMap` | 2: `zones`, `scale` | `HabitatMap`, `locate_tank`, `add_zone` | Zone |

### Вода и обслуживание

| Класс | Поля | Объявленные методы | Связи через поля |
|---|---|---|---|
| `WaterQuality` | 4: `ph`, `salinity`, `temperature`, `oxygen_level` | `WaterQuality`, `get_ph`, `get_salinity`, `get_temperature`, `get_oxygen_level`, `set_ph`, `set_salinity`, `set_temperature`, `set_oxygen_level`, `is_within_norm` | — |
| `ControlSystem` | 6: `id`, `employees`, `access_level`, `is_good_working`, `repair_dates`, `quality` | `ControlSystem`, `~ControlSystem`, `checking`, `improving`, `on_repair`, `get_id`, `get_access_level`, `get_is_good_working`, `get_repair_dates`, `get_employees`, `assign_employee`, `release_employee`, `can_work`, `perform_work`, `is_valid_date`, `mark_broken` | Employee, WaterQuality |
| `WaterSensor` ← `ControlSystem` | 4: `vol_co2`, `vol_alkali`, `max_co2`, `max_alkali` | `WaterSensor`, `checking`, `improving`, `on_repair` | — |
| `SalinityController` ← `ControlSystem` | 4: `vol_water`, `vol_brine`, `max_water`, `max_brine` | `SalinityController`, `checking`, `improving`, `on_repair` | — |
| `TemperatureController` ← `ControlSystem` | 2: `amperage`, `initial_amperage` | `TemperatureController`, `checking`, `improving`, `on_repair` | — |
| `Ozonator` ← `ControlSystem` | 4: `availible_vol_ozon`, `vol_coal`, `max_ozon`, `max_coal` | `Ozonator`, `checking`, `improving`, `on_repair` | — |

### Сотрудники и смены

| Класс | Поля | Объявленные методы | Связи через поля |
|---|---|---|---|
| `Employee` | 7: `rate`, `experience`, `id`, `amount_penalty`, `amount_bonus`, `salaries`, `access_level` | `Employee`, `~Employee`, `get_position`, `get_pin_rate`, `get_bon_rate`, `get_rate`, `get_experience`, `get_id`, `get_access_level`, `get_amount_penalty`, `get_amount_bonus`, `get_salaries`, `add_salary`, `update_last_salary`, `add_penalty`, `add_bonus` | — |
| `Janitor` ← `Employee` | 5: `mop`, `position`, `min_rate`, `pin_rate`, `bon_rate` | `Janitor`, `get_position`, `get_pin_rate`, `get_bon_rate`, `set_mop`, `clean_area` | Mop |
| `Plumber` ← `Employee` | 5: `equipment`, `position`, `min_rate`, `pin_rate`, `bon_rate` | `Plumber`, `get_position`, `get_pin_rate`, `get_bon_rate`, `set_equipment`, `repair_leak` | PlumberEquipment |
| `Electrician` ← `Employee` | 5: `equipment`, `position`, `min_rate`, `pin_rate`, `bon_rate` | `Electrician`, `get_position`, `get_pin_rate`, `get_bon_rate`, `set_equipment`, `inspect_circuits` | ElectricianEquipment |
| `Cashier` ← `Employee` | 5: `tickets_sold`, `position`, `min_rate`, `pin_rate`, `bon_rate` | `Cashier`, `get_position`, `get_pin_rate`, `get_bon_rate`, `sell_ticket`, `get_tickets_sold` | — |
| `Shift` | 8: `employee`, `start_time`, `end_time`, `incidents`, `has_penalty`, `has_bonus`, `paid`, `shift_rate` | `Shift`, `generate_incidents`, `generate_events`, `calculate_shift_rate`, `get_incidents`, `get_employee`, `get_shift_rate` | Employee |
| `Schedule` | 2: `shifts`, `employees` | `Schedule`, `add_shift`, `calculate_monthly_salaries` | Employee, Shift |

### Инвентарь и буфет

| Класс | Поля | Объявленные методы | Связи через поля |
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

### Посетители и услуги

| Класс | Поля | Объявленные методы | Связи через поля |
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

## Содержательные поведения

В подсчёт включены только методы из списка. В него входят проверки правил, расчёты, поиск, отчёты и операции с состоянием модели. Конструкторы, деструкторы, простые аксессоры, пустые обработчики и методы, которые только делегируют вызов, не учитываются. Переопределения учитываются отдельно, если применяют разные правила.

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

## Собственные исключения

Все исключения объявлены в [exceptions.h](include/exceptions.h).

| Класс | Когда возникает |
|---|---|
| `OceanariumError` | Базовая ошибка домена. Также используется для общих ошибок, например исчерпания вместимости аквариума |
| `NullPointerError` | Обязательный объект не передан |
| `DuplicateEntityError` | Объект, смена, услуга или запись уже добавлены |
| `UnsuitableHabitatError` | Объём или параметры воды не подходят животному |
| `EntityNotFoundError` | Искомый объект отсутствует |
| `InvalidActionError` | Операция недопустима в текущем состоянии |
| `SpeciesMismatchError` | Для размножения выбраны животные разных видов |
| `EmployeePermissionDeniedError` | У сотрудника нет допуска к обслуживанию оборудования |
| `WaterQualityViolationError` | Показатель воды находится вне допустимого диапазона |
| `InvalidDateError` | Дата обслуживания некорректна |
| `StaffManagementError` | Нарушено правило работы со сменами сотрудников |
| `EquipmentFailureError` | Инструмент сломан или не пригоден к использованию |
| `ResourceDepletedError` | Недостаточно воды, заряда или других ресурсов |
| `InsufficientFundsError` | Не хватает денег для операции |
| `ServiceNotPurchasedError` | Посетитель пытается воспользоваться услугой без покупки |
