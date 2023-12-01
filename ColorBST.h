/*
PIC 10C Homework 5, ColorBST.h
Purpose: Define a template class for a color binary search tree
PIC 10C Homework 5
Author: Kelsey Lin
Date: 02/28/2023
*/

#pragma once

#include<iostream>
using namespace std;

template<typename T>
class Node;

template<typename T>
class ColorBST;

template<typename T>
class Node
{
public:
	Node();
	void insert_node(Node<T>* new_node);
private:
	T data;
	char color;
	Node<T>* left;
	Node<T>* right;
	friend class ColorBST<T>;
};

template<typename T>
class ColorBST
{
public:
	ColorBST();
	~ColorBST(); //destructor, class responsible for dynamically allocating new Node objects
	void deleter(Node<T>* cur); // helper for destructor using recursion
	void insert(T data, char c = 'r');
	void inOrder(Node<T>* cur) const; // in-order traversal: visit in alphabetical order
	void reverseInOrder(Node<T>* cur) const; // reverse in-order traversal: descending alphabetical
	void ascending_print() const;
	void descending_print() const;
	T largest() const; // returns largest element
	bool BlackRoot(); // true if tree has black root
	bool DoubleRed(Node<T>* node, Node<T>* parent); // helper function for NoDoubleRed()
	Node<T>* findParent(Node<T>* cur) const; // helper function for DoubleRed
	bool NoDoubleRed(); // true if tree has no double-red nodes
	int calculateBlackDepth(Node<T>* cur); // helper function calculating black-depth of sub-tree
	bool BlackDepth(); // true if black depth property preserved

private:
	Node<T>* root; // this is a pointer to a Node object: to the root/top of tree
};

template<typename T>
Node<T>::Node()
	: data(), color(), left(nullptr), right(nullptr)
{
	// Node constructor
}

template<typename T>
ColorBST<T>::ColorBST()
{
	// ColorBST constructor
	root = nullptr;
}

template<typename T>
ColorBST<T>::~ColorBST()
{
	deleter(root);
}

template<typename T> // helper function for destructor
void ColorBST<T>::deleter(Node<T>* cur)
{
	if (cur != nullptr) // while still pointing to a node that needs to be deleted
	{
		deleter(cur->left);
		deleter(cur->right);
		delete cur;
	}

	root = nullptr;
}

template<typename T>
void ColorBST<T>::insert(T data, char color) // helper function for insert_node
{
	// create new Node<T> object with data, color, and initialize left/right pointers 
	Node<T>* new_node = new Node<T>;
	new_node->data = data;
	new_node->color = color;
	new_node->left = nullptr;
	new_node->right = nullptr;

	// if current ColorBST is empty, set as root
	if (root == nullptr)
	{
		root = new_node;
	}
	else
	{
		root->insert_node(new_node);
	}

}

template<typename T>
void Node<T>::insert_node(Node<T>* new_node) // correctly placing node within ColorBST
{
	if (new_node->data < data) // left-side
	{
		if (left == nullptr)
		{
			left = new_node; // left child of root
		}
		else
		{
			left->insert_node(new_node); // add as a child elsewhere
		}
	}
	else if (new_node->data > data) // right-side
	{
		if (right == nullptr)
		{
			right = new_node; // right child of root
		}
		else
		{
			right->insert_node(new_node); // add as a child elsewhere
		}
	}
}

template<typename T>
void ColorBST<T>::inOrder(Node<T>* cur) const
{
	if (cur == nullptr) // base case/if empty then return
	{
		return;
	}

	inOrder(cur->left); // process nodes in left sub-ColorBST

	std::cout << cur->data << " "; // process current node

	inOrder(cur->right); // process nodes in right sub-ColorBST

}

template<typename T>
void ColorBST<T>::reverseInOrder(Node<T>* cur) const
{
	if (cur == nullptr) // base case/if empty then return
	{
		return;
	}

	reverseInOrder(cur->right);

	std::cout << cur->data << " "; // print out data of current node

	reverseInOrder(cur->left);

}

template<typename T> // smallest to largest values
void ColorBST<T>::ascending_print() const
{
	inOrder(root);
	std::cout << std::endl;
}

template<typename T> // largest to smallest values
void ColorBST<T>::descending_print() const
{
	reverseInOrder(root);
	std::cout << std::endl;
}

template<typename T>
T ColorBST<T>::largest() const
{
	if (root == nullptr)
	{
		return -1;
	}

	Node<T>* cur = root;
	// start from root and traverse to largest node value
	while (cur->right != nullptr)
	{
		cur = cur->right;
	}

	// once reached nullptr means at largest 

	return cur->data;

}

template<typename T>
bool ColorBST<T>::BlackRoot() // returns true if ColorBST has a black root
{
	Node<T>* cur = root;

	if (cur == nullptr)
	{
		return true; // empty tree, considered black node so return true
	}

	return (cur->color == 'b'); // otherwise root is black
}

template<typename T> // finds the parent node of a given node
Node<T>* ColorBST<T>::findParent(Node<T>* cur) const
{
	Node<T>* parent = nullptr; // initialize parent Node pointer

	Node<T>* temp = root; // start at root and traverse down before reaches cur node for parent node
	while (temp != cur)
	{
		parent = temp;
		if (cur->data < temp->data)
		{
			temp = temp->left;
		}
		else if (cur->data >= temp->data)
		{
			temp = temp->right;
		}
	}

	return parent;
}

template<typename T>
bool ColorBST<T>::DoubleRed(Node<T>* node, Node<T>* parent)
{
	if (node == nullptr)
	{
		return true;
	}

	parent = findParent(node); // this will be the parent node

	// check current node color and color of its children
	if (node->color == 'r' && parent != nullptr && parent->color == 'r')
	{
		return false; // no-double red violated
	}

	return DoubleRed(node->right, node) && DoubleRed(node->left, node); // recursively continue
}

template<typename T>
bool ColorBST<T>::NoDoubleRed()
{
	return DoubleRed(root, nullptr); // start from root and traverse down
}

template<typename T>
int ColorBST<T>::calculateBlackDepth(Node<T>* cur) // used as helper for bool BlackDepth()
{
	if (cur == nullptr)
	{
		return 0; // base case
	}

	// use recursion to check nodes
	int leftHeight = calculateBlackDepth(cur->left);
	int rightHeight = calculateBlackDepth(cur->right);

	int nodeBD = 0;
	if (cur->color == 'b')
	{
		nodeBD = 1; // black node
	}

	// calculate black depth:
	if (leftHeight != rightHeight || rightHeight == -1 || leftHeight == -1)
	{
		return -1; // different nodes contain different black-depth values
	}
	else
	{
		return leftHeight + nodeBD; // black-depth value (is consistent)
	}
}

template<typename T>
bool ColorBST<T>::BlackDepth()
{
	if (root == nullptr)
	{
		return true; // empty ColorBST
	}

	return calculateBlackDepth(root) != -1;
	// true means black-depth is maintained
	// false means black-depth is violated
}
