#include <iostream>
#include <pthread.h>
#include <cstring>
using namespace std;

int* a;       
int* temp;     
int n;        
int thread_count; 


void* Thread_Count_sort(void* rank) {
    long my_rank = (long) rank;

    int local_n = n / thread_count;              
    int my_first_i = my_rank * local_n;            
    int my_last_i = (my_rank == thread_count - 1)  
                     ? n
                     : my_first_i + local_n;

    for (int i = my_first_i; i < my_last_i; i++) {
        int count = 0;
        for (int j = 0; j < n; j++) {
            if (a[j] < a[i])
                count++;
            else if (a[j] == a[i] && j < i)
                count++;
        }
        temp[count] = a[i];
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <num_hilos>" << endl;
        return 1;
    }

    thread_count = strtol(argv[1], NULL, 10);

    cout << "Ingrese el tamaño del arreglo: ";
    cin >> n;

    a = new int[n];
    temp = new int[n];

    cout << "Ingrese " << n << " números: ";
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    pthread_t* thread_handles = new pthread_t[thread_count];
    for (long thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, Thread_Count_sort, (void*) thread);
    }

    for (int thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }
    memcpy(a, temp, n * sizeof(int));
    cout << "Arreglo ordenado: ";
    for (int i = 0; i < n; i++) {
        cout << a[i] << " ";
    }
    cout << endl;
    delete[] a;
    delete[] temp;
    delete[] thread_handles;

    return 0;
}
