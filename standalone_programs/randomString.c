//Random String v1.4.3 by TheMan132435421

#include <cs50.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <danilkvyat.h>

//contains RAND_MAX for random number generation
#define _DEFAULT_SOURCE

//random number functions in stdlib.h
long random(void);
void srandom(unsigned int seed);

//strlen but for numbers
unsigned short numberstrlen(int input);
unsigned short numberstrlen(int input)
{
    unsigned short output=0;

    if(input < 0)
    {
        output++;
    }

    do
    {
        input=input / 10;
        output++;
    }
    while(input <= -1 || input >= 1);

    return output;
}

int writeData(FILE* file, int retCode, int argc, string argv[]);
int writeData(FILE* file, int retCode, int argc, string argv[])
{
    //time info
    time_t orig_format;
    time(&orig_format);

    fprintf(file, "%i", retCode);
    for(short i=0; i<5-numberstrlen(retCode); i++)
    {
        fprintf(file, " ");
    }

    if(argc <= 999)
    {
        fprintf(file, "%i", argc);
        for(short i=0; i<5-numberstrlen(argc); i++)
        {
            fprintf(file, " ");
        }
    }
    else
    {
        fprintf(file, "999  ");
    }

    if(strlen(argv[0])<=30)
    {
        fprintf(file, "%s", argv[0]);
        for(short i=0; i<32-strlen(argv[0]); i++)
        {
            fprintf(file, " ");
        }
    }
    else
    {
        for(short i=0; i<30; i++)
        {
            fprintf(file, "%c", argv[0][i]);
        }
        fprintf(file, "  ");
    }

    fprintf(file, "%s", asctime(gmtime(&orig_format)));

    return 0;
}

int main(int argc, string argv[])
{
    unsigned short min_rand_length=16;
    unsigned short max_rand_length=1024;
    unsigned short chars_length=2048;


    //time info
    time_t orig_format;
    time(&orig_format);
    //pointer to log
    FILE *info;
    string str=".data";

    //if invalid command-line arguments, reject and return 1
    if(argc!=2 && argc!=3)
    {
        info=fopen(str, "a");
        if(info!=NULL)
        {
            writeData(info, 1, argc, argv);
            fclose(info);
        }
        else
        {
            printf("**FATAL ERROR\n");
            return 1;
        }
        printf("Error: expected './randomString [length (integer %i-%i)]'. Do not use commas\n", min_rand_length, max_rand_length);

        //abort with error
        return 1;
    }
    //check argv[1] for non-numbers. if so, abort
    for(int i=0; i<strlen(argv[1]); i++)
    {
        if((argv[1][i]<(char)48 || argv[1][i]>(char)57) && argv[1][i]!=(char)45)
        {
            info=fopen(str, "a");
            if(info!=NULL)
            {
                writeData(info, 1, argc, argv);
                fclose(info);
            }
            else
            {
                printf("**FATAL ERROR\n");
                return 1;
            }
            printf("Error: length must be an integer between %i and %i. Do not use commas\n", min_rand_length, max_rand_length);

            //abort with error
            return 1;
        }
    }


    //collect length from argv[1]
    int length=atoi(argv[1]);

    //check length. if not within limits, abort
    if(length<min_rand_length || length>max_rand_length)
    {
        info=fopen(str, "a");
        if(info!=NULL)
        {
            writeData(info, 1, argc, argv);
            fclose(info);
        }
        else
        {
            printf("**FATAL ERROR\n");
            return 1;
        }
        printf("Error: length must be an integer between %i and %i\n", min_rand_length, max_rand_length);

        //abort with error
        return 1;
    }

    //reminder of extra command-line arguments
    if(argc<=2)
    {
        printf("Note: Enter './randomString [length] a' for alphanumeric random strings\n\n");
    }
    else
    {
        printf("Printing alphanumeric string.\n\n");
    }


    //////////////////////////////////////////////////////////////


    //set starting index (to be determined randomly)
    unsigned short startIndex;

    //possible text (randomly determined numbers)
    unsigned short text[chars_length];

    //common letters
    string commonLetters="aeiouaeioudestinywatoderstletheandarefornotbuthadhaswasalloneoutyouhisherandeeeee";


    //determine random seed based on time
    srandom(time(NULL));

    //load text with random characters or numbers
    for(long i=0; i<chars_length; i++)
    {
        if(argc<=2)
        {
            //random number 1-26, plus length of common letters, plus 6
             text[i]=(short)( random() / ((double) RAND_MAX + 1)*(26+strlen(commonLetters)+6) ) + 1;
        }
        else
        {
            //random number 1-26, plus length of common letters, plus 8, plus 10 numbers
             text[i]=(short)( random() / ((double) RAND_MAX + 1)*(26+strlen(commonLetters)+8+10) ) + 1;
        }
    }


    //determine starting index, but only if segmentation faults are impossible
    startIndex=(short)( random() / ((double) RAND_MAX + 1)*(chars_length - length - 2) );



    if(argc<=2)
    {
        //loop throguh char list and print random characters (note: ASCII conversions only possible between char/int, so conversion to char was necessary)
        for(int i=startIndex; i<startIndex+length; i++)
        {
            //if integer within alphabet values, print character
            if(text[i]<=26)
            {
                printf("%c", (char)text[i] + 96);
            }
            //print index from common letters if 26-35
            else if(text[i]>26 && text[i]<=26+strlen(commonLetters))
            {
                printf("%c", (char)commonLetters[text[i]-27]);
            }
            //if none of the above, print a space
            else
            {
                printf(" ");
            }
        }
    }
    else
    {
        //loop throguh char list and print random characters
        for(int i=startIndex; i<startIndex+length; i++)
        {
            //if integer within alphabet values, print character
            if(text[i]<=26)
            {
                printf("%c", (char)text[i] + 96);
            }
            //print number if 26-35 (26 is 0, 27 is 1, etc.)
            else if(text[i]>26 && text[i]<=35)
            {
                printf("%c", (char)text[i] + 22);
            }
            //print from common letters if greater than 35
            else if(text[i]>35 && text[i]<=35+strlen(commonLetters))
            {
                printf("%c", (char)commonLetters[text[i]-36]);
            }
            //if none of the above, print a space
            else
            {
                printf(" ");
            }
        }
    }

    info=fopen(str, "a");
    if(info!=NULL)
    {
        writeData(info, 0, argc, argv);
        fclose(info);
    }
    else
    {
        printf("**FATAL ERROR\n");
        return 1;
    }

    printf("\n\n");
}