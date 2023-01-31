#ifndef AVLTREE_H
#define AVLTREE_H
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#define DISTANCE 10


template <typename Key, typename Info>
class avltree{
private:
    struct Node{
        int height;
        Key key;
        Info info;
        Node* left;
        Node* right;
    };
    Node* root;

    int height(Node* node){
        if(node == nullptr) //node without child has height of -1
            return -1;
        else{
            int lheight = height(node->left);
            int rheight = height(node->right);
            if(lheight > rheight)
                return (lheight + 1);
            else
                return (rheight + 1);
        }
    }

    int getBalanceFactor(Node* node) { //balance factor = left subtree - right subtree
        if(node == nullptr)
            return -1;
        else
            return (height(node->left) - height(node->right));
    }

    Node* rightRotation(Node* x){
        Node* temp = x->left;
        if(x == root){
            root = temp;
        }
        x->left = temp->right;
        temp->right = x;
        return temp;
    }

    Node* leftRotation(Node* x){
        Node* temp = x->right;
        if(x == root){
            root = temp;
        }
        x->right = temp->left;
        temp->left = x;
        return temp;
    }

    /***************Recursive functions***************/
    Node* insert(Node*& r, Node* new_node) {
        if (r == nullptr) {
            return new_node;
        }
        if (new_node->key < r->key)
            r->left = insert(r->left, new_node);

        else if (new_node->key > r->key)
            r->right = insert(r->right, new_node);
        else{
            std::cout << "No duplicates are allowed!" << std::endl;
            return r;
        }

        // Balance the tree
        int bf = getBalanceFactor(r);
        if(bf > 1 && new_node->key < r->left->key)
            return rightRotation(r);
        else if(bf < -1 && new_node->key > r->right->key)
            return leftRotation(r);
        else if(bf > 1 && new_node->key > r->left->key) {
            r->left = leftRotation(r->left);
            return rightRotation(r);
        }
        else if(bf < -1 && new_node->key < r->right->key){
            r->right = rightRotation(r->right);
            return leftRotation(r);
        }
        return r;
    }

    Node* deleteNode(Node*& r, Key key){

        if(r == nullptr)
            return r;
        if(key < r->key)
            r->left = deleteNode(r->left, key);
        else if(key > r->key)
            r->right = deleteNode(r->right, key);
        else{ // r->key == key
            if(r->left == nullptr){ //if r has no left child or both children
                Node* temp = r->right;
                delete r;
                return temp;
            }
            else if(r->right == nullptr){ //if r has no right child
                Node* temp = r->left;
                delete r;
                return temp;
            }
            else{ //if r has both children
                Node* temp = r->right;
                while(temp->left != nullptr) //finds the most left node of the right subtree of r
                    temp = temp->left;       //and replaces r with it(temp)
                r->key = temp->key;
                r->info = temp->info;
                r->right = deleteNode(r->right, temp->key);
            }

        }

        // Balance the tree
        int bf = getBalanceFactor(r);
        if(bf > 1 && getBalanceFactor(r->left) >= 0)
            return rightRotation(r);
        else if(bf > 1 && getBalanceFactor(r->left) < 0){
            r->left = leftRotation(r->left);
            return rightRotation(r);
        }
        else if(bf < -1 && getBalanceFactor(r->right) <= 0)
            return leftRotation(r);
        else if(bf < -1 && getBalanceFactor(r->right) > 0){
            r->right = rightRotation(r->right);
            return leftRotation(r);
        }
        return r;
    }

    Node* search(Node* r, Key key) const {
        if(r == nullptr)
            return nullptr;
        if(r->key == key)
            return r;
        if(r->key > key)
            return search(r->left, key);
        else
            return search(r->right, key);
    }

    Node* clear(Node*& r){
        if(r != nullptr){
            clear(r->left);
            clear(r->right);
            delete r;
            r = nullptr;
        }
        return r;
    }

    Node* copy_tree(Node* r){  //creates a deep copy of the tree
        Node* new_node = new Node;
        if(r == nullptr)
            return (new_node = nullptr);
        else{
            new_node->key = r->key;
            new_node->info = r->info;
            new_node->left = copy_tree(r->left);
            new_node->right = copy_tree(r->right);
            return new_node;
        }

    }

    Node* max_info(Node* r) const {
        if(r == nullptr)
            return nullptr;
        Node* max = r;
        Node* temp = max_info(r->left);
        if(temp != nullptr && temp->info > max->info)
            max = temp;
        temp = max_info(r->right);
        if(temp != nullptr && temp->info > max->info)
            max = temp;
        return max;
    }

    void display(Node* r, int distance) const {
        if(r == nullptr)
            return;
        distance += DISTANCE;
        display(r->right, distance);
        std::cout << std::endl;
        for(int i = DISTANCE; i < distance; i++)
            std::cout << ' ';
        std::cout << r->key << ' ';
        display(r->left, distance);
    }

    void postOrder(Node* r) const {
        if(r == nullptr)
            return;
        postOrder(r->left);
        postOrder(r->right);
        std::cout << r->key << ' ';
    }

public:
    avltree();
    ~avltree();
    avltree(const avltree& rhs);
    avltree& operator=(const avltree& rhs);

    void insert(Key key, Info info);
    bool isEmpty() const;
    bool search(Key key) const;
    void deleteNode(Key key);
    void clear();
    void display() const;
    void postOrder() const;
    Node* create(){
        Node* new_node = new Node;
        return new_node;
    }

    Info& operator[](Key key);
}; // End of class avltree

template <typename Key, typename Info>
avltree<Key, Info>::avltree() {
    root = nullptr;
}

template <typename Key, typename Info>
avltree<Key, Info>::~avltree() {
    clear();
}

template <typename Key, typename Info>
avltree<Key, Info>::avltree(const avltree& rhs){
    this->root = copy_tree(rhs.root);
}

template <typename Key, typename Info>
avltree<Key, Info>& avltree<Key, Info>::operator=(const avltree<Key, Info>& rhs) {
    if(this != &rhs){
        this->clear();
        Node* temp = rhs.root;
        this->root = copy_tree(temp);
    }
    return *this;
}

template <typename Key, typename Info>
void avltree<Key, Info>::insert(Key key, Info info) {
    Node *new_node = new Node;
    new_node->key = key;
    new_node->info = info;
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->height = 0;
    if(root == nullptr){
        root = new_node;
        return;
    }
//    if(new_node == insert(root, new_node))
//        delete new_node;
    insert(root, new_node);
}

template <typename Key, typename Info>
void avltree<Key, Info>::display() const {
    display(root, 5);
}

template <typename Key, typename Info>
void avltree<Key, Info>::postOrder() const {
    postOrder(root);
}

template <typename Key, typename Info>
bool avltree<Key, Info>::search(Key key) const {
    if(search(root, key) == nullptr)
        return false;
    else
        return true;
}

template <typename Key, typename Info>
void avltree<Key, Info>::clear() {
    clear(root);
}

template <typename Key, typename Info>
void avltree<Key, Info>::deleteNode(Key key) {
    deleteNode(root, key);
}

template <typename Key, typename Info>
bool avltree<Key, Info>::isEmpty() const {
    if(root == nullptr)
        return true;
    else
        return false;
}

template <typename Key, typename Info>
Info& avltree<Key, Info>::operator[](Key key) {
    Node* temp = search(root, key);
    if(temp == nullptr){
        insert(key, Info());
        temp = search(root, key);
    }
    return temp->info;
}

// function that counts the number of words in a file and returns an avltree with the words and their frequency
template <typename Key, typename Info>
avltree<Key, Info> count_words(std::istream& is){
    avltree<Key, Info> tree;
    std::string word;

    if(!is.good()){ // if the file is not opened correctly
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }
    while(!is.eof()){ // while the file is not finished reading the words
        is >> word;
        if(tree.search(word))
            tree[word] = tree[word] + 1; // if the word is already in the tree, increase its frequency by 1
        else
            tree.insert(word, 1); // if the word is not in the tree, insert it
    }
    return tree;
}

#endif //AVLTREE_H
