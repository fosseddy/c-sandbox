#include <stdio.h>

void bubble_s(int *arr, int len)
{
    for (int i = 0; i < len - 1; ++i) {
        for (int j = 0; j < len - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];

                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
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


    return 0;
}
