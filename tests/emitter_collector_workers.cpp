#include <mpi.h>
#include <iostream>
#include <random>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int emitter_rank = 0;
    const int collector_rank = size - 1;

    if (size != 12) {
        if (rank == 0) {
            std::cerr << "Este programa requer 12 processos." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd()); // Use mt19937 como o mecanismo de geração de números aleatórios

    // Crie uma distribuição uniforme entre 1 e 10
    std::uniform_int_distribution<> dis(1, 10);

    if (rank == emitter_rank) {

        for (int dest = 1; dest < size-1; ++dest)
        {
            // Gere um número aleatório
            int random_number = dis(gen);

            MPI_Send(&random_number, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);

            //std::cout << "Emissor: Trabalhador " << dest << ", Valor: " << random_number << std::endl;

        }

    } else if (rank == collector_rank) {
        // Coletor recebe mensagens dos trabalhadores
        int total_data = 0;
        int received_data;
        MPI_Status status;

        for (int worker = 1; worker < size-1; ++worker)
        {
            MPI_Recv(&received_data, 1, MPI_INT, worker, 0, MPI_COMM_WORLD, &status);
            total_data += received_data;
        }

        std::cout << "Total recebido coletor: " << total_data << std::endl;

    } else {
        // Trabalhadores recebem mensagens do emissor
        int myrank;
        MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
        
        int received_data;
        MPI_Status status;
        MPI_Recv(&received_data, 1, MPI_INT, emitter_rank, 0, MPI_COMM_WORLD, &status);

        // Trabalho realizado pelos trabalhadores
        received_data *= 2;

        std::cout << "Trabalhador: " << myrank << ", Valor: " << received_data << std::endl;

        // Trabalhadores enviam dados para o coletor
        MPI_Send(&received_data, 1, MPI_INT, collector_rank, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
