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

// Рандомизированное бинарное дерево поиска для отсортированных данных
class SortedRandomizedBST {
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
    
    // Оптимальное построение дерева из отсортированного массива
    Node* buildBalancedFromSortedArray(const vector<int>& sortedArray, int start, int end) {
        if (start > end) return nullptr;
        
        // Выбираем случайный корень для поддержания рандомизации
        int mid = start + uniform_int_distribution<>(0, end - start)(gen);
        Node* node = new Node(sortedArray[mid]);
        
        node->left = buildBalancedFromSortedArray(sortedArray, start, mid - 1);
        node->right = buildBalancedFromSortedArray(sortedArray, mid + 1, end);
        
        updateSize(node);
        return node;
    }
    
public:
    SortedRandomizedBST() : root(nullptr), gen(random_device{}()) {}
    
    // Вставка элемента (всегда в конец для отсортированных данных)
    void insertSorted(int key) {
        if (!root) {
            root = new Node(key);
            return;
        }
        
        // Для отсортированных данных всегда вставляем в правое поддерево
        Node* node = root;
        Node* parent = nullptr;
        
        while (node) {
            parent = node;
            node = node->right;
        }
        
        parent->right = new Node(key);
        
        // Обновляем размеры на пути к корню
        node = root;
        while (node) {
            updateSize(node);
            node = node->right;
        }
        
        // С вероятностью 1/(n+1) делаем вставку в корень
        if (uniform_int_distribution<>(0, getSize(root))(gen) == 0) {
            root = insertRoot(root, key);
        }
    }
    
    // Построение дерева из отсортированного массива
    void buildFromSortedArray(const vector<int>& sortedArray) {
        clear();
        if (!sortedArray.empty()) {
            root = buildBalancedFromSortedArray(sortedArray, 0, sortedArray.size() - 1);
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

// Функция для тестирования с отсортированными данными
void testSortedRandomizedBST() {
    const int REPETITIONS = 50;
    const int OPERATIONS = 1000;
    
    for (int i = 10; i <= 18; ++i) {
        const size_t N = 1 << i; // 2^i
        
        vector<double> maxDepths;
        vector<double> buildTimes;
        vector<double> insertTimes;
        vector<double> deleteTimes;
        vector<double> searchTimes;
        vector<int> allBranchDepths;
        
        // cout << "Testing Sorted Randomized BST with N = 2^" << i << " = " << N << "..." << endl;
        
        for (int rep = 0; rep < REPETITIONS; ++rep) {
            SortedRandomizedBST tree;
            
            // 1. Генерация отсортированных значений
            vector<int> elements(N);
            for (int j = 0; j < N; ++j) {
                elements[j] = j; // Упорядоченная последовательность
            }
            
            // 2. Оптимальное построение дерева из отсортированного массива
            auto start = high_resolution_clock::now();
            tree.buildFromSortedArray(elements);
            auto end = high_resolution_clock::now();
            buildTimes.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);
            
            // 3. Замер максимальной глубины
            maxDepths.push_back(tree.getMaxDepth());
            
            // 4. 1000 операций вставки в конец и замер времени
            start = high_resolution_clock::now();
            for (int j = 0; j < OPERATIONS; ++j) {
                tree.insertSorted(N + j); // Вставляем элементы больше всех существующих
            }
            end = high_resolution_clock::now();
            insertTimes.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);
            
            // 5. 1000 операций удаления и замер времени
            start = high_resolution_clock::now();
            for (int j = 0; j < OPERATIONS; ++j) {
                int elem = elements.empty() ? N + j : 
                          elements[rand() % elements.size()];
                tree.remove(elem);
                if (!elements.empty()) {
                    elements.erase(remove(elements.begin(), elements.end(), elem), elements.end());
                }
            }
            end = high_resolution_clock::now();
            deleteTimes.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);
            
            // 6. 1000 операций поиска и замер времени
            start = high_resolution_clock::now();
            for (int j = 0; j < OPERATIONS; ++j) {
                int elem = elements.empty() ? N + j : 
                          elements[rand() % elements.size()];
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
        double avgBuildTime = accumulate(buildTimes.begin(), buildTimes.end(), 0.0) / buildTimes.size();
        double avgInsertTime = accumulate(insertTimes.begin(), insertTimes.end(), 0.0) / insertTimes.size();
        double avgDeleteTime = accumulate(deleteTimes.begin(), deleteTimes.end(), 0.0) / deleteTimes.size();
        double avgSearchTime = accumulate(searchTimes.begin(), searchTimes.end(), 0.0) / searchTimes.size();
        
        double avgBranchDepth = accumulate(allBranchDepths.begin(), allBranchDepths.end(), 0.0) / allBranchDepths.size();
        int minBranchDepth = *min_element(allBranchDepths.begin(), allBranchDepths.end());
        int maxBranchDepth = *max_element(allBranchDepths.begin(), allBranchDepths.end());
        
        // Вывод результатов
        /*cout << "Results for sorted N = " << N << ":" << endl;
        cout << "  Average build time: " << avgBuildTime << " ms" << endl;
        cout << "  Average max depth: " << avgMaxDepth << " (expected ~" << log2(N) << ")" << endl;
        cout << "  Average insert time for " << OPERATIONS << " ops: " << avgInsertTime << " ms" << endl;
        cout << "  Average delete time for " << OPERATIONS << " ops: " << avgDeleteTime << " ms" << endl;
        cout << "  Average search time for " << OPERATIONS << " ops: " << avgSearchTime << " ms" << endl;
        cout << "  Branch depths - avg: " << avgBranchDepth << ", min: " << minBranchDepth 
             << ", max: " << maxBranchDepth << endl;
        cout << endl;*/
        cout << N << "," << avgInsertTime << "," << avgDeleteTime << "," << avgSearchTime << "," << avgMaxDepth << endl;
        

        if (i == 18) {
            // 1. Сохраняем все значения максимальных глубин
            ofstream maxDepthFile("sorted_rbst_max_depths.csv");
            maxDepthFile << "MaxDepth\n";
            for (double depth : maxDepths) {
                maxDepthFile << depth << "\n";
            }
            
            // 2. Улучшенная гистограмма высот веток
            vector<int> branchDepths = allBranchDepths;
            if (!branchDepths.empty()) {
                // Удаляем выбросы - только 99% данных
                sort(branchDepths.begin(), branchDepths.end());
                int cutoff = branchDepths[branchDepths.size() * 0.99];
                branchDepths.erase(remove_if(branchDepths.begin(), branchDepths.end(), 
                                        [cutoff](int x) { return x > cutoff; }), 
                                branchDepths.end());
                
                // Автоподбор бинов с учетом распределения
                int min_depth = *min_element(branchDepths.begin(), branchDepths.end());
                int max_depth = *max_element(branchDepths.begin(), branchDepths.end());
                int range = max_depth - min_depth;
                int num_bins = 20; // Фиксированное количество бинов
                int bin_size = max(1, range / num_bins);
                
                map<int, int> branchDepthHist;
                for (int depth : branchDepths) {
                    int bin = (depth - min_depth) / bin_size * bin_size + min_depth;
                    branchDepthHist[bin]++;
                }
                
                ofstream branchDepthFile("sorted_rbst_branch_depths.csv");
                branchDepthFile << "Depth,Count\n"; // Изменили заголовок
                
                // Нормализуем диапазоны и сортируем по глубине
                vector<pair<int, int>> sorted_bins(branchDepthHist.begin(), branchDepthHist.end());
                sort(sorted_bins.begin(), sorted_bins.end());
                
                for (const auto& [depth, count] : sorted_bins) {
                    branchDepthFile << depth << "," << count << "\n";
                }
            }
            
            cout << "Improved histogram data saved to sorted_rbst_branch_depths.csv" << endl;
        }
    }
}

int main() {
    srand(time(nullptr)); // Инициализация генератора случайных чисел
    testSortedRandomizedBST();
    return 0;
}
