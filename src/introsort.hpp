#pragma once
#include <vector>
#include <algorithm>
#include <cstdint>

namespace a3i {

using std::vector;

static inline void insort(vector<long long>& a, int l, int r){
    for (int i = l + 1; i < r; ++i){
        long long x = a[i];
        int j = i - 1;
        while (j >= l && a[j] > x){ a[j + 1] = a[j]; --j; }
        a[j + 1] = x;
    }
}

static inline void sift(vector<long long>& a, int l, int heapSize, int i){
    while (true){
        int L = 2*i + 1, R = 2*i + 2, mx = i;
        if (L < heapSize && a[l + L] > a[l + mx]) mx = L;
        if (R < heapSize && a[l + R] > a[l + mx]) mx = R;
        if (mx == i) break;
        std::swap(a[l + i], a[l + mx]); i = mx;
    }
}
static inline void heap_range(vector<long long>& a, int l, int r){
    int n = r - l; if (n <= 1) return;
    for (int i = n/2 - 1; i >= 0; --i) sift(a, l, n, i);
    for (int i = n - 1; i > 0; --i){ std::swap(a[l], a[l + i]); sift(a, l, i, 0); }
}

static inline int med3(vector<long long>& a, int i, int j, int k){
    const long long &x=a[i], &y=a[j], &z=a[k];
    if (x < y){ if (y < z) return j; else if (x < z) return k; else return i; }
    else { if (x < z) return i; else if (y < z) return k; else return j; }
}

static inline int flog2_u32(uint32_t x){
#if defined(__GNUC__) || defined(__clang__)
    return 31 - __builtin_clz(x);
#else
    int lg = 0; while (x >>= 1) ++lg; return lg;
#endif
}

static inline void part3(vector<long long>& a, int l, int r, int& lt, int& gt){
    int m = l + ((r - l) >> 1);
    int p = med3(a, l, m, r - 1);
    long long pivot = a[p];
    lt = l; int i = l; gt = r - 1;
    while (i <= gt){
        if (a[i] < pivot) { std::swap(a[lt++], a[i++]); }
        else if (a[i] > pivot) { std::swap(a[i], a[gt--]); }
        else { ++i; }
    }
}

static const int INSERTION_THRESHOLD = 16;

static void intro_impl(vector<long long>& a, int l, int r, int maxDepth){
    while (r - l >= INSERTION_THRESHOLD){
        if (maxDepth <= 0){ heap_range(a, l, r); return; }
        --maxDepth;
        int lt, gt; part3(a, l, r, lt, gt);
        int L1 = l, R1 = lt;
        int L2 = gt + 1, R2 = r;
        if ((R1 - L1) < (R2 - L2)){
            if (R1 - L1 > 1) intro_impl(a, L1, R1, maxDepth);
            l = L2; r = R2;
        } else {
            if (R2 - L2 > 1) intro_impl(a, L2, R2, maxDepth);
            l = L1; r = R1;
        }
    }
    insort(a, l, r);
}

inline void quick_heap_insertion_sort(vector<long long>& a){
    int n = (int)a.size(); if (n <= 1) return;
    int depth = 2 * flog2_u32((uint32_t)n);
    intro_impl(a, 0, n, depth);
}

} // namespace a3i
