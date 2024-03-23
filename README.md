# parallel-programming-task

Comando compilação:
$ mpic++ -o3 -std=c++1y -Wall parallel.cpp -o parallel.exe

Comando para executar
$ mpirun -np 12 --machinefile /home/mpihpc/.cluster_hostfile ./parallel.exe 50000
