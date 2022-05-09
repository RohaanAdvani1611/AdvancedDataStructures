#include<bits/stdc++.h>
using namespace std;

// Binomial Tree Node Structure
struct Node {
	int data;
	// Degree is number of child nodes
	int degree;
	Node *child;
	Node *sibling;
	Node *parent;
};

// Newnode Function
Node* newNode(int key) {
	Node *temp = new Node;
	temp->data = key;
	temp->degree = 0;
	temp->child = NULL;
	temp->parent = NULL;
	temp->sibling = NULL;
	return temp;
}

// Merge Binomial Trees Function
Node* mergeBT(Node *b1, Node *b2) {
    // Greater value tree made child of smaller value tree
	if (b1->data > b2->data)
		swap(b1, b2);
	b2->parent = b1;
	b2->sibling = b1->child;
	b1->child = b2;
	b1->degree++;
	return b1;
}

// list<Node *> is pointer to Binomial Heap
// Union Binomial Heap Function
list<Node*> unionBH(list<Node*> h1, list<Node*> h2) {
	list<Node*> newheap;
	list<Node*>::iterator it1 = h1.begin();
	list<Node*>::iterator it2 = h2.begin();
	// Push Element with lesser degree to newheap
	while (it1 != h1.end() && it2 != h2.end()) {
		if((*it1)->degree <= (*it2)->degree) {
			newheap.push_back(*it1);
			it1++;
		}
		else {
			newheap.push_back(*it2);
			it2++;
		}
	}
	// Handle remaining elements h1
	while (it1 != h1.end()) {
		newheap.push_back(*it1);
		it1++;
	}
    // Handle remaining elements h2
	while (it2 != h2.end()) {
		newheap.push_back(*it2);
		it2++;
	}
	return newheap;
}

// Adjust (sorts heap in increasing order of degree) Function
list<Node*> adjust(list<Node*> bh) {
    // For null / single node heap
	if (bh.size() <= 1)
		return bh;

	list<Node*>::iterator it1, it2, it3;
	it1 = it2 = it3 = bh.begin();
    
    // Adjust iterators in order
	if (bh.size() == 2) {
		it2 = it1;
		it2++;
		it3 = bh.end();
	}
	else {
		it2++;
		it3=it2;
		it3++;
	}
	while (it1 != bh.end()) {
		// Last element remaining
		if (it2 == bh.end())
			it1++;
		// If merging not possible move to next heap 
		else if ((*it1)->degree < (*it2)->degree) {
			it1++;
			it2++;
			if(it3!=bh.end())
				it3++;
		}
		// If degree of 3 consecutive trees are same
		else if (it3!=bh.end() && (*it1)->degree == (*it2)->degree && (*it1)->degree == (*it3)->degree) {
			it1++;
			it2++;
			it3++;
		}
		// If degree of 2 Binomial Tree are same
		else if ((*it1)->degree == (*it2)->degree) {
			Node *temp;
			*it1 = mergeBT(*it1,*it2);
			it2 = bh.erase(it2);
			if(it3 != bh.end())
				it3++;
		}
	}
	return bh;
}

// Inserting a Binomial Tree into Binomial Heap Function
list<Node*> insert(list<Node*> bh, Node *tree) {
	list<Node*> temp;
	// Insert Binomial Tree into temp heap
	temp.push_back(tree);
	// Perform union operation on 2 heaps
	temp = unionBH(bh, temp);
	// Adjust new heap and return
	return adjust(temp);
}

// Remove min element function
list<Node*> removeMin(Node *tree) {
	list<Node*> heap;
	Node *temp = tree->child;
	Node *rem;
	// Extract min and push remaining element to heap
	while (temp) {
		rem = temp;
		temp = temp->sibling;
		rem->sibling = NULL;
		heap.push_front(rem);
	}
	return heap;
}

// Enqueue element function
list<Node*> enqueue(list<Node*> bh, int key) {
    // Build Node and insert into heap
	Node *temp = newNode(key);
	return insert(bh, temp);
}

// Get Minimum Element Function
Node* getMin(list<Node*> bh) {
	list<Node*>::iterator it = bh.begin();
	Node *temp = *it;
	// Keep storing minimum element in temp 
	while (it != bh.end()) {
		if ((*it)->data < temp->data)
			temp = *it;
		it++;
	}
	return temp;
}

// Extract Minimum Element Function
list<Node*> extractMin(list<Node*> bh) {
	list<Node*> newheap, rem;
	Node *temp;
	// Get minimum
	temp = getMin(bh);
	list<Node*>::iterator it;
	it = bh.begin();
	// Insert other element into new heap except tree having min
	while (it != bh.end()) {
		if (*it != temp)
			newheap.push_back(*it);
		it++;
	}
	// Remove min and get remaining heap
	rem = removeMin(temp);
	// Perform union on heaps
	newheap = unionBH(newheap,rem);
	// Adjust and return
	newheap = adjust(newheap);
	return newheap;
}

// Print Tree Function
void printTree(Node *h) {
	while (h) {
		cout << h->data << " ";
		printTree(h->child);
		h = h->sibling;
	}
}

// Print Heap Function
void printHeap(list<Node*> _heap) {
	list<Node*> ::iterator it;
	it = _heap.begin();
	while (it != _heap.end()) {
		printTree(*it);
		it++;
	}
}

// Test Functions - Driver Code
int main() {
    // Initialize Priority Queue
	list<Node*> pq;
	// Insert data in the Priority Queue
	pq = enqueue(pq,100);
	pq = enqueue(pq,200);
	pq = enqueue(pq,250);
	pq = enqueue(pq,350);
	pq = enqueue(pq,500);
	pq = enqueue(pq,700);
	cout << "Initial Priority Queue : ";
	printHeap(pq);

    // Get Minimum element of Priority Queue
	Node *temp = getMin(pq);
	cout << "\nMinimum element of Priority Queue : "<< temp->data << "\n";

	// Extract Minimum element of Priority Queue
	pq = extractMin(pq);
	cout << "\nPriority Queue after deletion of minimum element : ";
	printHeap(pq);
	
	pq = extractMin(pq);
	cout << "\nPriority Queue after deletion of minimum element : ";
	printHeap(pq);

	return 0;
}
