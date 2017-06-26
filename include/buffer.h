#ifndef BUFFER_H
#define BUFFER_H
#include<vector>
#include<stdexcept>

/**
 * \brief A circular buffer template class.
 * 
 * This class acts like std::vector, except it "forgets" pushed
 * elements that no longor fit inside.
 */
template<typename T>
class buffer{
	private:
	std::vector<T> container;
	size_t eff_size;
	size_t size;

	public:

	/**
	 * \brief Constructor.
	 *
	 * \param size The allocated size of the buffer.
	 */
	buffer(size_t size);

	/**
	 * \brief Access specified element.
	 *
	 * \param idx Index of the given element.
	 * \returns The specified element.
	 * \throws std::out_of_range
	 *
	 *
	 * The maximal idx available increases for every push() operation.
	 * After the buffer gets full, then the minimal available index increases as well.
	 * If idx is not in legal range it throws std::out_of_range.
	 */
	T& operator[](size_t idx);

	/**
	 * \brief Pushes an element.
	 *
	 * \param value The pushed element.
	 *
	 * The pushed element will be available at object[get_eff_size()-1].
	 */
	virtual void push(const T& value);

	/**
	 * \brief Gets the allocated size of the buffer.
	 */
	inline size_t get_size() { return size; }

	/**
	 * \brief Gets the apparent size of the buffer.
	 *
	 * The apparent size increases every time an element is pushed.
	 */
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

	return container[idx % size];
}

template<typename T>
void buffer<T>::push(const T& item)
{
	eff_size++;
	this->operator[](eff_size-1)=item;
}

#endif
