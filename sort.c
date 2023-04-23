#include <stdio.h>

void swap(int *arr, int i, int j)
{
    int tmp = arr[i];

    arr[i] = arr[j];
    arr[j] = tmp;
}

void bubble_s(int *arr, int len)
{
    if (len <= 1) {
        return;
    }

    for (int i = 0; i < len - 1; ++i) {
        for (int j = 0; j < len - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr, j, j + 1);
            }
        }
    }
}

void quick_s(int *arr, int len)
{
    int last = 0;

    if (len <= 1) {
        return;
    }

    for (int i = 1; i < len; ++i) {
        if (arr[i] < arr[0]) {
            last++;
            swap(arr, last, i);
        }
    }

    swap(arr, 0, last);

    quick_s(arr, last);
    quick_s(arr + last + 1, len - last - 1);
}

void print_arr(int *arr, int len)
{
    printf("{ ");

    for (int i = 0; i < len; ++i) {
        printf("%d ", arr[i]);
    }

    printf("}\n");
}

int main(void)
{
    int arr[] = {6, 2, 9, 0, 5, 1, 8};
    int len = sizeof(arr) / sizeof(*arr);

    print_arr(arr, len);
    bubble_s(arr, len);
    print_arr(arr, len);

    int arr2[] = {6, 2, 9, 0, 5, 1, 8};
    int len2 = sizeof(arr) / sizeof(*arr);

    print_arr(arr2, len2);
    quick_s(arr2, len2);
    print_arr(arr2, len2);

    return 0;
}
