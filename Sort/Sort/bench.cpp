#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <ctime>

extern void printSort2(std::vector<int> v, int size);

//--------------------------------------
// バブルソート
void bubble2(std::vector<int>& v)
{
    int n = static_cast<int>(v.size());
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (v[j] > v[j + 1])
            {
                std::swap(v[j], v[j + 1]);
            }
        }
    }
}
//--------------------------------------
// 選択ソート
void selection2(std::vector<int>& v)
{
    int n = static_cast<int>(v.size());
    for (int i = 0; i < n - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < n; j++)
        {
            if (v[j] < v[minIndex])
            {
                minIndex = j;
            }
        }
        std::swap(v[i], v[minIndex]);
    }
}
//--------------------------------------
// 挿入ソート
void insertion2(std::vector<int>& v)
{
    int n = static_cast<int>(v.size());
    for (int i = 1; i < n; i++)
    {
        int key = v[i];
        int j = i - 1;
        while (j >= 0 && v[j] > key)
        {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = key;
    }
}

//--------------------------------------
// ヒープソート
void heapify2(std::vector<int>& v, int n, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && v[l] > v[largest])
    {
        largest = l;
    }
    if (r < n && v[r] > v[largest])
    {
        largest = r;
    }
    if (largest != i)
    {
        std::swap(v[i], v[largest]);
        heapify2(v, n, largest);
    }
}

void heap2(std::vector<int>& v)
{
    int n = static_cast<int>(v.size());
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify2(v, n, i);
    }
    for (int i = n - 1; i >= 0; i--)
    {
        std::swap(v[0], v[i]);
        heapify2(v, i, 0);
    }
}

//--------------------------------------
// マージソート
void merge2(std::vector<int>& v, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<int> L(v.begin() + left, v.begin() + mid + 1);
    std::vector<int> R(v.begin() + mid + 1, v.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        v[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    }
    while (i < n1) v[k++] = L[i++];
    while (j < n2) v[k++] = R[j++];
}

void mergeSort2(std::vector<int>& v, int left, int right)
{
    if (left < right)
    {
        int mid = (left + right) / 2;
        mergeSort2(v, left, mid);
        mergeSort2(v, mid + 1, right);
        merge2(v, left, mid, right);
    }
}

//--------------------------------------
// クイックソート
int partition2(std::vector<int>& v, int low, int high)
{
    int pivot = v[high];
    int i = low - 1;
    for (int j = low; j < high; j++)
    {
        if (v[j] <= pivot)
        {
            i++;
            std::swap(v[i], v[j]);
        }
    }
    std::swap(v[i + 1], v[high]);
    return i + 1;
}

void quickSort2(std::vector<int>& v, int low, int high)
{
    if (low < high)
    {
        int pi = partition2(v, low, high);
        quickSort2(v, low, pi - 1);
        quickSort2(v, pi + 1, high);
    }
}

//--------------------------------------
// qsort（C標準ライブラリ）
int compareInt(const void* a, const void* b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    return (arg1 > arg2) - (arg1 < arg2);
}

//--------------------------------------
// ベンチマーク関数
template<typename F>
double benchmark(F func, std::vector<int>& data)
{
    std::cout << "初期状態:";
    printSort2(data, 10);

    auto start = std::chrono::high_resolution_clock::now();
    func(data);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "最終結果:";
    printSort2(data, 10);
    return elapsed.count();
}

//--------------------------------------
// メイン
void bench()
{
	// 乱数の初期化
    srand(static_cast<unsigned int>(time(nullptr)));

    const int N = 100000;
    //const int N = 100;
    //const int N = 10;
    std::vector<int> base(N);

    // N個のランダムデータを生成
    for (int i = 0; i < N; i++)
    {
        base[i] = rand();
    }

    std::cout << "データ数: " << N << " 個" << std::endl;
    std::cout << "処理時間 (ms):" << std::endl;
    std::cout << "----------------------------------\n";

    auto run = [&](std::string name, auto func)
    {
        std::vector<int> data = base; // 毎回コピー（同じ入力データ）
        double t = benchmark(func, data);
        std::cout << name << " : " << t << " ms";
        // ソート結果を確認したい場合は最初の数値だけ出す
        std::cout << "（先頭値: " << data[0] << ", 末尾値: " << data.back() << "）" << std::endl;
        std::cout << std::endl;
    };


    run("バブルソート   ", [](auto& d) { bubble2(d); });
    run("選択ソート     ", [](auto& d) { selection2(d); });
    run("挿入ソート     ", [](auto& d) { insertion2(d); });
    run("ヒープソート   ", [](auto& d) { heap2(d); });
    run("マージソート   ", [](auto& d) { mergeSort2(d, 0, static_cast<int>(d.size()) - 1); });
    run("クイックソート ", [](auto& d) { quickSort2(d, 0, static_cast<int>(d.size()) - 1); });
    run("qsort (C標準)  ", [](auto& d) { qsort(d.data(), static_cast<int>(d.size()), sizeof(int), compareInt); });

    std::cout << "----------------------------------\n";
}
