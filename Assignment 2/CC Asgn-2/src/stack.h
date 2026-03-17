#pragma once
#include <iostream>
using namespace std;

template <typename T>
class Snode
{
public:
	T data;
	Snode* next;
	Snode(T d, Snode* n = NULL)
	{
		data = d;
		next = n;
	}
};

template <typename T>
class Stack
{
public:
	Snode<T>* topNode;
	Stack(Snode<T>* t = NULL);

	// Copy Constructor
	Stack(const Stack<T>& other);

	// Copy Assignment Operator
	Stack<T>& operator=(const Stack<T>& other);

	void push(T val);

	void pop();

	T top();

	bool empty() const;

	void print();

	void print(Snode<T>* t);

	~Stack();
};

///////////////////////////////////
//                               //
//           stack.cpp           //
//                               //
///////////////////////////////////

template <typename T>
Stack<T>::Stack(Snode<T>* t)
{
	topNode = t;
}

template <typename T>
void Stack<T>::push(T val)
{
	Snode<T>* newnode = new Snode<T>(val);
	if (topNode == NULL)
		topNode = newnode;
	else
	{
		Snode<T>* temp = topNode;
		topNode = newnode;
		topNode->next = temp;
		temp = NULL;
	}
}

template <typename T>
void Stack<T>::pop()
{
	if (topNode == NULL)
		return;

	Snode<T>* temp = topNode;
	topNode = topNode->next;
	delete temp;
	temp = NULL;
}

template <typename T>
T Stack<T>::top()
{
	return topNode->data;
}

template <typename T>
bool Stack<T>::empty() const
{
	return topNode == NULL;
}

template <typename T>
void Stack<T>::print()
{
	cout << "\nStack:\n";
	cout << "\n|       |\n";
	Snode<T>* temp = topNode;
	while (temp)
	{
		cout << "| " << temp->data << "\t|\n";
		temp = temp->next;
	}
	cout << "|_______|\n";
}

template <typename T>
void Stack<T>::print(Snode<T>* t)
{
	cout << "\nStack:\n";
	cout << "\n|   |\n";
	Snode<T>* temp = topNode;
	while (temp)
	{
		cout << "| " << temp->data->data << " |\n";
		temp = temp->next;
	}
	cout << "|___|\n";
}

template <typename T>
Stack<T>::~Stack()
{
	while (topNode)
	{
		pop();
	}
}

// ---------------------------------------------------------
// Deep Copy Constructor
// ---------------------------------------------------------
template <typename T>
Stack<T>::Stack(const Stack<T>& other)
{
	topNode = NULL;

	// If the other stack is empty, we just leave this one empty
	if (other.topNode == NULL)
		return;

	// Copy the first (top) node
	topNode = new Snode<T>(other.topNode->data);

	// Keep track of where we are in both lists
	Snode<T>* currentSrc = other.topNode->next;
	Snode<T>* currentDest = topNode;

	// Loop through the rest of the source stack and duplicate each node
	while (currentSrc != NULL)
	{
		currentDest->next = new Snode<T>(currentSrc->data);
		currentDest = currentDest->next;
		currentSrc = currentSrc->next;
	}
}

// ---------------------------------------------------------
// Deep Copy Assignment Operator
// ---------------------------------------------------------
template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T>& other)
{
	// 1. Check for self-assignment (e.g., st1 = st1)
	if (this == &other)
		return *this;

	// 2. Clean up any existing memory in the current stack
	while (topNode)
	{
		pop();
	}

	// 3. Copy the data over (exact same logic as Copy Constructor)
	if (other.topNode == NULL)
		return *this;

	topNode = new Snode<T>(other.topNode->data);

	Snode<T>* currentSrc = other.topNode->next;
	Snode<T>* currentDest = topNode;

	while (currentSrc != NULL)
	{
		currentDest->next = new Snode<T>(currentSrc->data);
		currentDest = currentDest->next;
		currentSrc = currentSrc->next;
	}

	return *this; // Return the current object so chaining works (a = b = c)
}