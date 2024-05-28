#include <iostream>
#include <fstream>
#include <sstream> //to read file into stream
using namespace std;

void frequencies(char words[], int fre[])
{

}

int main()
{
        char aplhabets[97];
    int freq [97];
    ifstream textfile;   //creating object of the fstream object


    textfile.open ("file.txt"); //open file
    stringstream buffer_holder;
    buffer_holder << textfile.rdbuf(); //read all the file content at once
    string text = buffer_holder.str(); //coverting and storing that content into string
    textfile.close();//closing the file
    cout<<text<<endl;
}