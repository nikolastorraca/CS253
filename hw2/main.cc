/*
 *  HW2 Shell
 *  Program Description: This program will read and execute commands using functional notation
 *  Developed by: Nik Torraca
 *
 */
#include <iostream>
#include <sys/types.h>    // fork() & wait()
#include <unistd.h>    // fork(), execvp()
#include <cstdlib>    // exit()
#include <wait.h>    // wait()
#include <vector>    // vector<>
#include <string>    // string
#include <fstream>    // fstream
using namespace std;

bool execute(vector<string> command);
int whiteSpace(string &textIn, int index, int &len);
int word(string &textIn, int index, int &len);
int paren(string &textIn, int index, int &len);
bool processLine(string text); 

bool execute(vector<string> command) {
    // Convert vector of C++ strings to an array of C strings for exectp():
    vector<char *> argv;        // vector of C string pointers
    for (auto &v : command)        // for each C++ string:
        argv.push_back(v.data());    //    add address of C-string inside it
    argv.push_back(nullptr);        // argv ends with a null pointer
    
   // Duplicate ourself: -1:error, 0:child, >0:parent
    const auto pid = fork();        // step #1: copy ourself
    if (pid < 0)
        return false;            // fork failure?
    if (pid == 0) {            // child?
        execvp(argv[0], argv.data());    // step #2: replace child with new prog
        exit(0xff);            // only got here if execvp() failed.
    }
    int status;
    wait(&status);            // step #3: wait for child to finish
    return status != 0xff00;        // true iff we succeeded
}

int paren(string &textIn, int index, int &len){

    if(index < len)
    {
        while(textIn[index] == '(' || textIn[index] == ')')
        {
            //cout << index << '\n';                    DEEBUG
            //cout << "Were increasing the index\n";     DEGBUG
            index++;   
            //cout << index << '\n'; DEBUG
        } 
    }
    //cout << "We found a paren" << index << "\n"; DEBUG
    return index;    
}
int whiteSpace(string &textIn, int index, int &len){
    //static int len = textIn.length();

    //Make sure were not at end of string
    if(index < len)
    {
    //Keep going through string as long as it's whitespace
        while(isspace(textIn[index]) != 0 && index < len)
        {
            index++;
        }
    //Once we no longer have whitespace, call word method  
    }
    //cout << "We found whitespace!" << index << "\n"; DEBUG
    return index;
}

int word(string &textIn, int index, int &len){
    //Declare variables to be used in this method
    //static int len = textIn.length();
    static vector<string> wordVec;
    static int newIndex;
    string newWord;
    int diff;
    bool escape = false;

    if(index < len)
    {    
        newIndex = index;
      
    //While the text is not whitespace
    while(isspace(textIn[newIndex]) == 0 && newIndex < len && textIn[newIndex] != '(' && textIn[newIndex] != ')')
    {
        if(textIn[newIndex] != '\\')
            newIndex++;
        else
        {
            //if(textIn[newIndex+1] == '(' || textIn[newIndex+1] == ')')
                //escape = true;
        //cout << textIn.substr(index, 5) << '\n'; DEBUG
        //cout << "We found a back-slash!\n"; DEBUG
        textIn.erase(newIndex, 1);
        //cout << "We erased!\n"; DEBUG
        len -=1;
        escape = true    
        }
    }
        diff = newIndex-index;
        //cout << textIn.substr(index, diff) << " " << len << " " << index << " " << newIndex << '\n'; DEBUG 
        wordVec.push_back(textIn.substr(index, diff));
        return newIndex;     
    }
    if(index == len)
    {
   
    for (auto val : wordVec){
        cout << val << ' ';
        
    }
    execute(wordVec);
    wordVec.clear();
    
    }
    return 0;
}


//Input format                
//   The input will be a series of lines.
//   Each line will consist of a number of whitespace-separated words.
//   The first word in a line will be the command, the following words will be arguments.
//   Ignore lines that contain no words.                 
//   A backslash escapes the next character, making it not special.
//   The backslash itself does not become part of the word. For example:
//    'foo\ bar'   = 'foo bar'
//    'S\pock\(o\) = 'Spock(o)'
//    '\ Kirk\ '   = ' Kirk  '
//    'Bo\nes'     = 'Bones'
//   The effect of a backslash as the last character of a line is undefined. 
//   \n and \t are not translated into newline and tab.
//   In the examples below, the echo program is doing that translation, not hw2.                 
//
//( and ) are always words by themselves, even if they occur next to other text, unless escaped.                 
//
//A non-empty input line must be of this form:
//
//command(optional arguments)
//for example:
//
//echo(hi there)
//If an input line is not of this form, produce an error message and stop the program.                 
//
//No special shell syntax is recognized. For example, the characters #*[];'"><|&~ have no particular significance.                 


bool processLine(string text) {

    //cout << "You made it here at least" << '\n'; DEBUG
    cout << "Here's your line: " << text << '\n';
    if(text.empty())
    {
        return true;
    }
    //cout << text << '\n'; DEBUG
    //return true; DEBUG
    //While loops runs until the length of the line is reached
    int index = 0;
    int len = text.length();
    while(index < len)
    {
    //The next character is NOT whitespace, so call the appropriate function
    if(isspace(text[index]) == 0)
    {
        if(text[index] != '(' && text[index] != ')')
            index = word(text, index, len);
        else
        index = paren(text, index, len);
    }
    //The next character is whitespace, so call the whiteSpace method
    else
       index = whiteSpace(text, index, len); 
    }
    word(text, index, len);
    cout << "\n";
    return true;
}

int main(int argc, char *argv[]){
    vector<string> lines;
    //If arguements are given
    if(argc > 1) {
        //Loop through arguments
        for(int i = 1; i < argc; i++) {
            ifstream f(argv[i]);
            //Is the input a file? If not, ignore
            if(!f.good()) continue;

            //Read a line, and add it to the vector
            string line;
            while(getline(f, line)){
                lines.push_back(line);
            }
        }
    }
    //Else, you are reading from cin
    else {
        //Loop throught the lines and add them to a vector
        for(string str; getline(cin, str); ) {
            lines.push_back(str);
        }
    }

    for(auto line : lines) {
        bool result = processLine(line);    
    }

    return 0;
}
