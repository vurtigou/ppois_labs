#!/bin/sh
set -eu
output=$("$1" <<'COMMANDS'

help
open
open
add nemo 10
add nemo 1
add bad -1
feed
feed unknown
feed nemo
transfer nemo
transfer nemo
water
water 30
ticket
ticket 20
ticket 10
payroll
payroll
report
close
close
whatever
quit
COMMANDS
)
printf '%s\n' "$output" | grep -Fq 'Feeding portion: 500 g'
printf '%s\n' "$output" | grep -Fq 'Water temperature: 20 C'
printf '%s\n' "$output" | grep -Fq 'Animals in reserve: 1'
printf '%s\n' "$output" | grep -Fq 'Payments made: 1'
printf '%s\n' "$output" | grep -Eq 'Tickets sold:[[:space:]]+1$'
printf '%s\n' "$output" | grep -Fq 'Unknown animal ID: unknown'
printf '%s\n' "$output" | grep -Fq 'Animal ID already exists'
"$1" </dev/null >/dev/null

expect() {
    if ! printf '%s\n' "$output" | grep -Fq "$1"; then
        printf 'Missing CLI output: %s\n%s\n' "$1" "$output" >&2
        exit 1
    fi
}

output=$("$1" 2>&1 <<'COMMANDS'
open
zone 0
open
exhibit reef open
zone 3
exhibit reef open
exhibit service open
exhibit service close
exhibit nonexistent open
exhibit reef wave
tank Reserve-1
tank-move Reserve-1 reef
tank-move Unknown reef
add nemo 10
add auto 5
add coral 3 reef
add lion 2 reserve lionfish
add lion2 3 reserve lionfish
add wrong 2 reef unknown
add "" 1
compatibility nemo lion
compatibility nemo coral
stock
feed lion krill
feed nemo krill
feed nemo seaweed
cub mini nemo 2
cub invalid mini 2
cub invalid nemo 0
feed mini
weigh mini 2.06
weigh nemo 10kg
breed nemo coral 01122026
breed lion lion2 02122026
breed nemo nemo 01122026
breed nemo lion 01122026
breed nemo coral 31022026
transfer mini reserve
transfer lion invalid
vet coral "Needs observation" other
vet coral "" quarantine
vet coral "Needs observation" quarantine
animals
water-set reserve 4 30 23 7
release coral reserve
animals
release coral reef
vet coral Healthy
release coral reef
transfer nemo reserve
water-set reserve 7.4 30 23 7
transfer nemo reserve
transfer nemo reef
log 9999-01-01 -
log - 0001-01-01
log
contact "+375 (29) 123-45-67" info@example.org
contact wrong nowhere
distance Minsk 53.9,27.5667
distance Invalid bad
report
quit
COMMANDS
)
expect "Error: The zone is closed."
expect "Error: Unknown exhibit: nonexistent"
expect "Error: Exhibit action must be open or close."
expect "Tank Reserve-1 now in Tropical reef"
expect "Error: Unknown tank: Unknown"
expect "Error: Unknown species: unknown"
expect "Error: Animal ID cannot be empty."
expect "Species compatibility: incompatible"
expect "Species compatibility: compatible"
expect "plankton: 25 kg | krill: 10 kg"
expect "Feeding portion: 100 g"
expect "Error: Feed is incompatible with the animal's diet."
expect "Error: Unknown feed: seaweed"
expect "Error: Parent must be an adult in a regular habitat."
expect "Feeding portion: 60 g"
expect "Recorded weight for mini: 2.06 kg"
expect "Error: Invalid number: 10kg"
expect "Breeding scheduled for: 01122026"
expect "Breeding scheduled for: 02122026"
expect "Select two distinct animals."
expect "Cannot breed animals of different species"
expect "Error: Invalid breeding date."
expect "Animal mini cannot be moved"
expect "Error: Unknown habitat: invalid"
expect "Error: Use quarantine as the third argument."
expect "Error: Diagnosis cannot be empty."
expect "coral | 3 kg | Quarantine-1"
expect "Habitat: Unsuitable environment for animal coral"
expect "Returned from quarantine: coral"
expect "Animal coral not found in quarantine tank"
expect "fish-1 | 5 kg | Reef-1"
expect "Error: AnimalTransfer: Destination habitat is not suitable"
expect "Audit entries: 0"
expect "Contact: valid"
expect "Contact: invalid"
expect "Distance to Minsk: 0 km"
expect "Error: Invalid coordinates format"
expect "Animals in quarantine: 0"

output=$("$1" 2>&1 <<'COMMANDS'
drain
add drift 5
water-set reef 3 43 30 2
water-status reserve
drain
water-status
filter
system temperature status
system ph status 4
system bogus status
system ph assign 4
system ph assign 4
system ph repair 1 01122026
system ph repair 4 31022026
system ph repair 4 01122026
system ph repair 4 01122026
system ph release 4
system ph status
system salinity status
system oxygen status
water-set reef 20 30 22 7
water-status reef
hire janitor 5 2
hire janitor 5 1
hire diver 7 2
hire plumber 9 -2
staff
payroll 5
shift 5 09:00 10:00
shift 5 9:00 10:00
shift 5 24:00 25:00
shift 5 11:00 10:00
shift 999 09:00 10:00
payroll 5
payroll 5
work clean
work wash
work refill
work pressure
work leak
work circuits
work recharge
work insulate
work repair-tools
work unknown
report
quit
COMMANDS
)
expect "Water is fine. No emergency drain needed."
expect "Reef animals remaining: 0"
expect "Water temperature: 20 C | pH: 6.5 | salinity: 35 | oxygen: 5 | within limits"
expect "temperature: working | minimum access: 1 | assigned: 1"
expect "Use status, assign EMPLOYEE, release EMPLOYEE or repair EMPLOYEE DDMMYYYY."
expect "Error: Unknown system: bogus"
expect "Maintenance recorded: ph"
expect "already assigned to control system"
expect "is not allowed to service control system"
expect "Invalid date, expected ddmmyyyy"
expect "already recorded for system"
expect "ph: working | minimum access: 1 | assigned: 0 | repairs: 1"
expect "pH out of range"
expect "Error: Employee ID already exists."
expect "Error: Unknown employee role: diver"
expect "Error: Invalid employee ID or experience."
expect "5 | Janitor | experience: 2"
expect "Payments made: 0"
expect "Payments made: 1"
expect "Error: Use HH:MM for shift time."
expect "Error: Shift time out of range."
expect "Error: Shift must end after it starts on the same day."
expect "Error: Unknown employee: 999"
expect "Error: Unknown work operation: unknown"
expect "Animals in reserve: 1"

output=$("$1" 2>&1 <<'COMMANDS'
visitor 2 16 100 "Anna Example"
visitor 3 25 100 "Alex Example"
visitor 4 20 30 "No Ticket"
visitor 2 20 80 "Duplicate"
visitor 0 20 80 "Invalid"
visitor 8 18 50 "unfinished
balance 2
group-add 101 2
group-add 101 4
group-add 101 2
group-add 0 2
ticket 20 2 2
ticket 20 2
balance 2
group-entry 101
group-entry 101
group-add 101 3
group-add 102 4
group-entry 102
ticket 10 3
ticket 0 999
service 4 audio
service 2 unknown
rent 2 1
service 2 audio
service 3 audio
rent 2 1
rent 3 2 wrong
rent 3 1
balance 3
rent 3 9
return-headphones 2
service 2 lounge
menu
order 2 beer
order 2 bread
order 2 unknown
order 2 bread 1
rent 2 1 vip
service 3 vip
service 3 vip
rent 3 1 vip
return-headphones 3 vip
order 3 juice 6
order 3 beer 1
order 3 juice 7
report
quit
COMMANDS
)
expect 'Error: Unclosed quote.'
expect 'Error: Visitor ID already exists.'
expect 'Error: Visitor needs a positive ID and a name.'
expect 'Anna Example | balance: 100 | ticket: no'
expect 'Anna Example | balance: 86 | ticket: yes'
expect 'Error: Group ID must be positive.'
expect 'Error: Only a cashier can sell tickets.'
expect 'Group 101 admitted: 1'
expect 'Error: This group is already admitted.'
expect 'Group 102: no valid tickets'
expect 'Error: Unknown visitor: 999'
expect "didn't buy this service."
expect 'Error: Unknown service: unknown'
expect 'Choose standard or vip audio guide.'
expect 'Headphones unavailable. Money refunded.'
expect 'Alex Example | balance: 90 | ticket: yes | services: 0'
expect 'Error: Unknown headset: 9'
expect 'Headphones returned by Anna Example'
expect 'beer: 6 (5%, 18+)'
expect 'Underage visitor cannot buy alcohol.'
expect 'Error: Unknown buffet item: unknown'
expect 'Visitor has no VIP package.'
expect 'Service purchased: vip'
expect 'Headphones returned by Alex Example'
expect 'VIP lounge number must be from 1 to 6.'
expect 'Buffet cash register: 13'
expect 'Total revenue:   $57'
