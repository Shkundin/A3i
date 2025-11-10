#pragma once
#include <vector>
#include <random>
#include <algorithm>

namespace a3i {

using std::vector;

struct Rand {
    std::mt19937_64 rng;
    explicit Rand(uint64_t seed) : rng(seed) {}
    int uniform_int(int l, int r){ std::uniform_int_distribution<int> d(l, r); return d(rng); }
};

static int partition_random(vector<long long>& a, int l, int r, Rand& R){ // [l, r)
    int p = R.uniform_int(l, r - 1);
    long long pivot = a[p];
    std::swap(a[p], a[r - 1]);
    int i = l;
    for (int j = l; j < r - 1; ++j) if (a[j] < pivot) std::swap(a[i++], a[j]);
    std::swap(a[i], a[r - 1]);
    return i;
}

static void quicksort_std_impl(vector<long long>& a, int l, int r, Rand& R){
    if (r - l <= 1) return;
    int m = partition_random(a, l, r, R);
    quicksort_std_impl(a, l, m, R);
    quicksort_std_impl(a, m + 1, r, R);
}

inline void quicksort_std(vector<long long>& a, uint64_t seed){
    Rand R(seed);
    quicksort_std_impl(a, 0, (int)a.size(), R);
}

} // namespace a3i
