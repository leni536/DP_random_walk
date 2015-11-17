#ifndef BUFFER_H
#define BUFFER_H
#include<vector>
#include<stdexcept>

template<typename T>
class buffer{
	private:
	std::vector<T> container;
	size_t eff_size;
	size_t size;

	public:
	buffer(size_t size);
	T& operator[](size_t idx);
	virtual void push(const T&);
	inline size_t get_size() { return size; }
	inline size_t get_eff_size() { return eff_size; }
};

template<typename T>
buffer<T>::buffer(size_t size): container(size), size(size), eff_size(0)
{
}

template<typename T>
T& buffer<T>::operator[](size_t idx)
{
	if ( idx >= eff_size )
		throw std::out_of_range("Buffer over-index");
	if ( eff_size >= size )
		if ( idx < eff_size-size )
			throw std::out_of_range("Buffer under-index");
	if ( idx < 0 )
		throw std::out_of_range("Buffer under-index");

	return container[idx % size];
}

template<typename T>
void buffer<T>::push(const T& item)
{
	eff_size++;
	this->operator[](eff_size-1)=item;
}

#endif
