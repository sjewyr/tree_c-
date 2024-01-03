#include <iostream>
#include <stack>
#include <time.h>

class Node{
    public:
    int v, k, size;
    bool isVirtual;
    Node *left, *right;
    Node(int key, int val, bool virt = false){
        v = val;
        k = key;
        if (!virt){
            left = new Node(-1, -1, virt = true);
            size = 1;
            right = 0;
            isVirtual = false;
        }
        else{
            size = 0;
            isVirtual = true;
            left = right = 0;
        }
    }
    void fix_size(){
        size = 1;
        isVirtual = false;
        if (left && left->size != 0) size += left->size;
        if (left && left->right) size += left->right->size; 
    }
    
    Node* rotate_left(){
        Node* p = left->right;     
        if (!p){
            return this;
        }
        p->right = right;
        right = p->left->right;
        p->left->right = nullptr;
        if (p->left && !p->left->isVirtual) left->right = p->left;
        else left->right = nullptr;
        p->left = this;
        p->size = this->size;
        fix_size();
        return p;
    }
    Node* rotate_right(){
        Node* q = left;
        if (!q || q->isVirtual) return this;
        Node* C = left->right;
        q->right = right;
        right = nullptr;
        left = q->left->right;
        if (!left){
            left = new Node(-1,-1, true);
        }
        q->left->right = this;
        left->right = C;
        q->size = this->size;
        fix_size();
        return q;
    }

    Node* insert_root(int key, int val){
        if (!this || isVirtual){
            if (!this)
                return new Node(key, val);
            if (isVirtual){
                Node* n = new Node(key, val);
                n->right = right;
                return n;
            }
        }
        if (key<k){
            left = left->insert_root(key, val);
            return rotate_right();
        }
        else{
            left->right = left->right->insert_root(key, val);
            return rotate_left();
        }
    }
    Node* insert(int key, int val){
        if (!this || isVirtual){
            if (!this)
                return new Node(key, val);
            if (isVirtual){
                Node* n = new Node(key, val);
                n->right = right;
                return n;
            }
        }
        if (rand()%(size+1)==0) return insert_root(key, val);
        if (k>key){
            left = left->insert(key, val);
        }
        else{
            left->right = left->right->insert(key, val);
        }
        fix_size();
        return this;
    }
    void recInOrder(){
         if (this && left!=0) left->recInOrder();
         std::cout << k << " " << v << "\n";
         if (left!=0 && left->right!=0) left->right->recInOrder();
    }

    // void recRevOrder(){
    //     if (left!=0 && left->right!=0) left->right->recRevOrder();
    //     if (size!=0) std::cout << k << " " << v << "\n";
    //     if (this && left!=0 && left->size != 0) left->recRevOrder();
    // }  

    bool find(int key){
        if (key > k && left->right){
            return left->right->find(key);
        }
        if (key < k && !left->isVirtual){
            return left->find(key);
        }
        if (key == k){
            return true;
        }
        return false;
    }
    int value(int key){
        if (key > k && left->right){
            return left->right->find(key);
        }
        if (key < k && !left->isVirtual){
            return left->find(key);
        }
        if (key == k){
            return v;
        }
        return -1;
    }

        
};

class Tree{
    public:
    Node* root;
    int elements; 
    Tree(int key, int val){
        root = new Node(key, val);
        elements = 1;
    }
    void insert(int key, int val){
        root = root->insert(key, val);
        elements++;
    }


        class iterator_in {
        public:
            iterator_in(Node* r) {
                current = r;
                while (current && current->left && !current->left->isVirtual){
                    stack.push(current);
                    current = current->left;
                }
            }

            int operator*() const {
                return current->k;
            }

            iterator_in& operator++() {
                if (current->left && current->left->right){
                    stack.push(current);
                    current = current->left->right;
                    while (current->left && !current->left->isVirtual){
                        stack.push(current);
                        current = current->left;
                    }
                }
                else{
                    Node* prev = current;
                    if (stack.empty()) {
                            current = nullptr;
                            return *this;
                        }
                    current = stack.top();
                    stack.pop();
                    while (current && current->left->right == prev && !current->isVirtual){
                        prev = current;
                        if (stack.empty()) {
                            current = nullptr;
                            return *this;
                        }
                        current = stack.top();
                        stack.pop();
                    }
                }
                // if (current->isVirtual) return ++(*this);
                return *this;
            }

            bool operator!=(const iterator_in& other) const {
                return current != other.current;
            }
        
        private:
            Node* current;
            std::stack<Node*> stack;
        };
        class iterator_pre {
            std::stack<Node*> stack;
            Node* current;
            Node* end, *prev;
            public:
            iterator_pre(Node* r){
                current = r;
                while (r && r->left->right){
                    r = r->left->right;
                }
                end = r;
                stack.push(r);
                
            }
            bool operator!=(const iterator_pre& other) const {
                return current != other.current;
            }
            int operator* () const{
                return current->k;
            }
            iterator_pre& operator++() {
                
                if (current->left && !current->left->isVirtual){
                    stack.push(current);
                    current = current->left;
                    }
                else if (current->left->right){
                    stack.push(current);
                    current = current->left->right;
                }
                else if (current == end){
                    current=nullptr;
                    return *this;
                }
                else
                {
                    Node* prev = current;
                    while (current && !current->isVirtual){
                        prev = current;
                        if (stack.empty()) {
                            current = nullptr;
                            return *this;
                        }
                        current = stack.top();
                        stack.pop();
                        if (current->left == prev && prev->right){
                            stack.push(current);
                            current = prev->right;
                            break;
                        }

                        
                    }
                    
                    
                    
                }
                return *this;
            }
            

        };
        class iterator_post {
            std::stack<Node*> stack;
            Node* current;
            Node* end;
            public:
            iterator_post(Node* r){
                current = r;

                if (current && current->left){
                    while (current->left){
                        if (!current->left->isVirtual){
                        stack.push(current);
                        current = current->left;
                        }
                        else if (current->left->isVirtual && current->left->right){
                            stack.push(current);
                            current = current->left->right;
                        } 
                        else{
                            break;
                        }
                    }
                }
                end = r;
                
            }
            bool operator!=(const iterator_post& other) const {
                return current != other.current;
            }
            int operator* () const{
                return current->k;
            }
            iterator_post& operator++() {
                if (current == end){
                    current=nullptr;
                    return *this;
                }
                else
                {
                    Node* prev = nullptr;
                    while (1){
                        prev = current;
                        if (stack.empty()){
                            current = nullptr;
                            return *this;
                        }
                        current = stack.top();
                        stack.pop();
                        if (current->left == prev && current->left->right){
                            stack.push(current);
                            current = current->left->right;
                            while (current->left){
                                if (!current->left->isVirtual){
                                    stack.push(current);
                                    current = current->left;
                                }
                                else if (current->left->right){
                                    stack.push(current);
                                    current = current->left->right;
                                }
                                else{
                                    return *this;
                                }
                            }
                        }
                        else if (current->left->right == prev){
                            return *this;
                        }
                        else if (current->left == prev && !current->left->right){
                            return *this;
                        }
                   }
                }
            }
            

        };
        iterator_in begin(){ return iterator_in(root);}
        iterator_in end() { return iterator_in(nullptr);}
        iterator_pre cbegin(){return iterator_pre(root);}
        iterator_pre cend(){return iterator_pre(nullptr);}
        iterator_post rbegin(){return iterator_post(root);}
        iterator_post rend(){return iterator_post(nullptr);}
        bool find(int key){
            return root->find(key);
        }
        int value(int key){
            return root->value(key);
        }
        Tree union_(Tree B){
            auto it_ = begin();
            Tree C = Tree(*it_, value(*it_));
            ++it_;
            for (auto it = it_; it != end(); ++it){
                int key = *it;
                if (C.find(key)){
                    std::cout << "Duplicate with key " << key << "\n";
                    continue;
                }
                else{
                    C.insert(key, value(key));
                }
            }
             for (auto it = B.rbegin(); it != B.rend(); ++it){
                int key = *it;
                if (C.find(key)){
                    std::cout << "Duplicate with key " << key << "\n";
                    continue;
                }
                else{
                    C.insert(key, B.value(key));
                }
            
            }
            C.root->fix_size();
            return C;
        }
    };



    int main(int argc, char const *argv[])
    {
        srand(time(0));
        int k = rand();
        int v = rand();
        Tree A = Tree(k, v);
        int n1 = rand()%12;
        int n2 = rand()%12;
        std::cout << "\nElements in tree A: " << k << " ";
        for (int i = 0; i < n1; i++){
            k = rand();
            v = rand();
            std::cout << k << " ";
            A.insert(k, v);
        }
        std::cout<<"\n";
        k = rand();
        v = rand();
        Tree B = Tree(k, v);
        std::cout << "\nElements in tree B: " << k << " ";
        for (int i = 0; i < n2; i++){
            int k = rand();
            int v = rand();
            std::cout << k << " ";
            B.insert(k, v);
        }

        std::cout << "\nPreorder Tree A with " << A.elements << " elements \n";
        for (auto it = A.cbegin(); it != A.cend(); ++it){
                int key = *it;
                std::cout << key<< " ";
        }
        std::cout << "\nInorder Tree B with " << B.elements << " elements \n";
        for (auto key : B){
            std::cout << key << " ";
        }
        std::cout << "\n";
        A = A.union_(B);
        std::cout << "\nUnion testing Tree A (postorder) with " << A.elements << " elements \n";
        for (auto it = A.rbegin(); it != A.rend(); ++it){
            int key = *it;
            std::cout << key << " ";
        }
        return 0;
    }
    