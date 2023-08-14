#include <iostream>
#include <future>
#include <chrono>
#include <random>
#include "RequestHandler.h"

bool make_thread = true;
RequestHandler pool;

void merge(int* arr, int l, int m, int r)
{
    int nl = m - l + 1;
    int nr = r - m;

    // создаем временные массивы
    int* left = new int[nl];
    int* right = new int[nr];

    // копируем данные во временные массивы
    for (int i = 0; i < nl; i++)
        left[i] = arr[l + i];
    for (int j = 0; j < nr; j++)
        right[j] = arr[m + 1 + j];

    int i = 0, j = 0;
    int k = l;  // начало левой части

    while (i < nl && j < nr) {
        // записываем минимальные элементы обратно во входной массив
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        }
        else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }
    // записываем оставшиеся элементы левой части
    while (i < nl) {
        arr[k] = left[i];
        i++;
        k++;
    }
    // записываем оставшиеся элементы правой части
    while (j < nr) {
        arr[k] = right[j];
        j++;
        k++;
    }
    delete[] left;
    delete[] right;
}

void print(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

void mergeSort(int* arr, int l, int r) {
    if (l >= r) return;
    int m = (l + r - 1) / 2;
    if (make_thread && (m - l > 100000))
    {
        std::future<void> f = std::async(std::launch::async, [=]() {
            mergeSort(arr, l, m);
            });
        f.get();
        mergeSort(arr, m + 1, r);
    }
    else {
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
    }    
    merge(arr, l, m, r);
}

using namespace std;

void quicksort(int* array, long left, long right) {
    if (left >= right) return;
    long left_bound = left;
    long right_bound = right;

    long middle = array[(left_bound + right_bound) / 2];

    do {
        while (array[left_bound] < middle) {
            left_bound++;
        }
        while (array[right_bound] > middle) {
            right_bound--;
        }

        //Меняем элементы местами
        if (left_bound <= right_bound) {
            std::swap(array[left_bound], array[right_bound]);
            left_bound++;
            right_bound--;
        }
    } while (left_bound <= right_bound);

    if (make_thread && (right_bound - left > 10000))
    {
        pool.pushRequest(quicksort, array, left, right_bound);
        quicksort(array, left_bound, right);
    }
    else {
        // запускаем обе части синхронно
        quicksort(array, left, right_bound);
        quicksort(array, left_bound, right);
    }
}

int main()
{
    srand(0);
    long arr_size = 10000000;
    int* array = new int[arr_size];
    for (long i = 0; i < arr_size; i++) {
        array[i] = rand() % 500000;
    }
    time_t start, end;

    // многопоточный запуск
    time(&start);
    quicksort(array, 0, arr_size);
    //mergeSort(array, 0, arr_size - 1);
    time(&end);

    double seconds = difftime(end, start);
    printf("The multitime: %f seconds\n", seconds);

    for (long i = 0; i < arr_size - 1; i++) {
        if (array[i] > array[i + 1]) {
            std::cout << "Unsorted" << std::endl;
            break;
        }
    }
    for (long i = 0; i < arr_size; i++) {
        array[i] = rand() % 500000;
    }

    // однопоточный запуск
    make_thread = false;
    time(&start);
    quicksort(array, 0, arr_size);
    //mergeSort(array, 0, arr_size - 1);
    time(&end);
    seconds = difftime(end, start);
    printf("The time: %f seconds\n", seconds);
    
    delete[] array;
    return 0;
}
