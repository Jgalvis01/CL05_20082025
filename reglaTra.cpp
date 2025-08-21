#include <iostream>
#include <cstdlib>
#include <omp.h>
using namespace std;

// Función a integrar (puedes cambiarla por lo que necesites)
double f(double x) {
    return (x * x);  // Ejemplo: f(x) = x^2
}

void Trap(double a, double b, int n, double* global_result_p) {
    double h, x, my_result;
    double local_a, local_b;
    int i, local_n;
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();

    h = (b - a) / n;
    local_n = n / thread_count;
    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;

    my_result = (f(local_a) + f(local_b)) / 2.0;
    for (i = 1; i <= local_n - 1; i++) {
        x = local_a + i * h;
        my_result += f(x);
    }
    my_result = my_result * h;

    #pragma omp critical
    *global_result_p += my_result;
}

int main() {
    double global_result = 0.0;
    double a, b;
    int n;

    cout << "Ingrese a, b y n: ";
    cin >> a >> b >> n;

    #pragma omp parallel
    Trap(a, b, n, &global_result);

    cout << "Con n = " << n << " trapecios, nuestra estimacion" << endl;
    cout << "de la integral desde " << a << " hasta " << b 
         << " = " << global_result << endl;

    return 0;
}
