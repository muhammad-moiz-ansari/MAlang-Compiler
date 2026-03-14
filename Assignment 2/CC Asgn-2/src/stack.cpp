#include <iostream>
#include "stack.h"
using namespace std;

template <typename T>
Stack<T>::Stack(Snode<T>* t)
{
	top = t;
}

template <typename T>
void Stack<T>::push(T val)
{
	Snode<T>* newnode = new Snode<T>(val);
	if (top == NULL)
		top = newnode;
	else
	{
		Snode<T>* temp = top;
		top = newnode;
		top->next = temp;
		temp = NULL;
	}
}

template <typename T>
void Stack<T>::pop()
{
	if (top == NULL)
		return;

	Snode<T>* temp = top;
	top = top->next;
	delete temp;
	temp = NULL;
}

template <typename T>
Snode<T>* Stack<T>::peek()
{
	return top;
}

template <typename T>
void Stack<T>::print()
{
	cout << "\nStack:\n";
	cout << "\n|       |\n";
	Snode<T>* temp = top;
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
	Snode<T>* temp = top;
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
	while (top)
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