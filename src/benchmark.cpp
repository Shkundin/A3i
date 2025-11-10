#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <chrono>
#include <random>
#include <string>
#include <algorithm>
#include "introsort.hpp"
#include "quicksort_std.hpp"

using namespace std;
using namespace a3i;

enum class Dataset { Random, NearlySorted, Duplicates };

static vector<long long> gen_random(size_t n, uint64_t seed){
    mt19937_64 g(seed);
    vector<long long> a(n);
    for (auto &x : a) x = (long long)g();
    return a;
}
static vector<long long> gen_nearly_sorted(size_t n, uint64_t seed, double swap_ratio=0.01){
    vector<long long> a(n);
    iota(a.begin(), a.end(), 0LL);
    mt19937_64 g(seed);
    size_t swaps = max<size_t>(1, (size_t)(n * swap_ratio));
    uniform_int_distribution<size_t> d(0, n ? n-1 : 0);
    for (size_t k=0;k<swaps;++k){ size_t i=d(g), j=d(g); if(i<n && j<n) swap(a[i],a[j]); }
    return a;
}
static vector<long long> gen_duplicates(size_t n, uint64_t seed, int distinct=100){
    mt19937_64 g(seed);
    uniform_int_distribution<int> d(0, distinct-1);
    vector<long long> a(n);
    for (auto &x : a) x = d(g);
    return a;
}
static vector<long long> make_data(Dataset t, size_t n, uint64_t seed){
    switch(t){
        case Dataset::Random:       return gen_random(n, seed);
        case Dataset::NearlySorted: return gen_nearly_sorted(n, seed);
        case Dataset::Duplicates:   return gen_duplicates(n, seed);
    }
    return {};
}
static const char* dataset_name(Dataset t){
    switch(t){
        case Dataset::Random:       return "Random";
        case Dataset::NearlySorted: return "NearlySorted";
        case Dataset::Duplicates:   return "Duplicates";
    }
    return "Unknown";
}

class SortTester {
public:
    template <typename SortFn>
    static long long time_ns_median(SortFn sorter, const vector<long long>& base, int R){
        vector<long long> times; times.reserve(R);
        for(int i=0;i<R;++i){
            vector<long long> a = base;
            auto t0 = chrono::steady_clock::now();
            sorter(a);
            auto t1 = chrono::steady_clock::now();
            long long dt = chrono::duration_cast<chrono::nanoseconds>(t1 - t0).count();
            if (!is_sorted(a.begin(), a.end())) { cerr << "Not sorted!\n"; exit(1); }
            times.push_back(dt);
        }
        nth_element(times.begin(), times.begin()+times.size()/2, times.end());
        return times[times.size()/2];
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<size_t> Ns   = { 10000, 50000, 100000, 200000, 500000 };
    vector<Dataset> sets= { Dataset::Random, Dataset::NearlySorted, Dataset::Duplicates };
    const int R = 7;
    const uint64_t seed_data = 123456789ULL;
    const uint64_t seed_qs   = 987654321ULL;

    ofstream csv_std("results/qs_std.csv");
    ofstream csv_hyb("results/qs_hybrid.csv");
    csv_std << "dataset,N,ns\n";
    csv_hyb << "dataset,N,ns\n";

    for (auto ds : sets){
        for (auto n : Ns){
            vector<long long> base = make_data(ds, n, seed_data + n);

            auto t_std = SortTester::time_ns_median(
                [&](vector<long long>& a){ quicksort_std(a, seed_qs + n); },
                base, R);

            auto t_hyb = SortTester::time_ns_median(
                [&](vector<long long>& a){ quick_heap_insertion_sort(a); },
                base, R);

            csv_std << dataset_name(ds) << "," << n << "," << t_std << "\n";
            csv_hyb << dataset_name(ds) << "," << n << "," << t_hyb << "\n";

            cerr << dataset_name(ds) << " N=" << n
                 << "  QS_Std=" << (t_std/1e6) << " ms"
                 << "  QS_Hybrid=" << (t_hyb/1e6) << " ms\n";
        }
    }
    cerr << "Done. Files: results/qs_std.csv, results/qs_hybrid.csv\n";
    return 0;
}
