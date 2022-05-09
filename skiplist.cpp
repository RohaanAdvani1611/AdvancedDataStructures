// A3: Implementation of Skip List

#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
using namespace std;
using std::cout; 
using std::cerr;
using std::endl; 
using std::string;
using std::ifstream;

class Node {
public:
    int key;
	// Stores pointers to node at different levels
	Node **forward;
	Node(int, int);
};

Node::Node(int key, int level) {
	this->key = key;
	// Allocate memory to forward
	forward = new Node*[level+1];
	memset(forward, 0, sizeof(Node*)*(level+1));
};

class SkipList {
	int MAXLVL;
	// P is probability of getting heads for random lvl generation
	float P;
	// current level
	int level;
	Node *header;
public:
	SkipList(int, float);
	int randomLevel();
	Node* createNode(int, int);
	void insert(int);
	void Delete(int);
	void search(int);
	void display();
};

SkipList::SkipList(int MAXLVL, float P) {
	this->MAXLVL = MAXLVL;
	this->P = P;
	level = 0;
	// create header node and initialize key to -1
	header = new Node(-1, MAXLVL);
};

int SkipList::randomLevel() {
	float r = (float)rand()/RAND_MAX;
	int lvl = 0;
	while(r < P && lvl < MAXLVL) {
		lvl++;
		r = (float)rand()/RAND_MAX;
	}
	return lvl;
};

Node* SkipList::createNode(int key, int level) {
	Node *n = new Node(key, level);
	return n;
};

void SkipList::insert(int key) {
	Node *current = header;
	// create update array and initialize it
	Node *update[MAXLVL+1];
	memset(update, 0, sizeof(Node*)*(MAXLVL+1));
	/*  start from highest level 
		move forward while key > next node
		Else insert current in update and go down and continue search */
	for(int i = level; i >= 0; i--) {
		while(current->forward[i] != NULL && current->forward[i]->key < key)
			current = current->forward[i];
		update[i] = current;
	}
	// reached level 0 and forward pointer to right, which is desired position to insert key.
	current = current->forward[0];
	/* If current is NULL - end of the level
	   current's key is != key - insert node between update[0] and current node */
	if (current == NULL || current->key != key) {
		// Generate a random level for node
		int rlevel = randomLevel();
		if(rlevel > level) {
			for(int i=level+1;i<rlevel+1;i++)
				update[i] = header;
			level = rlevel;
		}
		Node* n = createNode(key, rlevel);
		for(int i=0;i<=rlevel;i++) {
			n->forward[i] = update[i]->forward[i];
			update[i]->forward[i] = n;
		}
	}
};

void SkipList::Delete(int key) {
	Node *current = header;
	Node *update[MAXLVL+1];
	memset(update, 0, sizeof(Node*)*(MAXLVL+1));
	for(int i = level; i >= 0; i--) {
		while(current->forward[i] != NULL && current->forward[i]->key < key)
			current = current->forward[i];
		update[i] = current;
	}
	current = current->forward[0];
	// If current node is target node
	if(current != NULL and current->key == key) {
		// start from lowest level and rearrange pointers just like we do in SLL 
		for(int i=0;i<=level;i++) {
			if(update[i]->forward[i] != current)
				break;
			update[i]->forward[i] = current->forward[i];
		}
		// Remove levels having no elements
		while(level>0 && header->forward[level] == 0)
			level--;
	}
};

void SkipList::search(int key) {
	Node *current = header;
	int comparisons = 0;
	for(int i = level; i >= 0; i--) {
		while(current->forward[i] && current->forward[i]->key < key){
			current = current->forward[i];
		    comparisons++;
		}
	}
	current = current->forward[0];
	if(current and current->key == key)
		cout<<"Key Found: "<<key<<"\n";
	else 
	    cout<<"Key Not Found"<<"\n";
	cout<<"Number of Comparisons : "<<comparisons;
	cout<<"\n";
};

void SkipList::display() {
	for(int i=0;i<=level;i++) {
		Node *node = header->forward[i];
		int count = 0;
		cout<<"Level "<<i<<": ";
		while(node != NULL) {
			cout<<node->key<<" ";
			node = node->forward[i];
			count++;
		}
		cout<<"\tCount : "<<"("<<count<<")";
		cout<<"\n\n";
	}
	cout<<"\n";
};

// Driver to test above code
int main() {
	// Seed random number generator
	srand((unsigned)time(0));
	
	string filename("numbers.txt");
    int number;
    
    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"<< filename << "'" << endl;
        return EXIT_FAILURE;
    }
    
    // create SkipList object with MAXLVL and P
	SkipList lst(100, 0.8);
	
    while (input_file >> number) {
        lst.insert(number);
    }
    cout << endl;
    input_file.close();

	lst.display();

	//Search for node 19
	lst.search(19);
	cout<<"\n";
	
	//Search for node 20
	lst.search(20);
	cout<<"\n";

	//Delete node 19
	lst.Delete(19);
	lst.display();
	
	return EXIT_SUCCESS;
}
