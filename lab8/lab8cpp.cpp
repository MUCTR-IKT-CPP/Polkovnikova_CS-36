#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <cmath>
#include <random>
#include <algorithm>

// Бинарная мин-куча
class BinaryMinHeap {
private:
    std::vector<int> heap;

    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }

    void heapify(int i) {
        int smallest = i;
        int l = left(i);
        int r = right(i);

        if (l < heap.size() && heap[l] < heap[smallest])
            smallest = l;
        if (r < heap.size() && heap[r] < heap[smallest])
            smallest = r;

        if (smallest != i) {
            std::swap(heap[i], heap[smallest]);
            heapify(smallest);
        }
    }

public:
    void insert(int value) {
        heap.push_back(value);
        int i = heap.size() - 1;
        while (i > 0 && heap[parent(i)] > heap[i]) {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    int findMin() {
        if (heap.empty()) return -1;
        return heap[0];
    }

    void deleteMin() {
        if (heap.empty()) return;
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty())
            heapify(0);
    }
};

// Фибоначчиева куча
class FibonacciHeap {
private:
    struct Node {
        int value;
        Node* parent;
        Node* child;
        Node* left;
        Node* right;
        int degree;
        bool mark;

        Node(int val) : value(val), parent(nullptr), child(nullptr),
                        left(this), right(this), degree(0), mark(false) {}
    };

    Node* min;
    int size;

    void consolidate() {
        int maxDegree = static_cast<int>(std::log2(size)) + 1;
        std::vector<Node*> degreeTable(maxDegree, nullptr);

        std::vector<Node*> roots;
        Node* current = min;
        if (current) {
            do {
                roots.push_back(current);
                current = current->right;
            } while (current != min);
        }

        for (Node* x : roots) {
            int d = x->degree;
            while (degreeTable[d]) {
                Node* y = degreeTable[d];
                if (x->value > y->value)
                    std::swap(x, y);
                link(y, x);
                degreeTable[d] = nullptr;
                d++;
            }
            degreeTable[d] = x;
        }

        min = nullptr;
        for (Node* node : degreeTable) {
            if (node) {
                if (!min || node->value < min->value)
                    min = node;
            }
        }
    }

    void link(Node* y, Node* x) {
        y->left->right = y->right;
        y->right->left = y->left;

        y->parent = x;
        if (!x->child) {
            x->child = y;
            y->right = y;
            y->left = y;
        } else {
            y->left = x->child;
            y->right = x->child->right;
            x->child->right->left = y;
            x->child->right = y;
        }

        x->degree++;
        y->mark = false;
    }

    void clear(Node* node) {
        if (!node) return;
        Node* current = node;
        do {
            Node* next = current->right;
            clear(current->child);
            delete current;
            current = next;
        } while (current != node);
    }

public:
    FibonacciHeap() : min(nullptr), size(0) {}

    void insert(int value) {
        Node* node = new Node(value);
        if (!min) {
            min = node;
        } else {
            node->right = min;
            node->left = min->left;
            min->left->right = node;
            min->left = node;
            if (node->value < min->value)
                min = node;
        }
        size++;
    }

    int findMin() {
        if (!min) return -1;
        return min->value;
    }

    void deleteMin() {
        if (!min) return;

        Node* child = min->child;
        if (child) {
            Node* current = child;
            do {
                Node* next = current->right;
                current->parent = nullptr;
                current = next;
            } while (current != child);

            min->left->right = child;
            child->left->right = min->right;
            min->right->left = child->left;
            child->left = min->left;
        }

        Node* oldMin = min;
        if (min->right == min)
            min = nullptr;
        else {
            min->left->right = min->right;
            min->right->left = min->left;
            min = min->right;
            consolidate();
        }

        delete oldMin;
        size--;
    }

    ~FibonacciHeap() {
        if (min) {
            clear(min);
        }
    }
};

// Функция для тестирования кучи
template<typename HeapType>
void testHeap(int N, std::ofstream& out, const std::string& heapName) {
    HeapType heap;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);

    // Наполнение кучи
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        heap.insert(dis(gen));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto fillTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Тест поиска минимума
    double maxFindTime = 0;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; i++) {
        auto opStart = std::chrono::high_resolution_clock::now();
        heap.findMin();
        auto opEnd = std::chrono::high_resolution_clock::now();
        double opTime = std::chrono::duration_cast<std::chrono::nanoseconds>(opEnd - opStart).count();
        maxFindTime = std::max(maxFindTime, opTime);
    }
    end = std::chrono::high_resolution_clock::now();
    auto findTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Тест удаления минимума
    double maxDeleteTime = 0;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; i++) {
        auto opStart = std::chrono::high_resolution_clock::now();
        heap.deleteMin();
        auto opEnd = std::chrono::high_resolution_clock::now();
        double opTime = std::chrono::duration_cast<std::chrono::nanoseconds>(opEnd - opStart).count();
        maxDeleteTime = std::max(maxDeleteTime, opTime);
    }
    end = std::chrono::high_resolution_clock::now();
    auto deleteTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Тест вставки
    double maxInsertTime = 0;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; i++) {
        auto opStart = std::chrono::high_resolution_clock::now();
        heap.insert(dis(gen));
        auto opEnd = std::chrono::high_resolution_clock::now();
        double opTime = std::chrono::duration_cast<std::chrono::nanoseconds>(opEnd - opStart).count();
        maxInsertTime = std::max(maxInsertTime, opTime);
    }
    end = std::chrono::high_resolution_clock::now();
    auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Запись результатов
    out << heapName << "," << N << ",fill," << fillTime / 1000.0 << "\n";
    out << heapName << "," << N << ",find," << findTime / 1000.0 << "," << maxFindTime << "\n";
    out << heapName << "," << N << ",delete," << deleteTime / 1000.0 << "," << maxDeleteTime << "\n";
    out << heapName << "," << N << ",insert," << insertTime / 1000.0 << "," << maxInsertTime << "\n";
}

int main() {
    std::ofstream out("heap_results.csv");
    out << "HeapType,N,Operation,AvgTime,MaxTime\n";

    std::vector<int> sizes = {1000, 10000, 100000, 1000000, 10000000};

    for (int N : sizes) {
        std::cout << "Testing with N = " << N << "\n";
        testHeap<BinaryMinHeap>(N, out, "Binary");
        testHeap<FibonacciHeap>(N, out, "Fibonacci");
    }

    out.close();
    std::cout << "Results written to heap_results.csv\n";
    return 0;
}
