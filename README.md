# parallel-programming-task

Comando compilação:
$ mpic++ -o3 -std=c++1y -Wall sender_collector_worker.cpp -o exe

Comando para executar
$ mpirun -np 12 --machinefile /home/mpihpc/.cluster_hostfile ./exe 12
