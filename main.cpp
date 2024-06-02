#include <iostream>
#include "haffman_header.hpp"
using namespace std;

int main()
{
  int choice;
    string text, travseral;
    string compressfilename, binfilename;
    string key, keytodecode;
    string filename;
label:
    printInfo("Haffman Press");
    cout << "\n1. Compress the .txt File";
    cout << "\n2. Decompress the .bin File";
    cout << "\n3. Read Text form .txt File";
    cout << "\n4. Exit";
    cout << "\n\n Enter: ";
    cin >> choice;

    if (choice == 1)
    {
        printInfo("Compression");
        node *root = beginInitialCompress(text, compressfilename);
        cout << endl;
        key = getKeyFromUser();

        storePreDataToFile(key, serialNumber, root);

        if (compressthefile(root, text))
            cout << "\nCompression Successfully\n";
        else
            cout << "\nCompression Unsuccessful\n";

        goto label;
    }
    else if (choice == 2)
    {
        printInfo("Decompression");
        string keytodecode;
        int decodingtotalbits;
        string travseral;
        while (true)
        {
            keytodecode = getKeyFromUser();
            if (validkeyandbits(keytodecode, decodingtotalbits, travseral, binfilename))
                break;
        }
           int index =0;
        node* recreatedRoot = recreateHuffmanTreeFromFile(travseral,index);
         string bitString = readbinary(decodingtotalbits,binfilename);
         string bitts = bitString.substr(64, decodingtotalbits);
        string decoded_text = decode_binary(bitts, recreatedRoot);
        cout << "\nDecoded Text: " <<endl<< decoded_text << endl;

        string decodefilename ;
        cout<<"\nEnter a .txt file for text: "; cin>>decodefilename;
        while (!isTxtFile(decodefilename) || fileExists(decodefilename))
        {
            if (!isTxtFile(decodefilename))
        {
            cout << "\nThe file must have a .txt extension. Please try again. \n";
        }
            else if (fileExists(decodefilename))
        {
            cout << "\nThe file already exist in the current directory. Please try again. \n";
        }
        cout << "Enter .txt file name: ";
        cin >> decodefilename;
        }

        ofstream fout(decodefilename);
        fout<<decoded_text<<endl;
        fout.close();
        
        goto label;
    }
    else if (choice == 3)
    {
        printInfo("Print .txt File");
        filename = takefilename();
        text = convertFileToString(filename);
        cout << "\nText:\n"
             << text << endl;
            char enterr;
        cout<<"\nEnter 1 to continue. ";
          cin>>enterr;
        if(enterr=='1') goto label;
         else goto label2;
    }
    else if (choice == 4)
    {
        label2:
        printInfo("Exit");
        cout << "\nExiting....." << endl;
    }
    else 
    {
        cout<<"\nWrong Input\n";
        char enterr;
         cout<<"\nEnter 1 to continue. ";
          cin>>enterr;
        if(enterr=='1') goto label;
         else goto label2;
    }
    
}