/*
 *  HW2 Shell
 *  Program Description: This program will read and execute commands using functional notation
 *  Developed by: Nik Torraca
 *
 */
#include <iostream>
#include <fstream>    // fstream
#include <sys/types.h>  // fork() & wait()
#include <unistd.h>     // fork(), execvp()
#include <cstdlib>      // exit()
#include <wait.h>       // wait()
#include <vector>       // vector<>
#include <string>       // string

using namespace std;

bool execute(vector<string> command);
int whiteSpace(string &textIn, int index, int &len);
int word(string &textIn, int index, int &len);
int paren(string &textIn, int index, int &len);
bool processLine(string text); 

bool execute(vector<string> command) {
    // Convert vector of C++ strings to an array of C strings for exectp():
    vector<char *> argv;                // vector of C string pointers
    for (auto &v : command)             // for each C++ string:
        argv.push_back(v.data());       //    add address of C-string inside it
    argv.push_back(nullptr);            // argv ends with a null pointer

    // Duplicate ourself: -1:error, 0:child, >0:parent
    const auto pid = fork();            // step #1: copy ourself
    if (pid < 0)
        return false;                   // fork failure?
    if (pid == 0) {                     // child?
        execvp(argv[0], argv.data());   // step #2: replace child with new prog
        exit(0xff);                     // only got here if execvp() failed.
    }
    int status;
    wait(&status);                      // step #3: wait for child to finish
    return status != 0xff00;            // true iff we succeeded
}

int paren(string &textIn, int index, int &len){

    if(index < len)
    {
        //Find the index where there is no paren
        while(textIn[index] == '(' || textIn[index] == ')')
        {
            index++;   
        } 
    }
    return index;    
}
int whiteSpace(string &textIn, int index, int &len){

    //Make sure were not at end of string
    if(index < len)
    {
    //Keep going through string as long as it's whitespace
        while(isspace(textIn[index]) != 0 && index < len)
        {
            index++;
        }  
    }
    //When string is no longer whitespace, return index
    return index;
}

int word(string &textIn, int index, int &len){

    //Declare variables to be used in this method
    static vector<string> wordVec;
    static int newIndex;
    string newWord;
    int diff;
    bool escape = false;

    if(index < len)
    {    
        newIndex = index;
      
    //While the text is not whitespace
    while(isspace(textIn[newIndex]) == 0 && newIndex < len)
    {
        //increment index if the character is not \ or ( or ) or an escape sequence
        if((textIn[newIndex] != '\\' && textIn[newIndex] != '(' && textIn[newIndex] != ')') || escape == true){
            escape = false;
            newIndex++;
        }
        else
        {
            //backslash is found. Get rid of, and note that its an escape sequence 
            if(textIn[newIndex] == '\\')
            {
                textIn.erase(newIndex, 1);
                len -=1;
                escape = true;
            }
            //otherwise its a paren
            else
                break;
        }
    }
        //add word to vector, and return index
        diff = newIndex-index; 
        wordVec.push_back(textIn.substr(index, diff));
        return newIndex;     
    }
    //end of line
    if(index == len)
    {
   
        for (auto val : wordVec)
        {
            cout << val << ' ';  
        }
        //call execute with vector, and clear for next line
        //execute(wordVec);
        wordVec.clear();
    }
    return 0;
}

bool processLine(string text) {
    
    //is line empty? Then return to main and get next string
    if(text.empty())
    {
        return true;
    }

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
    
    //vector to store lines
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
        //if result is false, something went wrong in processLine. This prints message to user  
        if(result == false) {
            cerr << argv[0] << ": can't run: " << line;
            return 1;
        }
    }
    return 0;
}
