#include<cstddef>
#include<new>
#include<utility>
#include<iostream>
template<typename T = int>
class Vector {
private:
    size_t sz = 0, cap = 0;
    unsigned char* data = nullptr;

public:
    Vector() noexcept {
        sz = 0;
        cap = 0;
        data = nullptr;
    }
    Vector(int in) noexcept {
        sz = 0;
        cap = in;
        data = new unsigned char[sizeof(T) * cap];
    }
    Vector(const Vector& other) {
        sz = other.sz;
        cap = other.cap;
        data = new unsigned char[sizeof(T) * sz];
        try {
            for (size_t i = 0; i < sz; ++i)
                new(data + sizeof(T) * i) T(*(reinterpret_cast<T*>(other.data + sizeof(T) * i)));
        } catch (...) {
            delete[] data;
            sz = 0;
            cap = 0;
            data = nullptr;
        }
    }
    ~Vector() noexcept {
        for (size_t i = 0; i < sz; ++i)
            reinterpret_cast<T*>(data + sizeof(T) * i)->~T();
        delete[] data;
    }
    Vector& operator=(const Vector& other) {
        unsigned char* tmp = new unsigned char[sizeof(T) * other.sz];
        try {
            for (size_t i = 0; i < other.sz; ++i)
                new(tmp + sizeof(T) * i) T(*(reinterpret_cast<T*>(other.data + sizeof(T) * i)));
        } catch (...) {
            delete[] tmp;
            return (*this);
        }
        delete[] data;
        sz = other.sz;
        cap = other.cap;
        data = tmp;
        return (*this);
    }
    void reserve(size_t num) {
        unsigned char* tmp = new unsigned char[sizeof(T) * num];
        try {
            for (size_t i = 0; i < sz; ++i) {
                new(tmp + sizeof(T) * i) T(*(reinterpret_cast<T*>(data + sizeof(T) * i)));
            }
        } catch (...) {
            delete[] tmp;
            return;
        }
        cap = num;
        for (size_t i = 0; i < sz; ++i)
            reinterpret_cast<T*>(data + sizeof(T) * i)->~T();
        delete[] data;
        data = tmp;
    }
    void clear() noexcept {
        for (size_t i = 0; i < sz; ++i)
            reinterpret_cast<T*>(data + sizeof(T) * i)->~T();
        delete[] data;
        data = nullptr;
        cap = 0;
        sz = 0;
    }
    void swap(Vector& other) noexcept {
        std::swap(sz, other.sz);
        std::swap(cap, other.cap);
        std::swap(data, other.data);
    }
    size_t size() const noexcept {
        return sz;
    }
    T& operator[](int i) noexcept {
        return *(reinterpret_cast<T*>(data + i * sizeof(T)));
    }
    size_t capacity() const noexcept {
        return cap;
    }
    const T& operator[](int i) const noexcept {
        return *(reinterpret_cast<const T*>(data + i * sizeof(T)));
    }
    void resize(int new_s) {
        if (new_s == sz)
            return;
        if (new_s < sz) {
            while (sz > new_s) {
                reinterpret_cast<T*>(data + sizeof(T) * (sz - 1))->~T();
                --sz;
            }
        } else if (new_s > cap) {
            unsigned char* tmp = new unsigned char[sizeof(T) * new_s];
            try {
                for (size_t i = 0; i < sz; ++i)
                    new(tmp + sizeof(T) * i) T((*this)[i]);
                for (size_t i = sz; i < new_s; ++i)
                    new(tmp + sizeof(T) * i) T();
            } catch (...) {
                delete[] tmp;
                return;
            }
            clear();
            sz = new_s;
            cap = new_s;
            data = tmp;
        } else {
            try {
                for (size_t i = sz; i < new_s; ++i)
                    new(data + sizeof(T) * i) T();
            } catch(...) {
                return;
            }
            sz = new_s;
        }
    }
    void pop_front() {
        for (int i = 0; i + 1 < sz; ++i)
            (*this)[i] = (*this)[i + 1];
        --sz;
        resize(sz);
    }
    void push_front(const T& oth) {
        ++sz;
        resize(sz);
        for (int i = sz - 1; i > 0; --i)
            (*this)[i] = (*this)[i - 1];
        (*this)[0] = oth;
    }
    void resize(int new_s, const T& val) {
        if (new_s == sz)
            return;
        if (new_s < sz) {
            while (sz > new_s) {
                reinterpret_cast<T*>(data + sizeof(T) * (sz - 1))->~T();
                --sz;
            }
        } else if (new_s > cap) {
            unsigned char* tmp = new unsigned char[sizeof(T) * new_s];
            try {
                for (size_t i = 0; i < sz; ++i)
                    new(tmp + sizeof(T) * i) T((*this)[i]);
                for (size_t i = sz; i < new_s; ++i)
                    new(tmp + sizeof(T) * i) T(val);
            } catch (...) {
                delete[] tmp;
                return;
            }
            clear();
            sz = new_s;
            cap = new_s;
            data = tmp;
        } else {
            try {
                for (size_t i = sz; i < new_s; ++i)
                    new(data + sizeof(T) * i) T(val);
            } catch(...) {
                return;
            }
            sz = new_s;
        }
    }
    T* begin() {
        return reinterpret_cast<T*>(data);
    }
    const T* begin() const {
        return reinterpret_cast<const T*>(data);
    }
    T* end() {
        return reinterpret_cast<T*>(data + sizeof(T) * sz);
    }
    const T* end() const {
        return reinterpret_cast<const T*>(data + sizeof(T) * sz);
    }
    void push_back(const T& in) {
        if (cap == 0) {
            try {
                reserve(1);
            } catch(...) {
                return;
            }
        } else if (sz == cap) {
            try {
                reserve(2 * cap);
            } catch (...) {
                return;
            }
        }
        try {
            new(data + sz * sizeof(T)) T(in);
            ++sz;
        }
        catch (...) {
            return;
        }
        return;
    }
    void push_back(T&& in) {
        if (cap == 0) {
            try {
                reserve(1);
            } catch(...) {
                return;
            }
        } else if (sz == cap) {
            try {
                reserve(2 * cap);
            } catch (...) {
                return;
            }
        }
        try {
            new(data + sz * sizeof(T)) T(std::move(in));
            ++sz;
        }
        catch (...) {
            return;
        }
        return;
    }
    void pop_back() noexcept {
        reinterpret_cast<T*>(data + sizeof(T) * (sz - 1))->~T();
        --sz;
    }
};
