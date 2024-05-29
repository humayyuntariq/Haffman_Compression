## Reading Content form the file
    
    ifstream textfile;   //creating object of the fstream object
    textfile.open ("file.txt"); //open file
    stringstream buffer_holder;
    buffer_holder << textfile.rdbuf(); //read all the file content at once
    string text = buffer_holder.str(); //coverting and storing that content into string
    textfile.close();//closing the file
    cout<<text<<endl;

## Find the frequency of unique alphabets in text including space and line breaker

    void frequencies(string text, char words[], int freq[], int &char_count)
    {
        bool present = false;
        int freq_loca=0 ;
        char_count=0;

    for (int i=0; i<text.length(); i++)//loop will continue until the end of text
    {
        present = false;

        for (int m=0; m<char_count; m++)
        {
            if (text[i] == words[m])
            {
                present = true;
                freq_loca = m;
                break;
            }
        }
            if (present)
            {
                freq[freq_loca] += 1;
            }
            else 
            {
                words[char_count] = text[i];
                //cout<<words[char_count];
                freq[char_count] += 1;
                char_count +=1;
            }
        
    }
    }

## Other stufff

    text.length() will return total numbers of character in text, including line breaker and null characrer and space

Type (node*):

    Indicates that the elements in the priority queue are pointers to node.

Container (std::vector<node*>):

    Specifies that the underlying container for storing the elements is a std::vector of node*.

Compare (Compare):

    The third parameter is the custom comparator used to order the elements in the priority queue.
    
## Creating Priorty Queue using vector and storing it using comparator to compare
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

        delete minNode; // Clean up
    }
Yes, when you push a node into the priority queue, it will automatically be stored and the queue will maintain the heap property based on the custom comparator you defined.