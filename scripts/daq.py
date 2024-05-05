import os
import subprocess
import re
import statistics
import sys

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
    randLimit = 10
    response_times = {}
    # run the code
    print("Starting runs...")
    for _ in range(runs):
        clean_terminal()
        print("Run {}/{}".format(_+1, runs))
        print("Running code...")
        for size in sizes:
            clean_terminal()
            print(f'Running with matrix of size {size} and sequentially')
            command = f'make run_seq ARGS="{size} {1} {0} {0} {randLimit}"'
            result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            output = result.stderr if result.stderr else result.stdout

            time_pattern = r'real\s+(\d+m\d+\.\d+s)\nuser\s+(\d+m\d+\.\d+s)\nsys\s+(\d+m\d+\.\d+s)'
            jacobi_time_pattern = r'JacobiTime:\s+(\d+\.\d+ms)'
            match = re.search(time_pattern, result.stderr)
            response_sequential_minutes = float(match.group(1).split('m')[0])
            response_sequential_seconds = float(match.group(1).split('m')[1].split('s')[0])
            response_sequential = response_sequential_minutes * 60 + response_sequential_seconds
            match = re.search(jacobi_time_pattern, result.stdout)
            response_jacobi = float(match.group(1).split('ms')[0])

            response_times.setdefault(size, {}).setdefault(sequential, []).append((response_sequential, response_jacobi))

            for thread in threads:
                clean_terminal()
                print(f'Running with matrix of size {size} and {thread} threads')

                command = f'make run_par ARGS="{size} {thread} {0} {0} {randLimit}"'
                result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
                output = result.stderr if result.stderr else result.stdout

                jacobi_time_pattern = r'JacobiTime:\s+(\d+\.\d+ms)'
                time_pattern = r'real\s+(\d+m\d+\.\d+s)\nuser\s+(\d+m\d+\.\d+s)\nsys\s+(\d+m\d+\.\d+s)'
                match = re.search(time_pattern, result.stderr)
                response_parallel_minutes = float(match.group(1).split('m')[0])
                response_parallel_seconds = float(match.group(1).split('m')[1].split('s')[0])
                response_parallel = response_parallel_minutes * 60 + response_parallel_seconds
                match = re.search(jacobi_time_pattern, result.stdout)
                response_jacobi = float(match.group(1).split('ms')[0])
                response_times[size].setdefault(parallel, {}).setdefault(thread, []).append((response_parallel, response_jacobi))

        clean_terminal()

    #clear entire last line in terminal
    clean_terminal()
    print("Code ran successfully!")
    return response_times

def interpret_data(raw_data, show_data:bool=False):
    print("Interpreting data...")
    try:
        data = {}
        for size in raw_data:
            sequential_times =  [i[0] for i in raw_data[size][sequential]]
            sequential_median = round(statistics.median(sequential_times),5)
            sequential_mean = round(statistics.mean(sequential_times),5)
            sequential_stdev = round(statistics.stdev(sequential_times),5) if len(sequential_times) > 1 else 0

            sequential_jacobi = [i[1] for i in raw_data[size][sequential]]
            sequential_jacobi_median = round(statistics.median(sequential_jacobi),5)
            sequential_jacobi_mean = round(statistics.mean(sequential_jacobi),5)
            sequential_jacobi_stdev = round(statistics.stdev(sequential_jacobi),5) if len(sequential_jacobi) > 1 else 0

            for thread in raw_data[size][parallel]:
                key = (size, thread)
                data[key] = {}
                data[key][sequential] = {}
                data[key][sequential][full_program] = {}
                data[key][sequential][full_program][median] = sequential_median
                data[key][sequential][full_program][mean] = sequential_mean
                data[key][sequential][full_program][stdev] = sequential_stdev

                data[key][sequential][jacobi] = {}
                data[key][sequential][jacobi][median] = sequential_jacobi_median
                data[key][sequential][jacobi][mean] = sequential_jacobi_mean
                data[key][sequential][jacobi][stdev] = sequential_jacobi_stdev

                parallel_times = [i[0] for i in raw_data[size][parallel][thread]]
                parallel_times_jacobi = [i[1] for i in raw_data[size][parallel][thread]]

                data[key][parallel] = {}
                data[key][parallel][full_program] = {}
                data[key][parallel][full_program][median] = round(statistics.median(parallel_times),5)
                data[key][parallel][full_program][mean] = round(statistics.mean(parallel_times),5)
                data[key][parallel][full_program][stdev] = round(statistics.stdev(parallel_times),5) if len(parallel_times) > 1 else 0

                data[key][parallel][jacobi] = {}
                data[key][parallel][jacobi][median] = round(statistics.median(parallel_times_jacobi),5)
                data[key][parallel][jacobi][mean] = round(statistics.mean(parallel_times_jacobi),5)
                data[key][parallel][jacobi][stdev] = round(statistics.stdev(parallel_times_jacobi),5) if len(parallel_times_jacobi) > 1 else 0

                data[key][speedup] = {}
                data[key][efficiency] = {}

                data[key][speedup][full_program] = round(data[key][sequential][full_program][mean] / data[key][parallel][full_program][mean],5)
                data[key][efficiency][full_program] = round(data[key][speedup][full_program] / key[1],5)

                data[key][speedup][jacobi] = round(data[key][sequential][jacobi][mean] / data[key][parallel][jacobi][mean],5)
                data[key][efficiency][jacobi] = round(data[key][speedup][jacobi] / key[1],5)
    except:
        exit('Error: could not interpret data')
    clean_terminal()
    print("Data interpreted successfully!")

    if(show_data):
        print("{:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10}".format('Size', 'Threads', 'Median Seq', 'Mean Seq', 'Stdev Seq', 'Median Seq Jacobi', 'Mean Seq Jacobi', 'Stdev Seq Jacobi', 'Median Par', 'Mean Par', 'Stdev Par', 'Full Speedup', 'Full Efficiency', 'Jacobi Speedup', 'Jacobi Efficiency'))
        for key in data:
            print("{:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10}".format(key[0], key[1], data[key][sequential][full_program][median], data[key][sequential][full_program][mean], data[key][sequential][full_program][stdev], data[key][sequential][jacobi][median], data[key][sequential][jacobi][mean], data[key][sequential][jacobi][stdev], data[key][parallel][full_program][median], data[key][parallel][full_program][mean], data[key][parallel][full_program][stdev], data[key][speedup][full_program], data[key][efficiency][full_program], data[key][speedup][jacobi], data[key][efficiency][jacobi]))

    return data

def generate_csv(data, file_name='data.csv'):
    file_location = 'res/'
    if not os.path.exists(file_location):
        os.makedirs(file_location)
    file_path = file_location + file_name
    print(f'Generating CSV file in...')
    try:
        with open(file_path, 'w') as file:
            file.write('Size,Threads,MedianSeq(F),MeanSeq(F),StdevSeq(F),MedianPar(F),MeanPar(F),StdevPar(F),Speedup(F),Efficiency(F),MedianSeq(J),MeanSeq(J),StdevSeq(J),MedianPar(J),MeanPar(J),StdevPar(J),Speedup(J),Efficiency(J)\n')
            for key in data:
                file.write(f'{key[0]},{key[1]},{data[key][sequential][full_program][median]},{data[key][sequential][full_program][mean]},{data[key][sequential][full_program][stdev]},{data[key][parallel][full_program][median]},{data[key][parallel][full_program][mean]},{data[key][parallel][full_program][stdev]},{data[key][speedup][full_program]},{data[key][efficiency][full_program]},{data[key][sequential][jacobi][median]},{data[key][sequential][jacobi][mean]},{data[key][sequential][jacobi][stdev]},{data[key][parallel][jacobi][median]},{data[key][parallel][jacobi][mean]},{data[key][parallel][jacobi][stdev]},{data[key][speedup][jacobi]},{data[key][efficiency][jacobi]}\n')
    except:
        exit('Error: could not write to file')
    clean_terminal()
    print(f"CSV file successfuly generated at {file_path}!")

compile()
sizes = [500, 1000, 2000, 4000]
threads = [2, 4, 8, 12]
runs = int(sys.argv[1]) if len(sys.argv) > 1 else 10
raw_data = run(runs, sizes, threads)
data = interpret_data(raw_data, show_data=False)
generate_csv(data)
