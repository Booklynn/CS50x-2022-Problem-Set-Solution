#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;
    do
    {
        //Prompt to enter number 1-8
        n = get_int("Height: ");
    }
    while (n <= 0 || n > 8);

    for (int i = 0; i < n; i++)
    {
        //Print spaces
        for (int j = 0; j < (n - i) - 1; j++)
        {
            printf(" ");
        }

        // Print grid of bricks
        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }
        printf("\n");
    }
}