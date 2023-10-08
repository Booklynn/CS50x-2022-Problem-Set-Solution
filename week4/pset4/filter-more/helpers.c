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

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
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
    // create copy image
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create copy image
    RGBTRIPLE temp_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp_image[i][j] = image[i][j];
        }
    }

    // Initialise arrays for GX and GY
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        // Loop through columns
        for (int j = 0; j < width; j++)
        {
            // Initialise ints
            float gx_red;
            float gx_blue;
            float gx_green;
            float gy_red;
            float gy_blue;
            float gy_green;

            gx_red = gx_blue = gx_green = gy_red = gy_blue = gy_green = 0;

            // loop vertical and horizontal
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {

                    // Check if pixel is outside rows
                    if (i + k < 0 || i + k >= height)
                    {
                        continue;
                    }

                    // Check if pixel is outside columns
                    if (j + l < 0 || j + l >= width)
                    {
                        continue;
                    }

                    // sum GX
                    gx_red += temp_image[i + k][j + l].rgbtRed * gx[k + 1][l + 1];
                    gx_green += temp_image[i + k][j + l].rgbtGreen * gx[k + 1][l + 1];
                    gx_blue += temp_image[i + k][j + l].rgbtBlue * gx[k + 1][l + 1];

                    // sum GY
                    gy_red += temp_image[i + k][j + l].rgbtRed * gy[k + 1][l + 1];
                    gy_green += temp_image[i + k][j + l].rgbtGreen * gy[k + 1][l + 1];
                    gy_blue += temp_image[i + k][j + l].rgbtBlue * gy[k + 1][l + 1];
                }
            }

            // Sobel operator
            int red = round(sqrt(gx_red * gx_red + gy_red * gy_red));
            int green = round(sqrt(gx_green * gx_green + gy_green * gy_green));
            int blue = round(sqrt(gx_blue * gx_blue + gy_blue * gy_blue));

            // Cap at 255
            if (red > 255)
            {
                red = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (blue > 255)
            {
                blue = 255;
            }
            
            // Assign new values to pixels
            image[i][j].rgbtRed = red;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtBlue = blue;
        }
    }
    return;
}