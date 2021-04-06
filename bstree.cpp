#include <iostream>

using namespace std;


class node {
public:
  int value;
  node* left_child;
  node* right_child;

  node (int value) {
    this->value = value;
    left_child = NULL;
    right_child = NULL;
  }


  void insert(int x) {
    if (x <= this->value) {
      if (this->left_child == NULL) {
        this->left_child = new node(x);
      } else {
        this->left_child->insert(x);
      }
    } else {
      if (this->right_child == NULL) {
        this->right_child = new node(x);
      } else {
        this->right_child->insert(x);
      }
    }
  }


  bool find(int x) {
    if (this == NULL) {
      return false;
    }
    else if (this->value == x) {
      return true;
    }

    if (x < this->value) {
      return this->left_child->find(x);
    } else {
      return this->right_child->find(x);
    }
  }


  void inorder() {
    if (this == NULL) {
      return;
    }

    this->left_child->inorder();
    cout << this->value << endl;
    this->right_child->inorder();
  }
};

int main() {

  int n;
  cin >> n;

  int a[n + 1];
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }

  node* root = new node(a[0]);
  for (int i = 1; i < n; i++) {
    root->insert(a[i]);
  }

  // root->inorder();

  int q;
  int e;
  cin >> q;

  for (int i = 0; i < q; i++) {
    cin >> e;
    cout << root->find(e) << endl;
  }

  delete root;

  return 0;
}