/*I declare that the following source code was written solely by me. 
I understand that copying any source code, in whole or in part, constitutes cheating, 
and that I will receive a zero on this project if I am found in violation of this policy.
*/
#include "Vector.h"
#include <stdexcept>
#include <algorithm>

using namespace std;

Vector::Vector() {
    capacity = CHUNK;
    n_elems = 0;
    data_ptr = new int[capacity];
}

Vector::Vector(const Vector& v) {
    capacity = v.capacity;
    n_elems = v.n_elems;
    data_ptr = new int[capacity];
    copy(v.data_ptr, v.data_ptr + n_elems, data_ptr);
}

Vector& Vector::operator=(const Vector& v) {
    if (this != &v) {
        delete[] data_ptr;
        capacity = v.capacity;
        n_elems = v.n_elems;
        data_ptr = new int[capacity];
        copy(v.data_ptr, v.data_ptr + n_elems, data_ptr);
    }
    return *this;
}

Vector::~Vector() {
    delete[] data_ptr;
}

int Vector::front() const {
    if (n_elems == 0) {
        throw range_error("Vector is empty");
    }
    return data_ptr[0];
}

int Vector::back() const {
    if (n_elems == 0) {
        throw range_error("Vector is empty");
    }
    return data_ptr[n_elems - 1];
}

int Vector::at(size_t pos) const {
    if (pos >= n_elems) {
        throw range_error("Index out of range");
    }
    return data_ptr[pos];
}

size_t Vector::size() const {
    return n_elems;
}

bool Vector::empty() const {
    return n_elems == 0;
}

int& Vector::operator[](size_t pos) {
    return data_ptr[pos];
}

void Vector::push_back(int item) {
    if (n_elems == capacity) {
        grow();
    }
    data_ptr[n_elems++] = item;
}

void Vector::pop_back() {
    if (n_elems == 0) {
        throw range_error("Vector is empty");
    }
    --n_elems;
}

void Vector::erase(size_t pos) {
    if (pos >= n_elems) {
        throw range_error("Index out of range");
    }
    copy(data_ptr + pos + 1, data_ptr + n_elems, data_ptr + pos);
    --n_elems;
}

void Vector::insert(size_t pos, int item) {
    if (n_elems == capacity) {
        grow();
    }
    copy_backward(data_ptr + pos, data_ptr + n_elems, data_ptr + n_elems + 1);
    data_ptr[pos] = item;
    ++n_elems;
}

void Vector::clear() {
    n_elems = 0;
}

int* Vector::begin() {
    return n_elems == 0 ? nullptr : data_ptr;
}

int* Vector::end() {
    return n_elems == 0 ? nullptr : data_ptr + n_elems;
}

bool Vector::operator==(const Vector& v) const {
    if (n_elems != v.n_elems) {
        return false;
    }
    for (size_t i = 0; i < n_elems; ++i) {
        if (data_ptr[i] != v.data_ptr[i]) {
            return false;
        }
    }
    return true;
}

bool Vector::operator!=(const Vector& v) const {
    return !(*this == v);
}

// Private
void Vector::grow() {
    size_t new_capacity = capacity * 1.6;
    int* new_data_ptr = new int[new_capacity];
    copy(data_ptr, data_ptr + n_elems, new_data_ptr);
    delete[] data_ptr;
    data_ptr = new_data_ptr;
    capacity = new_capacity;
}
