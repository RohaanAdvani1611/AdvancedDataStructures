#include<bits/stdc++.h>
#include<conio.h>
#include <queue>
#define NINF INT_MIN
using namespace std;

struct Node{
	Node* parent;
	Node* children;
	Node* lc;
	Node* rc;
	int info;
	int deg;
	char mark;
	char c;
};

struct Node* search_fiboh(int info);
struct Node* minimum = NULL;
int node_num = 0;

void insersion_f(int val) {
	struct Node* f_newnode = (struct Node*)malloc(sizeof(struct Node));
	f_newnode->info = val;
	f_newnode->deg = 0;
	f_newnode->mark = 'W';
	f_newnode->c = 'N';
	f_newnode->parent = NULL;
	f_newnode->children = NULL;
	f_newnode->lc = f_newnode;
	f_newnode->rc = f_newnode;
	if (minimum != NULL) {
		(minimum->lc)->rc = f_newnode;
		f_newnode->rc = minimum;
		f_newnode->lc = minimum->lc;
		minimum->lc = f_newnode;
		if (f_newnode->info < minimum->info)
			minimum = f_newnode;
	}
	else 
		minimum = f_newnode;
	node_num++;
}


void link_fiboheap(struct Node* p1, struct Node* p){
	(p1->lc)->rc = p1->rc;
	(p1->rc)->lc = p1->lc;
	if (p->rc == p)
		minimum = p;
	p1->lc = p1;
	p1->rc = p1;
	p1->parent = p;
	if (p->children == NULL)
		p->children = p1;
	p1->rc = p->children;
	p1->lc = (p->children)->lc;
	((p->children)->lc)->rc = p1;
	(p->children)->lc = p1;
	if (p1->info < (p->children)->info)
		p->children = p1;
	p->deg++;
}

void form_fiboheap(){
	int temp1;
	float temp2 = (log(node_num)) / (log(2));
	int temp3 = temp2;
	struct Node* array_f[temp3];
	for (int i = 0; i <= temp3; i++)
		array_f[i] = NULL;
	Node* p = minimum;
	Node* p1;
	Node* p2;
	Node* p3 = p;
	do {
		p3 = p3->rc;
		temp1 = p->deg;
		while (array_f[temp1] != NULL) {
			p1 = array_f[temp1];
			if (p->info > p1->info) {
				p2 = p;
				p = p1;
				p1 = p2;
			}
			if (p1 == minimum)
				minimum = p;
			link_fiboheap(p1, p);
			if (p->rc == p)
				minimum = p;
			array_f[temp1] = NULL;
			temp1++;
		}
		array_f[temp1] = p;
		p = p->rc;
	} while (p != minimum);
	minimum = NULL;
	for (int j = 0; j <= temp3; j++) {
		if (array_f[j] != NULL) {
			array_f[j]->lc = array_f[j];
			array_f[j]->rc = array_f[j];
			if (minimum != NULL) {
				(minimum->lc)->rc = array_f[j];
				array_f[j]->rc = minimum;
				array_f[j]->lc = minimum->lc;
				minimum->lc = array_f[j];
				if (array_f[j]->info < minimum->info)
					minimum = array_f[j];
			}
			else 
				minimum = array_f[j];
			
			if (minimum == NULL)
				minimum = array_f[j];
			else if (array_f[j]->info < minimum->info)
				minimum = array_f[j];
		}
	}
}


struct Node* Extract_min(){
	struct Node *ret = minimum;
	if (minimum == NULL)
		cout << "HEAP EMPTY" << endl;
	else {
		Node* temp = minimum;
		Node* pointer_f;
		pointer_f = temp;
		Node* x = NULL;
		if (temp->children != NULL) {
			x = temp->children;
			do {
				pointer_f = x->rc;
				(minimum->lc)->rc = x;
				x->rc = minimum;
				x->lc = minimum->lc;
				minimum->lc = x;
				if (x->info < minimum->info)
					minimum = x;
				x->parent = NULL;
				x = pointer_f;
			} while (pointer_f != temp->children);
		}
		(temp->lc)->rc = temp->rc;
		(temp->rc)->lc = temp->lc;
		minimum = temp->rc;
		if (temp == temp->rc && temp->children == NULL)
			minimum = NULL;
		else {
			minimum = temp->rc;
			form_fiboheap();
		}
		node_num--;
	}
	return ret;
}


void out(struct Node* got, struct Node* temp){
	if (got == got->rc)
		temp->children = NULL;
	(got->lc)->rc = got->rc;
	(got->rc)->lc = got->lc;
	if (got == temp->children)
		temp->children = got->rc;
	temp->deg = temp->deg - 1;
	got->rc = got;
	got->lc = got;
	(minimum->lc)->rc = got;
	got->rc = minimum;
	got->lc = minimum->lc;
	minimum->lc = got;
	got->parent = NULL;
	got->mark = 'B';
}


void fiboh_out(struct Node* temp){
	Node* p5 = temp->parent;
	if (p5 != NULL) {
		if (temp->mark == 'W') 
			temp->mark = 'B';
		else {
			out(temp, p5);
			fiboh_out(p5);
		}
	}
}


void val_decrease(struct Node* minimum, int old_val, int val){
	if (minimum == NULL){
		cout << "HEAP EMPTY" << endl;
		return;
	}
	struct Node *got = search_fiboh(old_val);
	if(got == NULL){
		cout << "ELEMENT NOT FOUND" << endl;
		return;
	}
	if(old_val < val){
		cout << "NEW VALUE GREATER THAN CURRENT VALUE" << endl;
		return;
	}
	got->info = val;
	struct Node* temp = got->parent;
	if (temp != NULL && got->info < temp->info) {
		out(got, temp);
		fiboh_out(temp);
	}
	if (got->info < minimum->info)
		::minimum = got;
}

struct Node* search_fiboh(int info){
	struct Node *got = NULL;
	struct Node *curr = minimum;
    do{
        queue<struct Node*> q;
        q.push(curr);
        while (!q.empty()) {
            struct Node *p = q.front();
            q.pop();
			if (p->info == info){
				got = p;
				break;
			}
            if (p->children) {
                struct Node *temp = p->children;
                do{
                    q.push(temp);
                    temp = temp->rc;
                }while (temp != p->children);
            }
        }
        curr = curr->rc;
        cout<<endl<<endl;
    }while (curr != minimum);
	return got;
}

void printHeap(){
    struct Node *curr = minimum;
    do{
        cout<<"DEGREE : "<<curr->deg<<endl;
        cout<<"HEAP : ";
        queue<struct Node*> q;
        q.push(curr);
        while (!q.empty()) {
            struct Node *p = q.front();
            q.pop();
			if (p->mark == 'B')
				cout<<p->info<<"(m) ";
			else
				cout<<p->info<<" ";
            if (p->children) {
                struct Node *temp = p->children;
                do{
                    q.push(temp);
                    temp = temp->rc;
                }while (temp != p->children);
            }
        }
        curr = curr->rc;
        cout<<endl<<endl;
    }while (curr != minimum);
}

int main(){
	struct Node* temp;
	while(1){
		cout << "FIBONACCI HEAP :" << endl;
		cout << "1. INSERT NODES \n2. EXTRACT MIN\n3. DECREASE KEY\n4. PRINT HEAP\n5. EXIT" << endl;
		int choice;
		cout << "ENTER CHOICE : ";
		cin >> choice;
		switch(choice){
			case 1:{
				cout << "ENTER NUMBER OF NODES : ";
				int n;
				cin >> n;
				for(int i = 0; i < n; i++){
					int info;
					cout << "ENTER NUMBER TO INSERT : ";
					cin >> info;
					insersion_f(info);
				}
			}
			break;
			case 2:{
				temp = Extract_min();
				cout<<"MINIMUM NODE EXTRACTED : " << temp->info<<endl;
			}
			break;
			case 3:{
				int info;
				cout << "ENTER KEY TO DECREASE: ";
				cin >> info;
				int new_val;
				cout << "ENTER NEW VALUE OF KEY : ";
				cin >> new_val;
				val_decrease(minimum, info, new_val);
			}
			break;
			case 4:{
				printHeap();
			}
			break;
			case 5:{
				exit(1);
			}
		}
	}
	return 0;
}
