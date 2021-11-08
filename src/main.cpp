#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>


void sift_test();

int main(void)
{
    sift_test();

    return 0;
}


void brute_force(float* data, float* query, std::vector<int>& result, size_t dim, uint32_t k, size_t node_count)
{
    // TODO
    // scan the data and compute a distance between each vector and query
    // store K nearest neighbors in the result vector
}

void sift_test() {
    //vectors count
    size_t node_count = 1000000;
    //query count
    size_t qsize = 10000;
    //vector dimension
    size_t vecdim = 128;
    //answers count for 1 query
    size_t answer_size = 100;
    //numberr of nearest neigbors to find
    uint32_t k = 10;

    /////////////////////////////////////////////////////// READ DATA
    float* mass = new float[node_count * vecdim];
    std::ifstream input("sift1M/sift1M.bin", std::ios::binary);
    if (!input.is_open()) std::runtime_error("Input data file not opened!");
    input.read((char*)mass, node_count * vecdim * sizeof(float));
    input.close();

    float* massQ = new float[qsize * vecdim];
    std::ifstream inputQ("sift1M/siftQ1M.bin", std::ios::binary);
    if (!input.is_open()) std::runtime_error("Input query file not opened!");
    inputQ.read((char*)massQ, qsize * vecdim * sizeof(float));
    inputQ.close();

    unsigned int* massQA = new unsigned int[qsize * answer_size];
    std::ifstream inputQA("sift1M/knnQA1M.bin", std::ios::binary);
    if (!input.is_open()) std::runtime_error("Input result file not opened!");
    inputQA.read((char*)massQA, qsize * answer_size * sizeof(int));
    inputQA.close();


    /////////////////////////////////////////////////////// QUERY PART
    std::cout << "Start querying\n";
    std::vector<std::pair<float, float>> precision_time;  

    int sum = 0;
    int min_time;
    for (int i = 0; i < 3; i++)
    {
#ifdef COLLECT_STAT
        hnsw.stat_.clear();
#endif
        std::vector<int> result;
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < qsize; i++)
        {
            brute_force(mass, &massQ[i * vecdim], result, vecdim, k, node_count);
        }
        auto end = std::chrono::steady_clock::now();
        int time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        sum += time;
        min_time = i == 0 ? time : std::min(min_time, time);

        result.clear();
    }
    std::cout << "avg: " << (float)sum / (qsize * 3) << " [us]; " << "min: " << min_time / qsize<< " [us]; \n";
    


    delete[] mass;
    delete[] massQ;
    delete[] massQA;
}