#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

bool only_alphabetical(string key);
int count_alphabetical(string key);
bool unique_alphabetical(string key);
char substitution_cipher(char plaintext, string key);

int main(int argc, string argv[])
{
    string s_key = argv[1];

    /* Exit program if not provide command-line arguments
        and more than 1 command-line argument */
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // Exit program if lenght of key is not equal to 26 lenght
    else if (count_alphabetical(s_key) != 26)
    {
        printf("Usage: ./Key must contain 26 characters.\n");
        return 1;
    }
    // Exit program if a key is alphabetical
    else if (only_alphabetical(s_key) != true)
    {
        printf("Usage: ./Key must only contain alphabetic characters.\n");
        return 1;
    }
    // Exit program if a duplicate key found
    else if (unique_alphabetical(s_key) != true)
    {
        printf("Usage: ./Key must not contain repeated characters.\n");
        return 1;
    }
    else
    {
        // Ask for input plaintext
        string plaintext = get_string("plaintext: ");

        printf("ciphertext: ");

        for (int i = 0; i < strlen(plaintext); i++)
        {
            // Encipher
            char ciphertext = substitution_cipher(plaintext[i], s_key);
            printf("%c", ciphertext);
        }
        printf("\n");
        return 0;
    }
}

bool only_alphabetical(string key)
{
    // Check if key is an alphabetic
    for (int i = 0; i < strlen(key); i++)
    {
        if (!isalpha(key[i]))
        {
            return false;
        }
    }
    return true;
}

int count_alphabetical(string key)
{
    // Count key
    int count_key = 0;
    for (int i = 0; i < strlen(key); i++)
    {
        count_key++;
    }
    return count_key;
}

bool unique_alphabetical(string key)
{
    // Key must be unique
    for (int i = 0; i < strlen(key); i++)
    {
        for (int k = i + 1; k < strlen(key); k++)
        {
            if (toupper(key[i]) == toupper(key[k]))
            {
                return false;
            }
        }
    }
    return true;
}

char substitution_cipher(char plaintext, string key)
{
    // Compare index char of plaintext with index key
    if (islower(plaintext))
    {
        // Convert plaintext to 0 - 25
        int p = (int)plaintext - 97;
        return tolower(key[p]);
    }
    else if (isupper(plaintext))
    {
        // Convert plaintext to 0 - 25
        int p = (int)plaintext - 65;
        return toupper(key[p]);
    }
    // If the char is not a letter, return the same char unchanged.
    return plaintext;
}