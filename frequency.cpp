#include <iostream>
#include <fstream>
#include <sstream> //to read file into stream
using namespace std;
    

void frequencies(string text, char words[], int freq[], int &char_count)
{
    bool present = false;
    int freq_loca=0 ;
     char_count=0;

    for (int i=0; i<text.length(); i++)//loop will continue until the end of text
    {
        present = false;        //this bool variable will be true when it found this alphabet is already present in array

        for (int m=0; m<char_count; m++) //loop will continnue until the number of character peresnt in array
        {
            if (text[i] == words[m]) //if the character form text matches with character in array
            {
                present = true; 
                freq[m] += 1;      //increment the frequency where we found the duplicate
                break;              //duplicate found and increment in frequeney then break the loop
            }
        }
            if (!present)           //if character not found
            {
                words[char_count] = text[i];        //store the text character into the words
                //cout<<words[char_count];
                freq[char_count] += 1;          //increment the frequency form 0 to 1
                char_count +=1;                //also increment the number of character
            }
        
    }

}

int main()
{
    char alphabets[97];
    int freq [97] = {0};
    int char_count=0;

    ifstream textfile;   //creating object of the fstream object


    textfile.open ("file.txt"); //open file
    stringstream buffer_holder;
    buffer_holder << textfile.rdbuf(); //read all the file content at once
    string text = buffer_holder.str(); //coverting and storing that content into string
    textfile.close();//closing the file
    cout<<text<<endl<<endl;
    
    frequencies(text, alphabets,freq,char_count);
    for (int i=0; i<char_count; i++)
    {
        if (alphabets[i]=='\n')
            cout<<"/n: "<<freq[i]<<endl;
        else
        cout<<alphabets[i]<<": "<<freq[i]<<endl;
    }
   
}