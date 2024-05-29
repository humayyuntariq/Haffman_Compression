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
        cout << "Character: " << ptr2->ch << ", Frequency: " << ptr2->freq << std::endl;

        pq1.pop();
        node *ptr3 = pq1.top(); 
        cout << "Character: " << ptr3->ch << ", Frequency: " << ptr3->freq << std::endl;


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



void printing (node* root)
{
    if (root!=nullptr)
    {
        cout<<root->ch<<" ";
        printing(root->left);
        printing(root->right);
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
    cout<<endl<<"Priorty Queue"<<endl;

    //creating pority queue
    priority_queue<node*, vector<node*>, Compare_node> pq1;
   
    for (int i=0; i<char_count; i++)
    {
        node *ptr = new node(alphabets[i], freq[i]);
        pq1.push(ptr);
    }

     // Display and remove elements from the priority queue to verify
    while (!pq1.empty()) {
        node* minNode = pq1.top();
        pq1.pop();

        cout << "Character: " << minNode->ch << ", Frequency: " << minNode->freq << std::endl;

    }

    // node* root = build_haffman(pq1);
    // pq1.pop();

    // printing(root);
    // cout<<endl;
    
}