#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //try to open the input file
    FILE* input=fopen("input.txt", "r");
    if(input==NULL)
    {
        printf("**Error: input file 'input.txt' cannot be accessed.\n");
        return 1;
    }
    FILE* output=fopen("output.txt", "w");
    if(output==NULL)
    {
        printf("**Error: input file 'input.txt' cannot be accessed.\n");
        return 1;
    }


    char currentChar=(char)0;

    //loop through the input file
    while(currentChar != EOF)
    {
        //print quotation marks
        fprintf(output, "%c", (char)34);


        while(true)
        {
            //take current character
            currentChar=fgetc(input);

            //if end of file is reached, stop
            if(currentChar==EOF || currentChar=='\n')
            {
                break;
            }

            //print the character
            fprintf(output, "%c", currentChar);
        }

        //print quotation marks and comma
        fprintf(output, "%c, ", (char)34);
    }

    fclose(input);
    fclose(output);
    printf("done\n\n");
    return 0;
}