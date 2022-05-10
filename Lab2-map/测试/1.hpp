#include "exceptions.hpp"
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include <cmath>

namespace sjtu {
    template<class Key,class T,class Compare = std::less<Key>>class map {
    private:
        int current_size=0;
        Compare Cmp;
    public:
        struct Node {
            pair<Key,T>* data=NULL;
            Node *left = NULL;
            Node *right = NULL;
            Node* pre=NULL;
            Node* next=NULL;
            int height = 1;

            Node() = default;

            Node(pair<Key,T>* d, Node *l=NULL, Node *r=NULL,Node* p=NULL,Node* n=NULL, int h=1) : data(d), left(l),right(r),pre(p),next(n),
                                                           height(h) {}

            Node *First(){
                Node* ptr=this;
                if(ptr==NULL) return ptr;
                while(ptr->left!=NULL){
                    ptr=ptr->left;
                }
                return ptr;
            }

            Node *Last(){
                Node* ptr=this;
                if(ptr==NULL) return ptr;
                while(ptr->right!=NULL){
                    ptr=ptr->right;
                }
                return ptr;
            }
        };

        void Link(Node* &prep,Node* ptr){
            if(ptr==NULL){
                if(prep!=NULL) prep->next=ptr;
                return;
            }
            Link(prep,ptr->left);
            ptr->pre=prep;
            if(prep!=NULL){
                prep->next=ptr;
            }
            prep=ptr;
            Link(prep,ptr->right);
        }

        Node* Create(Node* ptr){
            if(ptr==NULL) return ptr;
            Node* q=NULL;
            auto q1=new value_type (*(ptr->data));
            Node* ptr_new=new Node (q1,Create(ptr->left), Create(ptr->right),q,q,ptr->height);
            return ptr_new;
        }

        void Destroy(Node* &ptr){
            if(ptr==NULL) return;
            Destroy(ptr->left);
            Destroy(ptr->right);
            delete ptr->data;
            delete ptr;
            ptr=NULL;
        }
        
        Node* root=NULL;
        
        typedef pair<Key,T> value_type;

        class const_iterator;
        class iterator {

        private:
        public:
            map<Key,T,Compare>* this_ptr=NULL;
            Node* current_ptr=NULL;

            iterator()=default;
            iterator(map<Key,T,Compare>* p,Node* q):this_ptr(p),current_ptr(q){}
            iterator(const iterator &other) {
                this_ptr=other.this_ptr;
                current_ptr=other.current_ptr;
            }

            iterator operator++(int) {
                if(current_ptr!=NULL){
                    Node* p=current_ptr;
                    current_ptr= current_ptr->next;
                    return iterator(this_ptr,p);
                }
                else{
                    throw invalid_iterator();
                }
            }

            iterator& operator++() {
                if(current_ptr==NULL) throw invalid_iterator();
                current_ptr= current_ptr->next;
                return *this;
            }

            iterator operator--(int) {
                if(current_ptr==NULL){
                    if(this_ptr->root!=NULL){
                        Node* p=NULL;
                        current_ptr=this_ptr->root->Last();
                        return iterator(this_ptr,p);
                    }
                    else{
                        throw invalid_iterator();
                    }
                }
                else{
                    Node* p=current_ptr->pre;
                    if(p==NULL) throw invalid_iterator();
                    Node* q=current_ptr;
                    current_ptr=p;
                    return iterator(this_ptr,q);
                }
            }

            iterator & operator--() {
                if(current_ptr==NULL){
                    if(this_ptr->root==NULL) throw invalid_iterator();
                    else{
                        current_ptr=this_ptr->root->Last();
                        return *this;
                    }
                }
                else{
                    Node* p=current_ptr->pre;
                    if(p==NULL) throw invalid_iterator();
                    current_ptr=p;
                    return *this;
                }
            }

            value_type & operator*() const {
                return *(current_ptr->data);
            }

            bool operator==(const iterator &rhs) const {
                return this_ptr==rhs.this_ptr&&current_ptr==rhs.current_ptr;
            }
            bool operator==(const const_iterator &rhs) const {
                return this_ptr==rhs.this_ptr&&current_ptr==rhs.current_ptr;
            }
            bool operator!=(const iterator &rhs) const {
                return !(this_ptr==rhs.this_ptr&&current_ptr==rhs.current_ptr);
            }
            bool operator!=(const const_iterator &rhs) const {
                return !(this_ptr==rhs.this_ptr&&current_ptr==rhs.current_ptr);
            }

            value_type* operator->() const noexcept {
                return current_ptr->data;
            }
        };
        class const_iterator {
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
        private:
        public:
            const map<Key,T,Compare>* this_ptr;
            Node* current_ptr=NULL;

            const_iterator()=default;
            explicit const_iterator(const map<Key,T,Compare>* p,Node* q=NULL):this_ptr(p),current_ptr(q){}
            const_iterator(const const_iterator &other):this_ptr(other.this_ptr),current_ptr(other.current_ptr){}
            const_iterator(const iterator &other):this_ptr(other.this_ptr) ,current_ptr(other.current_ptr){}


            const_iterator operator++(int) {
                if(current_ptr==NULL) throw invalid_iterator();
                Node* p=current_ptr;
                current_ptr= current_ptr->next;
                return const_iterator(this_ptr,p);
            }

            const_iterator & operator++() {
                if(current_ptr==NULL) throw invalid_iterator();
                current_ptr= current_ptr->next;
                return *this;
            }

            const_iterator operator--(int) {
                if(current_ptr==NULL){
                    if(this_ptr->root!=NULL){
                        Node* p=NULL;
                        current_ptr=this_ptr->root->Last();
                        return const_iterator(this_ptr,p);
                    }
                    else{
                        throw invalid_iterator();
                    }
                }
                else{
                    Node* p=current_ptr->pre;
                    if(p==NULL) throw invalid_iterator();
                    Node* q=current_ptr;
                    current_ptr=p;
                    return const_iterator(this_ptr,q);
                }
            }

            const_iterator & operator--() {
                if(current_ptr==NULL){
                    if(this_ptr->root==NULL) throw invalid_iterator();
                    else{
                        current_ptr=this_ptr->root->Last();
                        return *this;
                    }
                }
                else{
                    Node* p=current_ptr->pre;
                    if(p==NULL) throw invalid_iterator();
                    current_ptr=p;
                    return *this;
                }
            }

            value_type & operator*() const {
                return *(current_ptr->data);
            }

            bool operator==(const iterator &rhs) const {
                return this_ptr==rhs.this_ptr&&current_ptr==rhs.current_ptr;
            }
            bool operator==(const const_iterator &rhs) const {
                return this_ptr==rhs.this_ptr&&current_ptr==rhs.current_ptr;
            }
            bool operator!=(const iterator &rhs) const {
                return !(this_ptr==rhs.this_ptr&&current_ptr==rhs.current_ptr);
            }
            bool operator!=(const const_iterator &rhs) const {
                return !(this_ptr==rhs.this_ptr&&current_ptr==rhs.current_ptr);
            }
            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type* operator->() const noexcept {
                return current_ptr->data;
            }
        };

        map():root(NULL),current_size(0) {}
        map(const map &other) {
            if(other.root==NULL) return;
            root= Create(other.root);
            Node* p=NULL;
            Link(p,root);
            current_size=other.current_size;
        }

        map & operator=(const map &other) {
            if(this==&other) return *this;
            Destroy(root);
            root= Create(other.root);
            Node* p=NULL;
            Link(p,root);
            current_size=other.current_size;
            return *this;
        }

        ~map() {
            clear();
        }

        int Height(Node* ptr){
            return ptr==NULL ? 0 : ptr->height;
        }

        void Rotate_L(Node* &ptr){
            if(ptr==NULL) return;
            Node* p=ptr->left;
            ptr->left=p->right;
            p->right=ptr;
            ptr->height=std::max(Height(ptr->left), Height(ptr->right))+1;
            p->height=std::max(Height(p->left), Height(ptr))+1;
            ptr=p;
        }
        void Rotate_R(Node* &ptr){
            if(ptr==NULL) return;
            Node* p=ptr->right;
            ptr->right=p->left;
            p->left=ptr;
            ptr->height=std::max(Height(ptr->left), Height(ptr->right))+1;
            p->height=std::max(Height(p->right), Height(ptr))+1;
            ptr=p;
        }
        void Rotate_LR(Node* &ptr){
            Rotate_R(ptr->left);
            Rotate_L(ptr);
        }
        void Rotate_RL(Node* &ptr){
            Rotate_L(ptr->right);
            Rotate_R(ptr);
        }

        T & at(const Key &key) {
            Node* ptr= Locate(root,key);
            if(ptr==NULL) throw index_out_of_bound();
            return ptr->data->second;
        }
        const T & at(const Key &key) const {
            Node* ptr= Locate(root,key);
            if(ptr==NULL) throw index_out_of_bound();
            return ptr->data->second;
        }

        T & operator[](const Key &key) {
            Node* ptr= Locate(root,key);
            if(ptr==NULL){
                return (insert(value_type(Key(key),T())).first)->second;
            }
            else return ptr->data->second;
        }

        const T & operator[](const Key &key) const {
            return this->at(key);
        }

        iterator begin() {
            //if(root==NULL) throw invalid_iterator();
            if(root==NULL) return iterator(this, root);
            return iterator(this, root->First());
        }
        const_iterator cbegin() const {
            //if(root==NULL) throw invalid_iterator();
            if(root==NULL) return const_iterator(this, root);
            return const_iterator(this, root->First());
        }

        iterator end() {
            Node* p=NULL;
            return iterator(this, p);
        }
        const_iterator cend() const {
            Node* p=NULL;
            return const_iterator(this, p);
        }

        bool empty() const {
            if(current_size==0) return true;
            else return false;
        }

        size_t size() const {
            return current_size;
        }

        void clear() {
            Destroy(root);
            current_size=0;
        }

        Node* Insert(Node* &ptr,pair<Node*,bool> parent,const value_type &value,bool &ck){
            if(root==NULL){
                auto p1=new value_type (value);
                root=new Node(p1,NULL,NULL,NULL,NULL,1);
                ck=true;
                return root;
            }
            if(ptr==NULL){
                auto p1=new value_type (value);
                ptr=new Node(p1,NULL,NULL,NULL,NULL,1);
                if(parent.first==NULL){//root, the first node
                    ck=true;
                    return ptr;
                }
                Node* p=parent.first->pre;
                Node* n=parent.first->next;
                if(parent.second){//right child is ptr
                    if(n!=NULL){
                        n->pre=ptr;
                    }
                    ptr->next=n;
                    ptr->pre=parent.first;
                    parent.first->next=ptr;
                }
                else{//left child is ptr
                    if(p!=NULL){
                        p->next=ptr;
                    }
                    ptr->pre=p;
                    ptr->next=parent.first;
                    parent.first->pre=ptr;
                }
                ck=true;
                return ptr;
            }
            if(!(Cmp(ptr->data->first,value.first)||Cmp(value.first,ptr->data->first))){
                ck=false;
                return ptr;
            }
            Node* ans;
            bool ck1=false;
            if(Cmp(ptr->data->first,value.first)){
                ans= Insert(ptr->right,pair(ptr,true),value,ck1);
                if(Height(ptr->right)- Height(ptr->left)==2){
                    if(Cmp(ptr->right->data->first,value.first)) Rotate_R(ptr);
                    else Rotate_RL(ptr);
                }
            }
            else{
                ans= Insert(ptr->left,pair(ptr,false),value,ck1);
                if(Height(ptr->left)- Height(ptr->right)==2){
                    if(Cmp(value.first,ptr->left->data->first)) Rotate_L(ptr);
                    else Rotate_LR(ptr);
                }
            }
            ptr->height=std::max(Height(ptr->left), Height(ptr->right))+1;
            if(ck1) ck=true;
            return ans;
        }

        pair<iterator, bool> insert(const value_type &value) {
            Node* p=NULL;
            bool ck=false;
            Node* ans= Insert(root,pair(p,false),value,ck);
            if(ck) current_size++;
            iterator a(this,ans);
            return pair(a,ck);
        }

        bool Adjust(Node* &ptr,bool child){
            if(ptr==NULL) return true;
            if(child){//right
                if(Height(ptr->left)-Height(ptr->right)==1) return true;
                if(Height(ptr->right)==Height(ptr->left)){
                    ptr->height--;
                    return false;
                }
                if(Height(ptr->left->right)>Height(ptr->left->left)){
                    Rotate_LR(ptr);
                    return false;
                }
                Rotate_L(ptr);
                if(Height(ptr->right)==Height(ptr->left)) return false;
                else return true;
            }
            else{//left
                if(Height(ptr->right)-Height(ptr->left)==1) return true;
                if(Height(ptr->right)==Height(ptr->left)){
                    ptr->height--;
                    return false;
                }
                if(Height(ptr->right->left)>Height(ptr->right->right)){
                    Rotate_RL(ptr);
                    return false;
                }
                Rotate_R(ptr);
                if(Height(ptr->right)==Height(ptr->left)) return false;
                else return true;
            }
        }

        bool Delete(Node* &ptr,const value_type &x,bool &ck){
            if(ptr==NULL) return true;
            if(!(Cmp(ptr->data->first,x.first)||Cmp(x.first,ptr->data->first))){
                if(ptr->left==NULL||ptr->right==NULL){
                    Node* tmp=ptr;
                    if(ptr->left==NULL) ptr=ptr->right;
                    else ptr=ptr->left;
                    Node* p=tmp->pre;
                    Node* n=tmp->next;
                    if(p!=NULL) p->next=n;
                    if(n!=NULL) n->pre=p;
                    delete tmp->data;
                    delete tmp;
                    tmp=NULL;
                    ck=true;
                    return false;
                }
                else{
                    Node* next_ptr=ptr->next;
                    if(next_ptr==ptr->right){
                        Node* tmp=ptr;
                        if(ptr->pre!=NULL) ptr->pre->next=next_ptr;
                        if(next_ptr->next!=NULL) next_ptr->next->pre=ptr;
                        Node* l=next_ptr->left;Node* r=next_ptr->right;Node* n=next_ptr->next;int h=next_ptr->height;
                        ptr=next_ptr;
                        ptr->left=tmp->left;
                        ptr->pre=tmp->pre;
                        ptr->height=tmp->height;
                        ptr->right=tmp;
                        ptr->next=tmp;
                        tmp->left=l;
                        tmp->next=n;
                        tmp->right=r;
                        tmp->height=h;
                        tmp->pre=ptr;
                    }
                    else{
                        Node* fa;
                        if(next_ptr->right==NULL) fa=next_ptr->next;
                        else fa=next_ptr->right->next;
                        fa->left=ptr;
                        Node* l=ptr->left;Node* r=ptr->right;Node* p=ptr->pre;int h=ptr->height;
                        fa->left->height=next_ptr->height;
                        fa->left->left=next_ptr->left;
                        fa->left->right=next_ptr->right;
                        fa->left->pre=next_ptr;
                        fa->left->next=next_ptr->next;
                        Node* tmp=ptr;
                        ptr=next_ptr;
                        ptr->left=l;
                        ptr->right=r;
                        ptr->pre=p;
                        ptr->next=tmp;
                        ptr->height=h;
                        if(ptr->pre!=NULL) ptr->pre->next=ptr;
                        if(tmp->next!=NULL) tmp->next->pre=tmp;
                    }
                    if(Delete(ptr->right, x,ck)) return true;
                    else return Adjust(ptr, true);
                    /*
                    Node* tmp=ptr->right;
                    while(tmp->left!=NULL){
                        tmp=tmp->left;
                    }
                    delete ptr->data;
                    ptr->data=new value_type (*(tmp->data));
                    if(Delete(ptr->right, *(tmp->data),ck)) return true;
                    else return Adjust(ptr, true);
                     */
                }
            }
            if(Cmp(x.first,ptr->data->first)){
                if(Delete(ptr->left, x,ck)) return true;
                else return Adjust(ptr, false);
            }
            else{
                if(Delete(ptr->right, x,ck)) return true;
                else return Adjust(ptr, true);
            }
        }
        
        void erase(iterator pos) {
            if(pos.this_ptr!=this||pos.current_ptr==NULL) throw invalid_iterator();
            auto x=*(pos.current_ptr->data);
            bool ck=false;
            //Node* p=NULL;
            Delete(root,x,ck);
            if(ck) current_size--;
        }

        size_t count(const Key &key) const {
            Node* ptr= Locate(root,key);
            if(ptr==NULL) return 0;
            else return 1;
        }

        Node* Locate(Node* ptr,const Key &key) const {
            if(ptr==NULL) return ptr;
            if(!(Cmp(ptr->data->first,key)||Cmp(key,ptr->data->first))) return ptr;
            if(Cmp(ptr->data->first,key)){
                return Locate(ptr->right,key);
            }
            else{
                return Locate(ptr->left,key);
            }
        }

        iterator find(const Key &key) {
            Node* ptr= Locate(root,key);
            if(ptr==NULL) return this->end();
            else{
                return iterator(this,ptr);
            }
        }

        const_iterator find(const Key &key) const {
            Node* ptr= Locate(root,key);
            if(ptr==NULL) return this->cend();
            else{
                return const_iterator(this,ptr);
            }
        }

    };

}