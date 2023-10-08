#include <cs50.h>
#include <stdio.h>

long get_credit_card_number(void);
string calculate_credit_card_company(long number);

int main(void)
{
    long number = get_credit_card_number();
    printf("%s\n", calculate_credit_card_company(number));
}

long get_credit_card_number(void)
{
    long number;
    // Ask for credit card number
    do
    {
        number = get_long("Number: ");
    }
    while (number <= 0);
    return number;
}

string calculate_credit_card_company(long number)
{
    //Init starting variables
    long credit_card_number_length = number;
    int total_credit_card_number_length = 0;
    int total_digits_without_multiplied = 0;
    int total_second_to_last_digits = 0;
    int total_first_to_last_digits = 0;

    //Luhn’s Algorithm
    while (number != 0)
    {
        //Find second_to_last_digit and digits_without_multiplied
        int second_to_last_digit = number % 100;
        int digits_without_multiplied = second_to_last_digit % 10;
        number = number / 10;
        second_to_last_digit = second_to_last_digit / 10;
        number = number / 10;
        second_to_last_digit *= 2;

        // Check if second_to_last_digit is not signle digit
        if (second_to_last_digit > 9)
        {
            // Spilt 2 digits into single digits
            while (second_to_last_digit != 0)
            {
                int single_digit = second_to_last_digit % 10;
                total_first_to_last_digits = single_digit + total_first_to_last_digits;
                second_to_last_digit = second_to_last_digit / 10;
            }
        }

        // Sum second_to_last_digits with total_digits_without_multiplied
        total_second_to_last_digits = second_to_last_digit + total_second_to_last_digits;
        total_digits_without_multiplied = digits_without_multiplied + total_digits_without_multiplied;
    }

    // Sum total for checking last digit
    int last_digits_sum = (total_second_to_last_digits + total_first_to_last_digits + total_digits_without_multiplied) % 10;

    // Check invalid credit card
    if (last_digits_sum != 0)
    {
        return "INVALID";
    }

    // Check credit card company name by length and first two digits
    while (credit_card_number_length != 0)
    {
        int first_credit_card_number = credit_card_number_length / 10;
        credit_card_number_length = credit_card_number_length / 10;
        total_credit_card_number_length++;
        if (first_credit_card_number == 4)
        {
            total_credit_card_number_length++;
            if (total_credit_card_number_length == 13 || total_credit_card_number_length == 16)
            {
                return "VISA";
            }
        }
        if (first_credit_card_number == 34 || first_credit_card_number == 37)
        {
            total_credit_card_number_length += 2;
            if (total_credit_card_number_length == 15)
            {
                return "AMEX";
            }
        }
        if (first_credit_card_number >= 51 && first_credit_card_number <= 55)
        {
            total_credit_card_number_length += 2;
            if (total_credit_card_number_length == 16)
            {
                return "MASTERCARD";
            }
        }
    }
    return "INVALID";
}