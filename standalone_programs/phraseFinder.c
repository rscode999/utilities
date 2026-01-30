//Phrase Finder v1.4.0 by TheMan132435421

//libraries
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <time.h>
#include <daniilkvayt.h>

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


//removes punctuation from a string of text
string removePunctuation(string text);
string removePunctuation(string text)
{
    for(int t=0; t<strlen(text); t++)
    {
        //if current character is alphanumeric, lowercase it
        if(isalnum(text[t]))
        {
            text[t]=tolower(text[t]);
        }
        else if(text[t]==39 || text[t]==96) //if apostrophe, replace aprostrophes with something that no one uses
        {
            text[t]=94;
        }
        else //if not apostrophe, turn character into a space
        {
            text[t]=' ';
        }
    }

    //output formatted text
    return text;
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
    //reminder of command-line arguments
    if(argc<=1)
   {
        printf("Note: enter './phraseFinder c' to find instances of words contained in other words\n\n");
   }

   //time info
    time_t orig_format;
    time(&orig_format);
    //pointer
    FILE *info;
    string str=".data";


    /*
    MAKING TARGET PHRASE AND TEXT:
    Bug fix to be able to compare the final character. The program said there were 2 instances of "the" in "the there", meaning the program counted words contained within other words.
    Being able to compare the final character lets the program see the ends of words.

    The user enters a raw text and target phrase. The real text and target phrase are arrays of characters.
    The first index of each character array becomes a space.
    Each character in the raw text and target phrase becomes a character in the text and target phrase.
    Then, the program inserts a space after all the characters are loaded into the text/target phrase.
    The rest of the characters in text and target phrases become NULLs to make them act like strings.

    Structure of final text or target phrase: " texttexttext NNN...NN", where N is a null character.
    The maximum permitted length for any entry is the maximum number of characters, plus 2 for the beginning and end spaces, plus 1 for a terminating NULL character
    */

   //prevents segmentation faults
    unsigned short max_strlen=2003;
    unsigned short max_chars=max_strlen-3;

    //user enters raw text
    string rawText;
    do
    {
        rawText=get_string("Enter text to search: ");
        if(strlen(rawText)>max_chars)
        {
            printf("Too many characters entered (%lu > %i)\n", strlen(rawText), max_chars);
        }
    }
    while(strlen(rawText)>max_chars);

    rawText=removePunctuation(rawText);


    //text, an array of 32767 characters (or however much max_strlen is- see above)
    char text[max_strlen];

    //first char will be a space.
    text[0]=' ';

    //repeat for the string length of the raw text, starting from 2nd character: load the current character in raw text into the text
    for(int i=1; i<=strlen(rawText); i++)
    {
        text[i]=rawText[i-1];
    }

    //insert a space just after the text
    text[strlen(rawText)+1]=' ';

    //fill the rest of the raw text with NULL characters
    for(int i=strlen(rawText)+2; i<max_strlen; i++)
    {
        text[i]=(char)0;
    }

    //////////////
    //Do the same to the target phrase

    string rawTargetPhrase;
    do
    {
        rawTargetPhrase=get_string("Enter phrase to find: ");
        if(strlen(rawTargetPhrase)>max_chars)
        {
            printf("You cannot enter more than %i characters.\n", max_chars);
        }
    }
    while(strlen(rawTargetPhrase)>max_chars);

    rawTargetPhrase=removePunctuation(rawTargetPhrase);

    char targetPhrase[max_strlen];

    //first character is a space
    targetPhrase[0]=' ';

    //fill indexes with raw target phrase's characters, starting from 2nd index
    for(int i=1; i<=strlen(rawTargetPhrase); i++)
    {
        targetPhrase[i]=rawTargetPhrase[i-1];
    }

    //the index after has a space
    targetPhrase[strlen(rawTargetPhrase)+1]=' ';

    //fill the rest of the indexes with NULLs
    for(int i=strlen(rawTargetPhrase)+2; i<max_strlen; i++)
    {
        targetPhrase[i]=(char)0;
    }

    ////////

    //Do the same for the contained target phrase, but don't add initial or final spaces
    char containedTarget[max_strlen];
    for(int i=0; i<=strlen(rawTargetPhrase); i++)
    {
        containedTarget[i]=rawTargetPhrase[i];
    }
    for(int i=strlen(rawTargetPhrase)+1; i<max_strlen; i++)
    {
        containedTarget[i]=0;
    }

    //////////////////////////////////////////////////////

    //If the target and text are the same string, say the total phrase count equals 1 and return 0 (end, declaring a successful run). Bug fix for if the target and text are the same length
    if(strcmp(rawTargetPhrase, rawText)==0 && strlen(rawTargetPhrase)==strlen(rawText))
    {
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
        printf("\nInstances of '%s': 1\n\n", rawText);
        return 0;
    }

    /*
    COMPARISON ALGORITHM
    Example text: "The pen is in my mouth". Target: " pen is "
    Repeat as many times as there are indexes in the target.
        Record characters.
    Compare the two strings. If they are the same, increase total.

    Repeat.
    */

   int count=0;
   int containedCount=0;
   char currentPhrase[max_chars];
    for(int i=0; i<max_chars; i++)
    {
        currentPhrase[i]=(char)0;
    }

    int recordIndex=0;



    //perform a count without bug fixes

    for(int t=0; t<strlen(text); t++)
    {
        //record, starting from the current character and repeating for as many letters as there are in the target phrase
        for(int r=t; r<strlen(targetPhrase)+t; r++)
        {
            currentPhrase[recordIndex]=text[r];
            recordIndex++;
        }
            recordIndex=0;

            //compare the current phrase and target phrase. If they are the same, add 1 to the total count
        if(strcmp(targetPhrase, currentPhrase)==0)
        {
                count++;
        }
            //reset currentPhrase
            for(int i=0; i<max_chars; i++)
            {
                currentPhrase[i]=(char)0;
            }
    }

    //remove the space from the start and end of the target phrase to make it look nicer, or at least turn it into a non-printable character
    targetPhrase[0]=(char)31;
    targetPhrase[strlen(targetPhrase)-1]=(char)31;

    //print the result
    printf("\nInstances of '%s': %i\n", targetPhrase, count);

    if(argc<=1)
    {
        printf("\n\n");
    }



    //do a count without bug fixes, meaning it finds all instances, even if contained within other words
    if(argc>1)
    {
        for(int t=0; t<strlen(text); t++)
        {
            //record, starting from the current character and repeating for as many letters as there are in the target phrase
            for(int r=t; r<strlen(containedTarget)+t; r++)
            {
                currentPhrase[recordIndex]=text[r];
                recordIndex++;
            }
            recordIndex=0;

            //compare the current phrase and target phrase. If they are the same, add 1 to the total count
            if(strcmp(containedTarget, currentPhrase)==0)
            {
                containedCount++;
            }

            //reset currentPhrase
            for(int i=0; i<max_chars; i++)
            {
                currentPhrase[i]=(char)0;
            }
        }

        //remove the space from the start and end of the target phrase to make it look nicer, or at least turn it into a non-printable character
        targetPhrase[0]=(char)31;
        targetPhrase[strlen(targetPhrase)-1]=(char)31;

        //print results
        printf("Instances of '%s', along with instances contained in other words: %i\n\n", targetPhrase, containedCount);
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

    //CONGRATUMALATIONS! YOU DID IT!
    return 0;
