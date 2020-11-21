#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    typedef uint8_t BYTE;

    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    if (fopen(argv[1], "r") == NULL)
    {
        printf("Forensic image cannot be opened for reading");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    int recovering = 1;
    int writing = 0;
    int fileNum = 0;
    char filename[8];
    FILE *img;

    while (recovering == 1)
    {
        BYTE buffer[512];
        int bytesRead = fread(buffer, 1, 512, file);

        if (bytesRead != 512 && writing == 1) //catch the end of file and close loop
        {
            //end of file
            fwrite(buffer, 1, bytesRead, img);
            recovering = 0;
            fclose(img);
            fclose(file);
            return 0;
        }
        else if (bytesRead != 512 && writing == 0)
        {
            recovering = 0;
            fclose(img);
            fclose(file);
            return 0;
        }

        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //start of a jpeg
            if (writing == 0) //not writing to file
            {
                writing = 1;
                sprintf(filename, "%03i.jpg", fileNum); //create filename
                fileNum++; //increment filenumber
                img = fopen(filename, "w"); //open a new jpg file
                fwrite(buffer, 1, 512, img); //write 512 bytes to new jpg
            }
            else
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", fileNum); //create filename
                fileNum++; //increment filenumber
                img = fopen(filename, "w"); //open a new jpg file
                fwrite(buffer, 1, 512, img); //write 512 bytes to new jpg
            }
        }
        else
        {
            //not a jpeg start
            if (writing == 1)
            {
                fwrite(buffer, 1, 512, img); //write 512 bytes to current jpg
            }
            else
            {
                continue;
            }
        }
    }
}