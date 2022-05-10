#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

#include <functional>
#include <cstddef>
#include <memory>
#include <iostream>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template< class Key, class T, class Compare = std::less<Key> >
class map {
    public:
    class iterator;
    class const_iterator;
    class nodes;
    class links;
    using value_type = pair<const Key, T>;
    using ans_type = pair<iterator, bool>;
    using node_ptr = nodes *;
    using link_ptr = links *;
    using mallocs = std::allocator <Key>;

    public:
    class nodes{
        public:
        value_type val;
        node_ptr left, right;
        bool color;
        link_ptr lip;

        public:
        nodes(const value_type &v, const bool &c) 
        : val(v), color(c), left(nullptr), right(nullptr), lip(nullptr) {}
        nodes(const nodes &o) 
        : val(o.val), color(o.color), left(nullptr), right(nullptr), lip(nullptr) {}
        ~nodes() {
            if (lip) delete lip;
        }
    };
    class links{
        public:
        node_ptr nop;
        link_ptr pre, nxt;

        public:
        links() : nop(), pre(), nxt() {}
        links(const node_ptr &f, link_ptr bro, bool ty): nop(f) {
            if (ty) {
                pre = bro->pre;
                nxt = bro;
                if (pre) pre->nxt = this;
                if (nxt) nxt->pre = this;
            }else{
                nxt = bro->nxt;
                pre = bro;
                if (pre) pre->nxt = this;
                if (nxt) nxt->pre = this;
            }
        }
        ~links(){
            if (pre) pre->nxt = nxt;
            if (nxt) nxt->pre = pre;
        }
    };

    public:
    class iterator {
    public:
        friend class map;
        friend class const_iterator;

    private:
        map *src;
        link_ptr cur;

    public:
        iterator() : src(), cur() {}
        iterator(map *s, link_ptr c) : src(s), cur(c) {}
        iterator(const iterator &o) :src(o.src), cur(o.cur) {}
        iterator(const const_iterator &o) :src(o.src), cur(o.cur) {}
        iterator operator++(int) {
            iterator tem(*this);
            ++(*this);
            return tem;
        }
        iterator & operator++() {
            if (cur->nxt) cur = cur->nxt;
            else throw invalid_iterator();
            return *this;
        }
        iterator operator--(int) {
            iterator tem(*this);
            --(*this);
            return tem;
        }
        iterator & operator--() {
            if (cur->pre) cur = cur->pre;
            if (cur == src->head) throw invalid_iterator();
            return *this;
        }
        value_type & operator*() const {
            if (!src || !cur || !cur->nop) throw invalid_iterator();
            return cur->nop->val;
        }
        value_type* operator->() const noexcept {
            if (!src || !cur || !cur->nop) throw invalid_iterator();
            return &(cur->nop->val);
        }
        bool operator==(const iterator &rhs) const {
            if (cur == rhs.cur) return 1;
            return 0;
        }
        bool operator==(const const_iterator &rhs) const {
            if (cur == rhs.cur) return 1;
            return 0;
        }
        bool operator!=(const iterator &rhs) const {
            if (*this == rhs) return 0;
            return 1;
        }
        bool operator!=(const const_iterator &rhs) const {
            if (*this == rhs) return 0;
            return 1;
        }
    };
    class const_iterator {
    public:
        friend class map;
        friend class iterator;

    private:
        const map *src;
        link_ptr cur;

    public:
        const_iterator() : src(), cur() {}
        const_iterator(const map *s, link_ptr c) : src(s), cur(c) {}
        const_iterator(const iterator &o) :src(o.src), cur(o.cur) {}
        const_iterator(const const_iterator &o) :src(o.src), cur(o.cur) {}
        const_iterator operator++(int) {
            const_iterator tem(*this);
            ++(*this);
            return tem;
        }
        const_iterator & operator++() {
            if (cur->nxt) cur = cur->nxt;
            else throw invalid_iterator();
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator tem(*this);
            --(*this);
            return tem;
        }
        const_iterator & operator--() {
            if (cur->pre) cur = cur->pre;
            if (cur == src->head) throw invalid_iterator();
            return *this;
        }
        const value_type & operator*() const {
            if (!src || !cur || !cur->nop) throw invalid_iterator();
            return cur->nop->val;
        }
        const value_type* operator->() const noexcept {
            if (!src || !cur || !cur->nop) throw invalid_iterator();
            return &(cur->nop->val);
        }
        bool operator==(const iterator &rhs) const {
            if (cur == rhs.cur) return 1;
            return 0;
        }
        bool operator==(const const_iterator &rhs) const {
            if (cur == rhs.cur) return 1;
            return 0;
        }
        bool operator!=(const iterator &rhs) const {
            if (*this == rhs) return 0;
            return 1;
        }
        bool operator!=(const const_iterator &rhs) const {
            if (*this == rhs) return 0;
            return 1;
        }
    };
    
    private:
    node_ptr root;
    link_ptr head, tail;
    size_t lenth;
    Compare cmp;
    mallocs alloc;

    private:
    bool color(const node_ptr &x) const {
        if (x) return x->color;
        return 0;
    }
    bool equal(const Key &a, const Key &b) const {
        return (!cmp(a, b) && !cmp(b, a));
    }
    void build_nodes(node_ptr &rt, const node_ptr &o) {
        if (!o) return;
        rt = new nodes(*o);
        if (o->left) build_nodes(rt->left, o->left);
        if (o->right) build_nodes(rt->right, o->right);
        return;
    }
    void build_links(node_ptr &rt) {
        if (!rt) return;
        if (rt->left) build_links(rt->left);
        rt->lip = new links(rt, tail, 1);
        if (rt->right) build_links(rt->right);
        return;
    }
    void clear(node_ptr &rt){
        if (!rt) return;
        if (rt->left) clear(rt->left);
        if (rt->right) clear(rt->right);
        delete rt; rt = nullptr;
        return;
    }
    node_ptr locate(const node_ptr &rt, const Key &key) const {
        if (!rt) return nullptr;
        Key cur = rt->val.first; 
        if (equal(cur, key)) return rt;
        if (cmp(key, cur)){
            return locate(rt->left, key);
        } 
        if (cmp(cur, key)) return locate(rt->right, key);
    }
    void rebuild(const Key *tar, const Key &key){
        alloc.destroy(tar);
        alloc.construct(tar, key);
        return;
    }
    void swap(node_ptr a, node_ptr b){
        link_ptr la = a->lip, lb = b->lip;
        value_type val(a->val);
        rebuild(&(a->val.first), b->val.first);
        rebuild(&(b->val.first), val.first);
        a->val.second = b->val.second;
        b->val.second = val.second;
        a->lip = lb; lb->nop = a;
        b->lip = la; la->nop = b;
        return;
    }
    void LL(node_ptr rt){
        node_ptr p = rt->left, t = p->left;
        swap(rt, p);
        node_ptr left(rt->left), right(rt->right);
        rt->left = t;
        rt->right = p;
        p->left = p->right;
        p->right = right;
        return;
    }
    void RR(node_ptr rt){
        node_ptr p = rt->right, t = p->right;
        swap(rt, p);
        node_ptr left(rt->left), right(rt->right);
        rt->left = p;
        rt->right = t;
        p->right = p->left;
        p->left = left;
        return;
    }
    void LR(node_ptr rt){
        node_ptr p = rt->left, t = p->right;
        swap(rt, t);
        node_ptr left(rt->left), right(rt->right);
        rt->right = t;
        p->right = t->left;
        t->left = t->right;
        t->right = right;
        return;
    }
    void RL(node_ptr rt){
        node_ptr p = rt->right, t = p->left;
        swap(rt, t);
        node_ptr left(rt->left), right(rt->right);
        rt->left = t;
        p->left = t->right;
        t->right = t->left;
        t->left = left;
        return;
    }
    int insert_adjust(node_ptr g, node_ptr p, node_ptr t){
        if (!p->color) return 0;
        if (p == root){
            p->color = 0;
            return 0;
        }
        if (g->left == p){
            if (p->left == t){
                LL(g); return 1;
            }  else {
                LR(g); return 2;
            }
        }else if (p->right == t){
                RR(g); return 1;
            }  else {
                RL(g); return 2;
            }
    }
    void erase_adjust(node_ptr &p, node_ptr &c, node_ptr &t,const Key &key){   //c black -> t black
        if (c->color) return;
        if (c == root)
            if (c->left && c->right && c->left->color == c->right->color){
                c->color = 1;
                c->left->color = c->right->color = 0;
                return;
            }
        if (!color(c->left) && !color(c->right)){
            if (!color(t->left) && !color(t->right)){
                p->color = 0;
                c->color = t->color = 1;
            }else{
                if (p->left == t){
                    if (color(t->left)){
                        t->left->color = 0;
                        LL(p);
                        p = t;
                    }else{
                        LR(p);
                        p = p->right;
                        p->color = 0;
                    }
                }else{
                    if (color(t->right)){
                        t->right->color = 0;
                        RR(p);
                        p = t;
                    }else{
                        RL(p);
                        p = p->left;
                        p->color = 0;
                    }
                }  
                c->color = 1;
            }
        }else{
                if (equal(c->val.first, key)){
                    if (c->left && c->right){   //not leaf
                        if (!c->right->color){
                            LL(c);
                            c = c->right;
                        }
                        return;
                    }
                    if (c->left){
                        LL(c);
                        p = c;
                        c = c->right;
                    }else{
                        RR(c);
                        p = c;
                        c = c->left;
                    }
                }else{
                    p = c;
                    if (cmp(key, p->val.first)) c = p->left, t = p->right;
                    else c = p->right, t = p->left;
                    if (!c->color){
                        if (t == p->right) RR(p); else LL(p);
                        p = t;
                        if (c == p->left) t = p->right;
                        else t = p->left;
                        erase_adjust(p, c, t, key);
                    }
                }
            }
    }
    
    public:
    map() : root(nullptr), lenth(0) {
        head = new links;
        tail = new links(nullptr, head, 0);
    }
    map(const map &o) {
        lenth = o.lenth;
        root = nullptr;
        build_nodes(root, o.root);
        head = new links;
        tail = new links(nullptr, head, 0);
        build_links(root);
    }
    map & operator=(const map &o) {
        if (&o == this) return *this;
        this->clear();
        lenth = o.lenth;
        build_nodes(root, o.root);
        build_links(root);
        return *this;
    }
    ~map() {
        this->clear();
        if (head) delete head;
        if (tail) delete tail;
    }

    public:
    T & at(const Key &key) {
        node_ptr loc = locate(root, key);
        if (loc) return loc->val.second;
        else throw index_out_of_bound();
    }
    const T & at(const Key &key) const {
        node_ptr loc = locate(root, key);
        if (loc) return loc->val.second;
        else throw index_out_of_bound();
    }
    T & operator[](const Key &key) {
        node_ptr loc = locate(root, key);
        if (loc) return loc->val.second;
        else return ((insert(value_type(key, T()))).first)->second;
    }
    const T & operator[](const Key &key) const {
        return at(key);
    }
    iterator begin() {
        return iterator(this, head->nxt);
    }
    const_iterator cbegin() const {
        return const_iterator(this, head->nxt);
    }
    iterator end() {
        return iterator(this, tail);
    }
    const_iterator cend() const {
        return const_iterator(this, tail);
    }
    bool empty() const {
        if (lenth) return 0;
        return 1;
    }
    size_t size() const {
        return lenth;
    }

    public:
    ans_type insert(const value_type &value) {
        if (!root) {
            root = new nodes(value, 0);
            root->lip = new links(root, tail, 1);
            ++lenth;
            return ans_type(iterator(this, head->nxt), 1);
        }
        Key key = value.first;
        node_ptr g, p, t;
        g = p = t = root;
        while (1) {
            if (t) {
                if (equal(t->val.first, key)) return ans_type(iterator(this, t->lip), 0);
                if (color(t->left) && color(t->right)) {
                    t->left->color = t->right->color = 0;
                    t->color = 1;
                    int ty = insert_adjust(g, p, t);
                    if (ty == 1) p = g;
                    if (ty == 2){
                        p = g;
                        if (cmp(key, p->val.first)) t = p->left;
                        else t = p->right;
                    }
                }
                g = p; p = t;
                if (cmp(key, t->val.first)) t = t->left;
                else t= t->right;
            }else{
                ++lenth;
                t = new nodes(value, 1);
                link_ptr tem;
                if (cmp(key, p->val.first)){
                    p->left = t;
                    tem = t->lip = new links(t, p->lip, 1);
                }else{
                    p->right = t;
                    tem = t->lip = new links(t, p->lip, 0);
                } 
                insert_adjust(g, p, t);
                root->color = 0;
                return ans_type(iterator(this, tem), 1);
            }
        }
    }
    void erase(iterator pos) {
        if (pos.src != this || pos == end()) throw invalid_iterator();
        if (!root) throw invalid_iterator();
        Key key = pos->first;
        if (equal(root->val.first, key) && !root->left && !root->right){
            --lenth;
            delete root; root = nullptr;
            return;
        }
        node_ptr t, p, c;
        t = p = c = root;
        while (1){
            erase_adjust(p, c, t, key);
            if (equal(c->val.first, key) && c->left && c->right){
                node_ptr tem = c->lip->nxt->nop;
                swap(c, tem);
                p = c;
                c = c->right;
                t = p->left;
                continue;
            }
            if (equal(c->val.first, key)) {
                --lenth;
                delete c;
                if (p->left == c) p->left = nullptr; else p->right = nullptr;
                c = nullptr;
                root->color = 0;
                return;
            }
            p = c;
            if (cmp(key, p->val.first)) c = p->left, t = p->right;
            else c = p->right, t = p->left;
        }
    }
    size_t count(const Key &key) const {
        node_ptr tem = locate(root, key);
        if (!tem) return 0;
        return 1;
    }
    iterator find(const Key &key) {
        node_ptr tem = locate(root, key);
        if (!tem) return end();
        return iterator(this, tem->lip);
    }
    const_iterator find(const Key &key) const {
        node_ptr tem = locate(root, key);
        if (!tem) return cend();
        return const_iterator(this, tem->lip);
    }
    void clear() {
        lenth = 0;
        clear(root);
    }
    void express() {
        std::cout << "#lenth = " << lenth << std::endl;
        express(root, 0);
        std::cout << std::endl;
    }
    void express(node_ptr rt, int deep){
        if (!rt) return;
        std::cout << rt->val.first << " #" << deep 
        << " : " << rt << " color : " << rt->color << " ( " << rt->left << " , " << rt->right << " )" << std::endl;
        if (rt->left) express(rt->left, deep+1);
        if (rt->right) express(rt->right, deep+1);
        return;
    }
};

}

#endif