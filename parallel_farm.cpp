#include <mpi.h>
#include <iostream>
#include <chrono>
#include <vector>

int prime_number ( int n ){
	int total = 0;
	for (int i = 2; i <= n; i++ ){
		int prime = 1;
		for (int j = 2; j < i; j++ ){
			if ( i % j == 0 ){
				prime = 0;
				break;
			}
		}
		total = total + prime;
	}

    std::cout << "Total: " << total << " recebido: " << n << std::endl;

	return total;
}

std::vector<int> get_int_collection(){

    std::vector<int> int_collection(10);

    int_collection[0] = 7;
    int_collection[1] = 23;
    int_collection[2] = 14;
    int_collection[3] = 13;
    int_collection[4] = 19;
    int_collection[5] = 56;
    int_collection[6] = 102;
    int_collection[7] = 249;
    int_collection[8] = 321;
    int_collection[9] = 9;

    return int_collection;

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

    double t_start = MPI_Wtime();

    if (rank == emitter_rank) {
        
        std::vector<int> int_collection = get_int_collection();

        MPI_Send(&t_start, 1, MPI_DOUBLE, collector_rank, 0, MPI_COMM_WORLD);

        int batch_processed = 0;
        int dest_worker = 1;
        int collection_size = int_collection.size();
        int tag = 1;
        for (int index = 0; index < collection_size; ++index) {

            int number = int_collection[index];
            
            if (batch_processed == qtd_workes) {
                dest_worker = 1;
                batch_processed = 0;
                tag++;
            }

            MPI_Send(&number, 1, MPI_INT, dest_worker, tag, MPI_COMM_WORLD);
            std::cout << "Emissor: Trabalhador " << dest_worker << ", Valor: " << number << std::endl;

            batch_processed++;
            dest_worker++;

        }

    } else if (rank == collector_rank) {
        // Coletor recebe mensagens dos trabalhadores
        int total_data = 0;
        int received_data;
        MPI_Status status;

        MPI_Recv(&t_start, 1, MPI_DOUBLE, emitter_rank, 0, MPI_COMM_WORLD, &status);

        for (int job = 0; job < 10; ++job)
        {
            MPI_Recv(&received_data, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            total_data += received_data;
            std::cout << "job  " << job << " valor: " << received_data << std::endl;
        }

        std::cout << "Total recebido coletor: " << total_data << std::endl;
        double t_end = MPI_Wtime();

        std::cout << "Execution time(s): " << (t_end-t_start) << std::endl;

    } else {
        
        int myrank;
        MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
        
        int received_data;
        int returned_number;
        //int num_jobs = 10;
        MPI_Status status;
        //bool recebeu_valor = true;
        int tag = 0;

        //sao 10 jobs
        //o worker pode receber N jobs qtd_workes
      int t = 0;
        do
        {
            
            tag++;

            MPI_Recv(&received_data, 1, MPI_INT, emitter_rank, tag, MPI_COMM_WORLD, &status);


            std::cout << "worker " << myrank << " received_data: " << received_data << " tag " << tag << std::endl;


           /* if (received_data == 0 || received_data == null)
            {
                
            }*/
            

            returned_number = prime_number(received_data);

            // Trabalhadores enviam dados para o coletor
            MPI_Send(&returned_number, 1, MPI_INT, collector_rank, 0, MPI_COMM_WORLD);
            t++;
            
        } while (t < 12);
        

        
    }

    MPI_Finalize();

    return 0;
}
