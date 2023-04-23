#include <stdio.h>
#include <assert.h>

int binary_s(int *arr, int len, int value)
{
    int low = 0;
    int high = len - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        int v = arr[mid];

        if (value < v) {
            high = mid - 1;
        } else if (value > v) {
            low = mid + 1;
        } else {
            return mid;
        }
    }

    return -1;
}

int main(void)
{
    int arr[] = {5, 6, 7, 8, 9, 10};
    int found;

    found = binary_s(arr, sizeof(arr)/sizeof(*arr), 6);
    assert(found == 1);

    found = binary_s(arr, sizeof(arr)/sizeof(*arr), 7);
    assert(found == 2);

    found = binary_s(arr, sizeof(arr)/sizeof(*arr), 10);
    assert(found == 5);

    found = binary_s(arr, sizeof(arr)/sizeof(*arr), 69);
    assert(found == -1);

    return 0;
}
