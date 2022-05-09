#include <bits/stdc++.h>
using namespace std;

class node{
	public:
	int key;
	node *left, *right;
};

node* newNode(int key){
	node* Node = new node();
	Node->key = key;
	Node->left = Node->right = NULL;
	return (Node);
}

node *rightRotate(node *x){
	node *y = x->left;
	x->left = y->right;
	y->right = x;
	return y;
}

node *leftRotate(node *x){
	node *y = x->right;
	x->right = y->left;
	y->left = x;
	return y;
}

// This function brings the key at root if key is present in tree.
// If key is not present, then it brings the last accessed item at root. 
// This function modifies the tree and returns the new root.
node *splay(node *root, int key) {
	// Base cases: root is NULL or key is present at root
	if (root == NULL || root->key == key)
		return root;

	// Key lies in left subtree
	if (root->key > key) {
		// Key is not in tree, we are done
		if (root->left == NULL) return root;

		// Zig-Zig (Left Left)
		if (root->left->key > key) {
			// First recursively bring the key as root of left-left
			root->left->left = splay(root->left->left, key);
			// Do first rotation for root, second rotation is done after else
			root = rightRotate(root);
		}
		// Zig-Zag (Left Right)
		else if (root->left->key < key) {
			// First recursively bring the key as root of left-right
			root->left->right = splay(root->left->right, key);
            // Do first rotation for root->left
			if (root->left->right != NULL)
				root->left = leftRotate(root->left);
		}
		// Do second rotation for root
		return (root->left == NULL)? root: rightRotate(root);
	}
	// Key lies in right subtree
	else{ 
		// Key is not in tree, we are done
		if (root->right == NULL) return root;

		// Zig-Zag (Right Left)
		if (root->right->key > key){
			// Bring the key as root of right-left
			root->right->left = splay(root->right->left, key);
			// Do first rotation for root->right
			if (root->right->left != NULL)
				root->right = rightRotate(root->right);
		}
		// Zag-Zag (Right Right)
		else if (root->right->key < key){
			// Bring the key as root of right-right and do first rotation
			root->right->right = splay(root->right->right, key);
			root = leftRotate(root);
		}
		// Do second rotation for root
		return (root->right == NULL)? root: leftRotate(root);
	}
}

node *search(node *root, int key){
    return splay(root, key);
}

// Function to insert a new key k in splay tree with given root
node *insert(node *root, int k) {

	if (root == NULL) return newNode(k);
	root = splay(root, k);
	if (root->key == k) return root;
	node *newnode = newNode(k);

	if (root->key > k){
		newnode->right = root;
		newnode->left = root->left;
		root->left = NULL;
	}
	else{
		newnode->left = root;
		newnode->right = root->right;
		root->right = NULL;
	}
	return newnode; // newnode becomes new root
}

int depth(node* root){
    if(root == NULL) 
		return -1;
	int l,r;
	l = depth(root->left);
	r = depth(root->right);
	if (l>r)
	    return ++l;
	else
	    return ++r;
}

void inorder(node *root) {
	if (root != NULL) {
	    inorder(root->left);
		cout<<root->key<<" ";
		inorder(root->right);
	}
}

int main() {
    srand(time(NULL));
    int n,d,tmp,tmp2;
    float avg;
    cout << "Enter Number of Inserts: ";
	cin >> n;
	int arr[n];
	
	node *root = newNode((rand()%100)+1);
	d = depth(root);
    arr[0]=d;
    
    for(int i = 1; i<n; i++){
	    tmp = (rand() % 100) + 1; // Number to insert
	    tmp2 = (rand() % 10) + 1; // Search Probability
        root = insert(root, tmp);
        if (tmp2<4){
            root = search(root, tmp);
            cout << root->key << endl;
        }
        d = depth(root);
        //cout << d << endl;
        arr[i]=d;
	}
	
	for(int j = 0; j<n; j++)
	    avg += arr[j];
	avg /= n;
	cout << "Average Depth : " << avg << endl;
    
//	inorder(root);
	return 0;
}
