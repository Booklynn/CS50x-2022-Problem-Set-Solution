#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// type for store byte data
typedef uint8_t BYTE;
int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }
    else
    {
        FILE *card_raw = fopen(argv[1], "r");
        if (card_raw == 0)
        {
            printf("Error: cannot open %s\n", argv[1]);
            return 2;
        }
        BYTE buffer[BLOCK_SIZE];
        int count_jpg_file = 0;
        FILE *img_file = NULL;
        char filename[8];

        while (fread(&buffer, BLOCK_SIZE, 1, card_raw) == 1)
        {
            // If start of a new JPEG (0xff 0xd8 0xff 0xe*):
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
            {
                // If not first JPEG, close previous
                if (count_jpg_file != 0)
                {
                    fclose(img_file);
                }

                // Initialise file
                sprintf(filename, "%03i.jpg", count_jpg_file);
                img_file = fopen(filename, "w");
                count_jpg_file++;
            }
            // If JPEG has been found, write to file
            if (count_jpg_file != 0)
            {
                fwrite(&buffer, BLOCK_SIZE, 1, img_file);
            }
        }
        fclose(card_raw);
        fclose(img_file);
        return 0;
    }
}