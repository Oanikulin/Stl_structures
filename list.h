#include<cstddef>
#include<new>
#include<memory>
#include<stdexcept>
#include<utility>
#include<iostream>

using std::shared_ptr;
template<typename T>
struct Node {
    shared_ptr<Node<T>> nxt;
    shared_ptr<Node<T>> prv;
    shared_ptr<T> val;
    Node(const T& in) {
        try {
            val = shared_ptr<T>(new T(in));
        } catch (...) {
            throw std::bad_alloc();
        }
    }
};

template<typename T>
class List {
private:
    size_t sz = 0;
    shared_ptr<Node<T>> fir, en;

public:

    class List_it {
    private:
        shared_ptr<Node<T>> val;
        char fl = 0;

    public:
        List_it(shared_ptr<Node<T>>& en, int n_fl = 0) {
            val = en;
            fl = n_fl;
        }
        bool operator == (const List_it& other) const {
            return (val == other.val && fl == other.fl);
        }
        bool operator != (const List_it& other) const {
            return !((*this) == other);
        }
        T& operator* () {
            return *(val->val);
        }
        List_it& operator++() {
            if (val->nxt == nullptr)
                fl = 1;
            else
                val = val->nxt;
            return (*this);
        }
        List_it& operator--() {
            if (fl)
                fl = 0;
            else
                val = val->prv;
            return (*this);
        }
        List_it operator++(int) {
            List_it tmp = List_it(val, fl);
            ++(*this);
            return tmp;
        }
        List_it operator--(int) {
            List_it tmp = List_it(val, fl);
            --(*this);
            return tmp;
        }
    };

    List(){
    }
    ~List() {
        auto cur = fir;
        for (auto& tmp = fir; tmp != nullptr; tmp = cur) {
            cur = tmp->nxt;
            tmp->nxt = nullptr;
        }
    }
    size_t size() const noexcept {
        return sz;
    }
    List(const List& other) {
        shared_ptr<Node<T>> nf, n_end, cur, sv;
        if (other.fir == nullptr) {
            fir = nullptr;
            en = nullptr;
            sz = 0;
            return;
        }
        try {
            nf = shared_ptr<Node<T>>(new Node<T>(*(other.fir->val)));
            cur = nullptr;
            sv = nf;
            auto tmp = other.fir->nxt;
            while (tmp != nullptr) {
                cur = shared_ptr<Node<T>>(new Node<T>(*(tmp->val)));
                tmp = tmp->nxt;
                cur->prv = nf;
                nf->nxt = cur;
                nf = cur;
                cur = nullptr;
                n_end = nf;
            }
        } catch (...) {
            return;
        }
        fir = sv;
        sz = other.sz;
        en = n_end;
        return;
    }
    List& operator=(const List& other) {
        shared_ptr<Node<T>> nf, n_end, cur, sv;
        if (other.fir == nullptr) {
            fir = nullptr;
            en = nullptr;
            sz = 0;
            return (*this);
        }
        try {
            nf = shared_ptr<Node<T>>(new Node<T>(*(other.fir->val)));
            cur = nullptr;
            sv = nf;
            auto tmp = other.fir->nxt;
            while (tmp != nullptr) {
                cur = shared_ptr<Node<T>>(new Node<T>(*(tmp->val)));
                tmp = tmp->nxt;
                cur->prv = nf;
                nf->nxt = cur;
                nf = cur;
                cur = nullptr;
                n_end = nf;
            }
        } catch (...) {
            return (*this);
        }
        fir = sv;
        sz = other.sz;
        en = n_end;
        return (*this);
    }
    void push_back(const T& elem) {
        if (fir == 0) {
            try {
                fir = shared_ptr<Node<T>>(new Node<T>(elem));
                en = fir;
                sz = 1;
            } catch(...) {
                return;
            }
            return;
        }
        shared_ptr<Node<T>> tmp = nullptr;
        try {
            tmp = shared_ptr<Node<T>>(new Node<T>(elem));
            en->nxt = tmp;
            tmp->prv = en;
            en = tmp;
            ++sz;
        } catch(...) {
            en->nxt = nullptr;
        }
    }
    void push_front(const T& elem) {
        if (fir == 0) {
            try {
                fir = shared_ptr<Node<T>>(new Node<T>(elem));
                en = fir;
            } catch(...) {
                return;
            }
            ++sz;
            return;
        }
        shared_ptr<Node<T>> tmp = nullptr;
        try {
            tmp = shared_ptr<Node<T>>(new Node<T>(elem));
            fir->prv = tmp;
            tmp->nxt = fir;
            fir = fir->prv;
            ++sz;
        } catch(...) {
            fir->prv = 0;
            if (tmp) {
                tmp->nxt = nullptr;
            }
        }
    }
   /* const List_it begin() const noexcept {
        return List_it(fir);
    }*/
    List_it begin() noexcept {
        return List_it(fir);
    }
    /*const List_it end() const noexcept {
        return List_it(en, 1);
    }*/
    List_it end() noexcept {
        return List_it(en, 1);
    }
    void pop_front() noexcept {
        auto tmp = fir;
        fir = fir->nxt;
        tmp->nxt = nullptr;
        if (fir) {
            fir->prv = nullptr;
        } else {
            en = nullptr;
        }
        --sz;
    }
    void pop_back() noexcept {
        auto tmp = en;
        en = en->prv;
        if (en) {
            en->nxt = nullptr;
        } else {
            fir = nullptr;
        }
        --sz;
    }
    bool operator == (const List& oth) const noexcept {
        if (sz != oth.sz) {
            return 0;
        }
        if (sz == 0)
            return 1;
        auto c = oth.fir;
        auto d = fir;
        do {
            if ((*(c->val)) != (*(d->val)))
                return 0;
            c = c->nxt;
            d = d->nxt;
        }  while (d != en);
        return 1;
    }
};
