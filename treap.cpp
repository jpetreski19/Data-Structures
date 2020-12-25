/**
 *	A C++ program that implements 'Implicit Treap'.
 *	Split and merge are the auxiliary operations used to implement other
 *	more complex operations.
 *
 *	All operations are approximately O(log n), since the priorities of the nodes are given randomly.
 */

#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#define endl "\n"
using namespace std;


class Node {
private:
	int value;		// The value at this node
	int priority;	// The priority (randomly generated) - Binary heap ordering according to these values
	int min_value;	// The minimum value contained in the subtree rooted at this node
	bool reverse;	// A flag that indicates whether this node is included in the reverse operation

	// Stats
	int size;		// Used for implicit indexing
	int height;

	// Pointers to left and right child
	Node* left_child;
	Node* right_child;


	/**
	 *	Resets the minimum value of a node. Used after a split is performed.
	 */
	void reset_min(Node* x) {
		if (x == nullptr) {
			return;
		}

		x->min_value = x->value;
	}


	/**
	 *	Utility function that swaps left and right children of a node x.
	 */
	void swap_children(Node* x) {
		if (x == nullptr) {
			return;
		}

		Node* temp = x->left_child;
		x->left_child = x->right_child;
		x->right_child = temp;
	}


	/**
	 *	Lazy propagation - propagates the updates to the children
	 *	only when that node is checked.
	 */
	void propagate(Node* x) {
		if (x == nullptr) {
			return;
		}

		if (x->reverse == 1) {
			if (x->left_child != nullptr) {
				x->left_child->reverse ^= true;
			}
			if (x->right_child != nullptr) {
				x->right_child->reverse ^= true;
			}

			swap_children(x);
			x->reverse = false;	// Mark as reversed
			recalc(x);
		}
	}


	/**
	 *	Recalculates the size, height and minimum value of the node x.
	 */
	void recalc(Node* x) {
		if (x == nullptr) {
			return;
		}

		// Size of subtree
		x->size = 1 + get_size(x->left_child) + get_size(x->right_child);

		if (x->left_child == nullptr and x->right_child == nullptr) {
			// x is a leaf (external node)

			x->height = 0;
			x->min_value = x->value;
		} else {
			// x is internal node

			x->height = 1 + max(get_height(x->left_child), get_height(x->right_child));
			x->min_value = min(get_min_value(x), min(get_min_value(x->left_child), get_min_value(x->right_child)));
		}
	}


public:
	static const int MAX_PRIORITY = 1000007;	// Specifies the default range of numbers when picking random priorities

	/**
	 *	Sets the attributes of the new instance of Node.
	 */
	Node (int value) {
		this->value = value;
		this->priority = get_random();
		this->min_value = value;
		this->reverse = false;

		this->left_child = nullptr;
		this->right_child = nullptr;
		recalc(this);	// Just sets height = 0 and size = 1
	}


	/**
	 *	Returns a random number from 0 to n. By default, n is set to MAX_PRIORITY.
	 *	m19937 is a lot better and faster random generator than rand().
	 */
	static int get_random(int n = MAX_PRIORITY) {
		mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
		return uniform_int_distribution<int>(0, n)(rng);
	}


	/**
	 *	Utility function that returns the maximum of 2 integers. Used to calculate height of a subtree.
	 */
	static int max(int a, int b) {
		if (a > b) {
			return a;
		}
		return b;
	}


	/**
	 *	Utility function that returns the minimum of 2 integers. Used to calculate minimum element of a subtree.
	 */
	static int min(int a, int b) {
		if (a < b) {
			return a;
		}
		return b;
	}


	/**
	 *	Merges 2 treaps into one.
	 *	When the 2 nodes we are comparing are not empty, then the one
	 *	with higher priority is taken to be the parent of the treap that will be formed
	 *	after merging its left or right child with the other treap.
	 *
	 *	2 cases:
	 *		1. When the left treap has higher priority:
	 *			- its right child becomes the mergeon of the 2 treaps:
	 *			  one rooted at the right child and the other at right.
	 *
	 *		2. When the right treap has higher priority:
	 *			- its left child becomes the new treap after the current treap rooted at 
	 *			  left child is merged with the one rooted at left.
	 *
	 *	Base cases:
	 *		1. If left child of the right treap is empty and the right treap has hogher priority,
	 *		   then just set the left child to be the remainder right treap.
	 *		2. The opposite of what happens in case 1 (this time with left treap and its right child).
	 *		   Principle is the same.
	 */
	Node* merge(Node* left, Node* right) {

		// Base case 1: right is the bottom node
		if (left == nullptr) {
			return right;
		}
		// Base case 2: left is the bottom node
		else if (right == nullptr) {
			return left;
		}

		// Propagate the updates that should be done to the children
		propagate(left);
		propagate(right);

		if (get_priority(left) > get_priority(right)) {
			left->right_child = merge(left->right_child, right);

			// Minimum element might have changed - reset and recalc
			reset_min(left);
			recalc(left);
			return left;

		} else {
			right->left_child = merge(left, right->left_child);

			// Minimum element might have changed - reset and recalc
			reset_min(right);
			recalc(right);
			return right;
		}
	}


	/**
	 *	Splits the given treap into 2 at a specified index.
	 *	The result of this method are 2 treaps: the one that contains all elements before index,
	 *	and the one containing the rest.
	 *
	 *	The method uses implicit indices to find the correct position where it should make the cut.
	 *	The index of an element is the size of the left subtree + 1 (or just the size of the left subtree if
	 *	0-based indexing is used).
	 *	Then each time, the method is called recusively:
	 *		1.	If the index where we should split is in the left subtree:
	 *			set the left child to be the root of the right treap of the split operation being performed on
	 *			the left subtree. That is, we want to maintain all element that are to the right 
	 *			in the current treap.
	 *		2.	If the index where we should split is the right subtree:
	 *			the principle is the same, just in this case, the right subtree of the treap rooted at
	 *			the current node is set to be root of the left treap of the split operation on that child.
	 *
	 *	left_res contains the 2 treaps after the split operation has been called recursively on the left child
	 *	of the current node. Similarly, right_res is the same but when called on right child.
	 *	Whenever there is a change, recalculate should be called to update the attributes.
	 */
	vector<Node*> split(Node* current, int index) {
		if (current == nullptr) {
			// End reached
			return {nullptr, nullptr};
		}

		// Make sure any changes done to this node are propagated to its children
		propagate(current);

		if (get_size(current->left_child) >= index) {
			// The position where the treap should be split
			// is in the left subtree

			vector<Node*> left_res = split(current->left_child, index);
			current->left_child = left_res[1];

			reset_min(current);
			recalc(current);	// Update attributes
			return {left_res[0], current};

		} else {
			// The position where the treap should be split
			// is in the right subtree

			// New index because we start to count from 0 in the other subtree
			index = index - get_size(current->left_child) - 1;

			vector<Node*> right_res = split(current->right_child, index);
			current->right_child = right_res[0];

			reset_min(current);
			recalc(current);	// Update attributes
			return {current, right_res[1]};
		}
	}


	/**
	 *	Returns the index of the smallest value in the treap.
	 *	Each node contains as an attribute the minimum value in the subtree rooted at that node.
	 *
	 *	- When going to the left subtree, the number of indices we passed does not change.
	 *	- Going to the right means everything on the left should be skipped and therefore
	 *	  the size of the left subtree + 1 for the current node is added.
	 */
	int find_min(Node* current, int indices_passed) {
		if (current == nullptr) {
			return -1;	// Empty treap
		}
		
		propagate(current);	// Update node
		int index = get_size(current->left_child) + indices_passed;

		// Check if minimum value is in the left subtree
		if (get_min_value(current) == get_min_value(current->left_child)) {
			return current->left_child->find_min(current->left_child, indices_passed);
		}
		// Check if minimum value is in the right subtree
		else if (get_min_value(current) == get_min_value(current->right_child)) {
			return current->right_child->find_min(current->right_child, index + 1);
		}

		return index;
	}


	/**
	 *	Utility function that always removes the first element of the array.
	 *	res[1] contains the right part after splitting the array at index 1.
	 */
	Node* remove_first(Node* root) {
		if (get_size(root) >= 1) {
			// The treap is not empty, so there is something that can be cut off

			vector<Node*> res = split(root, 1);
			delete res[0];
			return res[1];
		}

		// Empty treap
		return root;
	}


	/**
	 *	Helper function that contains the logic behind the task to reverse a segment.
	 *		Firstly, the subarray should be split at [1...from - 1] - the before array part.
	 *		Secondly, the rest of the array (the right part after calling split at from index) should be split 
	 *		at [from....to - from] - the segment we are looking for.
	 *		The remainder [to - from + 1.....n] is the after subarray part.
	 *
	 *		Once we have the 3 treaps at hand, we can just flip the reverse flag of the root of the
	 *		subarray and merge the treaps back in the same order (before subarray + subarray + after subarray).
	 *		The reverse flag will then be propagated to the children of the root of the segment we reverse.
	 */
	Node* reverse_subarray(Node* root, int from, int to) {

		// Split the treap to [1.....from - 1] and [from.....n]
		vector<Node*> res_left = split(root, from);
		Node* before_subarray = res_left[0];
		Node* rest = res_left[1];

		// Split the remaining treap [from.....n] to [from.....to] and [to + 1.....n]
		vector<Node*> res_right = split(rest, to - from + 1);
		Node* subarray = res_right[0];
		Node* after_subarray = res_right[1];

		// Free memory
		res_left.clear();
		res_right.clear();

		// Set flag and merge the segments back in the same order
		subarray->reverse ^= true;
		return merge(merge(before_subarray, subarray), after_subarray);
	}


	/**
	 *	Helper function to print the inorder traversal of the tree.
	 *	The result of inorder traversal is the current ordering of the array.
	 */
	void inorder() {
		if (this == nullptr) {
			return;
		}

		propagate(this);	// Do updates
		this->left_child->inorder();
		cout << this->value << " ";
		this->right_child->inorder();
	}


	// ===========================  Getters  ===============================


	int get_value(Node* x) {
		return (x == nullptr) ? -1 : x->value;
	}
	int get_min_value(Node* x) {
		return (x == nullptr) ? (1 << 30) : x->min_value;
	}
	int get_priority(Node* x) {
		return (x == nullptr) ? -1 : x->priority;
	}
	int get_size(Node* x) {
		return (x == nullptr) ? 0 : x->size;
	}
	int get_height(Node* x) {
		return (x == nullptr) ? 0 : x->height;
	}
};


int main() {
	ios::sync_with_stdio(false);

	// Get input
	int num_elements;
	cin >> num_elements;

	int array[num_elements + 1];
	for (int i = 0; i < num_elements; i++) {
		cin >> array[i];
	}


	// Building the treap:
	// The initial treap node. All other elements will be merged with this one.
	Node* root = new Node(array[0]);

	// Insert all elements as treaps with single node
	// by merging them with the initial 'root'.
	for (int i = 1; i < num_elements; i++) {
		root = root->merge(root, new Node(array[i]));
	}


	/**
	 *	The task: given a permutation of N elements. Sort the permutation in N
	 *	steps such that, for every step i, the segment between the ith index and the
	 *	index of the element with value i is reversed.
	 *	The required output is an array of second indexes before performing the ith step.
	 *
	 *	E.g.: array is:	4 2 1 3
	 *
	 *					Step 1: 1 2 4 3		(second index before operation is 3)
	 *					Step 2: 1 2 4 3		(second index: 2 - reverse from 2 to 2)
	 *					Step 3: 1 2 3 4		(second index: 4)
	 *					Step 4: 1 2 3 4		(second index: 4)
	 *
	 *			Output: 3 2 4 4
	 */


	// Queries
	for (int i = 0; i < num_elements; i++) {
		int index_of_i = root->find_min(root, 0);
		cout << index_of_i + i + 1 << " ";

		if (index_of_i != 0) {
			// If we should perform a change, reverse the segment
			// and remove the first element.
			// The first element should be removed because the array
			// up to that element is sorted, and no further actions are needed.
			// Also, this is mandatory to be able to find the next smallest element.

			root = root->reverse_subarray(root, 0, index_of_i);
			root = root->remove_first(root);

		} else {
			// No need to reverse, just pop the first element (segment already sorted).

			root = root->remove_first(root);
		}
	}

	delete root;	// Free memory
	cout << endl;
	return 0;
}