/*bstt.h*/
 
// Created by Niko Castellana
// Date: 3/2/2020
// Project # 4 
// Threaded binary search tree
//
// The purpose of this program is to create a
// threaded binary search tree that searches for key 
// and value pairs of any data type. The tree is threaded
// on a node's right pointer to another node, and the 
// thread makes searching faster by "walking" up the tree.
// 
 
#pragma once
 
#include <iostream>
#include <algorithm>
 
using namespace std;
 
template<typename KeyT, typename ValueT>
class bstt
{
private:
  struct NODE
  {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    bool   isThreaded;
  };
 
  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  NODE* Current; // pointer to current node for begin and next functions
  int   Size;  // # of nodes in the tree (0 if empty)
	
	/* Traverse through the tree using 
	 * postorder and free the nodes */
	void _clear(NODE* cur)
	{
		if(cur == nullptr) // Tree is empty or tree 
			return;        // has ended so return
		else
		{
			_clear(cur->Left); // Go Left
			
			/* Go Right when it's not threaded */
			if(!cur->isThreaded) 
			{
				_clear(cur->Right);
			}
			
			delete cur; // Free node
		}
	}
	
	
	/* Traverse through the tree using preorder 
	 * and copy the nodes from the original tree */
	void _copy(const NODE* cur)
	{
		if(cur == nullptr) // Tree is empty or tree
			return;        // has ended so return
		else
		{
		
			/* Insert key and value into the node */
			insert(cur->Key, cur->Value);
			_copy(cur->Left); // Go Left
			
			/* Go Right when it's not threaded */	
			if(!cur->isThreaded)
			{
				_copy(cur->Right);
			}
			
		}
	}
	
	
	/* Traverse through the tree using 
	 * inorder and print the nodes */
	void _print(NODE* cur, ostream& output)const
	{
		if(cur == nullptr) // Tree is empty or tree
			return;        // has ended so return
		else
		{
			_print(cur->Left, output); // Go Left
			
			/* Print (Key, Value, Thread) when node is threaded
			 * and there is no node right of the current node.*/
			if(cur->isThreaded && cur->Right != nullptr)
			{
				output << "(" << cur->Key << "," << cur->Value << ","
				<< cur->Right->Key << ")"<< endl;
			}
			else
			{
				/* Print (Key, Value) when node is not 
				 * threaded and go right to the next node */
				output << "(" << cur->Key << "," << cur->Value << ")"<< endl;
				_print(cur->Right, output); // Go Right
			}
				
		}
	}

	/* Returns a pointer to 
	 * the leftmost node */
	NODE* _begin(NODE* cur)
	{
		/* Check for empty tree */
		if(cur == nullptr)
		{
			return nullptr;
		}
		else
		{
			/* Loop until the leftmost node */
			while(cur->Left != nullptr)
			{
			cur = cur->Left; // Go Left
			}
		}
		
		return cur; // Return current node
	}
	

 
 
public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  bstt()
  {
    Root = nullptr;
    Size = 0;
  }
 
  //
  // copy constructor
  //
  bstt(const bstt& other)
  {
	Root = nullptr; 
	clear();
	_copy(other.Root);
	this->Size = other.Size;
  }
 
  //
  // destructor:
  //
  // Called automatically by system when tree is about to be destroyed;
  // this is our last chance to free any resources / memory used by
  // this tree.
  //
  virtual ~bstt()
  {
	clear();
  }
 
  //
  // operator=
  //
  // Clears "this" tree and then makes a copy of the "other" tree.
  //
  bstt& operator=(const bstt& other)
  {
     clear();
	_copy(other.Root);
	 
    return *this;
  }
 
  //
  // clear:
  //
  // Clears the contents of the tree, resetting the tree to empty.
  //
  void clear()
  {
	  
    _clear(Root);
	Root = nullptr;
	Size = 0;
	
  }
 
  // 
  // size:
  //
  // Returns the # of nodes in the tree, 0 if empty.
  //
  // Time complexity:  O(1) 
  //
  int size() const
  {
    return Size;
  }
 
  // 
  // search:
  //
  // Searches the tree for the given key, returning true if found
  // and false if not.  If the key is found, the corresponding value
  // is returned via the reference parameter.
  //
  // Time complexity:  O(lgN) on average
  //
  bool search(KeyT key, ValueT& value) const
  {
    NODE* cur = Root; // Current Node

	/* Loop through the tree */
    while (cur != nullptr)
	{
      if (key == cur->Key) 
	  {
		value = cur->Value; // Update value
		return true; // key and value pair found
	  }
		
	  /* Check if key is less than current key */
      if (key < cur->Key)  
      {
        cur = cur->Left; // Move left
      }
	  /* Check if key is greater than current key */
      else
      {
		if(cur->isThreaded)
		{
			cur = nullptr;
		}
		else
		{
			cur = cur->Right; // Move Right
		}
		 
      }
	} 
  
    // key and value pair not found
    return false;
  }
 
  //
  // insert
  //
  // Inserts the given key into the tree; if the key has already been insert then
  // the function returns without changing the tree.
  //
  // Time complexity:  O(lgN) on average
  //
  void insert(KeyT key, ValueT value)
  {
    NODE* prev = nullptr; // Previous Node
    NODE* cur = Root; // Current Node

    /* Search to see if tree already contains key */
    while (cur != nullptr)
    {
      if (key == cur->Key)  // Key already in tree
        return;

      if (key < cur->Key)  // Search left
      {
        prev = cur;
        cur = cur->Left;
      }
      else // Search right
      {
		  if(cur->isThreaded)
		  {
			  prev = cur;
			  cur = nullptr;
		  }
		  else // Not Threaded
		  {
			  cur = cur->Right; // Move right
		  }
       }
    }//while

	/* Key is not in tree, so a new 
	 * node is allocated to insert */
      NODE* newNode = new NODE();
	  newNode->Key = key;
	  newNode->Value = value;
      newNode->Left = nullptr;
      newNode->Right = nullptr;
	  newNode->isThreaded = true;
	  
    //
    // NOTE: cur is null, and prev denotes node where
    // we fell out of the tree.  if prev is null, then
    // the tree is empty and the Root pointer needs 
    // to be updated.
    //

      if(prev == nullptr)
         Root = newNode;
      else if (key < prev->Key)
	  {
		  prev->Left = newNode; // Insert new node to the left of the previous
		  newNode->Right = prev; // Point new node's right pointer 
								 // to the node on the right
	  }
      else
	  {   
		  newNode->Right = prev->Right;
		  prev->isThreaded = false; 
		  prev->Right = newNode; // Insert new node to the right of the previous
	  }
         
      Size++; // Update Size
  }
 
  //
  // []
  //
  // Returns the value for the given key; if the key is not found,
  // the default value ValueT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  ValueT operator[](KeyT key) const
  {
	  ValueT value; // Declare local value to call search and update
	  
    if(search(key,value))
	{
		return value; 
	}
	else
	{
		return ValueT{ };
	}
    
  }
 
  //
  // ()
  //
  // Finds the key in the tree, and returns the key to the "right".
  // If the right is threaded, this will be the next inorder key.
  // if the right is not threaded, it will be the key of whatever
  // node is immediately to the right.
  //
  // If no such key exists, or there is no key to the "right", the
  // default key value KeyT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  KeyT operator()(KeyT key) const
  {
    NODE* cur = Root;
	
	/* Search through tree */
	while(cur != nullptr)
	{
		/* Key found, return right node's key */
		if(key == cur->Key && cur->Right != nullptr)
		{
			return cur->Right->Key;
		}
		
		if(key < cur->Key)
		{
			cur = cur->Left; // Go Left
		}
		else
		{
			if (cur->isThreaded)
			{
				cur = nullptr;
			}
			else
			{
				cur = cur->Right; // Go Right
			}
			
		}
	}
	
	 return KeyT{ }; // Key not found
  }
 
  //
  // begin
  //
  // Resets internal state for an inorder traversal.  After the 
  // call to begin(), the internal state denotes the first inorder
  // key; this ensure that first call to next() function returns
  // the first inorder key.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  void begin()
  {
	Current = _begin(Root); // Initialize Current
							// to leftmost node
  }
 
  //
  // next
  //
  // Uses the internal state to return the next inorder key, and 
  // then advances the internal state in anticipation of future
  // calls.  If a key is in fact returned (via the reference 
  // parameter), true is also returned.
  //
  // False is returned when the internal state has reached null,
  // meaning no more keys are available.  This is the end of the
  // inorder traversal.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  bool next(KeyT& key)
  {
    
	if(Current != nullptr)
	{
	    if(Current->isThreaded)
		{
			key = Current->Key; // Update key
			Current = Current->Right; // Go right
			return true;
		}
		
	    if(!Current->isThreaded)
		{
			key = Current->Key; // Update key
			
			/* Get leftmost node of the
			 * current node's right node */
			Current = _begin(Current->Right);
			return true;
		}
	}
	 
	 return false;
  }
 
  //
  // dump
  // 
  // Dumps the contents of the tree to the output stream, using a
  // recursive inorder traversal.
  //
  void dump(ostream& output) const
  {
    output << "**************************************************" << endl;
    output << "********************* BSTT ***********************" << endl;
 
    output << "** size: " << this->size() << endl;
 
    //
    // inorder traversal, with one output per line: either 
    // (key,value) or (key,value,THREAD)
    //
    // (key,value) if the node is not threaded OR thread==nullptr
    // (key,value,THREAD) if the node is threaded and THREAD denotes the next inorder key
    //
 
	_print(Root, output); // Print tree
 
    output << "**************************************************" << endl;
  }
     
};