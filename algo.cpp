#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

/**
 * Выполняет сортировку вставками и считает количество проходов и перестановок.
 * 
 * @param arr Вектор чисел для сортировки.
 * @return Пара (количество проходов, количество перестановок).
 */
pair<int, uint64_t> insertionSort(vector<double>& arr) {
    int n = arr.size();
    int passes = 0; // Количество проходов (итераций внешнего цикла)
    uint64_t swaps = 0;  // Количество перестановок элементов

    for (int i = 1; i < n; i++) {
        double key = arr[i];
        int j = i - 1;
        passes++;  // Фиксируем проход по массиву

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j]; // Сдвигаем элементы вправо
            j--;
            swaps++; // Фиксируем перестановку
        }

        // Устанавливаем key на правильное место
        arr[j + 1] = key;
        if (j + 1 != i) { // Если key не остался на месте, то засчитываем перестановку
            swaps++;
        }
    }

    return {passes, swaps};
}

/**
 * Генерирует вектор случайных чисел в диапазоне [-1, 1].
 * 
 * @param N Количество чисел.
 * @return Вектор случайных чисел.
 */
vector<double> generateNumbers(int N) {
    vector<double> numbers(N);
    for (int i = 0; i < N; i++) {
        numbers[i] = (rand() / (double)RAND_MAX) * 2.0 - 1.0;
    }
    return numbers;
}

int main() {
    srand(time(nullptr));

    vector<int> sizes = {128000};

    for (int size : sizes) {
        vector<double> times, swapsList, passesList;

        for (int i = 0; i < 20; i++) {
            vector<double> numbers = generateNumbers(size);

            clock_t start = clock();
            auto [passes, swaps] = insertionSort(numbers);
            clock_t end = clock();

            double elapsedTime = double(end - start) / CLOCKS_PER_SEC;
            times.push_back(elapsedTime);
            swapsList.push_back(swaps);
            passesList.push_back(passes);
        }

        cout << "Время сортировки для " << size << " элементов: ";
        for (double t : times) cout << t << " ";
        cout << "\n";

        cout << "Количество перестановок для " << size << " элементов: ";
        for (uint64_t s : swapsList) cout << s << " ";
        cout << "\n";

        cout << "Количество проходов по массиву для " << size << " элементов: ";
        for (double p : passesList) cout << p << " ";
        cout << "\n\n";
    }

    return 0;
}
