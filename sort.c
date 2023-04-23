#include <stdio.h>

void bubble_s(int *arr, int len)
{
    // 6, 2, 9, 0, 5, 1, 8
    for (int i = 0; i < len; ++i) {
        for (int j = i + 1; j < len; ++j) {
            if (arr[i] > arr[j]) {
                int tmp = arr[i];

                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
    }
}

void print_arr(int *arr, int len)
{
    printf("{");

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


    return 0;
}
