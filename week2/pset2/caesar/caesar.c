#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool only_digits(string s_key);
char rotate(char plaintext, int key);

int main(int argc, string argv[])
{
    // Exit program if not provide argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else
    {
        // Check if this string key contain only digit 0-9
        string s_key = argv[1];
        bool is_digit = only_digits(s_key);

        if (is_digit != true)
        {
            // Exit program
            printf("Usage: ./caesar key\n");
            return 1;
        }
        else
        {
            // Convert string key to integer key
            int key = atoi(s_key);

            // Ask for input plaintext
            string plaintext = get_string("plaintext: ");

            printf("ciphertext: ");

            for (int i = 0; i < strlen(plaintext); i++)
            {
                // Caesar's algorithm
                char c_rotate = rotate(plaintext[i], key);
                printf("%c", c_rotate);
            }
            printf("\n");
            return 0;
        }
    }
}

bool only_digits(string s_key)
{
    for (int i = 0; i < strlen(s_key); i++)
    {
        // Return false if char is not digit
        if (!isdigit(s_key[i]))
        {
            return false;
        }
    }
    return true;
}

char rotate(char plaintext, int key)
{
    if (islower(plaintext))
    {
        // Find 1 - 25 (a - z)
        int p = (int)plaintext - 97;
        // Convert int back to char and wrap around from z to a
        char c = ((p + key) % 26) + 97;
        return c;
    }
    else if (isupper(plaintext))
    {
        // Find 1 - 25 (A - Z)
        int p = (int)plaintext - 65;
        // Convert int back to char and wrap around from Z to A
        char c = ((p + key) % 26) + 65;
        return c;
    }
    else
    {
        // If the char is not a letter, return the same char unchanged.
        return plaintext;
    }
}