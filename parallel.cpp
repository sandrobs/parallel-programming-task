#include <mpi.h>
#include <iostream>
#include <chrono>
#include <vector>

int prime_number ( int initial, int final ){
	int total = 0;
	for (int i = initial; i <= final; i++ ){
		int prime = 1;
		for (int j = 2; j < i; j++ ){
			if ( i % j == 0 ){
				prime = 0;
				break;
			}
		}
		total = total + prime;
	}

    //std::cout << "Total: " << total << " recebido: " << n << std::endl;

	return total;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int emitter_rank = 0;
    const int collector_rank = size - 1;
    const int qtd_workes = size - 2;

    if (size < 3) {
        if (rank == 0) {
            std::cerr << "Este programa requer 3 processos." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    int entry_number = atoi(argv[1]);

    double t_start = MPI_Wtime();

    if (rank == emitter_rank) {

        MPI_Send(&t_start, 1, MPI_DOUBLE, collector_rank, 0, MPI_COMM_WORLD);
        
        const int chunk_size = entry_number / qtd_workes; // Tamanho do intervalo para cada worker
        // Emissor
        for (int dest = 1; dest <= qtd_workes; ++dest)
        {
            int start = (dest-1) * chunk_size + 1;
            int end = (dest == qtd_workes) ? N : dest * chunk_size;
            MPI_Send(&start, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
            MPI_Send(&end, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
        }
       

    } else if (rank == collector_rank) {

        MPI_Status status;
        MPI_Recv(&t_start, 1, MPI_DOUBLE, emitter_rank, 0, MPI_COMM_WORLD, &status);

        int prime_total = 0;

        for (int src = 1; src <= qtd_workes; ++src)
        {
            int total_worker;
            MPI_Recv(&total_worker, 1, MPI_INT, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            prime_total += total_worker;
        }

        std::cout << "O número total de primos até " << entry_number << " é: " << prime_total << std::endl;

        double t_end = MPI_Wtime();

        std::cout << "Execution time(s): " << (t_end-t_start) << std::endl;

    } else {
        
        int myrank;
        MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

        // Trabalhadores
        int initial, final;
        MPI_Recv(&initial, 1, MPI_INT, emitter_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&final, 1, MPI_INT, emitter_rank, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::cout << "WORKER " << myrank << " INITIAL: " << initial << " FINAL: " << final << std::endl;

        int total_periodo = prime_number(initial, final);

        MPI_Send(&total_periodo, 1, MPI_INT, collector_rank, 0, MPI_COMM_WORLD); // Envia contagem local para o coletor
         
    }

    MPI_Finalize();

    return 0;
}
