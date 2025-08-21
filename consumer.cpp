#include <iostream>
#include <algorithm>
#include <pthread.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h> 

using namespace std;

const int N = 10; 
vector<int> buffer(N, 0);

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notFull = PTHREAD_COND_INITIALIZER;
pthread_cond_t notEmpty = PTHREAD_COND_INITIALIZER;

bool stop = false; 

void* producer(void* arg) {
    int id = *(int*)arg;
    while (!stop) {
        sleep(rand() % 3 + 1);

        pthread_mutex_lock(&mtx);
        while (find(buffer.begin(), buffer.end(), 0) == buffer.end()) {
            cout << "Producer " << id << " waiting (buffer full)" << endl;
            pthread_cond_wait(&notFull, &mtx);
        }

        for (int i = 0; i < N; i++) {
            if (buffer[i] == 0) {
                buffer[i] = rand() % 9 + 1;
                cout << "Producer " << id << " produced " << buffer[i] << " at pos " << i << endl;
                break;
            }
        }

        pthread_cond_signal(&notEmpty); 
        pthread_mutex_unlock(&mtx);
    }
    return nullptr;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    while (!stop) {
        pthread_mutex_lock(&mtx);
        bool empty = true;
        for (int x : buffer) if (x != 0) { empty = false; break; }
        while (empty) {
            cout << "Consumer " << id << " waiting (buffer empty)" << endl;
            pthread_cond_wait(&notEmpty, &mtx);
            empty = true;
            for (int x : buffer) if (x != 0) { empty = false; break; }
        }

        int k = 0;
        for (int i = 0; i < N; i++) {
            if (buffer[i] != 0) {
                k = buffer[i];
                buffer[i] = 0;
                cout << "Consumer " << id << " consumed " << k << " at pos " << i << endl;
                break;
            }
        }
        pthread_cond_signal(&notFull); 
        pthread_mutex_unlock(&mtx);

        sleep(k % 3 + 1);
    }
    return nullptr;
}

int main() {
    srand(time(0));

    int numProd = 2, numCons = 2;
    cout << "Starting simulation with " << numProd << " producers and " << numCons << " consumers\n";

    vector<pthread_t> producers(numProd), consumers(numCons);
    vector<int> ids(max(numProd, numCons));

    for (int i = 0; i < numProd; i++) {
        ids[i] = i + 1;
        pthread_create(&producers[i], nullptr, producer, &ids[i]);
    }
    for (int i = 0; i < numCons; i++) {
        ids[i] = i + 1;
        pthread_create(&consumers[i], nullptr, consumer, &ids[i]);
    }

    sleep(15);
    stop = true;
    pthread_cond_broadcast(&notFull);
    pthread_cond_broadcast(&notEmpty);

    for (auto& th : producers) pthread_join(th, nullptr);
    for (auto& th : consumers) pthread_join(th, nullptr);

    cout << "Simulation finished.\n";
    return 0;
}
