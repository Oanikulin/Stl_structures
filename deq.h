#include"list.h"
#include"vector.h"
#include<bits/stdc++.h>

using std::shared_ptr;
using std::vector;
template<typename T>
class double_vector {
private:
    int sz;
    int beg;
    int en;
    vector<T> data;

public:

    double_vector() {
        sz = 0;
        beg = 1;
        en = 1;
        data.resize(1);
    }
    void increase() {
       // std::cout << "increase " << 2 * sz << std::endl;
        vector<T> ndata(2 * sz);
        int j = 0;
        int cnt = 0;
        for (int i = beg; cnt < sz; i = (i + 1) % data.size()) {
            ndata[j] = data[i];
           // std::cout << beg << " " << en << " " << i << " " << *data[i] << std::endl;
            ++j;
           ++cnt;
        }
        beg = 0;
        en = j;
        swap(data, ndata);
       // std::cout << "increaed " << std::endl;
    }
    void push_front(const T& cur) {
        ++sz;
        beg = (beg - 1 + data.size()) % data.size();
        data[beg] = cur;
        if (sz == data.size()) {
            increase();
            return;
        }
    }
    void push_back(const T& cur) {
        if (sz == 0) {
            push_front(cur);
            return;
        }
        ++sz;
        data[en] = cur;
        if (sz == data.size()) {
            increase();
            return;
        }
        en = (en + 1) % data.size();
    }
    void pop_back() {
        --sz;
        en = (en - 1 + data.size()) % data.size();
    }
    void pop_front() {
        --sz;
        beg = (beg + 1) % data.size();
    }
    T& get(int in) {
        //std::cerr<< beg << " " << en << " " << data[(beg + in + data.size()) % data.size()]<< " " << (beg + in + data.size()) % data.size() << " " << data.size() << std::endl;
        return data[(beg + in + data.size()) % data.size()];
    }
    int get_beg() {
        return beg;
    }
    int get_s() {
        return sz;
    }
    int get_en() {
        return en;
    }
};

template<typename T>
class Deque { // why to make it constant though
private:
    double_vector<shared_ptr<T>> us;
    List<T> elem;
    int sz;
public:

    int size() const noexcept {
        return sz;
    }
    class vc_it {
        private:
            int in;
            Deque<T>* deq;

        public:
            void print() {
                std::cout << in << std::endl;
                std::cout <<(*deq)[in] << std::endl;
            }
            vc_it (int val, Deque* decc): in(val), deq(decc) {
            }
            bool operator == (const vc_it& other) const {
                return (in == other.in);
            }
            bool operator != (const vc_it& other) const {
                return !((*this) == other);
            }
            T& operator* () {
                return (*deq)[in];
            }
            vc_it& operator++() {
                ++in;
                return (*this);
            }
            vc_it& operator--() {
                --in;
                return (*this);
            }
            vc_it operator++(int) {
                vc_it tmp = vc_it(in, deq);
                ++(*this);
                return tmp;
            }
            vc_it operator--(int) {
                vc_it tmp = vc_it(in, deq);
                --(*this);
                return tmp;
            }
    };

    Deque(): us(), elem(), sz(0) {
    }
    Deque(T oth) {
        sz = 0;
        push_back(oth);
    }
    Deque(const Deque& oth) {
        elem = oth.elem;
        us.reserve(oth.sz);
        sz = oth.sz;
        int i = 0;
        for (auto tmp = oth.begin(); tmp != oth.end(); ++tmp)
            us.push_back(shared_ptr<T>(*tmp));
    }
    Deque& operator=(const Deque& other) {
        elem = other.elem;
        us.reserve(other.sz);
        sz = other.sz;
        int i = 0;
        for (auto tmp = other.begin(); tmp != other.end(); ++tmp)
            us.push_back(shared_ptr<T>(*tmp));
    }
    Deque& operator=(Deque&& other) {
        std::swap(elem, other.elem);
        std::swap(us, other.us);
        std::swap(sz, other.sz);
    }
    bool operator == (const Deque& other) {
        return (elem == other.elem);
    }
    T& operator[] (int in) {
        return *us.get(in);
    }
    const T& operator [] (int in) const {
        return *us.get(in);
    }
    void push_front(const T& elm) {
        elem.push_front(elm);
        ++sz;
        us.push_front(shared_ptr<T>(&(*elem.begin())));
    }
    void pop_front() {
        elem.pop_front();
        us.pop_front();
        --sz;
    }
    void push_back(const T& elm) {
        elem.push_back(elm);
        ++sz;
        us.push_back(shared_ptr<T>(&(*--elem.end())));
    }
    void pop_back() {
        elem.pop_back();
        us.pop_back();
        --sz;
    }
    vc_it begin() {
        return vc_it(0, this);
    }
    vc_it end() {
        return vc_it(sz, this);
    }
};
