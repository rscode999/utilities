#include <stdio.h>
#include <cs50.h>
#include <ctype.h>

int main(void)
{
    FILE* input=fopen("input.txt", "r");
    if(input==NULL)
    {
        printf("**Error: input file 'input.txt' cannot be accessed.\n");
        return 1;
    }

    int charCount=0;

    char currentChar=(char)0;
    while(currentChar != EOF)
    {
        currentChar=(int)fgetc(input);
        if(currentChar>=32 && currentChar<=126)
        {
            printf("%c", currentChar);
            charCount++;
        }
        else if(currentChar==10)
        {
            printf("\\");
            charCount++;
        }
    }

    fclose(input);
    printf("\nNumber of Characters: %i\n", charCount);
    return 0;
}