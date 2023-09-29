#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint8_t BYTE;

int main()
{

    //open drive to read
    FILE *in = fopen( "/dev/disk2", "rb");    

    //create file to write for output 
    FILE *out = NULL;

    //block of 512 bytes to check at once
    BYTE buff[512];

    //counter for naming serially
    int counter = 0;
    char fname[12];

    //if the file we tried is not opening
    if (NULL == in)
    {
        perror("Error opening  hard drive");
        return 1;
    }

    //itirate until the end of file, in each time, 512 bytes are stored in buff
    while (fread(&buff, 512, 1, in) == 1)
    {

        //if first 4 bytes matches of jpeg, we found new jpeg
        if ((buff[0] == 0xff) && (buff[1] == 0xd8) && (buff[2] == 0xff) && (buff[3] >= 0xe0) && (buff[3] <= 0xef))
        {
            printf(" jpeg found\n");

            // Close the previous file if it's open
            if (out != NULL)
            {
                fclose(out);
            }

            //create new file jpeg with name from fname 
            sprintf(fname, "%03i.jpg", counter);
            out = fopen(fname, "w");
            counter++;

            //copy current buffered bytes to new jpeg file we just opened
            fwrite(buff, 512, 1, out);
        }
        //same as above but for pdf
        else  if ((buff[0] == 0x25) && (buff[1] == 0x50) && (buff[2] == 0x44) && (buff[3] == 0x46) && (buff[4] == 0x2d))
        {
            printf(" jpeg found\n");

            // Close the previous file if it's open
            if (out != NULL)
            {
                fclose(out);
            }

            sprintf(fname, "%03i.pdf", counter);
            out = fopen(fname, "w");
            counter++;

            fwrite(buff, 512, 1, out);
        }

        //if no new jpeg is found, just write the buffered bytes to existing file
        else if (out != NULL)
        {
            fwrite(buff, 512, 1, out);
        }

    }
    //at the end, if last file is started writing and not closed  
    if (out != NULL)
    {
        fclose(out);
    }

    //close the file we opened to read
    fclose(in);
    return 0;
}
