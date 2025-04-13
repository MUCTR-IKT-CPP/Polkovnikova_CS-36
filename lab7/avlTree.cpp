#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <climits>
#include <fstream>
#include <map>

using namespace std;
using namespace std::chrono;

// Узел AVL-дерева
struct AVLNode {
    int key;
    AVLNode* left;
    AVLNode* right;
    int height;
    
    AVLNode(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

// AVL-дерево
class AVLTree {
private:
    AVLNode* root;
    
    // Получение высоты узла
    int getHeight(AVLNode* node) const {
        return node ? node->height : 0;
    }
    
    // Обновление высоты узла
    void updateHeight(AVLNode* node) {
        if (node) {
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        }
    }
    
    // Получение баланс-фактора
    int getBalance(AVLNode* node) const {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }
    
    // Правый поворот
    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        
        x->right = y;
        y->left = T2;
        
        updateHeight(y);
        updateHeight(x);
        
        return x;
    }
    
    // Левый поворот
    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        
        y->left = x;
        x->right = T2;
        
        updateHeight(x);
        updateHeight(y);
        
        return y;
    }
    
    // Балансировка узла
    AVLNode* balance(AVLNode* node) {
        if (!node) return nullptr;
        
        updateHeight(node);
        int balanceFactor = getBalance(node);
        
        // Лево-левый случай
        if (balanceFactor > 1 && getBalance(node->left) >= 0) {
            return rotateRight(node);
        }
        
        // Право-правый случай
        if (balanceFactor < -1 && getBalance(node->right) <= 0) {
            return rotateLeft(node);
        }
        
        // Лево-правый случай
        if (balanceFactor > 1 && getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        
        // Право-левый случай
        if (balanceFactor < -1 && getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        
        return node;
    }
    
    // Вставка узла
    AVLNode* insert(AVLNode* node, int key) {
        if (!node) return new AVLNode(key);
        
        if (key < node->key) {
            node->left = insert(node->left, key);
        } else if (key > node->key) {
            node->right = insert(node->right, key);
        } else {
            return node; // Дубликаты не допускаются
        }
        
        return balance(node);
    }
    
    // Поиск узла с минимальным ключом
    AVLNode* findMin(AVLNode* node) const {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }
    
    // Удаление узла
    AVLNode* remove(AVLNode* node, int key) {
        if (!node) return nullptr;
        
        if (key < node->key) {
            node->left = remove(node->left, key);
        } else if (key > node->key) {
            node->right = remove(node->right, key);
        } else {
            if (!node->left || !node->right) {
                AVLNode* temp = node->left ? node->left : node->right;
                
                if (!temp) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                
                delete temp;
            } else {
                AVLNode* temp = findMin(node->right);
                node->key = temp->key;
                node->right = remove(node->right, temp->key);
            }
        }
        
        if (!node) return nullptr;
        
        return balance(node);
    }
    
    // Поиск узла
    AVLNode* search(AVLNode* node, int key) const {
        if (!node) return nullptr;
        if (key == node->key) return node;
        return key < node->key ? search(node->left, key) : search(node->right, key);
    }
    
    // Максимальная глубина
    int maxDepth(AVLNode* node) const {
        if (!node) return 0;
        return 1 + max(maxDepth(node->left), maxDepth(node->right));
    }
    
    // Сбор глубин всех веток
    void collectBranchDepths(AVLNode* node, int currentDepth, vector<int>& depths) const {
        if (!node) return;
        
        if (!node->left && !node->right) {
            depths.push_back(currentDepth + 1);
            return;
        }
        
        collectBranchDepths(node->left, currentDepth + 1, depths);
        collectBranchDepths(node->right, currentDepth + 1, depths);
    }
    
public:
    AVLTree() : root(nullptr) {}
    
    // Вставка элемента
    void insert(int key) {
        root = insert(root, key);
    }
    
    // Удаление элемента
    void remove(int key) {
        root = remove(root, key);
    }
    
    // Поиск элемента
    bool contains(int key) const {
        return search(root, key) != nullptr;
    }
    
    // Получение максимальной глубины
    int getMaxDepth() const {
        return maxDepth(root);
    }
    
    // Получение глубин всех веток
    vector<int> getAllBranchDepths() const {
        vector<int> depths;
        collectBranchDepths(root, 0, depths);
        return depths;
    }
    
    // Очистка дерева
    void clear() {
        while (root) {
            remove(root->key);
        }
    }
};

// Функция для тестирования AVL-дерева
void testAVLTree() {
    const int REPETITIONS = 50;
    const int OPERATIONS = 1000;
    
    for (int i = 10; i <= 18; ++i) {
        const size_t N = 1 << i; // 2^i
        
        vector<double> maxDepths;
        vector<double> insertTimes;
        vector<double> deleteTimes;
        vector<double> searchTimes;
        vector<int> allBranchDepths;
        
        cout << "Testing AVL Tree with N = 2^" << i << " = " << N << "..." << endl;
        
        for (int rep = 0; rep < REPETITIONS; ++rep) {
            AVLTree tree;
            
            // 1. Генерация N случайных значений
            vector<int> elements(N);
            for (auto& elem : elements) {
                elem = rand() % (10 * N);
            }
            
            // 2. Заполнение дерева
            for (int elem : elements) {
                tree.insert(elem);
            }
            
            // 3. Замер максимальной глубины
            maxDepths.push_back(tree.getMaxDepth());
            
            // 4. 1000 операций вставки и замер времени
            auto start = high_resolution_clock::now();
            for (int j = 0; j < OPERATIONS; ++j) {
                int elem = rand() % (10 * N);
                tree.insert(elem);
            }
            auto end = high_resolution_clock::now();
            insertTimes.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);
            
            // 5. 1000 операций удаления и замер времени
            start = high_resolution_clock::now();
            for (int j = 0; j < OPERATIONS; ++j) {
                int elem = elements.empty() ? rand() % (10 * N) : 
                          (rand() % 2 ? elements[rand() % elements.size()] : rand() % (10 * N));
                tree.remove(elem);
            }
            end = high_resolution_clock::now();
            deleteTimes.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);
            
            // 6. 1000 операций поиска и замер времени
            start = high_resolution_clock::now();
            for (int j = 0; j < OPERATIONS; ++j) {
                int elem = elements.empty() ? rand() % (10 * N) : 
                          (rand() % 2 ? elements[rand() % elements.size()] : rand() % (10 * N));
                tree.contains(elem);
            }
            end = high_resolution_clock::now();
            searchTimes.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);
            
            // 7. Сбор глубин всех веток
            vector<int> depths = tree.getAllBranchDepths();
            allBranchDepths.insert(allBranchDepths.end(), depths.begin(), depths.end());
        }
        
        // Вычисление статистики
        double avgMaxDepth = accumulate(maxDepths.begin(), maxDepths.end(), 0.0) / maxDepths.size();
        double avgInsertTime = accumulate(insertTimes.begin(), insertTimes.end(), 0.0) / insertTimes.size();
        double avgDeleteTime = accumulate(deleteTimes.begin(), deleteTimes.end(), 0.0) / deleteTimes.size();
        double avgSearchTime = accumulate(searchTimes.begin(), searchTimes.end(), 0.0) / searchTimes.size();
        
        double avgBranchDepth = accumulate(allBranchDepths.begin(), allBranchDepths.end(), 0.0) / allBranchDepths.size();
        int minBranchDepth = *min_element(allBranchDepths.begin(), allBranchDepths.end());
        int maxBranchDepth = *max_element(allBranchDepths.begin(), allBranchDepths.end());
        
        // Вывод результатов
        cout << "Results for N = " << N << ":" << endl;
        cout << "  Average max depth: " << avgMaxDepth << " (expected ~" << log2(N) << ")" << endl;
        cout << "  Average insert time for " << OPERATIONS << " ops: " << avgInsertTime << " ms" << endl;
        cout << "  Average delete time for " << OPERATIONS << " ops: " << avgDeleteTime << " ms" << endl;
        cout << "  Average search time for " << OPERATIONS << " ops: " << avgSearchTime << " ms" << endl;
        cout << "  Branch depths - avg: " << avgBranchDepth << ", min: " << minBranchDepth 
             << ", max: " << maxBranchDepth << endl;
        cout << endl;

        if (i == 18) {
            // 1. Сохраняем все значения максимальных глубин (а не гистограмму)
            ofstream maxDepthFile("avl_max_depths.csv");
            maxDepthFile << "MaxDepth\n";
            for (double depth : maxDepths) {
                maxDepthFile << depth << "\n";
            }
            
            // 2. Гистограмма высот веток с автоматическим подбором бинов
            vector<int> branchDepths = allBranchDepths;
            if (!branchDepths.empty()) {
                int min_depth = *min_element(branchDepths.begin(), branchDepths.end());
                int max_depth = *max_element(branchDepths.begin(), branchDepths.end());
                int range = max_depth - min_depth;
                int bin_size = max(1, range / 15); // Автоподбор размера бина
                
                map<int, int> branchDepthHist;
                for (int depth : branchDepths) {
                    int bin = (depth - min_depth) / bin_size * bin_size + min_depth;
                    branchDepthHist[bin]++;
                }
                
                ofstream branchDepthFile("avl_branch_depths.csv");
                branchDepthFile << "Depth Range,Count\n";
                for (const auto& [depth, count] : branchDepthHist) {
                    branchDepthFile << depth << "-" << (depth + bin_size - 1) << "," << count << "\n";
                }
            }
            
            cout << "AVL Tree data saved to avl_max_depths.csv and avl_branch_depths.csv" << endl;
        }
    }
}

int main() {
    srand(time(nullptr)); // Инициализация генератора случайных чисел
    testAVLTree();
    return 0;
}
