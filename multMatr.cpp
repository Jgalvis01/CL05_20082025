#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

vector<vector<int>> generar(int n, int m) {
    vector<vector<int>> mat(n, vector<int>(m));
    srand(time(0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            mat[i][j] = rand() % 100;
        }
    }
    return mat;
}

int main() {
    int N, M ,P;
    cin >> N >> M >> P;
    vector<vector<int>> A = generar(N, M);
    vector<vector<int>> B = generar(M, P);
    vector<vector<int>> C(N, vector<int>(P, 0));
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            for (int k = 0; k < M; k++) {
                C[i][j] += A[i][k] * B[k][j];
                
            }
        }
    }
    cout << "Resultado de la multiplicación:\n";
    for (const auto& fila : C) {
        for (const auto& elem : fila) {
            cout << elem << " ";
        }
        cout << endl;
    }
    return 0;
}
