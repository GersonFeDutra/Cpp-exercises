// modules support is not complete
// module; // this compilation will define a module
//         // ... here we put stuff that Queue might need for its implementation ...
#include "queue.hpp"
#include <stdexcept>

//export module Queue; // defining the module called "Queue"
//export class Queue ...

Queue::Queue(int s) : elem{new double[s]}, sz{s}
{
	if (s < 0)
		throw std::length_error{"Queue constructor: negative size"};

	for (int i = 0; i != s; ++i) // initialize elements
		elem[i] = 0;
}

Queue::Queue(const Queue& a)        // copy constructor
	: elem{new double[a.sz]}, sz{a.sz} // allocate space for elements
{
	for (int i = 0; i != sz; i++)      // copy elements
		elem[i] = a.elem[i];
}

Queue& Queue::operator=(const Queue& a) // copy assignment
{
	double *p = new double[a.sz];
	for (int i = 0; i != a.sz; i++)
		p[i] = a.elem[i];

	delete[] elem; // delete old elements
	elem = p;
	sz = a.sz;

	return *this;
}


Queue::Queue(Queue&& a)
	: elem{a.elem},   // "grab the elements" from a
	sz{a.sz}
{
	a.elem = nullptr; // now a has no elements
	a.sz = 0;
}


Queue& Queue::operator=(Queue&& a)
{
	delete[] elem;
	elem = a.elem;
	sz = a.sz;
	a.elem = nullptr;
	a.sz = 0;

	return *this;
}


int Queue::size() const
{
	return sz;
}


Queue::Queue(std::initializer_list<double> lst)
	: elem{new double[lst.size()]}, sz{static_cast<int>(lst.size())}
{
	std::copy(lst.begin(), lst.end(), elem); // copy from lst into elem (§12.6)
}


void Queue::push_back(double v)
{
	double *new_elem = new double[sz + 1];
	//double *p, *q;
	//for (p = new_elem, q = elem; p < p + sz; p++, q++)
		//*p = *q;
	//*p = v;
	std::move(elem, elem + sz, new_elem);
	delete[] elem;
	elem = new_elem;

	*(elem + sz) = v;
	sz++;
}


double Queue::pop_front(void)
{
	if (elem == nullptr)
		throw new Queue::Empty_error("Queue is empty, can't pop front!");

	double value = *elem;

	if (sz - 1 > 0) {
		double *new_elem = new double [sz - 1];

		std::move(elem + 1, elem + sz, new_elem);
		delete[] elem;
		elem = new_elem;
	}
	else {
		elem = nullptr;
	}

	return value;
}


//std::ostream& operator<<(std::ostream& stream, const Queue& q)
std::ostream& operator<<(std::ostream& stream, Queue& q)
{
	stream << "<- ";
	if (q.is_empty()) {
		stream << "o";
	}
	else {
		// TODO -> const iterator
		//for (const auto i : q)
		for (auto i : q) {
			stream << i << " ";
		}
	}

	return stream;
}


std::istream& operator>>(std::istream& stream, Queue& q)
{
	double v;
	stream >> v;
	q.push_back(v);

	return stream;
}
