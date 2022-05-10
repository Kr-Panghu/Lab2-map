/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"
 
namespace sjtu {

template<
    class Key,
    class T,
    class Compare = std::less<Key>
> class map {
public:
    struct node{
        friend class map<Key, T, Compare>;
        pair<const Key, T>* data = nullptr;
        node* left = nullptr;
        node* right = nullptr;
        node* father = nullptr;
        int height = 1;
        node(): data(nullptr), left(nullptr), right(nullptr), father(nullptr), height(1){}
        node(const pair<Key, T> &element, node* lt, node* rt, node* fa, const int &h)
            :left(lt), right(rt), father(fa), height(h){data = new value_type (element);}
        ~node(){
            if (data) delete data;
            data = nullptr;
            left = nullptr;
            right = nullptr;
            father = nullptr;
            height = 1;
        }
    };
private:
    node* root;
    node* tail;
    Compare cmp;
    size_t amount = 0;
    size_t height(node* t) const{
        return t == nullptr ? 0 : t -> height;
    }
public:
    /**
     * the internal type of data.
     * it should have a default constructor, a copy constructor.
     * You can use sjtu::map as value_type by typedef.
     */
    typedef pair<const Key, T> value_type;
    /**
     * see BidirectionalIterator at CppReference for help.
     *
     * if there is anything wrong throw invalid_iterator.
     *     like it = map.begin(); --it;
     *       or it = map.end(); ++end();
     */
    class const_iterator;
    class iterator {
    private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        friend class map<Key, T, Compare>;
        friend class const_iterator;
        map<Key, T, Compare> *map_ptr;
        node* pos;
    public:
        iterator(): map_ptr(nullptr), pos(nullptr) {
            // TODO
        }
        iterator(const iterator &other) {
            // TODO
            map_ptr = other.map_ptr;
            pos = other.pos;
        }
		iterator(map<Key, T, Compare> *mp, node *np)
		    :map_ptr(mp), pos(np) {}
        /**
         * return a new iterator which pointer n-next elements
         *   even if there are not enough elements, just return the answer.
         * as well as operator-
         */
        /**
         * TODO iter++
         */
        iterator operator++(int) { //两种情况:是否是树根root
            if(pos == map_ptr -> tail) throw invalid_iterator();
            iterator res(map_ptr, pos);
            if(pos == map_ptr -> root){
                pos = pos -> right;
                if(pos){
                    while(pos -> left) pos = pos -> left;
                }
                else pos = map_ptr -> tail;
                return res;
            }
            else{ //两种情况:是父结点的左孩子还是右孩子
                if(pos == pos -> father -> left){
                    if(pos -> right){
                        pos = pos -> right;
                        while(pos -> left) pos = pos -> left;
                        return res;
                    }
                    else{
                        pos = pos -> father;
                        return res;
                    }
                }
                else{
                    if(pos -> right){
                        pos = pos -> right;
                        while(pos -> left) pos = pos -> left;
                        return res;
                    }
                    else{
                        while(pos -> father){
                            if(pos == pos -> father -> right){
                                pos = pos -> father;
                                continue;
                            }
                            pos = pos -> father;
                            return res;
                        }
                        pos = map_ptr -> tail;
                        return res;
                    }
                }
            }
        }
        /**
         * TODO ++iter
         */
        iterator & operator++() {
            if(pos == map_ptr -> tail) throw invalid_iterator();
            if(pos == map_ptr -> root){
                pos = pos -> right;
                if(pos){
                    while(pos -> left) pos = pos -> left;
                }
                else pos = map_ptr -> tail;
                return *this;
            }
            else{
                if(pos == pos->father->left){
                    if(pos -> right){
                        pos = pos -> right;
                        while(pos -> left) pos = pos -> left;
                        return *this;
                    }
                    else{
                        pos = pos -> father;
                        return *this;
                    }
                }
                else{ //父结点的右孩子
                    if(pos -> right){
                        pos = pos -> right;
                        while(pos -> left) pos = pos -> left;
                        return *this;
                    }
                    else{
                        while(pos -> father){
                            if(pos == pos -> father -> right){
                                pos = pos -> father;
                                continue;
                            }
                            pos = pos -> father;
                            return *this;
                        }
                        pos = map_ptr -> tail;
                        return *this;
                    }
                }
            }
        }
        /**
         * TODO iter--
         */
        iterator operator--(int) {
            iterator res(map_ptr, pos);
            if(pos == map_ptr -> tail){
                pos = map_ptr -> root;
                if(!pos) throw invalid_iterator();
                while(pos -> right) pos = pos -> right;
                return res;
            }
            if(pos == map_ptr -> root){ //判断是否是根
                pos = pos -> left;
                if(pos){
                    while(pos -> right) pos = pos -> right;
                    return res;
                }
                throw invalid_iterator();
            }
            else{
                if(pos == pos -> father -> left){ //判断是父结点的左孩子还是右孩子
                    if(pos -> left){
                        pos =  pos -> left;
                        while(pos -> right) pos = pos -> right;
                        return res;
                    }
                    else{
                        while(pos -> father){
                            if(pos == pos -> father -> left){
                                pos = pos -> father;
                                continue;
                            }
                            pos = pos -> father;
                            return res;
                        }
                        throw invalid_iterator();
                    }
                }
                else{
                    if(pos -> left){
                        pos = pos -> left;
                        while(pos -> right) pos = pos ->right;
                        return res;
                    }
                    else{
                        pos = pos -> father;
                        return res;
                    }
                }
            }
        }
        /**
         * TODO --iter
         */
        iterator & operator--() {
            if(pos == map_ptr -> tail){
                pos = map_ptr -> root;
                if(!pos) throw invalid_iterator();
                while(pos -> right) pos = pos -> right;
                return *this;
            }
            if(pos == map_ptr -> root){ //判断是否是根
                pos = pos -> left;
                if(pos){
                    while(pos -> right) pos = pos -> right;
                    return *this;
                }
                throw invalid_iterator();
            }
            else{
                if(pos == pos -> father -> left){ //判断是父结点的左孩子还是右孩子
                    if(pos -> left){
                        pos =  pos -> left;
                        while(pos -> right) pos = pos -> right;
                        return *this;
                    }
                    else{
                        while(pos -> father){
                            if(pos == pos -> father -> left){
                                pos = pos -> father;
                                continue;
                            }
                            pos = pos -> father;
                            return *this;
                        }
                        throw invalid_iterator();
                    }
                }
                else{
                    if(pos -> left){
                        pos = pos -> left;
                        while(pos -> right) pos = pos ->right;
                        return *this;
                    }
                    else{
                        pos = pos -> father;
                        return *this;
                    }
                }
            }
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        value_type & operator*() const {
            return pos -> data[0];
        }
        bool operator==(const iterator &rhs) const {
            return map_ptr == rhs.map_ptr && pos == rhs.pos;
        }
        bool operator==(const const_iterator &rhs) const {
            return map_ptr == rhs.map_ptr && pos == rhs.pos;
        }
        /**
         * some other operator for iterator.
         */

        bool operator!=(const iterator &rhs) const {
            return map_ptr != rhs.map_ptr || pos != rhs.pos;
        }

        bool operator!=(const const_iterator &rhs) const {
            return map_ptr != rhs.map_ptr || pos != rhs.pos;
        }
        /**
         * for the support of it->first.
         * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
         */
        value_type* operator->() const {
            if(pos) return pos -> data;
            return nullptr;
        }
        //value_type* operator->() const noexcept {}
    };
    class const_iterator {
    // it should has similar member method as iterator.
    //  and it should be able to construct from an iterator.
    private:
        // data members.
        friend class map<Key, T, Compare>;
        friend class iterator;
        const map<Key, T, Compare> *map_ptr;
        node* pos;
    public:
        const_iterator():map_ptr(nullptr), pos(nullptr){
            // TODO
        }

        const_iterator(const const_iterator &other)
            :map_ptr(other.map_ptr), pos(other.pos){}

        const_iterator(const iterator &other)
            :map_ptr(other.map_ptr), pos(other.pos){}

		const_iterator(const map<Key, T, Compare> *mp, node *np)
		    :map_ptr(mp), pos(np) {}

        const_iterator operator++(int) { 
            if(pos == map_ptr -> tail) throw invalid_iterator();
            const_iterator res(map_ptr, pos);
            if(pos == map_ptr -> root){
                pos = pos -> right;
                if(pos){
                    while(pos -> left) pos = pos -> left;
                }
                else pos = map_ptr -> tail;
                return res;
            }
            else{ 
                if(pos == pos->father->left){
                    if(pos -> right){
                        pos = pos -> right;
                        while(pos -> left) pos = pos -> left;
                        return res;
                    }
                    else{
                        pos = pos -> father;
                        return res;
                    }
                }
                else{
                    if(pos -> right){
                        pos = pos -> right;
                        while(pos -> left) pos = pos -> left;
                        return res;
                    }
                    else{
                        while(pos -> father){
                            if(pos == pos -> father -> right){
                                pos = pos -> father;
                                continue;
                            }
                            pos = pos -> father;
                            return res;
                        }
                        pos = map_ptr -> tail;
                        return res;
                    }
                }
            }
        }
        const_iterator & operator++() {
            if(pos == map_ptr -> tail) throw invalid_iterator();
            if(pos == map_ptr -> root){
                pos = pos -> right;
                if(pos){
                    while(pos -> left) pos = pos -> left;
                }
                else pos = map_ptr -> tail;
                return *this;
            }
            else{
                if(pos == pos->father->left){
                    if(pos -> right){
                        pos = pos -> right;
                        while(pos -> left) pos = pos -> left;
                        return *this;
                    }
                    else{
                        pos = pos -> father;
                        return *this;
                    }
                }
                else{ //父结点的右孩子
                    if(pos -> right){
                        pos = pos -> right;
                        while(pos -> left) pos = pos -> left;
                        return *this;
                    }
                    else{
                        while(pos -> father){
                            if(pos == pos -> father -> right){
                                pos = pos -> father;
                                continue;
                            }
                            pos = pos -> father;
                            return *this;
                        }
                        pos = map_ptr -> tail;
                        return *this;
                    }
                }
            }
        }
        const_iterator operator--(int) {
            const_iterator res(map_ptr, pos);
            if(pos == map_ptr -> tail){
                pos = map_ptr -> root;
                if(!pos) throw invalid_iterator();
                while(pos -> right) pos = pos -> right;
                return res;
            }
            if(pos == map_ptr -> root){ //判断是否是根
                pos = pos -> left;
                if(pos){
                    while(pos -> right) pos = pos -> right;
                    return res;
                }
                throw invalid_iterator();
            }
            else{
                if(pos == pos -> father -> left){ //判断是父结点的左孩子还是右孩子
                    if(pos -> left){
                        pos =  pos -> left;
                        while(pos -> right) pos = pos -> right;
                        return res;
                    }
                    else{
                        while(pos -> father){
                            if(pos == pos -> father -> left){
                                pos = pos -> father;
                                continue;
                            }
                            pos = pos -> father;
                            return res;
                        }
                        throw invalid_iterator();
                    }
                }
                else{
                    if(pos -> left){
                        pos = pos -> left;
                        while(pos -> right) pos = pos ->right;
                        return res;
                    }
                    else{
                        pos = pos -> father;
                        return res;
                    }
                }
            }
        }
        const_iterator & operator--() {
            if(pos == map_ptr -> tail){
                pos = map_ptr -> root;
                if(!pos) throw invalid_iterator();
                while(pos -> right) pos = pos -> right;
                return *this;
            }
            if(pos == map_ptr -> root){ //判断是否是根
                pos = pos -> left;
                if(pos){
                    while(pos -> right) pos = pos -> right;
                    return *this;
                }
                throw invalid_iterator();
            }
            else{
                if(pos == pos -> father -> left){ //判断是父结点的左孩子还是右孩子
                    if(pos -> left){
                        pos =  pos -> left;
                        while(pos -> right) pos = pos -> right;
                        return *this;
                    }
                    else{
                        while(pos -> father){
                            if(pos == pos -> father -> left){
                                pos = pos -> father;
                                continue;
                            }
                            pos = pos -> father;
                            return *this;
                        }
                        throw invalid_iterator();
                    }
                }
                else{
                    if(pos -> left){
                        pos = pos -> left;
                        while(pos -> right) pos = pos ->right;
                        return *this;
                    }
                    else{
                        pos = pos -> father;
                        return *this;
                    }
                }
            }
        }
        const value_type & operator*() const {
            return pos -> data[0];
        }
        bool operator==(const iterator &rhs) const {
            return map_ptr == rhs.map_ptr && pos == rhs.pos;
        }
        bool operator==(const const_iterator &rhs) const {
            return map_ptr == rhs.map_ptr && pos == rhs.pos;
        }
        /**
         * some other operator for iterator.
         */

        bool operator!=(const iterator &rhs) const {
            return map_ptr != rhs.map_ptr || pos != rhs.pos;
        }

        bool operator!=(const const_iterator &rhs) const {
            return map_ptr != rhs.map_ptr || pos != rhs.pos;
        }
        /**
         * for the support of it->first.
         * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
         */
        const value_type* operator->() const {
            if(pos) return pos -> data;
            return nullptr;
        }
    };

private:
    node* search(const Key &key, node *ptr) const {
        if (!ptr) return ptr;
        else {
            if (cmp(key, ptr->data->first)) return search(key, ptr->left);
            else if (cmp(ptr->data->first, key)) return search(key, ptr->right);
            else return ptr;
        }
    }

    void destroy(node* &ptr){ //注意引用, 根也要删除
        if(!ptr) return;
        destroy(ptr -> left);
        destroy(ptr -> right);
        delete ptr;
        ptr = nullptr;
    }

    void LL(node* &ptr){ //使用引用, ptr为修改部分的新的根
        node* Newptr = ptr -> left; //New root
        ptr -> left = Newptr -> right;
        if(Newptr -> right) Newptr -> right -> father = ptr;
        Newptr -> right = ptr;
        Newptr -> father = ptr -> father;
        ptr -> father = Newptr;
        ptr -> height = std::max(height(ptr -> lweft), height(ptr -> right)) + 1;
        Newptr -> height = std::max(height(Newptr -> left), height(ptr)) + 1;
        ptr = Newptr;
    }
    
    void RR(node* &ptr){
        node* Newptr = ptr -> right;
        ptr -> right = Newptr -> left;
        if(Newptr -> left) Newptr -> left -> father = ptr;
        Newptr -> left = ptr;
        Newptr -> father = ptr -> father;w
        ptr -> father = Newptr;
        ptr -> height = std::max(height(ptr -> left), height(ptr -> right)) + 1;
        Newptr -> height = std::max(height(Newptr -> right), height(ptr)) + 1;
        ptr = Newptr;
    }

    void LR(node* &ptr){
        RR(ptr -> left);
        LL(ptr);
    }

    void RL(node* &ptr){
        LL(ptr -> right);
        RR(ptr);
    }

    void push(const pair<const Key, T> &val, node* &ptr){
        if(ptr == nullptr) //在空树上插入
            ptr = new node(val, nullptr, nullptr, nullptr, 1);
        else if(cmp(val.first, ptr -> data -> first)){ //左子树上插入
            if(!ptr -> left){ 
                ptr -> left = new node(val, nullptr, nullptr, ptr, 1);
                ptr -> height = std::max(height(ptr -> left), height(ptr -> right)) + 1;
            }
            else push(val, ptr -> left);
            //插入后旋转
            if(height(ptr -> left) - height(ptr -> right) == 2){
                if(cmp(val.first, ptr -> left -> data -> first)) LL(ptr);
                else LR(ptr);
            }
        }
        else if(cmp(ptr -> data -> first, val.first)){ //右子树上插入
            if(!ptr -> right){
                ptr -> right = new node(val, nullptr, nullptr, ptr, 1);
                ptr -> height = std::max(height(ptr -> left), height(ptr -> right)) + 1;
            }
            else push(val, ptr -> right);
            
            if(height(ptr -> right) - height(ptr -> left) == 2){
                if(cmp(ptr -> right -> data -> first, val.first)) RR(ptr);
                else RL(ptr);
            }
        }
        //重新计算ptr的高度
        ptr -> height = std::max(height(ptr -> left), height(ptr -> right)) + 1;
    }

    bool remove(const Key &key, node* &ptr){ //bool:删除后树的高度是否不变
        if(ptr == nullptr) return true;
        if(!cmp(key, ptr -> data -> first) && !cmp(ptr -> data -> first, key)){ //删除根结点
            if(ptr -> left == nullptr || ptr -> right == nullptr){
                //被删结点是叶子结点
                if(ptr -> left == nullptr && ptr -> right == nullptr){
                    delete ptr;
                    ptr = nullptr;
                    return false;
                }
                //被删结点只有一个孩子
                node* Oldnode = ptr;
                node* fa = ptr -> father;
                ptr = (ptr -> left != nullptr) ? ptr -> left : ptr -> right;
                ptr -> father = fa;
                delete Oldnode;
                Oldnode = nullptr;
                return false;
            }
            else{ //被删结点有两个孩子
                node* tmp = ptr -> right;
                while(tmp -> left) tmp = tmp -> left;
                if(tmp != ptr -> right){
                    node* ptr_f = ptr -> father; //暂存一个tmp_ptr
                    node* ptr_l = ptr -> left;
                    node* ptr_r = ptr -> right;
                    int ptr_h = ptr -> height;
                    value_type ptr_v = ptr -> data[0];

                    ptr -> father = tmp -> father;
                    tmp -> father -> left = ptr;
                    ptr -> right = tmp -> right;
                    if(tmp -> right) tmp -> right -> father = ptr;
                    ptr -> left = nullptr;
                    ptr -> height = tmp -> height;

                    tmp -> father = ptr_f;
                    tmp -> left = ptr_l;
                    tmp -> right = ptr_r;
                    tmp -> height = ptr_h;
                    ptr_l -> father = tmp;
                    ptr_r -> father = tmp;

                    ptr = tmp;

                    if(remove(key, ptr -> right)) return true;
                    return adjust(ptr, 1);
                }
                else{ //tmp == ptr -> right;
                    node* ptr_f = ptr -> father;
                    node* ptr_l = ptr -> left;
                    int ptr_h = ptr -> height;
                    value_type ptr_v = ptr -> data[0];

                    ptr -> father = tmp;
                    ptr -> right = tmp -> right;
                    if(tmp -> right)  tmp -> right -> father = ptr;
                    ptr -> left = nullptr;
                    ptr -> height = tmp -> height;

                    tmp -> father = ptr_f;
                    tmp -> right = ptr;
                    tmp -> left = ptr_l;
                    tmp -> height = ptr_h;
                    ptr_l -> father = tmp;

                    ptr = tmp;

                    if(remove(key, ptr -> right)) return true;
                    return adjust(ptr, 1);
                }
            }
        }
        else if(cmp(key, ptr -> data -> first)) {
            if (remove(key, ptr -> left)) return true; //删除后左子树没有变矮
            return adjust(ptr, 0);
        }
        else{
            if(remove(key, ptr -> right)) return true; //删除后右子树没有变矮
            return adjust(ptr, 1);
        }
    }

    bool adjust(node* &ptr, const int &type){
        if(!type){ //type == 0, 在左子树上删除且使左子树变矮 
            if(height(ptr -> right) - height(ptr -> left) == 1) return true; //原本左右子树等高, 高度不变
            if(height(ptr -> right) == height(ptr -> left)){ //原本左子树高, 因此高度-1
                --(ptr -> height);
                return false;
            }
            //原本右子树高, 危机结点
            if(height(ptr -> right -> left) > height(ptr -> right -> right)){
                RL(ptr);
                return false;
            }
            RR(ptr);
            if(height(ptr -> right) == height(ptr -> left)) return false; else return true;
        }
        else{ //在右子树上删除,且右子树变矮
            if(height(ptr -> left) - height(ptr -> right) == 1) return true;
            if(height(ptr -> right) == height(ptr -> left)){
                --(ptr -> height);
                return false;
            }
            if(height(ptr -> left -> right) > height(ptr -> left -> left)){
                LR(ptr);
                return false;
            }
            LL(ptr);
            if(height(ptr -> right) == height(ptr -> left)) return false; else return true;
        }
    }

public:
    /**
     * TODO two constructors
     */
    map():root(nullptr), amount(0) {tail = new node;}

    map(const map &other) {
        tail = new node;
        root = nullptr;
        for(const_iterator it = other.cbegin(); it != other.cend(); ++it){
            push(it.pos -> data[0], root);
        }
        amount = other.amount;
    }
    /**
     * TODO assignment operator
     */
    map & operator=(const map &other) {
        if (this == &other) return *this;
        destroy(root);
        for(const_iterator it = other.cbegin() ; it != other.cend() ; ++it)
            push(it.pos->data[0], root);
        amount = other.amount;
        return *this;
    }
    /**
     * TODO Destructors
     */
    ~map() {
        clear();
        delete tail;
    }
    /**
     * TODO
     * access specified element with bounds checking
     * Returns a reference to the mapped value of the element with key equivalent to key.
     * If no such element exists, an exception of type `index_out_of_bound'
     */
    T & at(const Key &key) {
        node* res = search(key, root);
        if(!res) throw index_out_of_bound();
        else return res -> data -> second;
    }
    const T & at(const Key &key) const {
        node* res = search(key, root);
        if(!res) throw index_out_of_bound();
        else return res -> data -> second;
    }
    /**
     * TODO
     * access specified element
     * Returns a reference to the value that is mapped to a key equivalent to key,
     *   performing an insertion if such key does not already exist.
     */
    T & operator[](const Key &key) {
        //HINT: Performing an insertion if such key does not already exist.
        node* res = search(key, root);
        if(!res){
            pair<const Key, T> val(key, T());
            push(val, root);
            ++amount;
            res = search(key, root);
            return res -> data -> second;
        }
        return res -> data -> second;
    } 
    /**
     * behave like at() throw index_out_of_bound if such key does not exist.
     */

    // const T & operator[](const Key &key) const {
    //     node* res = search(key, root);
    //     if(!res){
    //         pair<const Key, T> val(key, T());
    //         push(val, root);
    //         ++amount;
    //         res = search(key, root);
    //         return res -> data -> second;
    //     }
    //     return res -> data -> second;
    // }

	const T & operator[](const Key &key) const {
	    node *ptr = search(key, root);
        if (!ptr) throw index_out_of_bound();
        else return ptr -> data -> second;
	}

    /**
     * return a iterator to the beginning
     */
    iterator begin() {
        if(!root) return iterator(this, tail);
        node* tmp = root;
        while(tmp -> left) tmp = tmp -> left;
        return iterator(this, tmp);
    }

    const_iterator cbegin() const {
        if(!root) return const_iterator(this, tail);
        node* tmp = root;
        while(tmp -> left) tmp = tmp -> left;
        return const_iterator(this, tmp);
    }
    /**
     * return a iterator to the end
     * in fact, it returns past-the-end.
     */
    iterator end() {
        //past-the-end
        return iterator(this, tail);
    }
    const_iterator cend() const {
        return const_iterator(this, tail);
    }
    /**
     * checks whether the container is empty
     * return true if empty, otherwise false.
     */
    bool empty() const {
        return (!amount);
    }
    /**
     * returns the number of elements.
     */
    size_t size() const {
        return amount;
    }
    /**
     * clears the contents
     */
    void clear() {
        destroy(root);
        amount = 0;
    }
    /**
     * insert an element.
     * return a pair, the first of the pair is
     *   the iterator to the new element (or the element that prevented the insertion),
     *   the second one is true if insert successfully, or false.
     */
    pair<iterator, bool> insert(const value_type &value) {
        node* tmp = search(value.first, root);
        if(tmp){
            iterator res(this, tmp);
            return pair<iterator, bool> (res, false);
        }
        push(value, root);
        ++amount;
        tmp = search(value.first, root);
        iterator res(this, tmp);
        return pair<iterator, bool> (res, true);
    }
    /**
     * erase the element at pos.
     *
     * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
     */
    void erase(iterator _pos) {
        if(_pos.pos == tail || _pos.map_ptr != this) throw invalid_iterator();
        if(!search(_pos.pos -> data -> first, root)) throw invalid_iterator();
        remove(_pos.pos -> data -> first, root);
        --amount;
    }
    /**
     * Returns the number of elements with key
     *   that compares equivalent to the specified argument,
     *   which is either 1 or 0
     *     since this container does not allow duplicates.
     * The default method of check the equivalence is !(a < b || b > a)
     */
    size_t count(const Key &key) const {
        if(search(key, root)) return 1;
        return 0;
    }
    /**
     * Finds an element with key equivalent to key.
     * key value of the element to search for.
     * Iterator to an element with key equivalent to key.
     *   If no such element is found, past-the-end (see end()) iterator is returned.
     */
    iterator find(const Key &key) {
        node* ptr = search(key, root);
        if(ptr) return iterator(this, ptr);
        else return end();
    }
    const_iterator find(const Key &key) const {
        node* ptr = search(key, root);
        if(ptr) return const_iterator(this, ptr);
        else return cend();
    }
};

}

#endif
