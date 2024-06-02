#include <iostream>
#include <fstream>
#include <sstream> //to read file into stream
#include <vector>
#include <queue>
#include <algorithm>
#include <filesystem>
#include <unordered_set>
using namespace std;
namespace fs = std::filesystem;

// global variable
char alphabets[97];
int freq[97] = {0};
int char_count = 0;
int totalbits = 0;
int serialNumber;

class node // done
{
public:
    char ch;
    int freq;
    node *left, *right;

    node(char chartr = '\0', int fre = 0)
    {
        ch = chartr;
        freq = fre;
        left = nullptr;
        right = nullptr;
    }
};
struct Compare_node // done
{
    bool operator()(node *const &node01, node *const &node02)
    {
        return node01->freq > node02->freq; // min heap property (for storing acesingdly)
    }
};

bool isTxtFile(const string &filename) // done
{
    // Check if the file ends with .txt
    return filename.size() > 4 && filename.substr(filename.size() - 4) == ".txt";
}

bool isbinFile(const string &filename) // done
{
    // Check if the file ends with .txt
    return filename.size() > 4 && filename.substr(filename.size() - 4) == ".bin";
}

bool fileExists(const string &filename) // done
{
    // Check if the file exists in the same directory as the executable
    return fs::exists(filename);
}

string takefilename() // done
{
    cout << "\nEnter .txt file name: ";
    string textfilename;

    cin >> textfilename;
    while (!isTxtFile(textfilename) || !fileExists(textfilename))
    {
        if (!isTxtFile(textfilename))
        {
            cout << "\nThe file must have a .txt extension. Please try again. \n";
        }
        else if (!fileExists(textfilename))
        {
            cout << "\nThe file does not exist in the current directory. Please try again. \n";
        }
        cout << "Enter .txt file name: ";
        cin >> textfilename;
    }
    return textfilename; // return filename if needed
}

string convertFileToString(string filename) // done
{
    ifstream textfile;
    textfile.open(filename); // open file
    stringstream buffer_holder;
    buffer_holder << textfile.rdbuf(); // read all the file content at once
    string text = buffer_holder.str(); // coverting and storing that content into string
    textfile.close();                  // closing the file

    return text;
}

void printTextofFile() // done
{
    string filename = takefilename();
    string text = convertFileToString(filename);
    cout << text << endl
         << endl;
}

void frequencies(string text, char words[], int freq[], int &char_count) // done
{
    bool present = false;
    int freq_loca = 0;
    char_count = 0;

    for (int i = 0; i < text.length(); i++) // loop will continue until the end of text
    {
        present = false; // this bool variable will be true when it found this alphabet is already present in array

        for (int m = 0; m < char_count; m++) // loop will continnue until the number of character peresnt in array
        {
            if (text[i] == words[m]) // if the character form text matches with character in array
            {
                present = true;
                freq[m] += 1; // increment the frequency where we found the duplicate
                break;        // duplicate found and increment in frequeney then break the loop
            }
        }
        if (!present) // if character not found
        {
            words[char_count] = text[i]; // store the text character into the words
            // cout<<words[char_count];
            freq[char_count] += 1; // increment the frequency form 0 to 1
            char_count += 1;       // also increment the number of character
        }
    }
}

node *build_haffman(priority_queue<node *, vector<node *>, Compare_node> &pq1)
{
    while (pq1.size() > 1)
    {

        node *ptr2 = pq1.top();
        // cout << "Character: " << ptr2->ch << ", Frequency: " << ptr2->freq << std::endl;

        pq1.pop();
        node *ptr3 = pq1.top();
        // cout << "Character: " << ptr3->ch << ", Frequency: " << ptr3->freq << std::endl;

        pq1.pop();
        node *ptr1 = new node('@', (ptr2->freq) + (ptr3->freq));

        ptr1->left = ptr2;
        ptr1->right = ptr3;

        pq1.push(ptr1);
    }
    // The remaining node is the root of the Huffman tree
    return pq1.top();
}

void writebinary(int size, int alpha_size, string text, char alpha[], string binar_Code[], int &totalbits, int serialNumber)
{
    int index;
    string character;
    unsigned char byte = 0;
    int bit_count = 0;
    string binfilename;
    cout << "Enter .bin file name: ";
    cin >> binfilename;
    while (!isbinFile(binfilename)|| fileExists(binfilename))
    {
        if (!isbinFile(binfilename))
        {
            cout << "\nThe file must have a .txt extension. Please try again. \n";
        }
        else if (fileExists(binfilename))
        {
            cout << "\nThe file already exist in the current directory. Please try again. \n";
        }        
        cout << "Enter .bin file name: ";
        cin >> binfilename;
    }

    ofstream myfile(binfilename, ios::binary);
    if (!myfile.is_open())
    {
        cout << "Error opening file!" << endl;
        return;
    }
    uint32_t serialholder = 0;
    myfile.write(reinterpret_cast<const char *>(&serialholder), sizeof(serialholder));

    // Write a placeholder for totalbits (4 bytes)
    uint32_t placeholder = 0;
    myfile.write(reinterpret_cast<const char *>(&placeholder), sizeof(placeholder));

    for (int i = 0; i < size; i++)
    {
        for (int m = 0; m < alpha_size; m++)
        {
            if (text[i] == alpha[m])
            {
                index = m;
                break;
            }
        }

        character = binar_Code[index];
        totalbits += binar_Code[index].length();
        for (char bit : character)
        {
            if (bit == '1')
            {
                byte |= (1 << (7 - bit_count));
            }
            bit_count++;

            if (bit_count == 8)
            {
                myfile.write(reinterpret_cast<const char *>(&byte), sizeof(byte));
                byte = 0;
                bit_count = 0;
            }
        }
    }

    if (bit_count > 0)
    {
        myfile.write(reinterpret_cast<const char *>(&byte), sizeof(byte));
    }

    // Seek back to the beginning and update the totalbits
    myfile.seekp(0, ios::beg);
    serialholder = htonl(serialNumber);
    myfile.write(reinterpret_cast<const char *>(&serialholder), sizeof(serialholder));
    uint32_t network_bits = htonl(totalbits);
    myfile.write(reinterpret_cast<const char *>(&network_bits), sizeof(network_bits));

    myfile.close();
}

int getLastSerialNumber(string filename, unordered_set<int> &usedNumbers)
{
    ifstream inFile(filename);
    string line;
    int lastSerialNumber = 0;

    if (!inFile.is_open())
    {
        cout << "Error: Could not open file '" << filename << "'." << endl;
        return lastSerialNumber; // Return 0 if the file can't be opened
    }

    while (std::getline(inFile, line))
    {
        size_t tildePos = line.find('~');
        size_t equalPos = line.find('=');

        if (tildePos != string::npos && equalPos != string::npos)
        {
            string numberStr = line.substr(tildePos + 1, equalPos - tildePos - 1);
            int serialNumber = stoi(numberStr);
            usedNumbers.insert(serialNumber);
            lastSerialNumber = max(lastSerialNumber, serialNumber);
        }
    }
    inFile.close();

    return ++lastSerialNumber;
}

void storePreorderToFile(node *root, ofstream &outFile)
{
    if (root == nullptr)
    {
        return;
    }
    if (root->ch == '\n')
        outFile << '~';
    else
        outFile << root->ch;
    storePreorderToFile(root->left, outFile);
    storePreorderToFile(root->right, outFile);
}

void storePreDataToFile(string key, int &serialNumber, node *root)
{
    ofstream outFile("Trees Data.txt", ios::app);
    if (!outFile.is_open())
    {
        cout << "Error opening preorder traversal file for writing!" << endl;
        return;
    }
    std::unordered_set<int> usedNumbers;
    serialNumber = getLastSerialNumber("Trees Data.txt", usedNumbers);

    // Ensure new serial number is unique

    if (outFile.is_open())
    {
        // Write the key to the file followed by a newline character
        outFile << key << "~" << serialNumber << "=";
        cout << endl;
    }
    else
    {
        cout << "Error: Could not open file 'Trees Data.txt'." << endl;
    }
    storePreorderToFile(root, outFile);
    outFile << endl;
    outFile.close();
}

bool findPath(node *root, char target, string &path)
{
    if (root == nullptr)
    {
        return false;
    }

    if (root->ch == target)
    {
        return true;
    }

    if (findPath(root->left, target, path))
    {
        path = "0" + path;
        return true;
    }

    if (findPath(root->right, target, path))
    {
        path = "1" + path;
        return true;
    }

    return false;
}

void findbinary(int size, node *root, char alphabets[], string binary_code[])
{
    for (int i = 0; i < size; i++)
    {
        findPath(root, alphabets[i], binary_code[i]);
    }
}

void printing(node *root)
{
    if (root != nullptr)
    {
        cout << root->ch << " ";
        printing(root->left);
        printing(root->right);
    }
}

string readbinary(int totalbits, string binfilename)
{
   ifstream myfile(binfilename, ios::binary);
   char byte;
   string bitString;
   int count= 0;
     totalbits+=64;
   while (myfile.get(byte))
   {
       for (int i = 7; i >= 0; --i)
       {
           if (bitString.size() < (totalbits))
           {
               if (byte & (1 << i))
               {
                   bitString += '1';
                   count++;
               }
               else
               {
                   bitString += '0'; count++;
               }
           }

       }
   }
 
   myfile.close();
   return bitString;
}

string decode_binary(string binary_data, node *root)
{
    string decoded_text = "";
    node *current = root;

    for (char bit : binary_data)
    {
        if (current == NULL)
        {

            cout << "Error: Current node is NULL!" << endl;
            return "";
        }

        if (bit == '0')
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }

        if (current != NULL && current->left == NULL && current->right == NULL)
        {

            decoded_text += current->ch;
            current = root;
        }
    }

    return decoded_text;
}

void printInfo(const string title, const int width = 80, const char borderChar = '=')
{
    system("clear");
    // Calculate centered position for title and compression ratio
    int titlePos = (width - title.length()) / 2;

    // Print optional top border
    if (borderChar != ' ')
    {
        cout << setfill(borderChar) << setw(width) << "" << endl;
    }

    // Print centered title (adjusted for indentation)
    cout << setfill(' ') << setw(titlePos - 2) << "" << title << endl;

    // Print optional bottom border
    if (borderChar != ' ')
    {
        cout << setfill(borderChar) << setw(width) << "" << endl;
    }
}

string getKeyFromUser()
{
    string key;
    int count = 0;
    while (count >= 0)
    {
        cout << "Enter a single-word key: ";
        cin >> key;

        // Check if the key contains any spaces
        for (int i = 0; i < key.size(); i++)
        {
            if (key[i] == '~')
            {
                cout << "\nInvalid key! The key should be a single word without spaces." << endl;
                count = 0;
                break;
            }
            else
                count = -1;
        }
    }
    return key;
}

node* recreateHuffmanTreeFromFile(const string& traversal, int& index)
{
    if (index >= traversal.size()) {
        return nullptr;
    }
    char current;
    if (traversal[index]=='~') current ='\n';
    else current = traversal[index];
    node* newNode = new node(current);

    if (current == '@') {
        index++;
        newNode->left = recreateHuffmanTreeFromFile(traversal, index);
        index++;
        newNode->right = recreateHuffmanTreeFromFile(traversal, index);
    }

    return newNode;
}
 
bool compressthefile(node *root, string &text)
{
    string *BinaryData = new string[char_count]; // creating string to store the binary data of values

    findbinary(char_count, root, alphabets, BinaryData);
    writebinary(text.length(), char_count, text, alphabets, BinaryData, totalbits, serialNumber);

    if (root == nullptr)
        return false;
    else
        return true;
}
node *beginInitialCompress(string &text, string &compressfilename)
{
    ifstream textfile;
    compressfilename = takefilename();
    text = convertFileToString(compressfilename);
    frequencies(text, alphabets, freq, char_count);

    // creating pority queue
    priority_queue<node *, vector<node *>, Compare_node> pq1;

    for (int i = 0; i < char_count; i++)
    {
        node *ptr = new node(alphabets[i], freq[i]);
        pq1.push(ptr);
    }

    node *root = build_haffman(pq1);
    return root;
}

bool validkeyandbits(const string& key, int& totalbits, string& travseral, string& binfilename) {

    ifstream inFile("Trees Data.txt");

    if (!inFile.is_open()) {
        cout << "Error: Could not open file 'Tree Data.txt'." << endl;
        return false;
    }

    string line;
    int serialNumber = -1;

    while (getline(inFile, line)) {
        size_t keyPos = line.find(key + "~");
        if (keyPos != string::npos) {
            size_t tildePos = line.find('~', keyPos);
            size_t equalPos = line.find('=', keyPos);

            if (tildePos != string::npos && equalPos != string::npos) {
                string numberStr = line.substr(tildePos + 1, equalPos - tildePos - 1);
                travseral = line.substr(equalPos+1);
                serialNumber = stoi(numberStr);
                break;
            }
        }
    }
    inFile.close();

    if (serialNumber == -1) {
        cout << "Error: Key not found." << endl;
        return false;
    }

    cout << "Enter .bin file name: ";
    cin >> binfilename;
     while (!isbinFile(binfilename) || !fileExists(binfilename))
    {
        if (!isbinFile(binfilename))
        {
            cout << "\nThe file must have a .bin extension. Please try again. \n";
        }
        else if (!fileExists(binfilename))
        {
            cout << "\nThe file does not exist in the current directory. Please try again. \n";
        }
        cout << "Enter .bin file name: ";
        cin >> binfilename;
    }

    ifstream binFile(binfilename, ios::binary);
    if (!binFile.is_open()) {
        cout << "Error: Could not open binary file." << endl;
        return false;
    }

   uint32_t fileSerialNumber;
if (binFile.read(reinterpret_cast<char*>(&fileSerialNumber), sizeof(fileSerialNumber))) {
    fileSerialNumber = ntohl(fileSerialNumber);
    int serial = static_cast<int>(fileSerialNumber);
} else {
    cout << "Error reading file" << endl;
}


    if (fileSerialNumber != serialNumber) {
        cout << "Error: Serial number does not match." << endl;
        binFile.close();
        return false;
    }

    uint32_t networkTotalBits;
    binFile.read(reinterpret_cast<char*>(&networkTotalBits), sizeof(networkTotalBits));
    totalbits = ntohl(networkTotalBits);
    binFile.close();
    return true;
}

void menu()
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
        cout<<"\nPress enter character to continue. ";
          cin>>enterr;
        goto label;
    }
    else if (choice == 4)
    {
        printInfo("Exit");
        cout << "\nExiting....." << endl;
    }
    else 
    {
        cout<<"\nWrong Input\n";
           cout<<"\nPress enter to continue."; cin.get();
           goto label;
    }
         

}
