//Benson Tran
//CSCI 3341: Intro to Operating Systems
//Professor: Ray Hashemi
//Assignment 1
//DUE: November 2, 2017

#include <iostream>
#include <pthread.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <vector>

using namespace std;
void* sifterT(void*);
void* decoderT(void*);
void*railFence(void*);
void* hillT(void*);
void* pinnacleT(void*);
bool isValidInput(string input);

int main() {
    string input;
    pthread_t sifter;

    //Ask user for input
    cout << "Enter your message: " << endl;
    getline(cin, input);

    //Sifter thread
    if (pthread_create(&sifter, NULL, sifterT, &input))
    {
        cout << "Error creating sifter thread";
        return 0;
    }

    if (pthread_join(sifter, NULL))
    {
        cout << "Error joining sifter thread";
        return 0;
    }

    return 0;
}

//Sifter Thread
void * sifterT(void * in)
{
    string *input = (string *) (in);

    string message = *input;

    bool valid = false;
    string temp = message;
    int counter = 0;

    //Checks if user input is valid
    //Use string.find to find if ***, **, and * exists
    //Remove asterisks from temp message
    while (counter <= 3)
    {
        cout << "Sifting..." << endl;

        string str3 = "***";
        size_t found3 = temp.find(str3);
        if (found3 != string::npos)
        {
            temp.erase(found3, 3);
            cout << "\nPART 3 IS VALID" << endl;
        }
        else
        {
            cout << "\nPART3 IS INVALID" << endl;
        }

        string str2 = "**";
        size_t found2 = temp.find(str2);
        if (found2 != string::npos)
        {
            temp.erase(found2, 2);
            cout  << "\nPART 2 IS VALID" << endl;
        }
        else
        {
            cout << "\nPART2 IS INVALID" << endl;
        }

        string str1 = "*";
        size_t found1 = temp.find(str1);
        if (found1 != string::npos)
        {
            temp.erase(found1, 1);
            cout << "\nPART 1 IS VALID" << endl;
        }
        else
        {
            cout << "\nPART1 IS INVALID" << endl;
        }

        //After removing *'s from temp, check if there are *'s any left in the message
        size_t checkForAsterisks = temp.find(str1);

        if((found3 != string::npos) && (found2 != string::npos) && (found1 != string::npos) && (checkForAsterisks == string::npos) && (isValidInput(temp)))
        {
            valid = true;
        }

        counter++;

        if (!valid)
        {
            //Break at 3 attempts
            if(counter >= 3)
            {
                cout << "Maximum amount of tries reached, exiting.." << endl;
                break;
            }

            cout << "Your input is invalid (eg: null portions, or other means of invalidity), please try again: " << endl;
            message = "";
            getline(cin, message);
            temp = message;
        }
        else
        {
            cout << "\nYOUR INPUT IS VALID" << endl << endl;

            counter = 4;
            pthread_t decoder;

            if (pthread_create(&decoder, NULL, decoderT, &message))
            {
                cout << "Error creating decoder thread";
                return 0;
            }

            if (pthread_join(decoder, NULL))
            {
                cout << "Error joining decoder thread";
                return 0;
            }
        }
    }

}

//Helper for Sifter: checks if string has numbers.
//eg: makes sure that the input sections aren't null
//Asterisk handling is checked in the sifter thread itself
bool isValidInput(string input)
{
    if (string::npos != input.find_first_of("0123456789"))
        return true;
    else
        return false;
}

//Decoder Thread
void * decoderT(void * in)
{
    string *input = (string *) (in);

    string message = *input;
    string temp = message;

    string str3 = "***";
    size_t found3 = message.find(str3);
    temp.replace(found3, 3, "   ");

    string str2 = "**";
    size_t found2 = temp.find(str2);
    temp.replace(found2, 2, "  ");

    string str1 = "*";
    size_t found1 = temp.find(str1);
    temp.replace(found1, 1, " ");

    cout << "Decoding..." << endl << endl;

    //*, **, *** order on input
    if ((found1 < found2) && (found2 < found3))
    {

        string part1 = message.substr(1, found2 - 1);
        string part2 = message.substr(found2 + 2, found3 - (found2 + 2));
        string part3 = message.substr(found3 + 3, message.length());
        cout << "Part 1: " << part1 << endl;
        cout << "Part 2: " << part2 << endl;
        cout << "Part 3: " << part3 << endl;

        pthread_t fence, hill, pinnacle;

        if (pthread_create(&fence, NULL,railFence , &part1))
        {
            cout << "Error creating Fence thread";
            return 0;
        }

        if (pthread_join(fence, NULL))
        {
            cout << "Error joining Fence thread";
            return 0;
        }

        if (pthread_create(&hill, NULL, hillT, &part2))
        {
            cout << "Error creating Hill thread";
            return 0;
        }

        if (pthread_join(hill, NULL))
        {
            cout << "Error joining Hill thread";
            return 0;
        }

        if (pthread_create(&pinnacle, NULL, pinnacleT, &part3))
        {
            cout << "Error creating Pinnacle thread";
            return 0;
        }

        if (pthread_join(pinnacle, NULL))
        {
            cout << "Error joining Pinnacle thread";
            return 0;
        }
    }
    else
    {
        //*, ***, ** order on input
        if ((found1 < found3) && (found3 < found2))
        {

            string part1 = message.substr(1, found3 - 1);
            string part3 = message.substr(found3 + 3, found2 - (found3 + 3));
            string part2 = message.substr(found2 + 2, message.length());
            cout << "Part 1: " << part1 << endl;
            cout << "Part 3: " << part3 << endl;
            cout << "Part 2: " << part2 << endl;

            pthread_t fence, hill, pinnacle;

            if (pthread_create(&fence, NULL,railFence , &part1))
            {
                cout << "Error creating Fence thread";
                return 0;
            }

            if (pthread_join(fence, NULL))
            {
                cout << "Error joining Fence thread";
                return 0;
            }

            if (pthread_create(&hill, NULL, hillT, &part2))
            {
                cout << "Error creating Hill thread";
                return 0;
            }

            if (pthread_join(hill, NULL))
            {
                cout << "Error joining Hill thread";
                return 0;
            }

            if (pthread_create(&pinnacle, NULL, pinnacleT, &part3))
            {
                cout << "Error creating Pinnacle thread";
                return 0;
            }

            if (pthread_join(pinnacle, NULL))
            {
                cout << "Error joining Pinnacle thread";
                return 0;
            }

        }
        else
        {
            //**, *, *** order on input
            if((found2 < found1) && (found1 < found3))
            {
                string part2 = message.substr(2, found1 - 2);
                string part1 = message.substr(found1 + 1, found3 - (found1 + 1));
                string part3 = message.substr(found3 + 3, message.length());
                cout << "Part 2: " << part2 << endl;
                cout << "Part 1: " << part1 << endl;
                cout << "Part 3: " <<part3 << endl;

                pthread_t fence, hill, pinnacle;

                if (pthread_create(&fence, NULL,railFence , &part1))
                {
                    cout << "Error creating Fence thread";
                    return 0;
                }

                if (pthread_join(fence, NULL))
                {
                    cout << "Error joining Fence thread";
                    return 0;
                }

                if (pthread_create(&hill, NULL, hillT, &part2))
                {
                    cout << "Error creating Hill thread";
                    return 0;
                }

                if (pthread_join(hill, NULL))
                {
                    cout << "Error joining Hill thread";
                    return 0;
                }

                if (pthread_create(&pinnacle, NULL, pinnacleT, &part3))
                {
                    cout << "Error creating Pinnacle thread";
                    return 0;
                }

                if (pthread_join(pinnacle, NULL))
                {
                    cout << "Error joining Pinnacle thread";
                    return 0;
                }
            }
            else
            {
                //**, ***, * order on input
                if ((found2 < found3) && (found3 << found1))
                {
                    string part2 = message.substr(2, found3 - 2);
                    string part3 = message.substr(found3 + 3, found1 - (found3 + 3));
                    string part1 = message.substr(found1 + 1, message.length());
                    cout << "Part 2: " << part2 << endl;
                    cout << "Part 3: " << part3 << endl;
                    cout << "Part 1: " << part1 << endl;

                    pthread_t fence, hill, pinnacle;

                    if (pthread_create(&fence, NULL,railFence , &part1))
                    {
                        cout << "Error creating Fence thread";
                        return 0;
                    }

                    if (pthread_join(fence, NULL))
                    {
                        cout << "Error joining Fence thread";
                        return 0;
                    }

                    if (pthread_create(&hill, NULL, hillT, &part2))
                    {
                        cout << "Error creating Hill thread";
                        return 0;
                    }

                    if (pthread_join(hill, NULL))
                    {
                        cout << "Error joining Hill thread";
                        return 0;
                    }

                    if (pthread_create(&pinnacle, NULL, pinnacleT, &part3))
                    {
                        cout << "Error creating Pinnacle thread";
                        return 0;
                    }

                    if (pthread_join(pinnacle, NULL))
                    {
                        cout << "Error joining Pinnacle thread";
                        return 0;
                    }
                }
                else
                {
                    //***, **, * order on input
                    if ((found3 < found2) && (found2 < found1))
                    {
                        string part3 = message.substr(3, found2 - 3);
                        string part2 = message.substr(found2 + 2, found1 - (found2 + 2));
                        string part1 = message.substr(found1 + 1, message.length());
                        cout << "Part 3: " << part3 << endl;
                        cout << "Part 2: " << part2 << endl;
                        cout << "Part 1: " << part1 << endl;

                        pthread_t fence, hill, pinnacle;

                        if (pthread_create(&fence, NULL,railFence , &part1))
                        {
                            cout << "Error creating Fence thread";
                            return 0;
                        }

                        if (pthread_join(fence, NULL))
                        {
                            cout << "Error joining Fence thread";
                            return 0;
                        }

                        if (pthread_create(&hill, NULL, hillT, &part2))
                        {
                            cout << "Error creating Hill thread";
                            return 0;
                        }

                        if (pthread_join(hill, NULL))
                        {
                            cout << "Error joining Hill thread";
                            return 0;
                        }

                        if (pthread_create(&pinnacle, NULL, pinnacleT, &part3))
                        {
                            cout << "Error creating Pinnacle thread";
                            return 0;
                        }

                        if (pthread_join(pinnacle, NULL))
                        {
                            cout << "Error joining Pinnacle thread";
                            return 0;
                        }
                    }
                    else
                    {
                        //***, *, ** order on input
                        if ((found3 < found1) && (found1 < found2))
                        {
                            string part3 = message.substr(3, found1 - 3);
                            string part1 = message.substr(found1 + 1, found2 - (found1 + 1));
                            string part2 = message.substr(found2 + 2, message.length());
                            cout << "Part 3: " << part3 << endl;
                            cout << "Part 1: " << part1 << endl;
                            cout << "Part 2: " << part2 << endl;

                            pthread_t fence, hill, pinnacle;

                            if (pthread_create(&fence, NULL,railFence , &part1))
                            {
                                cout << "Error creating Fence thread";
                                return 0;
                            }

                            if (pthread_join(fence, NULL))
                            {
                                cout << "Error joining Fence thread";
                                return 0;
                            }

                            if (pthread_create(&hill, NULL, hillT, &part2))
                            {
                                cout << "Error creating Hill thread";
                                return 0;
                            }

                            if (pthread_join(hill, NULL))
                            {
                                cout << "Error joining Hill thread";
                                return 0;
                            }

                            if (pthread_create(&pinnacle, NULL, pinnacleT, &part3))
                            {
                                cout << "Error creating Pinnacle thread";
                                return 0;
                            }

                            if (pthread_join(pinnacle, NULL))
                            {
                                cout << "Error joining Pinnacle thread";
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }
}

//Algorithm #1: Rail Fence algorithm
void* railFence(void * in)
{
    string *input = (string *) (in);
    string message = *input;

    //Return invalid message if this algorithm string has a 0 or does not contain a digit
    if((string::npos != message.find("0")) || (string::npos == message.find("1")))
    {
        cout << "Fence Thread: has an invalid input" << endl;
    }
    else
    {
        size_t lastDigit = message.find_last_of("123456789");
        string lastDigitString = message.substr(0, lastDigit + 1);

        //Array of the digits in the message
        int digits[lastDigit + 1];

        //Put digits into int array
        for (int d = 0; d < lastDigit + 1; d++)
        {
            digits[d] = stoi(lastDigitString.substr(d, 1));
        }

        //Find max in the array of digits
        int max = 0;
        for(int i = 0; i < lastDigit + 1; i++)
        {
            //Have to subtract '0' or the array will contain the ASCII value of the char
            if ((digits[i]) > max)
            {
                max = digits[i];
            }
        }

        //Return invalid if the max digit != length of digits array
        if (max != (lastDigit + 1))
        {
            cout << "Algorithm 1 has insufficient digit length" << endl;
        }
        else
        {
            int textLength = message.length() - lastDigit - 1;

            if ((textLength % max) != 0)
            {
                cout << "**Fence thread has insufficient text length, adding blank spaces...**" << endl << endl;
                int remainder = textLength % max;
                for (int i = 0; i < remainder; i++)
                {
                    message += " ";
                }

                char textArr [textLength/max][max];
                char messageArr[message.length() + 1];
                string newMessage = message.substr(max, message.length());
                strcpy(messageArr, newMessage.c_str());

                int messageArrCount = 0;
                //Fill array from string
                for (int i = 0; i < max; i++)
                {
                    for (int j = 0; j < (textLength/max); j++)
                    {
                        textArr[j][i] = messageArr[messageArrCount];
                        messageArrCount++;
                    }
                }

                //Set new array to values of swapping columns
                char textArr_2 [textLength/max][max];
                for (int i = 0; i < lastDigit + 1; i++)
                {
                    for (int j = 0; j < (textLength/max); j++)
                    {
                        textArr_2[j][i] =  textArr[j][digits[i] - 1];
                    }
                }

                //Print
                cout << "Fence Thread result is ";
                for (int i = 0; i < (textLength/max); i++)
                {
                    for (int j = 0; j < max; j++)
                    {
                        cout << textArr_2[i][j];
                    }
                }
                cout << endl;
            }
            else
            {
                char textArr [textLength/max][max];
                char messageArr[message.length() + 1];
                string newMessage = message.substr(max, message.length());
                strcpy(messageArr, newMessage.c_str());

                int messageArrCount = 0;
                //Fill array from string
                for (int i = 0; i < max; i++)
                {
                    for (int j = 0; j < (textLength/max); j++)
                    {
                        textArr[j][i] = messageArr[messageArrCount];
                        messageArrCount++;
                    }
                }

                //Set new array to values of swapping columns
                char textArr_2 [textLength/max][max];
                for (int i = 0; i < lastDigit + 1; i++)
                {
                    for (int j = 0; j < (textLength/max); j++)
                    {
                        textArr_2[j][i] =  textArr[j][digits[i] - 1];
                    }
                }

                //Print
                cout << "Fence Thread result is ";
                for (int i = 0; i < (textLength/max); i++)
                {
                    for (int j = 0; j < max; j++)
                    {
                        cout << textArr_2[i][j];
                    }
                }
                cout << endl;

            }
        }
    }
}

//Algorithm #2: Hill Algorithm
void* hillT (void* in)
{
    string *input = (string *) (in);
    string message = *input;

    //Trim spaces at front if necessary
    size_t firstChar = message.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    string trimmedMessage = message.substr(firstChar, message.length());

    //Find first number and split into section1
    size_t firstNum = trimmedMessage.find_first_of("123456789");
    string section1 = trimmedMessage.substr(0, firstNum - 1);

    //Check if section1 is even length
    if((section1.length() % 2) != 0)
    {
        cout << "Hill: Not an even length of characters." << endl;
    }
    else
    {
        size_t lastNum = trimmedMessage.find_last_of("0123456789");
        //Trim white space at the end of section2
        string section2 = trimmedMessage.substr(firstNum, lastNum - firstNum + 1);

        //Tokenize and break up section2 and put into vector
        vector<string> vsection2;
        int i = 0;
        string delim = " ";
        size_t pos = section2.find(delim);
        while (pos != string::npos)
        {
            vsection2.push_back(section2.substr(i, pos - i));
            i = ++pos;
            pos = section2.find(delim, pos);

            if(pos == string::npos)
            {
                vsection2.push_back(section2.substr(i, section2.length()));
            }
        }

        if (vsection2.size() != 4)
        {
            cout << "Hill: The digit section does not contain 4 digits" << endl;
        }
        else
        {
            //Put string into array
            char asection1[section1.length()];
            strcpy(asection1, section1.c_str());

            //Create a 2 index vector, loop and increment by 2 and convert/do matrix multiplication/reconvert
            int section1_2[2];
            string hill;
            for (int x = 0; x < section1.length(); x = x + 2)
            {
                if(isupper(asection1[x]))
                {
                    section1_2[0] = asection1[x] - 65;
                }
                else
                {
                    section1_2[0] = asection1[x] - 97;
                }
                if(isupper(asection1[x+1]))
                {
                    section1_2[1] = asection1[x+1] - 65;
                }
                else
                {
                    section1_2[1] = asection1[x+1] - 97;
                }

                hill += (65 + (((atoi(vsection2.at(0).c_str()) * section1_2[0]
                                 + atoi(vsection2.at(1).c_str()) * section1_2[1]) % 26 + 26) % 26));
                hill += (65 + (((atoi(vsection2.at(2).c_str()) * section1_2[0]
                                 + atoi(vsection2.at(3).c_str()) * section1_2[1]) % 26 + 26) % 26));
            }

            cout << "Hill Thread result is " << hill << endl;
        }
    }
}

//Algorithm #3: Pinnacle Algorithm
void* pinnacleT (void* in)
{
    string *input = (string *) (in);
    string message = *input;

    size_t firstChar = message.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    string trimmedMessage = message.substr(firstChar, message.length());

    //Find first number and split into section1
    size_t firstNum = trimmedMessage.find_first_of("123456789");
    string section1 = trimmedMessage.substr(0, firstNum - 1);

    //Check if section1 is divisible by 3
    if (section1.length() % 3 != 0)
    {
        cout << "Pinnacle: Character length is not divisible by 3." << endl;
    }
    else
    {
        size_t lastNum = trimmedMessage.find_last_of("0123456789");
        //Trim white space at the end of section2
        string section2 = trimmedMessage.substr(firstNum, lastNum - firstNum + 1);

        //Tokenize and break up section2 and put into vector
        vector<string> vsection2;
        int i = 0;
        string delim = " ";
        size_t pos = section2.find(delim);
        while (pos != string::npos)
        {
            vsection2.push_back(section2.substr(i, pos - i));
            i = ++pos;
            pos = section2.find(delim, pos);

            if(pos == string::npos)
            {
                vsection2.push_back(section2.substr(i, section2.length()));
            }
        }

        if(vsection2.size() != 9)
        {
            cout << "Pinnacle: The digit section does not contain 9 digits." << endl;
        }
        else
        {
            //Put string into array
            char asection1[section1.length()];
            strcpy(asection1, section1.c_str());
            int section1_2[3];
            string pinnacle;

            //Create a 3 index vector, loop and increment by 3 and convert/do matrix multiplication/reconvert
            for (int x = 0; x < section1.length(); x = x + 3)
            {
                if (isupper(asection1[x]))
                {
                    section1_2[0] = asection1[x] - 65;
                }
                else
                {
                    section1_2[0] = asection1[x] - 97;
                }
                if (isupper(asection1[x + 1]))
                {
                    section1_2[1] = asection1[x + 1] - 65;
                }
                else
                {
                    section1_2[1] = asection1[x + 1] - 97;
                }
                if (isupper(asection1[x + 2]))
                {
                    section1_2[2] = asection1[x + 2] - 65;
                }
                else
                {
                    section1_2[2] = asection1[x + 2] - 97;
                }

                pinnacle += (65 + (((atoi(vsection2.at(0).c_str()) * section1_2[0] + atoi(vsection2.at(1).c_str()) * section1_2[1]
                                     + atoi(vsection2.at(2).c_str()) * section1_2[2]) % 26 + 26) % 26));
                pinnacle += (65 + (((atoi(vsection2.at(3).c_str()) * section1_2[0] + atoi(vsection2.at(4).c_str()) * section1_2[1]
                                     + atoi(vsection2.at(5).c_str()) * section1_2[2]) % 26 + 26) % 26));
                pinnacle += (65 + (((atoi(vsection2.at(6).c_str()) * section1_2[0] + atoi(vsection2.at(7).c_str()) * section1_2[1]
                                     + atoi(vsection2.at(8).c_str()) * section1_2[2]) % 26 + 26) % 26));
            }
            cout << "Pinnacle Thread result is: " << pinnacle << endl;
        }
    }
}