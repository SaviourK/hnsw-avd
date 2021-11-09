#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

void sift_test();

int main(void)
{
    sift_test();

    return 0;
}


void brute_force(float* data, float* query, vector<int>& result, size_t dim, uint32_t k, size_t node_count)
{
    float diff = 0;
    int qI = 0;
    vector<pair<float, int>> diffs;
    int size = 0;
    bool sorted = false;
    for (int i = 0; i < node_count * dim; i++) {
        float tmp = data[i] - query[qI];
        diff += (tmp) * (tmp);
        qI++;
        if (qI == dim) {
            diff = sqrtf(diff);
            if (!sorted) {
                if (size < k) {
                    diffs.push_back(make_pair(diff, size));
                }
                else if (size == k) {
                    sort(diffs.begin(), diffs.end());
                    sorted = true;
                    if (diffs.back().first > diff) {
                        diffs.back() = make_pair(diff, size);
                        sort(diffs.begin(), diffs.end());
                    }
                }
            } else {
                if (diffs.back().first > diff) {
                    diffs.back() = make_pair(diff, size);
                    sort(diffs.begin(), diffs.end());
                }
            }
            size++;
            qI = 0;
            diff = 0;
        }
    }

    if (!sorted) {
        sort(diffs.begin(), diffs.end());
    }

    for (int i = 0; i < k; i++) {
        cout << "K: " << i << " position: " << diffs[i].second << " diff: " << diffs[i].first << "\n";
        result.push_back(diffs[i].second);
    }
}

void sift_test() {
    //vectors count
    size_t node_count = 1000000;
    //query count
    size_t qsize = 10;
    //vector dimension
    size_t vecdim = 128;
    //answers count for 1 query
    size_t answer_size = 100;
    //numberr of nearest neigbors to find
    uint32_t k = 10;
    
   
    /////////////////////////////////////////////////////// READ DATA
    float* mass = new float[node_count * vecdim];
    ifstream input("c:/All/VSB/2rocnik/AVD/sift1M/sift1M.bin", ios::binary);
    if (!input.is_open()) runtime_error("Input data file not opened!");
    input.read((char*)mass, node_count * vecdim * sizeof(float));
    input.close();

    float* massQ = new float[qsize * vecdim];
    ifstream inputQ("c:/All/VSB/2rocnik/AVD/sift1M/siftQ1M.bin", ios::binary);
    if (!input.is_open()) runtime_error("Input query file not opened!");
    inputQ.read((char*)massQ, qsize * vecdim * sizeof(float));
    inputQ.close();

    unsigned int* massQA = new unsigned int[qsize * answer_size];
    ifstream inputQA("c:/All/VSB/2rocnik/AVD/sift1M/knnQA1M.bin", ios::binary);
    if (!input.is_open()) runtime_error("Input result file not opened!");
    inputQA.read((char*)massQA, qsize * answer_size * sizeof(int));
    inputQA.close();

    /////////////////////////////////////////////////////// QUERY PART
    cout << "Start querying\n";
    vector<pair<float, float>> precision_time;  

    int sum = 0;
    int min_time;
    for (int i = 0; i < 3; i++)
    {
#ifdef COLLECT_STAT
        hnsw.stat_.clear();
#endif
        vector<int> result;
        auto start = chrono::steady_clock::now();

        int plus = 0;
        for (int i = 0; i < qsize; i++)
        {
            brute_force(mass, &massQ[i * vecdim], result, vecdim, k, node_count);
            cout << "QA " << massQA[i + plus] << " \n";
            plus += 99;

        }
        auto end = chrono::steady_clock::now();
        int time = chrono::duration_cast<chrono::microseconds>(end - start).count();
        sum += time;
        min_time = i == 0 ? time : min(min_time, time);

        result.clear();
    }
    cout << "avg: " << (float)sum / (qsize * 3) << " [us]; " << "min: " << min_time / qsize<< " [us]; \n";
    


    delete[] mass;
    delete[] massQ;
    delete[] massQA;

    
}