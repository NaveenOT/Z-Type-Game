#ifndef SKEW_H
#define SKEW_H
#include <iostream>
using namespace std;
class Skew{
	private:
	struct Node{
		Node* left;
		Node* right;
		int data;
		Node(int da):left(nullptr), right(nullptr), data(da){}
	};
	public:
	Node* root;
	Skew(): root(nullptr){}
	void insert(int data){
		Node* newNode = new Node(data);
		root = merge(newNode, root);
		
	}
	void deleteMin(){
		if(!root){
					cout << "Heap Empty" << endl;
			}else{
				root = merge(root->left, root->right);
			}
	}
	void printHeap(Node* t, int depth = 0){
		if(t){
			printHeap(t->right, depth+5);
			for(int i = 0; i < depth; i++){
				cout << " ";
			}
			cout << t->data << endl;
			printHeap(t->left, depth+5);
		}
	}
	void printHeap(){
		printHeap(root);
	}

	Node* merge1(Node* h1, Node* h2){
			if(h1->left == nullptr) h1->left = h2;
			else{
				h1->right = merge(h1->right, h2);
				swapChildren(h1);
			}
			return h1;
		}
		
	Node* merge(Node* h1, Node* h2){
		if(h1 == nullptr) return h2;
		if(h2 == nullptr) return h1;
		if(h1->data > h2->data) return merge1(h1, h2);
		else return merge1(h2, h1);
		
	}
	
	void swapChildren(Node* h1){
		Node* temp = h1->left;
		h1->left = h1->right;
		h1->right = temp;
		
	}
	int findMin(){
		if(!root){
			cout << "Heap Empty" << endl;
		}else{
			return root->data;
		}
	}
    int FindMin(){
        if(!root)
        return root->data;
    }
};
#endif
