#pragma once
#include <initializer_list>
#include <iostream>

#ifdef DEBUG
#endif

class Queue
{
public:
	class Empty_error : public std::length_error {
	public:
		Empty_error(const char* message) : std::length_error(message) {}
	};
	class Iterator {
	public:
		double *p;
		
		Iterator& operator++() { p++; return *this; }
		Iterator operator++(int)
		{
			Iterator it = *this;
			++(*this);
			p++;
			return it;
		}
		//double* operator->() { return p; }
		double& operator*() { return *p; }
		bool operator==(const Iterator& other) const { return other.p == p; }
		bool operator!=(const Iterator& other) const { return !(other == *this); }
		bool operator<(const Iterator& other) const { return p < other.p; }
		bool operator>(const Iterator& other) const { return p > other.p; }
		bool operator<=(const Iterator& other) const { return *this < other || *this == other; }
		bool operator>=(const Iterator& other) const { return *this > other || *this == other; }
	};
public:
	Queue(std::initializer_list<double>); // initialize with a list of doubles

	void push_back(double); // add element at end, increasing the size by one
	                        // ...
	double pop_front(void); //  remove element at end, decreasing the size by one

	Queue() {}                  // default constructor
	explicit Queue(int s);      // constructor: acquire resources

#ifdef DEBUG
	~Queue()
	{
		std::cerr << sz << " deleted\n";
		delete[] elem;
	}
#else
	~Queue() { delete[] elem; } // destructor: release resources
#endif

	Queue(const Queue&);            // copy constructor
	Queue& operator=(const Queue&); // copy assignment

	Queue(Queue&&);            // move constructor
	Queue& operator=(Queue&&); // move assignment

	int size() const;

	Iterator begin() { return {elem}; }
	Iterator end() { return {elem + sz}; }

	bool is_empty() const { return elem == nullptr; }

	//friend std::ostream& operator<<(std::ostream& stream, const Queue& q);
	friend std::ostream& operator<<(std::ostream& stream, Queue& q);
	//friend std::istream& operator>>(std::istream& stream, const Queue& q);
	friend std::istream& operator>>(std::istream& stream, Queue& q);

private:
	double *elem = nullptr; // elem points to an array of sz doubles
	int sz = 0;
};
