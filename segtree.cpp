/**
 *  C++ program that implements segment tree.
 *  Operations allowed:
 *  1. Update element of an array.
 *  2. Query sum of a given range.
 *    Types of queries:
 *    - smallest element in an interval,
 *    - greatest element in an interval,
 *    - sum of elements in an interval.
 *
 *  Time complexity of operation: O(log n).
 *  Memory complexity: O(N).
 */

#include <iostream>
#include <climits>

using namespace std;


class Node {
private:
  int leftmost;
  int rightmost;

  Node* right_child;
  Node* left_child;

  int sum;
  int max_val;
  int min_val;


  /**
   *  Updates attributes of the current node (this).
   */
  void recalc() {
    if (leftmost == rightmost) {
      // Leaf node - updates are already done.
      return;
    }

    sum = get_sum(left_child) + get_sum(right_child);
    min_val = min(get_min(left_child), get_min(right_child));
    max_val = max(get_max(left_child), get_max(right_child));
  }


public:
  /**
   *  Constructor - builds the segment tree.
   */
  Node (int left, int right, int array[]) {
    leftmost = left;
    rightmost = right;

    if (left == right) {
      // Leaf node - leftmost and rightmost are same values

      sum = array[left];    // Sum is the ith element of the array
      min_val = array[left];
      max_val = array[left];

      // Leaf nodes don't have children
      left_child = nullptr;
      right_child = nullptr;

    } else {
      // Internal node

      int mid = left + (right - left) / 2;  // Avoids overflow

      // Split the array in half
      left_child = new Node(left, mid, array);
      right_child = new Node(mid + 1, right, array);

      recalc();
    }
  }


  /**
   *  Updates a single point in the array.
   */
  void update(int index, int new_val) {
    if (leftmost == rightmost) {
      sum = new_val;
      min_val = new_val;
      max_val = new_val;
      return;
    }

    if (index <= left_child->rightmost) {
      left_child->update(index, new_val);
    } else {
      right_child->update(index, new_val);
    }

    recalc();
  }


  // ==================== Query functions =========================


  /**
   *  Returns the greatest element in the specified segment.
   *  3 cases:
   *    1. The segment the current node captures is completely within the specified segment.
   *    2. Segment current node captures is completely out of bounds.
   *    3. The segments overlap.
   */
  int find_max(int left, int right) {
    if (left <= leftmost and rightmost <= right) {
      // Case 1
      return get_max(this);
    }
    else if (left > rightmost or right < leftmost) {
      // Case 2
      return INT_MIN;
    } else {
      // Case 3
      return max(left_child->find_max(left, right), right_child->find_max(left, right));
    }
  }


  /**
   *  Returns the smallest element in the specified segment.
   *  3 cases:
   *    1. The segment the current node captures is completely within the specified segment.
   *    2. Segment current node captures is completely out of bounds.
   *    3. The segments overlap.
   */
  int find_min(int left, int right) {
    if (left <= leftmost and rightmost <= right) {
      // Case 1
      return get_min(this);
    }
    else if (left > rightmost or right < leftmost) {
      // Case 2
      return INT_MAX;
    } else {
      // Case 3
      return min(left_child->find_min(left, right), right_child->find_min(left, right));
    }
  }


  /**
   *  Returns the sum of elements in the specified segment.
   *  3 cases:
   *    1. The segment the current node captures is completely within the specified segment.
   *    2. Segment current node captures is completely out of bounds.
   *    3. The segments overlap.
   */
  int find_sum(int left, int right) {
    if (left <= leftmost and rightmost <= right) {
      // Case 1
      return get_sum(this);
    }
    else if (left > rightmost or right < leftmost) {
      // Case 2
      return 0;
    } else {
      // Case 3
      return left_child->find_sum(left, right) + right_child->find_sum(left, right);
    }
  }


  // ==================== Helper functions ========================


  static int max(int a, int b) {
    return (a > b) ? a : b;
  }

  static int min(int a, int b) {
    return (a < b) ? a : b;
  }

  void inorder() {
    if (this != nullptr) {
      this->left_child->inorder();
      cout << this->sum << " ";
      this->right_child->inorder();
    }
  }

  void print_bounds(Node* x) {
    if (x != nullptr) {
      cout << x->leftmost << " " << x->rightmost << endl;
    }
  }


  // ======================== Getters =============================


  int get_sum(Node* x) {
    return (x == nullptr) ? -1 : x->sum;
  }

  int get_min(Node* x) {
    return (x == nullptr) ? INT_MAX : x->min_val;
  }

  int get_max(Node* x) {
    return (x == nullptr) ? INT_MIN : x->max_val;
  }
};


int main() {
  int n;
  cin >> n;

  int array[n + 1];
  for (int i = 0; i < n; i++) {
    cin >> array[i];
  }

  // Creates an instance of the root of the segment tree
  // and builds the rest of the tree recursively.
  Node* root = new Node(0, n - 1, array);


  int q;    // Number of queries
  cin >> q;
  int q_type, x, y; // Query description

  for (int i = 0; i < q; i++) {
    // 2 types of queries:
    // 1 x y - update the element at position x to have the value y.
    // 2 x y - perform a query on the range from x to y (inclusive).

    cin >> q_type >> x >> y;

    if (q_type == 1) {
      if (x < 0 or x >= n) {
        cout << "Invalid index." << endl;
        continue;
      }
      root->update(x, y);
    } else if (q_type == 2) {
      if ((x < 0) or (x >= n) or (y < 0) or (y >= n)) {
        cout << "Invalid range." << endl;
        continue;
      }
      cout << root->find_sum(x, y) << endl;
    } else {
      cout << "Invalid query type." << endl;
    }
  }

  delete root;
  return 0;
}