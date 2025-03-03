#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;
using namespace chrono;

// Глобальные счётчики вызовов heapify
uint64_t totalHeapifyCalls = 0;       // Всего вызовов heapify
uint64_t internalHeapifyCalls = 0;    // Внутренние (рекурсивные) вызовы heapify
uint64_t maxRecursionDepth = 0;       // Максимальная глубина рекурсии

// Функция генерации случайных чисел в диапазоне [-1, 1]
vector<double> generateNumbers(int N) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(-1.0, 1.0);

    vector<double> numbers(N);
    for (double &num : numbers) {
        num = dist(gen);
    }
    return numbers;
}

// Вспомогательная функция для просеивания вниз в пирамидальной сортировке
void heapify(vector<double>& arr, int n, int i, int depth = 1, bool isInternal = false) {
    totalHeapifyCalls++; // Каждый вызов heapify
    if (isInternal) internalHeapifyCalls++; // Если рекурсивный вызов
    maxRecursionDepth = max(maxRecursionDepth, (uint64_t)depth); // Обновление максимальной глубины

    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;
    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest, depth + 1, true); // Передаём true, так как это внутренний вызов
    }
}

// Функция пирамидальной сортировки (Heap Sort)
void heapSort(vector<double>& arr) {
    int n = arr.size();

    // Обнуление счетчиков перед сортировкой
    totalHeapifyCalls = 0;
    internalHeapifyCalls = 0;
    maxRecursionDepth = 0;

    // Построение кучи
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // Извлечение элементов из кучи
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

int main() {
    vector<int> sizes = {1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000};

    // Открываем CSV-файл
    ofstream file("results1.csv");
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл для записи.\n";
        return 1;
    }

    // Заголовки столбцов в CSV
    file << "Серия,Размер массива,Попытка,Время (сек),Всего вызовов heapify,Внутренних вызовов heapify,Максимальная глубина рекурсии\n";

    for (size_t series = 0; series < sizes.size(); series++) {
        int size = sizes[series];

        for (int attempt = 0; attempt < 20; attempt++) {
            vector<double> numbers = generateNumbers(size);

            auto start = high_resolution_clock::now();
            heapSort(numbers);
            auto end = high_resolution_clock::now();

            double elapsedTime = duration<double>(end - start).count();

            // Записываем результаты в CSV
            file << (series + 1) << "," << size << "," << (attempt + 1) << ","
                 << elapsedTime << "," << totalHeapifyCalls << "," << internalHeapifyCalls << "," << maxRecursionDepth << "\n";
        }
    }

    file.close();
    cout << "Результаты сохранены в файл results1.csv\n";

    return 0;
}
