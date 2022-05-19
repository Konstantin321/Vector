#include <iostream>
#include "Vector.h"

/*
Value* _data = nullptr;
    size_t _size = 0;
    size_t _capacity = 0;
    float _multiplicativeCoef = 2.0f;
*/

void Vector::_updateCapacity(size_t capacity)
{
    if (capacity < _size)
    {
        capacity = _size;
    }
    
    _capacity = capacity;
    Value* newData = new Value[_capacity];

    if (_data)
    {
        for (size_t i = 0; i < _size; i++)
        {
            newData[i] = _data[i];
        }
        delete[] _data;
    }
    _data = newData;
}


Vector::Vector(const Value* rawArray, const size_t size, float coef): _size(size), _multiplicativeCoef(coef), _capacity(size)
{
    _data = new Value[_capacity];
    for (int i = 0; i < size; i++)
    {
        _data[i] = rawArray[i];
    }
}

// конструктор копирования
Vector::Vector(const Vector& other):
    _size(other._size),
    _capacity(other._size),
    _multiplicativeCoef(other._multiplicativeCoef)
{
    _data = new Value[_capacity];
    for (size_t i = 0; i < other._size; ++i) {
        _data[i] = other._data[i];
    }
}

// оператор присваивания копированием 
Vector& Vector::operator=(const Vector& other) 
{
    if (&other != this)
    {
        _updateCapacity(other._size);
        _size = other._size;
        _capacity = other._size;
        _multiplicativeCoef = other._multiplicativeCoef;

        for (size_t i = 0; i < other._size; i++)
        {
            _data[i] = other._data[i];
        }
    }
    return *this;
}

// конструктор перемещения
Vector::Vector(Vector&& other) noexcept:
    _data(other._data),
    _size(other._size),
    _capacity(other._size),
    _multiplicativeCoef(other._multiplicativeCoef)
{
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
    other._multiplicativeCoef = 0;
}

// оператор присваивания перемещением
Vector& Vector::operator=(Vector&& other) noexcept
{
    if (&other != this)
    {
        _data = other._data;
        _size = other._size;
        _capacity = other._size;
        _multiplicativeCoef = other._multiplicativeCoef;
        other._data = nullptr; 
        other._size = 0;
        other._capacity = 0;
        other._multiplicativeCoef = 0;
    }
    return *this;
}

Vector::~Vector()
{
    delete[] _data;
}

void Vector::pushBack(const Value& value)
{
    if (_size == _capacity)
    {
        _updateCapacity(_capacity * _multiplicativeCoef);
    }
    insert(value, _size);
}

void Vector::pushFront(const Value& value)
{
    if (_size == _capacity)
    {
        _updateCapacity(_capacity * _multiplicativeCoef);
    }
    insert(value, 0);
}


void Vector::insert(const Value& value, size_t pos)
{
    if (_capacity == 0)
    {
        _updateCapacity(1 * size_t(_multiplicativeCoef) );
    }
    else if (_size == _capacity)
    {
        _updateCapacity(_capacity * size_t(_multiplicativeCoef));
    }

    for (size_t i = _size; i > pos; i--)
    {
        _data[i] = _data[i - 1];
    }
    _data[pos] = value;
    _size++;
}

void Vector::insert(const Value* values, size_t size, size_t pos)
{
    while ( (_size + size) > _capacity)
    {
        _updateCapacity(_capacity * size_t(_multiplicativeCoef));
    }

    for (size_t i = _size + size, j = _size; i > pos; i--, j--)
    {
        _data[i] = _data[j];
    }

    size_t i = pos;
    for (size_t j = 0; j < size; j++)
    {
        _data[i] = values[j];
        i++;
    }
    
    _size += size;
}

void Vector::insert(const Vector& vector, size_t pos)
{
    insert(vector._data, vector._size, pos);
}

void Vector::popBack()
{
    erase(_size - 1, 1);
}

void Vector::popFront()
{
    erase(0, 1);
}

void Vector::erase(size_t pos, size_t count)
{
    if (_size == 0) {
        throw std::out_of_range("_size = 0");
    }
    
    size_t size = _size;
    
    if (pos + count >= size) 
    {
        _size -= _size - pos;
    }
    else 
    {
        for (size_t i = pos; i < size - count; ++i) 
        {
            _data[i] = _data[i + count];
        }
        _size -= count;
    }
}

void Vector::eraseBetween(size_t beginPos, size_t endPos)
{
    erase(beginPos, endPos - beginPos); // +1?
}


size_t Vector::size() const
{
    return _size;
}

size_t Vector::capacity() const
{
    return _capacity;
}

double Vector::loadFactor() const
{
    return (double)_size / (double)_capacity;
}

Value& Vector::operator[](size_t idx)
{
    return _data[idx];
}

const Value& Vector::operator[](size_t idx) const
{
    return _data[idx];
}

long long Vector::find(const Value& value) const
{
    for (long long i = 0; i < _size; i++)
    {
        if (_data[i] == value)
        {
            return i;
        }
    }
    return -1;
}

void Vector::reserve(size_t capacity)
{
    if (capacity <= _capacity)
    {
        return;
    }
    _updateCapacity(capacity);
}

void Vector::shrinkToFit()
{
    _updateCapacity(0);
}

Vector::Iterator::Iterator(Value* ptr)
{
    _ptr = ptr;
}

Value& Vector::Iterator::operator*()
{
    return *_ptr;
}

const Value& Vector::Iterator::operator*() const
{
    return *_ptr;
}

Value* Vector::Iterator::operator->()
{
    return _ptr;
}

const Value* Vector::Iterator::operator->() const
{
    return _ptr;
}

Vector::Iterator Vector::Iterator::operator++()
{
    ++_ptr;
    return *this;
}

Vector::Iterator Vector::Iterator::operator++(int)
{
    return Vector::Iterator(_ptr++);
}

bool Vector::Iterator::operator==(const Iterator& other) const
{
    return _ptr == other._ptr;
}

bool Vector::Iterator::operator!=(const Iterator& other) const
{
    return _ptr != other._ptr;
}

Vector::Iterator Vector::begin()
{
    return Vector::Iterator(_data);
}

Vector::Iterator Vector::end()
{
    return Vector::Iterator(_data + _size);
}


//______________
void Vector::print()
{
    if (_data)
    {
        std::cout << "Size: " << _size << "\nCapacity: " << _capacity << "\nCoef: " << _multiplicativeCoef << "\nArr:\n";
        for(int i = 0; i < _size; i++)
        {
            std::cout << _data[i] << " ";
        }
        std::cout << "\n";
    }
    else
    {
        std::cout << "nullptr\n";
    }
}