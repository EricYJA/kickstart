'''
 *@Credit: Eric Yuan Changcheng 116010276 CUHK(SZ)
 *@Course: CSC4005: Distributed and PC A3
 * This is the scripting file to run the program
'''

import subprocess
import time
import os

def run(program, times, SEQ_run_command, MPI_run_command, Pthread_run_command):
    if(program == "MPI"):
        for i in range(times):
            print("Experiment " + str(i + 1))
            print(MPI_run_command)
            subprocess.call(MPI_run_command, shell=True)
            time.sleep(1)
    elif (program == "Pthread"):
        for i in range(times):
            print("Experiment " + str(i + 1))
            print(Pthread_run_command)
            subprocess.call(Pthread_run_command, shell=True)
            time.sleep(1)
    elif (program == "SEQ"):
        for i in range(times):
            print("Experiment " + str(i + 1))
            print(SEQ_run_command)
            subprocess.call(SEQ_run_command, shell=True)
            time.sleep(1)

def main():
    # subprocess.call("mkdir data", shell=True)
    # subprocess.call("module load openmpi-3.1.2-gcc-8.2.0-qgxgzyn", shell=True)
    # subprocess.call("mpicc -o M_CAL mpi_nbody_cal.c -lX11 -lm", shell=True)
    # subprocess.call("gcc -o P_CAL pthread_nbody_cal.c -lX11 -lm -lpthread", shell=True)
    # subprocess.call("gcc -o S_CAL seq_nbody_cal.c -lX11 -lm", shell=True)

    subprocess.call("mpicc -o M_CAL mpi_show.c -lm -lX11", shell=True)
    subprocess.call("gcc -o P_CAL pthread_show.c -lm -lpthread -lX11", shell=True)
    subprocess.call("gcc -o S_CAL seq_show.c -lm -lX11", shell=True)

    program = str(input("Which program do you want to run?\nMPI\t\tPthread\t\tSEQ\t\tALL\n> "))
    times = int(input("How many times do you want to run? (default: 1)\n> ") or "1")
    displayFlag = str(input("Default: 1 | 0 to turnoff display\nDisplay?\n> ") or "1")
    num_roomsize = str(input("How big room do you want to run? (default: 200)\n> ") or "200")
    num_roomTmp = str(input("what's the initial tmp do you want to run? (default: 0)\n> ") or "0")
    num_iteration = str(input("How many iteration do you want to run? \n(default: 1000 | -1 for converge end)\n> ") or "1000")
    num_workers = "0"
    if (program != "SEQ"):
        num_workers = str(input("How many processes/threads do you want to run? (default: 4)\n> ") or "4")
    MPI_run_command = "mpirun -oversubscribe -np " + str(int(num_workers) + 1) + " M_CAL" + " " + num_roomsize + " " + num_roomTmp + " " + num_iteration + " " + displayFlag
    Pthread_run_command = os.path.join(".", "P_CAL") + " " + num_workers + " " + num_roomsize + " " + num_roomTmp + " " + num_iteration + " " + displayFlag
    SEQ_run_command = "./" + "S_CAL" + " " + num_roomsize + " " + num_roomTmp + " " + num_iteration + " " + displayFlag


    if (program != "ALL"):
        run(program, times, SEQ_run_command, MPI_run_command, Pthread_run_command)
    else:
        run("MPI", times, SEQ_run_command, MPI_run_command, Pthread_run_command)
        run("Pthread", times, SEQ_run_command, MPI_run_command, Pthread_run_command)
        run("SEQ", times, SEQ_run_command, MPI_run_command, Pthread_run_command)


if __name__ == "__main__":
    main()
