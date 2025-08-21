#include <iostream>
#include <pthread.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

long long suma = 0;
pthread_mutex_t mutex;

struct ThreadData
{
    vector<int>* arr;
    int start;
    int end;
};


vector<int> generador(int n) {
    vector<int> vec(n);
    srand(time(0));
    for (int i = 0; i < n; ++i) {
        vec[i] = rand() % 100;
    }
    return vec;
}

void* sumWorker(void* arg) {

    ThreadData* data = (ThreadData*)arg;
    long long sumaLocal = 0;
    for (int i = data->start; i < data->end; ++i) {
        sumaLocal += (*data->arr)[i];
    }
    cout << "Hilo procesando desde " << data->start << " hasta " << data->end
         << " suma parcial = " << sumaLocal << endl;
    pthread_mutex_lock(&mutex);
    suma += sumaLocal;
    pthread_mutex_unlock(&mutex);
    pthread_exit(nullptr);
}

int main(){

    vector<int> arr = generador(100000000);
    int numThreads = 12; 
    pthread_t threads[numThreads];
    ThreadData threadData[numThreads];
    int chunk = arr.size() / numThreads;

    pthread_mutex_init(&mutex, nullptr);

    for (int i = 0; i < numThreads; ++i) {
        threadData[i].arr = &arr;
        threadData[i].start = i * chunk;
        threadData[i].end = (i == numThreads - 1) ? arr.size() : threadData[i].start + chunk;
        pthread_create(&threads[i], nullptr, sumWorker, &threadData[i]);
    }

    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    pthread_mutex_destroy(&mutex);

    cout << "Suma total: " << suma << endl;

    return 0;
}