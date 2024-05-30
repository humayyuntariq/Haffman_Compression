#include <iostream>
#include <fstream>
#include <sstream> //to read file into stream
#include <vector>
#include <queue>
#include <algorithm> // For push_heap, pop_heap, make_heap
using namespace std;
    
class node
{
    public:
        char ch;
        int freq;
        node *left, *right;
    
    node (char chartr='\0', int fre=0)
    {
        ch=chartr; freq= fre;  left= nullptr; right = nullptr;
    }
};
struct Compare_node
{
    bool operator()(node* const& node01, node* const& node02)
    {
        return node01->freq > node02->freq; //min heap property (for storing acesingdly)
    }
};

node* build_haffman(priority_queue<node*, vector<node*>, Compare_node>& pq1) {
    while(pq1.size()>1)
    {
        
        node *ptr2 = pq1.top();
        // cout << "Character: " << ptr2->ch << ", Frequency: " << ptr2->freq << std::endl;

        pq1.pop();
        node *ptr3 = pq1.top(); 
        // cout << "Character: " << ptr3->ch << ", Frequency: " << ptr3->freq << std::endl;


        pq1.pop();
        node *ptr1= new node('@',(ptr2->freq) + (ptr3->freq));
        
        ptr1->left = ptr2;
        ptr1->right = ptr3;

        pq1.push(ptr1);
    }
    // The remaining node is the root of the Huffman tree
    return pq1.top();
}

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
bool findPath(node* root, char target, string& path) {
    if (root == nullptr) {
        return false;
    }

    if (root->ch == target) {
        return true;
    }

    if (findPath(root->left, target, path)) {
        path = "0" + path ;
        return true;
    }

    if (findPath(root->right, target, path)) {
        path = "1" + path;
        return true;
    }

    return false;
}

void findbinary(int size, node* root, char alphabets[], string binary_code[])
{
    for (int i=0; i<size; i++)
    {
        findPath(root, alphabets[i], binary_code[i]);
    }
}


void printing (node* root)
{
    if (root!=nullptr)
    {
        cout<<root->ch<<" ";
        printing(root->left);
        printing(root->right);
    }
}
void writebinary(int size, int alpha_size, string text, char alpha[], string binar_Code[], int &totalbits)
{
   int index;
    string character;
    unsigned char byte = 0;  // This will hold 8 bits at a time
    int bit_count = 0;       // To keep track of the number of bits in the current byte

    // Open file
    ofstream myfile("compressed.bin", ios::binary);
    if (!myfile.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    for (int i = 0; i < size; i++) {
        for (int m = 0; m < alpha_size; m++) {
            if (text[i] == alpha[m]) {
                index = m;
                break;
            }
        }

        character = binar_Code[index];
        totalbits += binar_Code[index].length();
        for (char bit : character) {
            if (bit == '1') {
                byte |= (1 << (7 - bit_count));  // Set the appropriate bit in byte
            }
            bit_count++;

            if (bit_count == 8) {  // If we have accumulated 8 bits, write the byte to the file
                myfile.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
                byte = 0;  // Reset byte
                bit_count = 0;  // Reset bit counter
            }
        }
    }

    // Write the remaining bits if any
    if (bit_count > 0) {
        myfile.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
    }

    myfile.close();
}

int main()
{
    char alphabets[97];
    int freq [97] = {0};
    int char_count=0;
    int totalbits= 0;

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
    string *BinaryData = new string[char_count]; //creating string to store the binary data of values

    

    //creating pority queue
    priority_queue<node*, vector<node*>, Compare_node> pq1;
   
    for (int i=0; i<char_count; i++)
    {
        node *ptr = new node(alphabets[i], freq[i]);
        pq1.push(ptr);
    }

     //// Display and remove elements from the priority queue to verify
    //  cout<<endl<<"Priorty Queue"<<endl;
    // while (!pq1.empty()) {
    //     node* minNode = pq1.top();
    //     pq1.pop();

    //     cout << "Character: " << minNode->ch << ", Frequency: " << minNode->freq << std::endl;

    // }

    node* root = build_haffman(pq1);
    pq1.pop();
      printing(root);
   findbinary(char_count, root, alphabets, BinaryData);
    writebinary(text.length(), char_count,text, alphabets, BinaryData, totalbits);

    ifstream myfile02("compressed.bin", ios::binary);
char byte;
    string bitString;

    while (myfile02.get(byte)) {
        for (int i = 7; i >= 0; --i) {
            if (bitString.size() < totalbits) {
                if (byte & (1 << i)) {
                    bitString += '1';
                } else {
                    bitString += '0';
                }
            }
        }
    }

    myfile02.close();
     cout << bitString << endl;
//printing binary code
//    for (int i=0; i<char_count; i++)
//     {
//         if (alphabets[i]=='\n')
//             cout<<"/n: "<<BinaryData[i]<<endl;
//         else
//         cout<<alphabets[i]<<": "<<BinaryData[i]<<endl;
//     }

  
    
    cout<<endl;
    
}