import subprocess
import time
import os

program = str(input("Which program do you want to run?\nMPI\t\tPthread\t\tSEQ\t\tALL\n> "))
times = int(input("How many times do you want to run?\n> "))
if (program != "SEQ"):
    sORd = str(input("Dynamic or static -- d or s\n> "))
    num_workers = str(input("How many processes/threads do you want to run?\n> "))
    load_command = "module load openmpi-3.1.2-gcc-8.2.0-qgxgzyn"
    MPI_run_command = "mpirun -oversubscribe -np " + num_workers + " MS_MPI"
    Pthread_run_command = os.path.join(".", "MS_Pthread") + " " + num_workers


run_command = "./" + "MS_SEQ"

if(program == "MPI"):
    if (sORd == "s"):
        MPI_file_name = "Mandelbrot_MPI_s.c"
    elif (sORd == "d"):
        MPI_file_name = "Mandelbrot_MPI_d.c"
    MPI_compile_command = "mpicc -o MS_MPI " + MPI_file_name + " -lX11"
    print(MPI_compile_command)
    subprocess.call(load_command, shell=True)
    subprocess.call(MPI_compile_command, shell=True)
    for i in range(times):
        print("Experiment " + str(i + 1))
        print(MPI_run_command)
        subprocess.call(MPI_run_command, shell=True)
        time.sleep(1)
elif (program == "Pthread"):
    if (sORd == "s"):
        Pthread_file_name = "Mandelbrot_Pthread_s.c"
    elif (sORd == "d"):
        Pthread_file_name = "Mandelbrot_Pthread_d.c"
    Pthread_compile_command = "gcc -o MS_Pthread " + Pthread_file_name + " -lpthread -lX11"
    subprocess.call(Pthread_compile_command, shell=True)
    for i in range(times):
        print("Experiment " + str(i + 1))
        print(Pthread_run_command)
        subprocess.call(Pthread_run_command, shell=True)
        time.sleep(1)
elif (program == "All"):
    if (sORd == "s"):
        MPI_file_name = "Mandelbrot_MPI_s.c"
        Pthread_file_name = "Mandelbrot_Pthread_s.c"
    elif (sORd == "d"):
        MPI_file_name = "Mandelbrot_MPI_d.c"
        Pthread_file_name = "Mandelbrot_Pthread_d.c"
    MPI_compile_command = "mpicc -o MS_MPI " + MPI_file_name + " -lX11"
    Pthread_compile_command = "gcc -o MS_Pthread " + Pthread_file_name + " -lpthread -lX11"
    subprocess.call(load_command, shell=True)
    subprocess.call(MPI_compile_command, shell=True)
    subprocess.call(Pthread_compile_command, shell=True)
    for i in range(times):
        print("Experiment " + str(i + 1))
        print(MPI_run_command)
        subprocess.call(MPI_run_command, shell=True)
        time.sleep(0.1)
        print(Pthread_run_command)
        subprocess.call(Pthread_run_command, shell=True)
        time.sleep(0.1)
elif (program == "SEQ"):
    file_name = str(input("The MPI program that you want to execute (with extension):\n> ") or "Mandelbrot_seq.c")
    compile_command = "gcc -o MS_SEQ " + file_name + " -lX11"
    print(compile_command)
    subprocess.call(compile_command, shell=True)
    for i in range(times):
        print("Experiment " + str(i + 1))
        print(run_command)
        subprocess.call(run_command, shell=True)
        time.sleep(1)
