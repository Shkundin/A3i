# A3i — Introsort: эксперимент

Проект для эмпирического сравнения:
- **QS_Std** — стандартный QuickSort (рандомный pivot),
- **QS_Hybrid** — гибридный Quick+Heap+Insertion (интроспективная сортировка: отсечка по глубине + HeapSort, порог для Insertion Sort = 16, трёхпутевое разбиение).

## Структура репозитория
- `src/`
  - `benchmark.cpp` — генерация данных, класс `SortTester`, запуск и запись CSV
  - `introsort.hpp` — гибридная сортировка (Quick+Heap+Insertion)
  - `quicksort_std.hpp` — стандартный QuickSort со случайным опорным
- `scripts/`
  - `plot.py` — построение графиков из CSV (matplotlib)
- `results/` — сюда складываются CSV и PNG-графики
- `README.md` — этот файл

---

## Как запустить (в браузере через GitHub Codespaces)
1. В репозитории нажми **Code → Codespaces → Create codespace on main**.
2. В открывшемся терминале выполни:
   ```bash
   # (если нужно) установка matplotlib
   pip3 install matplotlib

   # сборка
   g++ -O3 -march=native -std=c++17 -Isrc -o bench src/benchmark.cpp

   # запуск бенчмарков (создаст CSV в results/)
   ./bench

   # построение графиков (создаст PNG в results/)
   python3 scripts/plot.py
