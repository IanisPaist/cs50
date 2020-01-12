#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // check that only one command line argument
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    //create a pointer to a file and open it
    FILE *inputptr = fopen(argv[1], "r");
    if (inputptr == NULL)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    //useful declarations
    int counter = 0;
    bool firstjpg  = false;

    //create pointer to new file
    FILE *outptr = NULL;

    //create an array of data to store 512 bytes of info
    unsigned char jpeg[512];

    //loop into blocks of 512 bytes until eof
    while(fread(jpeg, 512, 1, inputptr) == 1)
    {
        //check for indicators of jpeg
        if (jpeg[0] == 0xff && jpeg[1] == 0xd8 && jpeg[2] == 0xff && (jpeg[3] & 0xf0) == 0xe0)
        {

            //check if first jpg
            if (firstjpg == false)
            {
                //create name of output file
                char outfilename[100];
                sprintf(outfilename, "%03i.jpg", counter);

                //open output file
                outptr = fopen(outfilename,"w");
                if (outptr == NULL)
                {
                    printf("Usage: ./recover image\n");
                    return 1;
                }

                //write to file
                fwrite(jpeg, 512, 1, outptr);

                //change firstjpg to now that first have been found
                firstjpg = true;
            }
            else
            {
                //close old file
                fclose(outptr);

                //increase counter by 1
                counter += 1;

                //create new output file
                char outfilename[100];
                sprintf(outfilename, "%03i.jpg", counter);

                //open output file
                outptr = fopen(outfilename,"w");
                if (outptr == NULL)
                {
                   printf("Usage: ./recover image\n");
                return 1;
                }

                //write to file
                fwrite(jpeg, 512, 1, outptr);
            }
        }
        else
        {
            if (firstjpg == true)
            {
                //write to file
                fwrite(jpeg, 512, 1, outptr);
            }
        }
    }
}
