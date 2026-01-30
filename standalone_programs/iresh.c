//IMPROVED REECE'S SHELL

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

//for test prints
int writeData(FILE* file, int retCode, int argc, string argv[]);
unsigned short numberstrlen(int input);

//finds a line in a text. outputs the lines containing a given number
int readNumericKey(FILE* file, int key, short targetStage);
int readNumericKey(FILE* file, int key, short targetStage)
{
    //where the desired line is
    int lineStartIndex=0;
    //current character
    int currentIndex=0;

    //current character read from the file
    char currentChar='a';

    //part of line (first number set is stage 1, second number set is stage 2)
    short stage=0;


    //value to hold the output
    char currentNumberString[4];
    for(short r=0; r<4; r++)
    {
        currentNumberString[r]=(char)0;
    }
    int currentNumber=0;

    //if a match was found
    char matchFound='n';



    //loop through file
    while(currentChar!=EOF)
    {
        //move past the blank spaces
        while((int)currentChar<=32 && currentChar!=EOF)
        {
            currentChar=fgetc(file);
            currentIndex++;
        }


        stage++;


        //reset comparison variables
        currentNumber=0;
        for(short r=0; r<4; r++)
        {
            currentNumberString[r]=(char)0;
        }


        //read the printing characters
        while((int)currentChar>32 && currentChar!=EOF)
        {
            //if current character is numeric and there's room in the current number string, add the character to the number string
            if((int)currentChar>=48 && (int)currentChar<=57 && currentNumber<3)
            {
                currentNumberString[currentNumber]=currentChar;
                currentNumber++;
            }

            currentChar=fgetc(file);
            currentIndex++;
        }

        //printf("%s (%i) at %i\n", currentNumberString, stage, currentIndex);

        //turn string into integer and compare to targets. if so, run down the line and print it
        if(atoi(currentNumberString)==key && stage==targetStage)
        {
            //declare match found
            matchFound='y';

            //move current index in file to the beginning of the contained line or the beginning of the file
            while(currentChar!=(char)10 && currentChar!=EOF && currentIndex!=lineStartIndex)
            {
                currentChar=fgetc(file);
                fseek(file, -2, SEEK_CUR);
                currentIndex--;
            }


            //print the characters until the newline character is reached
            while(currentChar!=(char)10 && currentChar!=EOF)
            {
                currentChar=fgetc(file);
                currentIndex++;
                //printing EOF characters is unprofessional
                if(currentChar!=EOF)
                {
                    printf("%c", currentChar);
                }
            }

            //reset line index, reset stage
            lineStartIndex=currentIndex;
            stage=0;
        }

        //if all numbers have been passed, move to the beginning of the next line and reset the stage
        if(stage>=2)
        {
            while(currentChar!=(char)10 && currentChar!=EOF)
            {
                currentChar=fgetc(file);
                currentIndex++;
            }
            stage=0;
            lineStartIndex=currentIndex;
        }

    }

    if(matchFound!='y')
    {
        printf("No matches found.\n");
        return 1;
    }

    return 0;
}



int readStringKey(FILE* file, string key);
int readStringKey(FILE* file, string key)
{
    //current character read from the file
    char currentChar='a';

    //current index of file
    int currentIndex=0;
    //where the next line starts
    int lineStartIndex=0;

    //part of line (first number set is stage 1, second number set is stage 2)
    short stage=0;


    //string to hold the output (bigger than the number one because it's a string)
    char currentString[36];
    for(short r=0; r<36; r++)
    {
        currentString[r]=(char)0;
    }
    int currentStringIndex=0;

    char matchFound='n';

    //loop through file (follows same basic pattern as the integer key)
    while(currentChar!=EOF)
    {
        //move past the blank spaces
        while((int)currentChar<=32 && currentChar!=EOF)
        {
            currentChar=fgetc(file);
            currentIndex++;
        }


        stage++;


        //reset comparison variables
        currentStringIndex=0;
        for(short r=0; r<36; r++)
        {
            currentString[r]=(char)0;
        }

        //read the printing characters
        while((int)currentChar>32 && currentChar!=EOF)
        {
            //if current character is a non-space printing character and there's room in the current string, add the character to the string
            if((int)currentChar>32 && (int)currentChar<126 && currentStringIndex<35)
            {
                currentString[currentStringIndex]=currentChar;
                currentStringIndex++;
            }

            currentChar=fgetc(file);
            currentIndex++;
        }


        //compare string to targets. if so, run down the line and print it
        if(strcmp(key, currentString)==0 && stage==3)
        {
            matchFound='y';

            //continuously advance file pointer 1 index back until it reaches the beginning of the line
            while(currentChar!=(char)10 && currentChar!=EOF && currentIndex!=lineStartIndex)
            {
                currentChar=fgetc(file);
                fseek(file, -2, SEEK_CUR);
                currentIndex--;
            }

            currentChar=(char)127;

            //print the characters until the newline character is reached
            while(currentChar!=(char)10 && currentChar!=EOF)
            {
                currentChar=fgetc(file);
                currentIndex++;
                //printing EOF characters is unprofessional
                if(currentChar!=EOF)
                {
                    printf("%c", currentChar);
                    //printf("%s (%i) at %i\n", currentString, stage, currentIndex);
                }
                else
                {
                    printf("\n");
                }
                //return 0;
            }
            stage=0;
            lineStartIndex=currentIndex;
        }
        //if all strings have been passed, move to the beginning of the next line and reset the stage
        else if(stage>=3)
        {
            while(currentChar!=(char)10 && currentChar!=EOF)
            {
                currentChar=fgetc(file);
                currentIndex++;

            }
            stage=0;
            lineStartIndex=currentIndex;
        }
    }

    if(matchFound!='y')
    {
        printf("No matches found.\n\n");
        return 1;
    }
    else
    {
        printf("\n");
    }

    return 0;
}


/////////////////////////////////////////////////////////////////

int main(int argc, string argv[])
{
    //user's input string
    string input;

    //file's title
    string fileTitle="input.txt";
    //file
    FILE* file;
    string x=".data";


    //where individual commands will be stored and interpreted. three arguments, 30 characters plus one for the (char)0 at the end. reset all to (char)0
    char arguments[3][31];
    for(int a=0; a<3; a++)
    {
        for(int ch=0; ch<31; ch++)
        {
            arguments[a][ch]=(char)0;
        }
    }

    //for looping through the input string
    int array=0; //also determines operation code
    int character=0;


    //determines if the program is still going
    char programRunning='y';


    file=fopen(x, "a");
    if(file==NULL)
    {
        printf("**FATAL ERROR\n");
        return 1;
    }
    else
    {
        writeData(file, 0, argc, argv);
        fclose(file);
    }

    //the whole thing
    while(programRunning=='y')
    {
        /////////////////////////////////////////////////////
        //LOADING

        input=get_string(">> ");

        //separate input into arguments
        array=0;
        character=0;
        for(int l=0; l<strlen(input); l++) //l represents current character in input string
        {
            //end loop if all arrays are filled
            if(array >= 3)
            {
                break;
            }


            if(input[l]==' ')
            {
                arguments[array][character]=(char)0; //removes trailing space
                character=0;
                array++;
            }
            else
            {
                //load the current character if there is room (if statement prevents memory leak)
                if(character<30 && array<3)
                {
                    arguments[array][character]=input[l];
                    //increase character number
                    character++;
                }
            }
        }
        //clear input because it's loaded into the arguments
        input="";


        //lowercase all the commands (except the last argument) for simplicity
        for(int a=0; a<2; a++)
        {
            for(int ch=0; ch<30; ch++)
            {
                arguments[a][ch]=tolower(arguments[a][ch]);
            }
        }


        ////////////////////////////////////////////////////////////////////////////
        //INTERPRETING (yes, this is cursed but it's the most efficient way)
        array=0;

        /*
        "array" operation codes:
        BEGINNING:
        0=none

        AFTER ARG 0:
        1=help
        2=read

        AFTER ARG 1:
        120="-a"
        121="-n"
        122="-r"
        */

        //for one argument
        if(array==0)
        {
            //if first command is 'f' or "file", ask the user to enter a new file name
            if((strlen(arguments[0])==1  && arguments[0][0]=='f') || (strcmp(arguments[0], "file")==0 && strlen(arguments[0])==4))
            {
                fileTitle=get_string("Enter new file name, including the file extension: ");
                printf("\n");
            }
            //if first command is '?' or "help", help. set arrcode if other arguments
            else if((strlen(arguments[0])==1  && (arguments[0][0]=='?' || arguments[0][0]=='h')) || (strcmp(arguments[0], "help")==0 && strlen(arguments[0])==4))
            {
                //if next argument is empty, help
                if(strlen(arguments[1])<=0)
                {
                    printf("POSSIBLE COMMANDS:\n");
                    printf("'file', 'f': Changes the target file. Requires file extension\n");
                    printf("'help', 'h': Prints a list of possible commands. Enter 'help [command]' for detailed information about the command\n");
                    printf("'pwf'      : Prints the working file name\n");
                    printf("'quit', 'q': Exits the shell\n");
                    printf("'read', 'r': Reads the target file\n");
                    printf("'test', 't': Writes a test message to the target file\n");
                    printf("\n");
                }
                else
                {
                    array=1;
                }
            }
            //if first command is "pwf", print the working file name
            else if (strcmp(arguments[0], "pwf")==0 && strlen(arguments[0])==3)
            {
                printf("Current file name: %s\n\n", fileTitle);
            }
            //if first command is 'r' or "read", read. set arrcode if there are other arguments
            else if((strlen(arguments[0])==1  && arguments[0][0]=='r') || (strcmp(arguments[0], "read")==0 && strlen(arguments[0])==4))
            {
                if(strlen(arguments[1])<=0)
                {
                    //open file in read mode. if accessible, read all contents of the file
                    file=fopen(fileTitle, "r");
                    if(file==NULL)
                    {
                        printf("*Error: selected file is inaccessible\n");
                    }
                    else
                    {
                        while(character!=EOF)
                        {
                            character=fgetc(file);
                            if(character!=EOF)
                            {
                                printf("%c", character);
                            }
                        }
                        fclose(file);
                        character=0;
                        printf("\n");
                    }
                }
                else
                {
                    array=2;
                }
            }
            //if first command is 't' or "test" and there is one argument, test
            else if(((strlen(arguments[0])==1 && arguments[0][0]=='t') || (strcmp(arguments[0], "test")==0 && strlen(arguments[0])==4)))
            {
                if(strlen(arguments[1])<=0)
                {
                    //try to open file. if possible, write test message
                    file=fopen(fileTitle, "a");
                    if(file==NULL)
                    {
                        printf("*Error: selected file is inaccessible\n");
                    }
                    else
                    {
                        writeData(file, 84, argc, argv);
                        fclose(file);
                        printf("Test successful\n");
                    }
                    printf("\n");
                }
            }
            //if first command is 'q' or "quit" (regardless of other arguments), prevent the program's master loop from running again
            else if(arguments[0][0]=='q')
            {
                programRunning='n';
            }
            //if anything else, claim invalid command
            else
            {
                printf("Invalid command. Use 'help' for a list of possible commands\n");
            }
        }
        //for two arguments
        if(array>0 && array<100)
        {
            //if first argument was a help:
            if(array==1)
            {
                if((strlen(arguments[1])==1 && arguments[1][0]=='f') || (strcmp(arguments[1], "file")==0 && strlen(arguments[1])==4))
                {
                    printf("FILE\n");
                    printf("-Syntax: 'file', 'f'\n");
                    printf("-Additional arguments: None\n\n");
                    printf("Prompts the user to change the target file name\n\n");
                }
                else if((strlen(arguments[1])==1 && (arguments[1][0]=='h' || arguments[1][0]=='?')) || (strcmp(arguments[1], "help")==0 && strlen(arguments[1])==4))
                {
                    printf("HELP\n");
                    printf("-Syntax: 'h', '?', 'help' ... 'help [args]', '? [args]'\n");
                    printf("-Additional arguments: 'f', 'h', '?', 'q', 'r'. NOTE: These arguments are not proceeded by a '-' character.\n\n");
                    printf("Without additional arguments: prints a list of available commands. With arguments: prints proper usage of the desired command.\n\n");
                }
                else if(strcmp(arguments[1], "pwf")==0 && strlen(arguments[1])==3)
                {
                    printf("PWF (PRINT WORKING FILE)\n");
                    printf("-Syntax: 'pwf'\n");
                    printf("-Additional arguments: None\n\n");
                    printf("Prints the target file name\n\n");
                }
                else if((strlen(arguments[1])==1 && arguments[1][0]=='q') || (strcmp(arguments[1], "quit")==0 && strlen(arguments[1])==4))
                {
                    printf("QUIT\n");
                    printf("-Syntax: 'q', 'quit'\n");
                    printf("-Additional arguments: None. Additional arguments may be used, but they will not have meaning\n\n");
                    printf("Exits the shell.\n\n");
                }
                else if((strlen(arguments[1])==1 && arguments[1][0]=='r') || (strcmp(arguments[1], "read")==0 && strlen(arguments[1])==4))
                {
                    printf("READ\n");
                    printf("-Syntax: 'r', 'read' ... 'read [args] [target]'\n");
                    printf("-Additional arguments: '-r' [target (integer)] (print return value matches), '-a' [target (integer)] (print argument value matches), '-n' [target (string)] (print name value matches. Target string is case-sensitive)\n\n");
                    printf("Without additional arguments: reads the selected file. With arguments: prints only the lines containing the selected target.\n\n");
                }
                else if((strlen(arguments[1])==1 && arguments[1][0]=='t') || (strcmp(arguments[1], "test")==0 && strlen(arguments[1])==4))
                {
                    printf("TEST\n");
                    printf("-Syntax: 't', 'test'\n");
                    printf("-Additional arguments: None\n\n");
                    printf("Prints a test message to the selected file\n\n");
                }
                else
                {
                    printf("Invalid command. Help entry arguments are not proceeded by a '-' symbol. Example: 'help q', not 'help -q'\n");
                }
            }
            //if the arrcode is 2 (accepting arguments for reading), the first character is a '-', the argument is 2 characters long, and there is a third argument
            else if(array==2 && arguments[1][0]=='-' && strlen(arguments[1])==2 && strlen(arguments[2])>0)
            {
                //if argument is 'a', check for non-numeric characters. if so, end the loop and declare invalid. if check passed, set arrcode to 120 ("read -a x")
                if(arguments[1][1]=='a')
                {
                    for(short c=0; c<strlen(arguments[2]); c++)
                    {
                        if(arguments[2][c]<48 || arguments[2][c]>57)
                        {
                            printf("Invalid command. Use 'help' for a list of possible commands\n");
                            break;
                        }
                        else
                        {
                            array=120;
                        }
                    }
                }
                //if argument is 'n', set arrcode to 121 ("read -n something")
                else if(arguments[1][1]=='n')
                {
                    array=121;
                }
                //if argument is 'r', check for non-numeric characters. if so, end the loop and declare invalid. if check passed, set arrcode to 122 ("read -r x")
                else if(arguments[1][1]=='r')
                {
                    for(short c=0; c<strlen(arguments[2]); c++)
                    {
                        if(arguments[2][c]<48 || arguments[2][c]>57)
                        {
                            printf("Invalid command. Use 'help' for a list of possible commands\n");
                            break;
                        }
                        else
                        {
                            array=122;
                        }
                    }
                }
            }
            //if anything else, claim invalid command
            else
            {
                printf("Invalid command. Use 'help' for a list of possible commands\n");
            }
        }
        //for three arguments
        if(array>=100 && array<200)
        {
            if(array==120)
            {
                //open file and read second stage matches "-a"
                file=fopen(fileTitle, "r");
                if(file==NULL)
                {
                    printf("*Error: selected file is inaccessible\n");
                }
                else
                {
                    readNumericKey(file, atoi(arguments[2]), 2);
                    fclose(file);
                    printf("\n");
                }
            }
            else if(array==121)
            {
                //open file and read third stage string matches "-n"
                file=fopen(fileTitle, "r");
                if(file==NULL)
                {
                    printf("*Error: selected file is inaccessible\n");
                }
                else
                {
                    readStringKey(file, arguments[2]);
                    fclose(file);
                }
            }
            else if(array==122)
            {
                //open file and read first stage matches "-r"
                file=fopen(fileTitle, "r");
                if(file==NULL)
                {
                    printf("*Error: selected file is inaccessible\n");
                }
                else
                {
                    readNumericKey(file, atoi(arguments[2]), 1);
                    fclose(file);
                    printf("\n");
                }
            }
            else
            {
                printf("Invalid command. Use 'help' for a list of possible commands\n");
            }
        }


        ///////////////////////////////////////////////////////////////////////////

        //clear the commands after execution
        for(int a=0; a<3; a++)
        {
            for(int ch=0; ch<31; ch++)
            {
                arguments[a][ch]=(char)0;
            }
        }
        array=0;
    }

    printf("\n");
}

//strlen but for numbers
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

//requires numberstrlen
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