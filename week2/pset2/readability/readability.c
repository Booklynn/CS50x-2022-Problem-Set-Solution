#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string texts);
int count_sentences(string text);

int main(void)
{
    //Ask for input
    string text = get_string("Text: ");

    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    //The average number of letters per 100 words in the text
    float L = ((float)letters / (float)words) * (float)100;
    //The average number of sentences per 100 words in the text
    float S = ((float)sentences / (float)words) * (float)100;

    //Coleman-Liau formula
    float index = 0.0588 * L - 0.296 * S - 15.8;
    index = (int)round(index);

    //If the index number is less than 1
    if (index <= 0)
    {
        printf("Before Grade 1\n");
    }
    //If the resulting index number is 16 or higher
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    //The grade level as defined by the Coleman-Liau index
    else
    {
        printf("Grade %i\n", (int)index);
    }
}

int count_letters(string text)
{
    int total_letters = 0;
    // Count only letters
    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            total_letters++;
        }
    }
    return total_letters;
}

int count_words(string text)
{
    int total_words = 1;
    // Find words by couting only space and then + 1
    for (int i = 0; i < strlen(text); i++)
    {
        if (isspace(text[i]))
        {
            total_words++;
        }
    }
    return total_words;
}

int count_sentences(string text)
{
    int total_sentences = 0;
    // Find sentences by finding . or a ! or a ?
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            total_sentences++;
        }
    }
    return total_sentences;
}