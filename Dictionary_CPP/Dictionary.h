/*
 * Dictionary.h
 *
 *  Created on: Apr 7, 2018
 *      Author: x-wing
 */

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <ostream>


/************************************************************
 * CLASS : Node
 * ----------------------------------------------------------
 * The structure of the Dictionary "key path" Node is
 * defined below. While performing a lookup with a given
 * key, the data linked to that key may be found in the very
 * last Node matched if the key has been inserted previously.
 * ----------------------------------------------------------
 * NODE:         _____[NODE_KEY : DATA]_____
 *              /             |             \
 *       ______/      ______[DOWN]_____      \______
 *  _____[LEFT]_____  KEY_i == NODE_KEY   ____[RIGHT]_____
 *  KEY_i < NODE_KEY                      KEY_i > NODE_KEY
 ***********************************************************/
template<class Data> class Node;
template<class Data> std::ostream& operator << (std::ostream&, const Node<Data>&);

template<class Data> class Node
{
public:
	Data  data;
	char  key;
	Node *left_node,*right_node,*down_node;

	friend std::ostream& operator << <>(std::ostream&, const Node<Data>&);
	Node(char k, Data d):data(d),key(k),left_node(0),right_node(0),down_node(0) {}
	Node<Data>* branch_left (char, Data);
	Node<Data>* branch_right(char, Data);
	Node<Data>* branch_down (char, Data);
};


/**
 * Print Node
 */
template<class Data>
std::ostream& operator << (std::ostream &out, const Node<Data> &node)
{
	out << node.data;
	return out;
}


template<class Data>
Node<Data>* Node<Data>::branch_left(char key, Data empty_data)
{
	if (left_node == NULL)
		left_node = new Node<Data>(key, empty_data);
	return left_node;
}


template<class Data>
Node<Data>* Node<Data>::branch_right(char key, Data empty_data)
{
	if (right_node == NULL)
		right_node = new Node<Data>(key, empty_data);
	return right_node;
}


template<class Data>
Node<Data>* Node<Data>::branch_down(char key, Data empty_data)
{
	if (down_node == NULL)
		down_node = new Node<Data>(key, empty_data);
	return down_node;
}


/************************************************************
 * CLASS : Dictionary
 * ----------------------------------------------------------
 * This Dictionary structure uses a tri-node design to
 * insert keys. Upon insertion & lookup of a key, the amount
 * of nodes visited to recover a datum will be at least the
 * byte-length of the key.
 * ----------------------------------------------------------
 * After inserting a handful of keys in the given order, the
 * Dictionary will have the following structure:
 *
 * KEYS:["DIFFUSE","DIFFIE","DIALECT","ROOT","ROBOT","ZEBRA"]
 *
 * HEAD->     [D]_______
 *             |        \
 *            [I]       [R]__
 *             |         |   \
 *     _______[F]     __[O]  [Z]
 *    /        |     /   |    |
 *  [A]       [F]  [B]  [O]  [E]
 *   |         |    |    |    |
 *  [L]     __[U]  [O]  [T]  [B]
 *   |     /   |    |    *    |
 *  [E]  [I]  [S]  [T]       [R]
 *   |    |    |    *         |
 *  [C]  [E]  [E]            [A]
 *   |    *    *              *
 *  [T]
 *   *
 ***********************************************************/
template<class Key, class Data> class Dictionary;
template<class Key, class Data> std::ostream& operator << (std::ostream &, const Dictionary<Key,Data> &);

template<class Key, class Data>
class Dictionary
{
private:
	const Data empty_data; // DEFAULT : default datum specified by user
	Data tmp_empty_data;   // TEMP    : reference is returned for empty keys
	Node<Data> *head;      // HEAD    : top of Dictionary tri-node tree

	Data& NILL() {return tmp_empty_data = empty_data;}

public:
	friend std::ostream& operator << <>(std::ostream&, const Dictionary<Key,Data>&);
	Dictionary(Data d) : head(NULL), empty_data(d) {}
	~Dictionary();
	void destroy_dictionary_tree(Node<Data>*);
	bool is_array(Key key);
	Data& operator[] (Key key);
};


/**
 * FRIEND: Print Dictionary Contents
 */
template<class Key, class Data>
std::ostream& operator << (std::ostream &out, const Dictionary<Key,Data> &dict)
{
	out << "templates are great\n";
	return out;
}


/**
 * Recursively de-allocate all memory for the tri-node Dictionary
 * starting with its top node.
 */
template<class Key, class Data>
void Dictionary<Key,Data> :: destroy_dictionary_tree(Node<Data> *node)
{
	if (node)
	{
		destroy_dictionary_tree(node->left_node);
		destroy_dictionary_tree(node->right_node);
		destroy_dictionary_tree(node->down_node);
		delete(node);
	}
}


/**
 * DESTRUCTOR : Call recursive helper function to free the memory of
 * 				all nodes attached to the head.
 */
template<class Key, class Data>
Dictionary<Key,Data> :: ~Dictionary()
{
	if (head) { destroy_dictionary_tree(head);}
}


/**
 * Check if the type of the parameter is an array. This
 * will fail for pointer types, ie. only type[] values will pass.
 */
template<class Key, class Data>
bool Dictionary<Key,Data> :: is_array(Key key)
{
	return std::is_array<Key>::value;
}


/**
 * Lookup a key and return the data linked to it.
 */
template<class Key, class Data>
Data& Dictionary<Key,Data> :: operator [] (Key key)
{
	int key_len;
	const char *keys;

	std::string str_key = "";
	char *sub_key = NULL;

	try
	{ throw key; }
	catch(void *pointer_key)
	{
		if (pointer_key == NULL)
			return NILL();

		if (is_array(key))
		{
			key_len = sizeof(Key);
			sub_key = (char*)pointer_key;
		}
		else
		{
			key_len = sizeof(void*);
			sub_key = (char*)&pointer_key;
		}
	}
	catch(std::string &e)
	{
		str_key = e;
		key_len = e.length();
		if (key_len <= 0)
			return NILL();
	}
	catch(...)
	{
		key_len = sizeof(Key);
		sub_key = (char*)&key;
	}

	const char *key_s = (sub_key == NULL) ? str_key.c_str() : (const char*)sub_key;

	/* Begin searching at the top-most node */
	if (head == NULL)
		head = new Node<Data>(key_s[0], empty_data);
	Node<Data> *node = head;

	int i = 0;
	while(i < key_len - 1)
	{
		if (key_s[i] == node->key)
		{
			++i;
			if (node->down_node == NULL)
				node->down_node = new Node<Data>(key_s[i], empty_data);
			node = node->down_node;
		}
		else if (key_s[i] < node->key)
		{
			if (node->left_node == NULL)
				node->left_node = new Node<Data>(key_s[i], empty_data);
			node = node->left_node;
		}
		else
		{
			if (node->right_node == NULL)
				node->right_node = new Node<Data>(key_s[i], empty_data);
			node = node->right_node;
		}
	}

	return node->data;
}


#endif /* DICTIONARY_H_ */
