#include <iostream>
#include <pthread.h>
#include <cmath>
using namespace std;

double global_result = 0.0;   
int n;                        
double a, b;                  
int thread_count;             
pthread_mutex_t mutex;        

double f(double x) {
    return x * x;  
}

void* Trap(void* rank) {
    long my_rank = (long) rank;

    double h = (b - a) / n;
    int local_n = n / thread_count;
    double local_a = a + my_rank * local_n * h;
    double local_b = local_a + local_n * h;

    double my_result = (f(local_a) + f(local_b)) / 2.0;
    for (int i = 1; i <= local_n - 1; i++) {
        double x = local_a + i * h;
        my_result += f(x);
    }
    my_result = my_result * h;

    pthread_mutex_lock(&mutex);
    global_result += my_result;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <num_hilos>" << endl;
        return 1;
    }

    thread_count = strtol(argv[1], NULL, 10);

    cout << "Ingrese a, b y n: ";
    cin >> a >> b >> n;

    pthread_t* thread_handles = new pthread_t[thread_count];

    pthread_mutex_init(&mutex, NULL);

    for (long thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, Trap, (void*) thread);
    }

    for (int thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    pthread_mutex_destroy(&mutex);

    cout << "Con n = " << n << " trapecios, nuestra estimacion" << endl;
    cout << "de la integral desde " << a << " hasta " << b 
         << " = " << global_result << endl;

    delete[] thread_handles;
    return 0;
}
