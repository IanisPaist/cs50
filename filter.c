#include "helpers.h"
#include "math.h"
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //interate over rows
    for (int i = 0; i < height; i++)
    {
        //iterate over columns, i.e each individual pixel
        for (int j = 0; j < width; j++)
        {
            float r = image[i][j].rgbtRed;
            float g = image[i][j].rgbtGreen;
            float b = image[i][j].rgbtBlue;

            float avg = round ((r + g + b) / 3);

            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    //interate over rows
    for (int i = 0; i < height; i++)
    {
        //iterate over columns, i.e each individual pixel
        for (int j = 0; j < width; j++)
        {

            float sephiaRed = round(image[i][j].rgbtRed * 0.393 + image[i][j].rgbtGreen * 0.769 + image[i][j].rgbtBlue * 0.189);
            if (sephiaRed > 255)
            {
                sephiaRed = 255;
            }

            float sephiaGreen = round(image[i][j].rgbtRed * 0.349 + image[i][j].rgbtGreen * 0.686 + image[i][j].rgbtBlue * 0.168);
            if (sephiaGreen > 255)
            {
                sephiaGreen = 255;
            }

            float sephiaBlue = round(image[i][j].rgbtRed * 0.272 + image[i][j].rgbtGreen * 0.534 + image[i][j].rgbtBlue * 0.131);
            if (sephiaBlue > 255)
            {
                sephiaBlue = 255;
            }

            //apply filter
            image[i][j].rgbtRed = sephiaRed;
            image[i][j].rgbtGreen = sephiaGreen;
            image[i][j].rgbtBlue = sephiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //create temp img
    RGBTRIPLE temp[height][width];

    //interate over rows
    for (int i = 0; i < height; i++)
    {
        //iterate over columns, i.e each individual pixel
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }

        //rewrite back to img
        int k = width - 1;
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][k];
            k--;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //create temp img
    RGBTRIPLE temp[height][width];

    //interate over rows
    for (int i = 0; i < height; i++)
    {
        //get number of rows in box of pixels
        int size_rows = 0;
        if(i == 0 || i == height - 1)
        {
            size_rows = 2;
        }
        else
        {
            size_rows = 3;
        }

        int rows[size_rows];

        if(i == 0)
        {
            rows[0] = 0;
            rows[1] = 1;
        }
        else if (i == height - 1)
        {
            rows[0] = -1;
            rows[1] = 0;
        }
        else
        {
            rows[0] = -1;
            rows[1] = 0;
            rows[2] = 1;
        }

        //iterate over columns, i.e each individual pixel
        for (int j = 0; j < width; j++)
        {
            //get number of columns in box of pixels
            int size_columns = 0;
            if(j == 0 || j == width - 1)
            {
                size_columns = 2;
            }
            else
            {
                size_columns = 3;
            }

            int columns[size_columns];

            if(j == 0)
            {
                columns[0] = 0;
                columns[1] = 1;
            }
            else if (j == width - 1)
            {
                columns[0] = -1;
                columns[1] = 0;
            }
            else
            {
                columns[0] = -1;
                columns[1] = 0;
                columns[2] = 1;
            }

            //now iterate over rows and columns in a box
            float red = 0;
            float green = 0;
            float blue = 0;

            for (int k = 0; k < size_rows; k++)
            {
                for (int m = 0; m < size_columns; m++)
                {
                    red = red + image[i + rows[k]][j + columns[m]].rgbtRed;
                    green = green + image[i + rows[k]][j + columns[m]].rgbtGreen;
                    blue = blue + image[i + rows[k]][j + columns[m]].rgbtBlue;
                }
            }

            temp[i][j].rgbtRed = round(red / (size_rows * size_columns));
            temp[i][j].rgbtGreen = round(green / (size_rows * size_columns));
            temp[i][j].rgbtBlue = round(blue / (size_rows * size_columns));
        }
    }

    //rewrite
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }


    return;
}
