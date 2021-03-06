#include <vector>
#include <stdexcept>
#include <iostream>
#include <initializer_list>
#include <new>
#include <memory>
using std::vector;
using std::make_shared;
using std::cout;
using std::endl;
template <typename> class BlobPtr;
template <typename T> class Blob {
	friend class BlobPtr<T>;
public:
	typedef T value_type;
	typedef typename vector<T>::size_type size_type;
	Blob() try : data(std::make_shared<vector<T>>()) {}
		catch(const bad_alloc &e) {handle_out_of_memory(e);}
	Blob(std::initializer_list<T> il) try : data(std::make_shared<vector<T>>(il)) {}
		catch(const bad_alloc &e) {handle_out_of_memory(e);}
	Blob(const Blob &b): data(make_shared<vector<T>>(*b.data)) {}
	Blob(Blob &&b) noexcept : data(std::move(b.data)) {b.data = nullptr;}
	Blob& operator=(const Blob &rhs) {
		data = make_shared<vector<T>>(*rhs.data);
		return *this;
	}
	Blob& operator=(Blob &&rhs) noexcept {
		if (*this != rhs) {
			data = std::move(rhs.data);
			rhs.data = nullptr;
		}
		return *this;
	}
	BlobPtr<T> begin() {return BlobPtr<T>(*this);}
	BlobPtr<T> end() {return BlobPtr<T>(*this, data->size());}
	size_type size() const {return data->size();}
	bool empty() const {return data->empty();}
	void push_back(const T &t) {data->push_back(t);}
	void push_back(T &&t) {data->push_back(std::move(t));}
	void pop_back();
	T& back();
	const T& back() const;
	T& operator[](size_type i);
	const T& operator[](size_type i) const;
	T& front();
	T& front() const;
private:
	void check(size_type i, const std::string &msg) const;
	std::shared_ptr<vector<T>> data;

};
template <typename T> inline T& Blob<T>::front() {
	check(0, "front on empty Blob");
	return data->front();
}

template <typename T> inline void Blob<T>::pop_back() {
	check(0, "pop_back on empty Blob");
	data->pop_back();
}
template <typename T> inline T& Blob<T>::back() {
	check(0, "back on empty Blob");
	return data->back();
}
template <typename T> inline const T& Blob<T>::back() const {
	check(0, "back on empty Blob");
	return data->back();
}
template <typename T> inline T& Blob<T>::operator[](size_type i) {
	check(i, "subscript out of range");
	return (*data)[i];
}
template <typename T> inline const T& Blob<T>::operator[](size_type i) const {
	check(i, "subscript out of range");
	return (*data)[i];
}

template <typename T> inline void Blob<T>::check(size_type i, const std::string &msg) const {
	if (i >= data->size())
		throw std::out_of_range(msg);
}
template <typename T> bool operator!=(const BlobPtr<T> lhs, const BlobPtr<T> rhs);
template <typename T>
class BlobPtr {
	friend bool operator!=<T>(const BlobPtr<T> lhs, const BlobPtr<T> rhs);
public:
	BlobPtr() try : curr(0) {}
		catch(const bad_alloc &e) {handle_out_of_memory(e);}
	BlobPtr(Blob<T> &a, size_t sz = 0) try : wptr(a.data), curr(sz) {}
		catch(const bad_alloc &e) {handle_out_of_memory(e);}
	T& operator*() const {
		auto p = check(curr, "dereference past end");
		return (*p)[curr];
	}
	BlobPtr& operator++();
	BlobPtr operator++(int);
	BlobPtr& operator--();
	BlobPtr operator--(int);
private:
	std::shared_ptr<vector<T>> check(std::size_t i, const std::string &msg) const;
	std::weak_ptr<vector<T>> wptr;
	std::size_t curr;
};
template <typename T>
bool operator!=(const BlobPtr<T> lhs, const BlobPtr<T> rhs) {
	return lhs.curr != rhs.curr;
}
template <typename T> 
inline std::shared_ptr<vector<T>> BlobPtr<T>::check(std::size_t i, const std::string &msg) const {
	auto p = wptr.lock();
	if (!p)
		throw std::runtime_error("unbound BlobPtr");
	if (i >= p->size())
		throw std::out_of_range(msg);
	return p;
}
template <typename T>  BlobPtr<T>& BlobPtr<T>::operator++() {
	check(curr, "increment past end of BlobPtr");
	++curr;
	return *this;
}
template <typename T>  BlobPtr<T>& BlobPtr<T>::operator++(int) {
	BlobPtr b = *this;
	++*this;
	return b;
}
template <typename T>  BlobPtr<T>& BlobPtr<T>::operator--() {
	check(curr, "decrement past begin of BlobPtr");
	--curr;
	return *this;
}
template <typename T>  BlobPtr<T>& BlobPtr<T>::operator--(int) {
	BlobPtr b = *this;
	--*this;
	return b;
}
int main()
{
	Blob<int> b = {1,2,3,4,5,6,7};
	for (BlobPtr<int> beg = b.begin(), end = b.end(); beg != end; ++beg) 
		cout << *beg << endl;
	return 0;
}