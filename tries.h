#ifndef TRIES_H
#define TRIES_H
#include <iostream>
using namespace std;
class Trie{
private:
  struct TrieNode{
  	TrieNode* c[26];
  	bool EOW;
  	TrieNode(){
  		for(int i = 0; i < 26; i++){
  			c[i] = nullptr;
  		}
  		EOW = false;
  	}
  };
  public:
  TrieNode* root;
  Trie(){
  	root = new TrieNode();
  }
  void insert(string word){
  	TrieNode* node = root;
  	for(char c: word){
  		int index = c - 'a';
  		if(node->c[index] == nullptr){
  			node->c[index] = new TrieNode();
  		}
  		node = node->c[index];
  	}
  	node->EOW = true;
  }
  bool search(string word){
    	TrieNode* node = root;
    	for(char c: word){
    		int index = c - 'a';
    		if(node->c[index] == nullptr){
    			return false;
    		}
    			node = node->c[index];
    	}
    	return node->EOW;
    }
     bool startsWith(string word){
    	TrieNode* node = root;
    	for(char c: word){
    		int index = c - 'a';
    		if(node->c[index] == nullptr){
    			return false;
    		}
    			node = node->c[index];
    	}
    	return true;
    }
    void autoComplete(string word){
    	TrieNode* node = root;
    	for(char ch: word){
    	int index = ch - 'a';
    	if(node->c[index] == nullptr){
    	cout << "Prefix not found " << endl;
    		return;
    		}
    	node = node->c[index];
    	}
    	cout << "Autocomplete Suggestions for "<<word << endl;
    	autCompleteHelper(node, word);
    }
    void autCompleteHelper(TrieNode* node, string word){
    	if(node->EOW){
    		cout << "|-" << word << endl;
    	}
    	for(char ch = 'a'; ch <= 'z'; ch++){
    		int index = ch - 'a';
    		if(node->c[index]){
    			autCompleteHelper(node->c[index], word + ch);
    		}
    	}
    }
    void remove(string word){
    	if(removeHelper(root, word)) cout << word << " removed successfully" << endl;
    	else{
    		cout << "WORD NOT FOUND" << endl;
    	}
    }
    bool removeHelper(TrieNode* node, string word, int depth = 0){
    	if(!node) return false;
    	if(depth == word.size()){
    		if(!node->EOW) return false;
    		node->EOW = false;
    		return true;
    	}
    	int index = word[depth] - 'a'; 	
    	if(removeHelper(node->c[index], word, depth+1)){
    		delete node->c[index];
    		node->c[index] = nullptr;
    		return !node->EOW && isEmpty(node);
    	}
    	return false;
    }
    void printTree(TrieNode* node, string word = "",int level = 0){
    	if(node->EOW){
    		cout << string(level*2, ' ') << "[ " << word << "]" << endl; 
    	}
    	for(char ch = 'a'; ch <= 'z'; ch++){
    		int index=ch - 'a';
    		if(node->c[index] != nullptr){
    			cout << string(level*2, ' ') << "|-- " << ch << endl;	
    			printTree(node->c[index], word+ch, level+1);
    		}
    	}
    }
    void printTree(){
    	printTree(root);
    }
    bool isEmpty(TrieNode* node) {
        for (int i = 0; i < 26; i++) {
            if (node->c[i] != nullptr)
                return false;
        }
        return true;
    }
  
};
#endif