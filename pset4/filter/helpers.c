#include "helpers.h"
#include "stdio.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j< width; j++)
        {
            float averageRGB = (float) (image[i][j].rgbtGreen + image[i][j].rgbtRed + image[i][j].rgbtBlue) / 3.0;
            int finalRGB = (int) round(averageRGB);
            image[i][j].rgbtGreen = finalRGB;
            image[i][j].rgbtRed = finalRGB;
            image[i][j].rgbtBlue = finalRGB;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int mapping[width];
    for (int i = 0, n = width / 2; i < n; i++)
    {
        mapping[i] = width - i - 1;
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0, n = width / 2; j < n; j++)
        {
            int tmpRed = image[i][j].rgbtRed;
            int tmpBlue = image[i][j].rgbtBlue;
            int tmpGreen = image[i][j].rgbtGreen;

            image[i][j].rgbtGreen = image[i][mapping[j]].rgbtGreen;
            image[i][j].rgbtRed = image[i][mapping[j]].rgbtRed;
            image[i][j].rgbtBlue = image[i][mapping[j]].rgbtBlue;

            image[i][mapping[j]].rgbtGreen = tmpGreen;
            image[i][mapping[j]].rgbtRed = tmpRed;
            image[i][mapping[j]].rgbtBlue = tmpBlue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmpImage[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //NOTE: Cannot use RGBTRIPLE as it has a max value of 255
            int pixelGreen = 0;
            int pixelBlue = 0;
            int pixelRed = 0;
            int pixelSum = 0;
            int loopX[2] = {-1, 1}; //left index 0, right index 1
            int loopY[2] = {-1, 1}; //top index 0, bottom index 1

            if (i == 0) //presets which pixels to loop through
            {
                loopY[0] = 0;
            }
            if (i == height - 1)
            {
                loopY[1] = 0;
            }
            if (j == 0)
            {
                loopX[0] = 0;
            }
            if (j == width -1)
            {
                loopX[1] = 0;
            }

            for (int k = loopY[0]; k <= loopY[1]; k++) //loops through the preset pixels to get values
            {
                for (int l = loopX[0]; l <= loopX[1]; l++)
                {
                    pixelRed += image[i + k][j + l].rgbtRed;
                    pixelBlue += image[i + k][j + l].rgbtBlue;
                    pixelGreen += image[i + k][j + l].rgbtGreen;
                    pixelSum++;
                }
            }
            tmpImage[i][j].rgbtRed = (int) round((float) pixelRed / (float) pixelSum); //sets pixel into tmp image
            tmpImage[i][j].rgbtBlue = (int) round((float) pixelBlue / (float) pixelSum);
            tmpImage[i][j].rgbtGreen = (int) round((float) pixelGreen / (float) pixelSum);
        }
    }

    for (int i = 0; i < height; i++) //make tmp image the actual image
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmpImage[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmpImage[height][width];
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int Gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //iterates over each pixel
            int pixGxRed[3][3]; //tmp array to store multiplied pixel values
            int pixGyRed[3][3];
            int pixGxBlue[3][3];
            int pixGyBlue[3][3];
            int pixGxGreen[3][3];
            int pixGyGreen[3][3];
            float pixXRed = 0.0;
            float pixYRed = 0.0;
            float pixXBlue = 0.0;
            float pixYBlue = 0.0;
            float pixXGreen = 0.0;
            float pixYGreen = 0.0;
            int pixFinalRed;
            int pixFinalGreen;
            int pixFinalBlue;
            int loopX[2] = {-1, 1}; //left index 0, right index 1
            int loopY[2] = {-1, 1}; //top index 0, bottom index 1

            if (i == 0) //presets which pixels to loop through & sets non-existent pixels to 0
            {
                loopY[0] = 0;
                for (int k = 0; k < 3; k++)
                {
                    pixGxRed[0][k] = 0;
                    pixGyRed[0][k] = 0;
                    pixGxBlue[0][k] = 0;
                    pixGyBlue[0][k] = 0;
                    pixGxGreen[0][k] = 0;
                    pixGyGreen[0][k] = 0;
                }
            }
            if (i == height - 1)
            {
                loopY[1] = 0;
                for (int k = 0; k < 3; k++)
                {
                    pixGxRed[2][k] = 0;
                    pixGyRed[2][k] = 0;
                    pixGxBlue[2][k] = 0;
                    pixGyBlue[2][k] = 0;
                    pixGxGreen[2][k] = 0;
                    pixGyGreen[2][k] = 0;
                }
            }
            if (j == 0)
            {
                loopX[0] = 0;
                for (int k = 0; k < 3; k++)
                {
                    pixGxRed[k][0] = 0;
                    pixGyRed[k][0] = 0;
                    pixGxBlue[k][0] = 0;
                    pixGyBlue[k][0] = 0;
                    pixGxGreen[k][0] = 0;
                    pixGyGreen[k][0] = 0;
                }
            }
            if (j == width -1)
            {
                loopX[1] = 0;
                for (int k = 0; k < 3; k++)
                {
                    pixGxRed[k][2] = 0;
                    pixGyRed[k][2] = 0;
                    pixGxBlue[k][2] = 0;
                    pixGyBlue[k][2] = 0;
                    pixGxGreen[k][2] = 0;
                    pixGyGreen[k][2] = 0;
                }
            }

            for (int k = loopY[0]; k <= loopY[1]; k++) //loops through the preset pixels to get values
            {
                for (int l = loopX[0]; l <= loopX[1]; l++)
                {
                    pixGxRed[k + 1][l + 1] = image[i + k][j + l].rgbtRed;
                    pixGyRed[k + 1][l + 1] = image[i + k][j + l].rgbtRed;
                    pixGxBlue[k + 1][l + 1] = image[i + k][j + l].rgbtBlue;
                    pixGyBlue[k + 1][l + 1] = image[i + k][j + l].rgbtBlue;
                    pixGxGreen[k + 1][l + 1] = image[i + k][j + l].rgbtGreen;
                    pixGyGreen[k + 1][l + 1] = image[i + k][j + l].rgbtGreen;
                }
            }

            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    pixGxRed[k][l] *= Gx[k][l]; //multiplies values with appropriate kernels
                    pixGyRed[k][l] *= Gy[k][l];
                    pixGxBlue[k][l] *= Gx[k][l];
                    pixGyBlue[k][l] *= Gy[k][l];
                    pixGxGreen[k][l] *= Gx[k][l];
                    pixGyGreen[k][l] *= Gy[k][l];
                }
            }

            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    pixXRed += pixGxRed[k][l]; //total up values in array to var
                    pixYRed += pixGyRed[k][l];
                    pixXBlue += pixGxBlue[k][l];
                    pixYBlue += pixGyBlue[k][l];
                    pixXGreen += pixGxGreen[k][l];
                    pixYGreen += pixGyGreen[k][l];
                }
            }

            pixFinalRed = (int) round(powf((powf(pixXRed, 2) + powf(pixYRed, 2)), 0.5)); //combine and round to int x and y values of pixel
            pixFinalBlue = (int) round(powf((powf(pixXBlue, 2) + powf(pixYBlue, 2)), 0.5));
            pixFinalGreen = (int) round(powf((powf(pixXGreen, 2) + powf(pixYGreen, 2)), 0.5));

            if (pixFinalRed > 255)
            {
                tmpImage[i][j].rgbtRed = 255;
            }
            else if (pixFinalRed < 0)
            {
                tmpImage[i][j].rgbtRed = 0;
            }
            else
            {
                tmpImage[i][j].rgbtRed = pixFinalRed;
            }

            if (pixFinalBlue > 255)
            {
                tmpImage[i][j].rgbtBlue = 255;
            }
            else if (pixFinalBlue < 0)
            {
                tmpImage[i][j].rgbtBlue = 0;
            }
            else
            {
                tmpImage[i][j].rgbtBlue = pixFinalBlue;
            }

            if (pixFinalGreen > 255)
            {
                tmpImage[i][j].rgbtGreen = 255;
            }
            else if (pixFinalGreen < 0)
            {
                tmpImage[i][j].rgbtGreen = 0;
            }
            else
            {
                tmpImage[i][j].rgbtGreen = pixFinalGreen;
            }
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = tmpImage[i][j].rgbtRed;
            image[i][j].rgbtBlue = tmpImage[i][j].rgbtBlue;
            image[i][j].rgbtGreen = tmpImage[i][j].rgbtGreen;
        }
    }
    return;
}
