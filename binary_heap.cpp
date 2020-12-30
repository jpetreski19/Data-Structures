/**
 *  Implementation of binary heap having the
 *  min-heap property.
 *  Can be used to implement priority queue.
 *
 *  Operations:
 *    - Insert operation complexity: O(log n).
 *    - Remove min operation complexity: O(log n).
 */

#include <iostream>
#include <vector>

using namespace std;

class MinHeap {
private:
  vector<int> heap; // The heap - array representation
  int last_pos;     // The index of the last position in the heap

  /**
   *  Swaps elements at the given indices.
   */
  void swap(int id_1, int id_2) {
    int temp = heap[id_1];
    heap[id_1] = heap[id_2];
    heap[id_2] = temp;
  }

  /**
   *  Returns the parent index of the node with given id.
   */
  int parent(int id) {
    if (id % 2 == 0) {
      return (id/2) - 1;
    }
    return id/2;
  }

  /**
   *  Puts the newly inserted value in the right position.
   */
  void sift_up(void) {
    int id = last_pos;

    while (heap[id] < heap[parent(id)]) {
      swap(id, parent(id));
      id = parent(id);
    }
  }

  /**
   *  Puts the first element in the heap in the right position.
   */
  void sift_down(void) {
    int id = 0;

    int left_id = id*2 + 1;
    int right_id = id*2 + 2;

    while (true) {
      if (right_id < (int)heap.size()) {
        // Check both children

        if (heap[id] > heap[left_id] or heap[id] > heap[right_id]) {
          // One of the children has smaller value.
          // If both of them are smaller, sift up the smaller one.

          if (heap[left_id] > heap[right_id]) {
            // Right child has smaller value
            swap(id, right_id);

            // Update indices
            id = right_id;
            left_id = id*2 + 1;
            right_id = id*2 + 2;

            continue;

          } else {
            // Left child has smaller value
            swap(id, left_id);

            // Update indices
            id = left_id;
            left_id = id*2 + 1;
            right_id = id*2 + 2;

            continue;
          }
        } else {
          // Min heap property restored

          break;
        }

      } else if (left_id < (int)heap.size()) {
        // Check only left child

        if (heap[id] > heap[left_id]) {
          // Left child has smaller value

          swap(id, left_id);

          // Update indices
          id = left_id;
          left_id = id*2 + 1;
          right_id = id*2 + 2;

        } else {
          // Min heap property restored

          break;
        }
      } else {
        // End of heap reached - property resotred

        break;
      }
    }
  }

public:
  /**
   *  Constructor
   */
  MinHeap () {
    last_pos = -1;  // Indicates empty heap
  }

  /**
   *  Inserts the new value in the heap.
   */
  void insert(int val) {
    heap.push_back(val);  // Add to the back of the heap
    last_pos++;

    sift_up();
  }

  /**
   *  Removes the min element from the heap.
   */
  int remove_min(void) {
    if (heap.empty()) {
      return -1;  // Error - empty heap
    }

    int min_val = heap[0];
    swap(0, last_pos);
    last_pos--;

    // Remove the min element and update heap
    heap.pop_back();
    sift_down();

    return min_val;
  }

  /**
   *  Utility function to print the binary heap.
   */
  void print_heap(void) {
    for (int i = 0; i < (int)heap.size(); i++) {
      cout << heap[i] << " ";
    }
    cout << endl;
  }
};


/**
 *  Driver program to test functionality of min binary heap.
 */
int main() {
  int queries;
  cin >> queries;

  MinHeap* min_heap = new MinHeap();
  int query_type, val;  // Query description

  for (int i = 0; i < queries; i++) {
    cin >> query_type;

    if (query_type == 1) {
      // Type 1: insert value in the heap

      cin >> val;
      min_heap->insert(val);

    } else {
      // Type 2: remove the min value

      cout << min_heap->remove_min() << endl;
    }
  }

  return 0;
}