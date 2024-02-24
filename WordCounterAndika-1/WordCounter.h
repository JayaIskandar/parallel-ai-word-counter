#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <omp.h>
#include <thread>

using namespace std;
using namespace std::chrono;

// Function to split a string into words
vector<string> splitWords(const string &text) {
    vector<string> words;
    istringstream iss(text);
    string word;
    while (iss >> word) {
        words.push_back(word);
    }
    return words;
}

// Map function
vector<pair<string, int>> Map(const string &text) {
    vector<pair<string, int>> wordCounts;
    vector<string> words = splitWords(text);
    for (const auto &word : words) {
        wordCounts.push_back({word, 1});
    }
    return wordCounts;
}

// Parallel Map function
vector<pair<string, int>> ParallelMap(const string &text) {
    vector<pair<string, int>> wordCounts;
    vector<string> words = splitWords(text);
    int num_threads = 0;
#pragma omp parallel
    {
#pragma omp single
        {
            num_threads = omp_get_num_threads();
        }

#pragma omp for
        for (size_t i = 0; i < words.size(); ++i) {
#pragma omp critical
            {
                wordCounts.push_back({words[i], 1});
            }
        }
    }
    cout << "Number of threads used (Parallel with OpenMP): " << num_threads << endl;
    return wordCounts;
}

// Threaded Map function
vector<pair<string, int>> ThreadedMap(const string &text) {
    vector<pair<string, int>> wordCounts;
    vector<string> words = splitWords(text);
    vector<thread> threads;
    for (size_t i = 0; i < words.size(); ++i) {
        threads.emplace_back([&wordCounts, &words, i]() {
            wordCounts.push_back({words[i], 1});
        });
    }
    for (auto &thread : threads) {
        thread.join();
    }
    cout << "Number of threads used (Parallel with threads): " << threads.size() << endl;
    return wordCounts;
}

// Reduce function
unordered_map<string, int> Reduce(const vector<vector<pair<string, int>>> &intermediates) {
    unordered_map<string, int> wordCount;
    for (const auto &intermediate : intermediates) {
        for (const auto &pair : intermediate) {
            const string &word = pair.first;
            const int count = pair.second;
            wordCount[word] += count;
        }
    }
    return wordCount;
}

#endif // WORDCOUNTER_H
