# Trabalho 1 de Computação de Alto Desempenho - SCC00903

#   Docente:
#       Paulo Sérgio Lopes de Souza
#   Alunos:
#       Beatriz Lomes da Silva -  12548038
#       Gustavo Moura Scarenci de Carvalho Ferreira -12547792
#       Hugo Hiroyuki Nakamura - 12732037
#       Matheus Henrique Dias Cirillo - 12547750

import os
import subprocess
import re
import statistics
import sys
import time

from tqdm import tqdm

root_folder = 'jacobi-openmp-ssc0903'
sequential = 'sequential'
parallel = 'parallel'
jacobi = 'jacobi'
full_program = 'full_program'
median = 'median'
mean = 'mean'
stdev = 'stdev'
speedup = 'speedup'
efficiency = 'efficiency'

def compile():
    command = 'pwd'
    output = subprocess.check_output(command, shell=True, text=True)
    try:
        root = output.split('/')[-1].strip()
        if root != root_folder:
            exit('Error: not in root folder')
    except:
        exit('Error: could not determine root folder')
    
    command = 'make'
    process = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if process.returncode != 0:
        exit('Error: could not compile code')

    print("Code compiled successfully!")

def clean_terminal():
    print('\033[F', end='')
    print('\033[K', end='')


def run(runs, sizes, threads):
    response_times = {}
    # run the code
    if 1 not in threads:
        threads = [1] + threads
    for r in tqdm(range(runs), desc='Runs'):
        pbar = tqdm(sizes, total=len(sizes), desc='Sizes', leave=False)
        for size in pbar:
            pbar.set_description(f'Loads ({size})')
            p2bar = tqdm(threads, desc='Threads', total=len(threads), leave=False)
            # add thread 1 at the beggining if it is not there
            for thread in p2bar:
                p2bar.set_description(f'Threads ({thread})')
                command = []
                if thread == 1:
                    command = f'make run_seq ARGS="{size} {thread} {0} {0}"'
                else:
                    command = f'make run_par ARGS="{size} {thread} {0} {0}"'
                result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

                jacobi_time_pattern = r'JacobiTime:\s+(\d+\.\d+s)'
                time_pattern = r'real\s+(\d+m\d+\.\d+s)\nuser\s+(\d+m\d+\.\d+s)\nsys\s+(\d+m\d+\.\d+s)'
                match = re.search(time_pattern, result.stderr)
                response_minutes = float(match.group(1).split('m')[0])
                response_seconds = float(match.group(1).split('m')[1].split('s')[0])
                response_total = response_minutes * 60 + response_seconds

                match = re.search(jacobi_time_pattern, result.stdout)
                response_jacobi = float(match.group(1).split('s')[0])

                response_times.setdefault(size, {}).setdefault(thread, []).append((response_total, response_jacobi))
        #finished a run
        data = interpret_data(response_times)
        generate_raw_csv(response_times, file_name=f'raw_data_{r+1}.csv')
        generate_csv(data, file_name=f'data_{r+1}.csv')
    return response_times

def interpret_data(raw_data, show_data:bool=False):
    try:
        data = {}
        for size in raw_data:
            for thread in raw_data[size]:
                key = (size, thread)
                full_program_times = [i[0] for i in raw_data[size][thread]]
                jacobi_times = [i[1] for i in raw_data[size][thread]]

                data[key] = {}
                data[key][full_program] = {}
                data[key][full_program][median] = round(statistics.median(full_program_times),5)
                data[key][full_program][mean] = round(statistics.mean(full_program_times),5)
                data[key][full_program][stdev] = round(statistics.stdev(full_program_times),5) if len(full_program_times) > 1 else 0

                data[key][jacobi] = {}
                data[key][jacobi][median] = round(statistics.median(jacobi_times),5)
                data[key][jacobi][mean] = round(statistics.mean(jacobi_times),5)
                data[key][jacobi][stdev] = round(statistics.stdev(jacobi_times),5) if len(jacobi_times) > 1 else 0

                data[key][speedup] = {}
                data[key][efficiency] = {}

                sequential_key = (size, 1)
                data[key][speedup][full_program] = round(data[sequential_key][full_program][mean] / data[key][full_program][mean],5)
                data[key][efficiency][full_program] = round(data[key][speedup][full_program] / key[1],5)

                data[key][speedup][jacobi] = round(data[sequential_key][jacobi][mean] / data[key][jacobi][mean],5)
                data[key][efficiency][jacobi] = round(data[key][speedup][jacobi] / key[1],5)
        x = 2
    except:
        exit('Error: could not interpret data')

    if(show_data):
        print("{:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10}".format('Size', 'Threads', 'Median Seq', 'Mean Seq', 'Stdev Seq', 'Median Seq Jacobi', 'Mean Seq Jacobi', 'Stdev Seq Jacobi', 'Median Par', 'Mean Par', 'Stdev Par', 'Full Speedup', 'Full Efficiency', 'Jacobi Speedup', 'Jacobi Efficiency'))
        for key in data:
            print("{:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10}".format(key[0], key[1], data[key][sequential][full_program][median], data[key][sequential][full_program][mean], data[key][sequential][full_program][stdev], data[key][sequential][jacobi][median], data[key][sequential][jacobi][mean], data[key][sequential][jacobi][stdev], data[key][full_program][median], data[key][full_program][mean], data[key][full_program][stdev], data[key][speedup][full_program], data[key][efficiency][full_program], data[key][speedup][jacobi], data[key][efficiency][jacobi]))

    return data

def generate_csv(data, file_name='data.csv'):
    file_location = 'out/'
    if not os.path.exists(file_location):
        os.makedirs(file_location)
    file_path = file_location + file_name
    try:
        with open(file_path, 'w') as file:
            file.write('Size,Threads,MedianSeq(F),MeanSeq(F),StdevSeq(F),MedianSeq(J),MeanSeq(J),StdevSeq(J),Speedup(F),Speedup(J),Efficiency(F),Efficiency(J)\n')
            for key in data:
                file.write(f"{key[0]},{key[1]},{data[key][full_program][median]},{data[key][full_program][mean]},{data[key][full_program][stdev]},{data[key][jacobi][median]},{data[key][jacobi][mean]},{data[key][jacobi][stdev]},{data[key][speedup][full_program]},{data[key][speedup][jacobi]},{data[key][efficiency][full_program]},{data[key][efficiency][jacobi]}\n")
                
    except:
        exit('Error: could not write to file')

def generate_raw_csv(data, file_name='raw_data.csv'):
    file_location = 'out/'
    if not os.path.exists(file_location):
        os.makedirs(file_location)
    file_path = file_location + file_name
    try:
        with open(file_path, 'w') as file:
            file.write('Run,Size,Threads,Time(F),Time(J)\n')
            for size in data:
                for thread in data[size]:
                    for run in range(len(data[size][thread])):
                        file.write(f"{run+1},{size},{thread},{data[size][thread][run][0]},{data[size][thread][run][1]}\n")
    except:
        exit('Error: could not write to file')


compile()
sizes = [1000, 3000, 5000]
threads = [2,4,8,12]
#get current OS time
tempo = time.time()
runs = int(sys.argv[1]) if len(sys.argv) > 1 else 3
raw_data = run(runs, sizes, threads)
generate_raw_csv(raw_data)
data = interpret_data(raw_data, show_data=False)
generate_csv(data)
tempo = time.time() - tempo
#convert seconds to hours:minutes:seconds
tempo = tempo/3600
hours = int(tempo)
tempo = (tempo - hours) * 60
minutes = int(tempo)
tempo = (tempo - minutes) * 60
seconds = int(tempo)
tempo = f"{hours}h {minutes}m {seconds}s"
print(f"Execution time: {tempo}")
