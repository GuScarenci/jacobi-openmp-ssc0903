import os
import subprocess
import re
import statistics
import sys

root_folder = 'jacobi-openmp-ssc0903'
sequential = 'sequential'
parallel = 'parallel'
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

def run(runs, sizes, threads):
    response_times = {}
    # run the code
    print("Starting data acquisition...")
    for _ in range(runs):
        for size in sizes:
            print('\033[F', end='')
            print(f'Running with matrix of size {size} and sequentially')
            command = f'make run_seq ARGS="{size} {1} {0}"'
            result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            output = result.stderr if result.stderr else result.stdout

            time_pattern = r'real\s+(\d+m\d+\.\d+s)\nuser\s+(\d+m\d+\.\d+s)\nsys\s+(\d+m\d+\.\d+s)'
            match = re.search(time_pattern, output)
            response_sequential = float(match.group(1).split('m')[1].split('s')[0])

            response_times.setdefault(size, {}).setdefault(sequential, []).append(response_sequential)

            for thread in threads:
                print('\033[F', end='')
                print('\033[K', end='')
                print(f'Running with matrix of size {size} and {thread} threads')

                command = f'make run_par ARGS="{size} {thread} {0}"'
                result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
                output = result.stderr if result.stderr else result.stdout

                time_pattern = r'real\s+(\d+m\d+\.\d+s)\nuser\s+(\d+m\d+\.\d+s)\nsys\s+(\d+m\d+\.\d+s)'
                match = re.search(time_pattern, output)
                response_parallel = float(match.group(1).split('m')[1].split('s')[0])
                response_times[size].setdefault(parallel, {}).setdefault(thread, []).append(response_parallel)

    #clear entire last line in terminal
    print('\033[F', end='')
    print('\033[K', end='')
    print("Code ran successfully!")
    return response_times

def interpret_data(raw_data, show_data:bool=False):
    print("Interpreting data...")
    try:
        data = {}
        for size in raw_data:
            sequential_times = raw_data[size][sequential]
            sequential_median = round(statistics.median(sequential_times),5)
            sequential_mean = round(statistics.mean(sequential_times),5)
            sequential_stdev = round(statistics.stdev(sequential_times),5)

            for thread in raw_data[size][parallel]:
                key = (size, thread)
                data[key] = {}
                parallel_times = raw_data[size][parallel][thread]
                data[key][sequential] = {}
                data[key][sequential][median] = sequential_median
                data[key][sequential][mean] = sequential_mean
                data[key][sequential][stdev] = sequential_stdev

                data[key][parallel] = {}
                data[key][parallel][median] = round(statistics.median(parallel_times),5)
                data[key][parallel][mean] = round(statistics.mean(parallel_times),5)
                data[key][parallel][stdev] = round(statistics.stdev(parallel_times),5)

                data[key][speedup] = round(data[key][sequential][mean] / data[key][parallel][mean],5)
                data[key][efficiency] = round(data[key][speedup] / key[1],5)
    except:
        exit('Error: could not interpret data')
    print('\033[F', end='')
    print('\033[K', end='')
    print("Data interpreted successfully!")

    if show_data:
        print("{:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10}".format('Size', 'Threads', 'Median Seq', 'Mean Seq', 'Stdev Seq', 'Median Par', 'Mean Par', 'Stdev Par', 'Speedup', 'Efficiency'))
        for key in data:
            print("{:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10} {:<10}".format(key[0], key[1], data[key][sequential][median], data[key][sequential][mean], data[key][sequential][stdev], data[key][parallel][median], data[key][parallel][mean], data[key][parallel][stdev], data[key][speedup], data[key][efficiency]))

    return data

def generate_csv(data, file_name='data.csv'):
    file_location = 'res/'
    if not os.path.exists(file_location):
        os.makedirs(file_location)
    file_path = file_location + file_name
    print(f'Generating CSV file in...')
    try:
        with open(file_path, 'w') as file:
            file.write('Size,Threads,Median Seq,Mean Seq,Stdev Seq,Median Par,Mean Par,Stdev Par,Speedup,Efficiency\n')
            for key in data:
                file.write(f'{key[0]},{key[1]},{data[key][sequential][median]},{data[key][sequential][mean]},{data[key][sequential][stdev]},{data[key][parallel][median]},{data[key][parallel][mean]},{data[key][parallel][stdev]},{data[key][speedup]},{data[key][efficiency]}\n')
    except:
        exit('Error: could not write to file')
    print('\033[F', end='')
    print('\033[K', end='')
    print(f"CSV file successfuly generated at {file_path}!")

compile()
sizes = [10, 100]
threads = [2, 4]
runs = int(sys.argv[1]) if len(sys.argv) > 1 else 10
raw_data = run(runs, sizes, threads)
data = interpret_data(raw_data)
generate_csv(data)