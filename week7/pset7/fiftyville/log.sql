-- Find crime scene from Humphrey Street log.
SELECT description FROM crime_scene_reports WHERE day = 28 AND month = 7 AND year = 2021 AND street = 'Humphrey Street';
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.

-- Find information from the 3 interview logs.
SELECT transcript FROM interviews WHERE day = 28 AND month = 7 AND year = 2021 AND id IN (161, 162, 163);
-- Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Find the license plate number for the car that left within a 10-minute window around the time of the theft logs.
SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND minute >= 15 AND minute <= 25;

-- Find the person ID from bank accounts that withdrawed money from ATM on Leggett Street logs.
SELECT bank_accounts.person_id
FROM bank_accounts
JOIN atm_transactions
ON bank_accounts.account_number = atm_transactions.account_number
WHERE day = 28
AND month = 7
AND year = 2021
AND atm_location = 'Leggett Street'
AND transaction_type = 'withdraw';

-- Find the caller number who talked for less than a minute logs.
SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration <= 60;

-- Find the receiver number who talked for less than a minute logs.
SELECT receiver FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration <= 60;

-- Find the airport ID of the destination for the flight that departed from Fiftyville in tomorrow's earliest logs.
SELECT destination_airport_id FROM flights WHERE day = 29 AND month = 7 AND year = 2021 AND hour = 8;

-- Find passengers passport number that departed from Fiftyville in tomorrow's earliest logs.
SELECT passport_number FROM passengers WHERE flight_id in (SELECT id FROM flights WHERE day = 29 AND month = 7 AND year = 2021 AND hour = 8);

-- Find The city the thief ESCAPED TO.
SELECT city
FROM airports
JOIN flights
ON airports.id = flights.destination_airport_id
WHERE airports.id = (
    SELECT destination_airport_id
    FROM flights
    WHERE day = 29
    AND month = 7
    AND year = 2021
    AND hour = 8
    )
    LIMIT 1;

-- Find The THIEF name.
SELECT people.name FROM people WHERE people.id IN (
    SELECT bank_accounts.person_id
    FROM atm_transactions
    JOIN bank_accounts
    ON atm_transactions.account_number = bank_accounts.account_number
    WHERE day = 28
    AND month = 7
    AND year = 2021
    AND atm_location = 'Leggett Street'
    AND transaction_type = 'withdraw'
    )
    AND people.license_plate IN (
        SELECT license_plate
        FROM bakery_security_logs
        WHERE day = 28
        AND month = 7
        AND year = 2021
        AND hour = 10
        AND minute >= 15
        AND minute <= 25
        )
        AND people.phone_number IN (
            SELECT caller
            FROM phone_calls
            WHERE day = 28
            AND month = 7
            AND year = 2021
            AND duration <= 60
            )
            AND people.passport_number in (
                SELECT passport_number
                FROM passengers
                WHERE flight_id in (
                    SELECT id
                    FROM flights
                    WHERE day = 29
                    AND month = 7
                    AND year = 2021
                    AND hour = 8
                    )
                    );

-- find The ACCOMPLICE name.
SELECT people.name FROM people WHERE people.phone_number IN (
    SELECT receiver
    FROM phone_calls
    WHERE day = 28
    AND month = 7
    AND year = 2021
    AND duration <= 60
    AND caller = (SELECT phone_number FROM people WHERE name = "Bruce")
    );