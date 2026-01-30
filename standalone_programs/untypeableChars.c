#include <stdio.h>


int main(void)
{
    FILE* output=fopen("chars.txt", "w");
    if(output==NULL)
    {
        printf("**File could not be reached.\n");
        return 1;
    }

    fprintf(output, "ASCII CHARACTERS:\n");

    for(unsigned short i=0; i<=8; i++)
    {
        fprintf(output, "%i: %c\n", i, (char)i);
    }
    fprintf(output, "9: Horizontal tab\n");
    fprintf(output, "10: RETURN\n");
    fprintf(output, "11: %c\n", (char)11);
    fprintf(output, "12: %c\n", (char)12);
    fprintf(output, "13: Carriage return\n");
    for(unsigned short i=14; i<=31; i++)
    {
        fprintf(output, "%i: %c\n", i, (char)i);
    }

    fprintf(output, "\nNEGATIVE CHARACTERS:\n");
    for(short i=-1; i>=-127; i--)
    {
        fprintf(output, "%i: %c\n", i, (char)i);
    }

    fclose(output);

    printf("Open 'chars.txt' to access the characters.\n");

    return 0;
}