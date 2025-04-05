#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <fstream>

using namespace std;
using namespace chrono;

struct Node {
    int key;
    Node* left;
    Node* right;
    int height;

    Node(int value) : key(value), left(nullptr), right(nullptr), height(1) {}
};

class BinarySearchTree {
public:
    Node* root;

    BinarySearchTree() : root(nullptr) {}

    void insert(int key) {
        root = insertRec(root, key);
    }

    Node* insertRec(Node* node, int key) {
        if (node == nullptr) return new Node(key);

        if (key < node->key)
            node->left = insertRec(node->left, key);
        else if (key > node->key)
            node->right = insertRec(node->right, key);

        return node;
    }

    bool search(int key) {
        return searchRec(root, key);
    }

    bool searchRec(Node* node, int key) {
        if (node == nullptr) return false;
        if (key == node->key) return true;
        if (key < node->key) return searchRec(node->left, key);
        return searchRec(node->right, key);
    }

    void remove(int key) {
        root = removeRec(root, key);
    }

    Node* removeRec(Node* node, int key) {
        if (node == nullptr) return node;

        if (key < node->key)
            node->left = removeRec(node->left, key);
        else if (key > node->key)
            node->right = removeRec(node->right, key);
        else {
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            Node* temp = minValueNode(node->right);
            node->key = temp->key;
            node->right = removeRec(node->right, temp->key);
        }
        return node;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr) current = current->left;
        return current;
    }

    void preOrder(Node* root) {
        if (root != nullptr) {
            cout << root->key << " ";
            preOrder(root->left);
            preOrder(root->right);
        }
    }

    // Очистка памяти
    void clear() {
        root = clearRec(root);
    }

    Node* clearRec(Node* node) {
        if (node == nullptr) return nullptr;
        node->left = clearRec(node->left);
        node->right = clearRec(node->right);
        delete node;
        return nullptr;
    }
};

class AVLTree {
public:
    Node* root;

    AVLTree() : root(nullptr) {}

    void insert(int key) {
        root = insertRec(root, key);
    }

    Node* insertRec(Node* node, int key) {
        if (node == nullptr) return new Node(key);

        if (key < node->key)
            node->left = insertRec(node->left, key);
        else if (key > node->key)
            node->right = insertRec(node->right, key);
        else
            return node;

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;
        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;
        return y;
    }

    int height(Node* node) {
        if (node == nullptr) return 0;
        return node->height;
    }

    int getBalance(Node* node) {
        if (node == nullptr) return 0;
        return height(node->left) - height(node->right);
    }

    bool search(int key) {
        return searchRec(root, key);
    }

    bool searchRec(Node* node, int key) {
        if (node == nullptr) return false;
        if (key == node->key) return true;
        if (key < node->key) return searchRec(node->left, key);
        return searchRec(node->right, key);
    }

    void remove(int key) {
        root = removeRec(root, key);
    }

    Node* removeRec(Node* node, int key) {
        if (node == nullptr) return node;

        if (key < node->key)
            node->left = removeRec(node->left, key);
        else if (key > node->key)
            node->right = removeRec(node->right, key);
        else {
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            Node* temp = minValueNode(node->right);
            node->key = temp->key;
            node->right = removeRec(node->right, temp->key);
        }
        return node;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr) current = current->left;
        return current;
    }

    void preOrder(Node* root) {
        if (root != nullptr) {
            cout << root->key << " ";
            preOrder(root->left);
            preOrder(root->right);
        }
    }

    // Очистка памяти
    void clear() {
        root = clearRec(root);
    }

    Node* clearRec(Node* node) {
        if (node == nullptr) return nullptr;
        node->left = clearRec(node->left);
        node->right = clearRec(node->right);
        delete node;
        return nullptr;
    }
};

void generateRandomArray(vector<int>& arr, int size) {
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 10000; // Генерация случайных чисел
    }
}

void generateSortedArray(vector<int>& arr, int size) {
    for (int i = 0; i < size; ++i) {
        arr[i] = i;
    }
}

bool linearSearch(const vector<int>& arr, int key) {
    for (int num : arr) {
        if (num == key) {
            return true;
        }
    }
    return false;
}


int main() {
    srand(time(0));  // Инициализация генератора случайных чисел
    
    ofstream outfile("output.txt");

    for (int i = 0; i < 20; ++i) {  // 20 циклов для каждой серии
        int n = 1 << (10 + (i % 10));  // Размер массива 2^(10 + i)

        vector<int> arr(n);
        bool isRandom = i < 10;  // Первая половина серий с случайными данными

        if (isRandom) {
            generateRandomArray(arr, n);  // Заполняем случайными числами
        } else {
            generateSortedArray(arr, n);  // Заполняем отсортированными числами
        }

        auto start1 = high_resolution_clock::now();
        for (int j = 0; j < 1000; ++j) {
            linearSearch(arr, searchKey);
        }
        auto end1 = high_resolution_clock::now();
        auto durationLinearSearch1 = duration_cast<microseconds>(end1 - start1);
        cout << "Series " << i+1 << " (Linear Search) - Time: " << durationLinearSearch1.count() << " microseconds, per operation: " << durationLinearSearch1.count() / 1000.0 << " microseconds" << endl;
        outfile << "Series " << i+1 << " (Linear Search) - Time: " << durationLinearSearch1.count() << " microseconds, per operation: " << durationLinearSearch1.count() / 1000.0 << " microseconds" << endl;


        // 1. Замер времени для Binary Search Tree
        BinarySearchTree bst;
        auto start = high_resolution_clock::now();
        for (int num : arr) {
            bst.insert(num);
        }
        auto end = high_resolution_clock::now();
        auto durationBST = duration_cast<microseconds>(end - start);
        cout << "Series " << i+1 << " (BST Insert) - Time: " << durationBST.count() << " microseconds" << endl;
        outfile << "Series " << i+1 << " (BST Insert) - Time: " << durationBST.count() << " microseconds" << endl;
        cout << "2." << endl;
        // 2. Замер времени для AVL Tree
        AVLTree avl;
        start = high_resolution_clock::now();
        for (int num : arr) {
            avl.insert(num);
        }
        end = high_resolution_clock::now();
        auto durationAVL = duration_cast<microseconds>(end - start);
        cout << "Series " << i+1 << " (AVL Insert) - Time: " << durationAVL.count() << " microseconds" << endl;
        outfile << "Series " << i+1 << " (AVL Insert) - Time: " << durationAVL.count() << " microseconds" << endl;
        
        // 3. Замер времени поиска в Binary Search Tree
        int searchKey = rand() % 10000;  // Случайный ключ для поиска
        start = high_resolution_clock::now();
        for (int j = 0; j < 1000; ++j) {
            bst.search(searchKey);
        }
        end = high_resolution_clock::now();
        auto durationBSTSearch = duration_cast<microseconds>(end - start);
        cout << "Series " << i+1 << " (BST Search) - Time: " << durationBSTSearch.count() << " microseconds, per operation: " << durationBSTSearch.count() / 1000.0 << " microseconds" << endl;
        outfile << "Series " << i+1 << " (BST Search) - Time: " << durationBSTSearch.count() << " microseconds, per operation: " << durationBSTSearch.count() / 1000.0 << " microseconds" << endl;

        // 4. Замер времени поиска в AVL Tree
        start = high_resolution_clock::now();
        for (int j = 0; j < 1000; ++j) {
            avl.search(searchKey);
        }
        end = high_resolution_clock::now();
        auto durationAVLSearch = duration_cast<microseconds>(end - start);
        cout << "Series " << i+1 << " (AVL Search) - Time: " << durationAVLSearch.count() << " microseconds, per operation: " << durationAVLSearch.count() / 1000.0 << " microseconds" << endl;
        outfile << "Series " << i+1 << " (AVL Search) - Time: " << durationAVLSearch.count() << " microseconds, per operation: " << durationAVLSearch.count() / 1000.0 << " microseconds" << endl;

        // 5. Замер времени удаления из Binary Search Tree
        start = high_resolution_clock::now();
        for (int j = 0; j < 1000; ++j) {
            bst.remove(searchKey);
        }
        end = high_resolution_clock::now();
        auto durationBSTRemove = duration_cast<microseconds>(end - start);
        cout << "Series " << i+1 << " (BST Remove) - Time: " << durationBSTRemove.count() << " microseconds, per operation: " << durationBSTRemove.count() / 1000.0 << " microseconds" << endl;
        outfile << "Series " << i+1 << " (BST Remove) - Time: " << durationBSTRemove.count() << " microseconds, per operation: " << durationBSTRemove.count() / 1000.0 << " microseconds" << endl;

        // 6. Замер времени удаления из AVL Tree
        start = high_resolution_clock::now();
        for (int j = 0; j < 1000; ++j) {
            avl.remove(searchKey);
        }
        end = high_resolution_clock::now();
        auto durationAVLRemove = duration_cast<microseconds>(end - start);
        cout << "Series " << i+1 << " (AVL Remove) - Time: " << durationAVLRemove.count() << " microseconds, per operation: " << durationAVLRemove.count() / 1000.0 << " microseconds" << endl;
        outfile << "Series " << i+1 << " (AVL Remove) - Time: " << durationAVLRemove.count() << " microseconds, per operation: " << durationAVLRemove.count() / 1000.0 << " microseconds" << endl;
        
        // Очищаем память
        bst.clear();
        avl.clear();
    }
    outfile.close();

    return 0;
}
