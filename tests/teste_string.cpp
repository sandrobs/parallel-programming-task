#include <iostream>
#include <vector>

// Função para calcular o número de jobs que cada worker vai receber
void calcularNumeroDeJobsPorWorker(int totalJobs, int totalWorkers, int rank, int& inicio, int& quantidade) {
    // Calculando o número de jobs para cada worker
    int jobsPorWorker = totalJobs / totalWorkers;
    int jobsExtras = totalJobs % totalWorkers; // Jobs que serão distribuídos a workers adicionais

    // Calculando a quantidade de jobs que este worker receberá
    if (rank < jobsExtras) {
        // Os primeiros workers receberão um job extra
        quantidade = jobsPorWorker + 1;
        inicio = rank * quantidade;
    } else {
        // Workers posteriores receberão apenas a quantidade padrão de jobs
        quantidade = jobsPorWorker;
        inicio = jobsPorWorker * jobsExtras + (rank - jobsExtras) * quantidade;
    }
}

int main() {
    int totalJobs = 10; // Exemplo: Total de jobs
    int totalWorkers = 1; // Exemplo: Total de workers

    // Exemplo: Calcular o número de jobs para cada worker
    for (int rank = 0; rank < totalWorkers; ++rank) {
        int inicio, quantidade;
        calcularNumeroDeJobsPorWorker(totalJobs, totalWorkers, rank, inicio, quantidade);
        std::cout << "Worker " << rank << ": " << quantidade << " jobs a partir do índice " << inicio << std::endl;
    }

    return 0;
}
