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

        //Print spaces in the middle
        printf("  ");
        // Print grid of bricks
        for (int l = 0; l < i + 1; l++)
        {
            printf("#");
        }
        printf("\n");
    }
}