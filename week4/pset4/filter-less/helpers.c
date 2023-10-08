#include "helpers.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Find avg rgb of each pixel and then convert each pixel to grayscale
            int avg_new_rgb = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            image[i][j].rgbtBlue = avg_new_rgb;
            image[i][j].rgbtGreen = avg_new_rgb;
            image[i][j].rgbtRed = avg_new_rgb;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // number of algorithms for converting an image to sepia
            int sepia_red = round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue);
            int sepia_green = round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue);
            int sepia_blue = round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue);

            // 255, the maximum value for an 8-bit color value. In that case, the red, green, and blue values should be capped at 255
            if (sepia_red > 255)
            {
                sepia_red = 255;
            }

            if (sepia_green > 255)
            {
                sepia_green = 255;
            }

            if (sepia_blue > 255)
            {
                sepia_blue = 255;
            }

            image[i][j].rgbtBlue = sepia_blue;
            image[i][j].rgbtGreen = sepia_green;
            image[i][j].rgbtRed = sepia_red;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // if (width )
    // int half = width / 2;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0, n = width / 2; j < n; j++)
        {
            // Swap left side to right side
            RGBTRIPLE swap = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = swap;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp_image[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int total_red = 0;
            int total_blue = 0;
            int total_green = 0;

            float count_pixel = 0.00;

            // Get pixels
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    int current_x = i + x;
                    int current_y = j + y;

                    if (current_x < 0 || current_x > (height - 1) || current_y < 0 || current_y > (width - 1))
                    {
                        continue;
                    }
                    // Get value of pixel
                    total_red += image[current_x][current_y].rgbtRed;
                    total_green += image[current_x][current_y].rgbtGreen;
                    total_blue += image[current_x][current_y].rgbtBlue;

                    count_pixel++;
                }
                // Cal avg of pixels
                temp_image[i][j].rgbtRed = round(total_red / count_pixel);
                temp_image[i][j].rgbtGreen = round(total_green / count_pixel);
                temp_image[i][j].rgbtBlue = round(total_blue / count_pixel);
            }
        }
    }
    // Copy pixel into image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp_image[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp_image[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp_image[i][j].rgbtBlue;
        }
    }
    return;
}