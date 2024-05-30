#include <iostream>
#include <string>
#include <memory>

using namespace std;

struct Node {
    char ch;
    unique_ptr<Node> left;
    unique_ptr<Node> right;

    Node(char c) : ch(c), left(nullptr), right(nullptr) {}
};

// Function to build the Huffman tree from preorder traversal
unique_ptr<Node> buildTree(const string& preorder, int& index) {
    if (index >= preorder.size()) {
        return nullptr;
    }

    char currentChar = preorder[index];
    unique_ptr<Node> node = make_unique<Node>(currentChar);
    
    if (currentChar == '@') {
        ++index; // Move to the next character
        node->left = buildTree(preorder, index);
        ++index; // Move to the next character
        node->right = buildTree(preorder, index);
    }
    return node;
}

// Function to decode the binary string using the Huffman tree
string decode(const string& binary, unique_ptr<Node>& root) {
    string decoded;
    Node* current = root.get();
    for (char bit : binary) {
        if (bit == '0') {
            current = current->left.get();
        } else {
            current = current->right.get();
        }

        // If leaf node is reached, append the character to the result
        if (!current->left && !current->right) {
            decoded += current->ch;
            current = root.get(); // Go back to the root for the next character
        }
    }
    return decoded;
}

int main() {
    string preorder = "@@@it@@@Bjd@a@ml@@@@-s@'h@\"@If@ @ne";
    string binary = "10101011011001010000101111010010001110101110110000011111100011001111111100011111100010011010110100000100011111110010010110011100001110";
    
    int index = 0;
    unique_ptr<Node> root = buildTree(preorder, index);

    string decodedText = decode(binary, root);

    cout << "Decoded Text: " << decodedText << endl;

    return 0;
}

