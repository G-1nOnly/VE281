#ifndef VE281P1_SORT_HPP
#define VE281P1_SORT_HPP

#include <vector>

template <typename T>
void swap_t(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

template <typename T, typename Compare>
void bubble_sort(std::vector<T> &vector, Compare comp = std::less<T>())
{
    // TODO: implement
    for (int i = int(vector.size() - 1); i > 0; i--)
    {
        for (int j = 0; j < i; j++)
        {
            if (comp(vector[j + 1], vector[j]))
                swap_t(vector[j], vector[j + 1]);
        }
    }
}

template <typename T, typename Compare>
void insertion_sort(std::vector<T> &vector, Compare comp = std::less<T>())
{
    // TODO: implement
    for (int i = 1; i < int(vector.size()); i++)
    {
        T temp = vector[i];
        int j;
        for (j = i; j > 0 && comp(temp, vector[j - 1]); j--)
            vector[j] = vector[j - 1];
        vector[j] = temp;
    }
}

template <typename T, typename Compare>
void selection_sort(std::vector<T> &vector, Compare comp = std::less<T>())
{
    // TODO: implement
    for (int i = 0; i < int(vector.size() - 1); i++)
    {
        int index = 0;
        T temp = vector[i];
        for (int j = i; j < int(vector.size()); j++)
        {
            if (comp(vector[j], temp))
            {
                index = j;
                temp = vector[j];
            }
        }
        if (index != 0)
            swap_t(vector[index], vector[i]);
    }
}

template <typename T, typename Compare>
void merge(std::vector<T> &vector, std::vector<T> &temp, int left, int mid, int right, Compare comp)
{
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right)
    {
        if (!comp(vector[j], vector[i]))
            temp[k++] = vector[i++];
        else
            temp[k++] = vector[j++];
    }
    while (i <= mid)
        temp[k++] = vector[i++];
    while (j <= right)
        temp[k++] = vector[j++];
    for (int t = left; t <= right; t++)
        vector[t] = temp[t];
}

template <typename T, typename Compare>
void merge_sort_helper(std::vector<T> &vector, std::vector<T> &temp, int left, int right, Compare comp)
{
    if (left >= right)
        return;
    int mid = (left + right) / 2;
    merge_sort_helper(vector, temp, left, mid, comp);
    merge_sort_helper(vector, temp, mid + 1, right, comp);
    merge(vector, temp, left, mid, right, comp);
}

template <typename T, typename Compare>
void merge_sort(std::vector<T> &vector, Compare comp = std::less<T>())
{
    // TODO: implement
    std::vector<T> temp(vector.size());
    merge_sort_helper(vector, temp, 0, int(vector.size() - 1), comp);
}

template <typename T, typename Compare>
int partition_extra(std::vector<T> &vector, int left, int right, Compare comp)
{
    std::vector<T> temp(right - left + 1);
    int res = left, temp_index = 0;
    T pivot_val = vector[left];
    for (int i = left + 1; i <= right; i++)
    {
        if (comp(vector[i], pivot_val))
        {
            temp[temp_index++] = vector[i];
            res++;
        }
    }
    temp[temp_index++] = pivot_val;
    for (int i = left + 1; i <= right; i++)
    {
        if (!comp(vector[i], pivot_val))
            temp[temp_index++] = vector[i];
    }
    for (int i = left; i <= right; i++)
        vector[i] = temp[i - left];
    return res;
}

template <typename T, typename Compare>
int partition_inplace(std::vector<T> &vector, int left, int right, Compare comp)
{
    int res = left;
    T pivot_val = vector[left];
    for (int i = left + 1; i <= right; i++)
    {
        if (comp(vector[i], pivot_val))
        {
            res++;
            swap_t(vector[i], vector[res]);
        }
    }
    swap_t(vector[left], vector[res]);
    return res;
}

template <typename T, typename Compare>
void quicksort_extra_helper(std::vector<T> &vector, int left, int right, Compare comp)
{
    if (left >= right)
        return;
    int pivot = partition_extra(vector, left, right, comp);
    quicksort_extra_helper(vector, left, pivot, comp);
    quicksort_extra_helper(vector, pivot + 1, right, comp);
}

template <typename T, typename Compare>
void quicksort_inplace_helper(std::vector<T> &vector, int left, int right, Compare comp)
{
    if (left >= right)
        return;
    int pivot = partition_inplace(vector, left, right, comp);
    quicksort_inplace_helper(vector, left, pivot, comp);
    quicksort_inplace_helper(vector, pivot + 1, right, comp);
}

template <typename T, typename Compare>
void quick_sort_extra(std::vector<T> &vector, Compare comp = std::less<T>())
{
    // TODO: implement
    quicksort_extra_helper(vector, 0, int(vector.size() - 1), comp);
}

template <typename T, typename Compare>
void quick_sort_inplace(std::vector<T> &vector, Compare comp = std::less<T>())
{
    // TODO: implement
    quicksort_inplace_helper(vector, 0, int(vector.size() - 1), comp);
}

#endif //VE281P1_SORT_HPP