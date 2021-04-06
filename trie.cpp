/**
 *  Implementation of a prefix tree data type.
 *  The below implementation can:
 *    - add a word to the data type
 *    - check if a word exists in the data type
 *      - a word can contain only alphabetic characters and
 *        '.' - which means any character can match this position.
 *
 *  Jovan Petreski - 06/04/2021
 */

#include <iostream>
#include <vector>

#define endl '\n'
#define ALPHABET_SIZE 26
using namespace std;

class TrieNode {
private:
  char key;                   // The letter this node holds
  vector<TrieNode*> children; // Indexed from 0 to ALPHABET_SIZE - 1
  bool word_end;              // true if a word ends at this node

public:
  /**
   *  Used only to create instance
   *  of the root node (no key value needed).
   */
  TrieNode() {
    word_end = false;
    children.resize(ALPHABET_SIZE);
  }

  /**
   *  Used to create instances of all
   *  other nodes in the tree (must have key).
   */
  TrieNode(char key) {
    this->key = key;
    word_end = false;
    children.resize(ALPHABET_SIZE);
  }

  /**
   *  Adds a word to the data structure.
   */
  void add(string word) {
    TrieNode* curr_node = this;

    for (int i = 0; i < (int)word.length(); i++) {
      if (curr_node->children[word[i] - 'a'] == nullptr) {
        curr_node->children[word[i] - 'a'] = new TrieNode(word[i]);
      }

      curr_node = curr_node->children[word[i] - 'a'];
      if (i == (int)word.length() - 1) {
        curr_node->word_end = true;
      }
    }
  }

  /**
   *  Returns true if word exists in the
   *  data structure of a match can be found
   *  (in case there are '.' in the word).
   *  @param pos  - the character of the word we are at
   *  @param word - the word
   */
  bool find_word(int pos, string word) {
    if (pos == (int)word.length() - 1) {
      // Base case - the last letter

      if (word[pos] == '.') {
        // Any char can match

        for (int i = 0; i < (int)children.size(); i++) {
          if (children[i] != nullptr and children[i]->word_end) {
            return true;
          }
        }

        return false;
      } else {
        return (children[word[pos] - 'a'] != nullptr and children[word[pos] - 'a']->word_end);
      }
    }

    // Step case
    if (word[pos] == '.') {
      // Any char can match

      for (int i = 0; i < (int)children.size(); i++) {
        if (children[i] != nullptr and children[i]->find_word(pos + 1, word)) {
          return true;
        }
      }

      return false;
    } else {
      return (children[word[pos] - 'a'] == nullptr) ? false :
              children[word[pos] - 'a']->find_word(pos + 1, word);
    }
  }

  /**
   *  Helper function.
   */
  bool exists(string word) {
    return find_word(0, word);
  }

  /**
   *  Helper function to print
   *  all children of a node.
   */
  void print_children() {
    for (int i = 0; i < (int)children.size(); i++) {
      cout << children[i] << " ";
    } cout << endl;
  }
};


/**
 *  Driver program to test functionality.
 */
int main(int argc, char** argv) {
  ios::sync_with_stdio(false);

  int queries;
  cin >> queries;

  int type;
  string word;
  TrieNode* root = new TrieNode();  // Empty node (no key)

  for (int i = 0; i < queries; i++) {
    cin >> type >> word;

    if (type == 1) {
      root->add(word);
    } else {
      cout << root->exists(word) << endl;
    }
  }

  delete root;  // Free memory
  return 0;
}