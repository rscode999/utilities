#Python Random String Generator v1.0.1

#import the crap
import numpy
import sys
import random
import datetime

#len but for numbers
def numberlen(number):
    output=0;

    if number<0:
        while True:
            number=number/10;
            output=output+1;

            if number>-1:
                return output+1;
    else:
        while True:
            number=number/10;
            output=output+1;

            if number<1:
                return output;

def main():
    extFile=".data"

    min_length=16
    max_length=1024

    #if incorrect number of command-line arguments, error message and abort
    if not len(sys.argv)==2 and not len(sys.argv)==3:
        try:
            f=open(extFile, "a");
            writeData(f, 1, sys.argv);
            f.close()
        except:
            print("**FATAL ERROR")
            return 1
        print("Error: expected 'python randomString.py [length (integer 16-1024)]'. Do not use commas")
        return 1

    #if non-number found in 2nd command-line argument, error message and abort
    for c in sys.argv[1]:

        if not c.isnumeric():
            try:
                f=open(extFile, "a");
                writeData(f, 1, sys.argv);
                f.close()
            except:
                print("**FATAL ERROR")
                return 1
            print("Error: length must be an integer between " + str(min_length) + " and " + str(max_length) + ". Do not use commas")
            return 1

    #extract length from command line index 1
    length=int(sys.argv[1])

    #if length is invalid, print error message and abort
    if length<min_length or length>max_length:
        try:
            f=open(extFile, "a");
            writeData(f, 1, sys.argv);
            f.close()
        except:
            print("**FATAL ERROR")
            return 1

        print("Error: length must be an integer between " + str(min_length) +  " and " + str(max_length))
        return 1


    if len(sys.argv)==2:
        print("Note: Enter 'python randomString.py [length] a' for alphanumeric random strings")
    else:
        print("Printing alphanumeric string.")


    print()

    #text (as numbers) and the randomly determined starting index
    text=numpy.empty(2048, dtype=numpy.uint8)
    startIndex=random.randint(0,2048-length-2)


    #biased letters and length of biased letter string
    commonLetters="aeioudestinywatoderstletheandarefornotbuthadhaswasalloneoutyouhisherandeeeee"
    commonLetters_length=len(commonLetters)


    try:
        f=open(extFile, "a");
        writeData(f, 0, sys.argv);
        f.close()
    except:
        print("**FATAL ERROR")
        return 1


    #load text with random integers. 0-25=A-Z, 26-35=0-9, others are biased letters
    if len(sys.argv)<=2:
        for l in range(0,2048):
            text[l]=random.randint(0, 25+commonLetters_length+12)
    else:
        for l in range(0,2048):
            text[l]=random.randint(0, 25+10+commonLetters_length+15)





    #convert random numbers to letters, depending on if alphanumeric strings is selected

    if len(sys.argv)<=2:
        # 0-25=A-Z, 26-length of commonLetters=common letters
        for p in range(startIndex, startIndex+length):
            if text[p]<=25: #0-25
                print(chr(text[p]+97), end='')

            elif 25 < text[p] < 26+commonLetters_length: #26-length
                print(commonLetters[text[p]-26], end='')

            else:
                print(" ", end='')

    else:
        # 0-25=A-Z, 26-35=numners 0-9, 36-length of commonLetters=common letters
        for p in range(startIndex, startIndex+length):
            if text[p]<=25: #0-25
                print(chr(text[p]+97), end='')

            elif 26 < text[p] <= 36: #27-36
                print(chr(text[p]+21), end='')

            elif 36 < text[p] < 36+commonLetters_length: #37-length
                print(commonLetters[text[p]-37], end='')

            else:
                print(" ", end='')


    print()
    print()


def writeData(file, retCode, argv):
    file.write(str(retCode));
    for i in range(0, 5-numberlen(retCode)):
        file.write(" ");

    if len(argv)<=999:
        file.write(str(len(argv)));
        for i in range(0, 5-numberlen(len(argv))):
            file.write(" ");
    else:
        file.write("999  ");

    if len(argv[0])<=30:
        file.write(argv[0]);
        for i in range(0, 32-len(argv[0])):
            file.write(" ");
    else:
        for i in range(0, 30):
            file.write(argv[0][i]);
        file.write("  ");

    file.write(str(datetime.datetime.now()));

    file.write("\n");


#call main function
if __name__ == "__main__":
    main()


