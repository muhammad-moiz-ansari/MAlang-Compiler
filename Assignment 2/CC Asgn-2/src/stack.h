#pragma once

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
	Snode<T>* top;
	Stack(Snode<T>* t = NULL);

	void push(T val);

	void pop();

	Snode<T>* peek();

	void print();

	void print(Snode<T>* t);

	~Stack();
};