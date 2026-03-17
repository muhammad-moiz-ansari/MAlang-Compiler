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
//
//int main()
//{
//	Stack<int> st;
//
//	st.push(12);
//	st.push(24534);
//	st.push(43);
//	st.push(4354);
//	st.push(534);
//	st.print();
//
//	cout << st.peek()->data << endl;
//	st.pop();
//	cout << st.peek()->data << endl;
//	st.pop();
//	cout << st.peek()->data << endl;
//	st.pop();
//
//	st.print();
//
//	Stack<Snode<char>*> stn;
//
//	stn.push(new Snode<char>('z'));
//	stn.push(new Snode<char>('z'));
//	stn.push(new Snode<char>('i'));
//	stn.push(new Snode<char>('o'));
//	stn.push(new Snode<char>('M'));
//	stn.print(stn.peek());
//
//	cout << stn.peek()->data->data << endl;
//	stn.pop();
//	cout << stn.peek()->data->data << endl;
//	stn.pop();
//	cout << stn.peek()->data->data << endl;
//	stn.pop();
//
//	stn.print(stn.peek());
//}