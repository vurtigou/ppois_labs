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
printf '%s\n' "$output" | grep -Fq 'Unknown animal ID unknown'
printf '%s\n' "$output" | grep -Fq 'Animal ID already exists'
"$1" </dev/null >/dev/null
