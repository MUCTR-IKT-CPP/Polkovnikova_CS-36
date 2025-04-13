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

// Узел дерева
struct Node {
    int key;
    Node* left;
    Node* right;
    int size; // Размер поддерева
    
    Node(int k) : key(k), left(nullptr), right(nullptr), size(1) {}
};

// Рандомизированное бинарное дерево поиска
class RandomizedBST {
private:
    Node* root;
    mt19937 gen;
    
    // Обновление размера поддерева
    void updateSize(Node* node) {
        if (node) {
            node->size = 1 + getSize(node->left) + getSize(node->right);
        }
    }
    
    // Получение размера поддерева
    int getSize(Node* node) const {
        return node ? node->size : 0;
    }
    
    // Правый поворот
    Node* rotateRight(Node* node) {
        Node* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        updateSize(node);
        updateSize(newRoot);
        return newRoot;
    }
    
    // Левый поворот
    Node* rotateLeft(Node* node) {
        Node* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        updateSize(node);
        updateSize(newRoot);
        return newRoot;
    }
    
    // Вставка в корень
    Node* insertRoot(Node* node, int key) {
        if (!node) return new Node(key);
        
        if (key < node->key) {
            node->left = insertRoot(node->left, key);
            return rotateRight(node);
        } else {
            node->right = insertRoot(node->right, key);
            return rotateLeft(node);
        }
    }
    
    // Слияние двух деревьев
    Node* merge(Node* left, Node* right) {
        if (!left) return right;
        if (!right) return left;
        
        if (uniform_int_distribution<>(0, getSize(left) + getSize(right) - 1)(gen) < getSize(left)) {
            left->right = merge(left->right, right);
            updateSize(left);
            return left;
        } else {
            right->left = merge(left, right->left);
            updateSize(right);
            return right;
        }
    }
    
    // Рекурсивное удаление
    Node* remove(Node* node, int key) {
        if (!node) return nullptr;
        
        if (key == node->key) {
            Node* result = merge(node->left, node->right);
            delete node;
            return result;
        } else if (key < node->key) {
            node->left = remove(node->left, key);
        } else {
            node->right = remove(node->right, key);
        }
        
        updateSize(node);
        return node;
    }
    
    // Поиск узла
    Node* search(Node* node, int key) const {
        if (!node) return nullptr;
        if (key == node->key) return node;
        return key < node->key ? search(node->left, key) : search(node->right, key);
    }
    
    // Максимальная глубина
    int maxDepth(Node* node) const {
        if (!node) return 0;
        return 1 + max(maxDepth(node->left), maxDepth(node->right));
    }
    
    // Сбор глубин всех веток
    void collectBranchDepths(Node* node, int currentDepth, vector<int>& depths) const {
        if (!node) return;
        
        if (!node->left && !node->right) {
            depths.push_back(currentDepth + 1);
            return;
        }
        
        collectBranchDepths(node->left, currentDepth + 1, depths);
        collectBranchDepths(node->right, currentDepth + 1, depths);
    }
    
public:
    RandomizedBST() : root(nullptr), gen(random_device{}()) {}
    
    // Вставка элемента
    void insert(int key) {
        if (!root) {
            root = new Node(key);
            return;
        }
        
        // С вероятностью 1/(n+1) вставляем в корень
        if (uniform_int_distribution<>(0, getSize(root))(gen) == 0) {
            root = insertRoot(root, key);
        } else {
            Node* node = root;
            Node* parent = nullptr;
            
            while (node) {
                parent = node;
                if (key < node->key) {
                    node = node->left;
                } else {
                    node = node->right;
                }
            }
            
            if (key < parent->key) {
                parent->left = new Node(key);
            } else {
                parent->right = new Node(key);
            }
            
            // Обновляем размеры на пути к корню
            node = root;
            while (node) {
                updateSize(node);
                node = key < node->key ? node->left : node->right;
            }
        }
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

// Функция для тестирования
void testRandomizedBST() {
    const int REPETITIONS = 50;
    const int OPERATIONS = 1000;
    
    for (int i = 10; i <= 18; ++i) {
        const size_t N = 1 << i; // 2^i
        
        vector<double> maxDepths;
        vector<double> insertTimes;
        vector<double> deleteTimes;
        vector<double> searchTimes;
        vector<int> allBranchDepths;
        
        cout << "Testing N = 2^" << i << " = " << N << "..." << endl;
        
        for (int rep = 0; rep < REPETITIONS; ++rep) {
            RandomizedBST tree;
            
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

        // Для последней серии тестов (N=262144) сохраняем сырые данные
        if (i == 18) {
            // 1. Гистограмма максимальных высот
            map<int, int> maxDepthHist;
            int binSize = 2; // Размер бина для группировки
            
            for (double depth : maxDepths) {
                int bin = static_cast<int>(depth) / binSize * binSize;
                maxDepthHist[bin]++;
            }
            
            ofstream maxDepthFile("max_depths.csv");
            maxDepthFile << "Depth Range,Count\n";
            for (const auto& [depth, count] : maxDepthHist) {
                maxDepthFile << depth << "-" << (depth + binSize - 1) << "," << count << "\n";
            }
            
            // 2. Гистограмма высот веток
            map<int, int> branchDepthHist;
            int branchBinSize = 2; // Размер бина для группировки
            
            for (int depth : allBranchDepths) {
                int bin = depth / branchBinSize * branchBinSize;
                branchDepthHist[bin]++;
            }
            
            ofstream branchDepthFile("branch_depths.csv");
            branchDepthFile << "Depth Range,Count\n";
            for (const auto& [depth, count] : branchDepthHist) {
                branchDepthFile << depth << "-" << (depth + branchBinSize - 1) << "," << count << "\n";
            }
            
            cout << "Histogram data saved to max_depths.csv and branch_depths.csv" << endl;
        }
    }
}

int main() {
    srand(time(nullptr)); // Инициализация генератора случайных чисел
    testRandomizedBST();
    return 0;
}
