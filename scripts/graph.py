import matplotlib.pyplot as plt
import numpy as np

# Data from the table
data = {
    1000: {1: 1.000, 2: 12.992, 4: 24.020, 8: 30.189, 12: 13.862},
    3000: {1: 1.000, 2: 9.720, 4: 13.432, 8: 13.645, 12: 12.408},
    5000: {1: 1.000, 2: 9.113, 4: 12.788, 8: 13.245, 12: 12.475}
}

# Set a common y-axis limit
y_max = 35

# Function to plot the graph for each matrix size
def plot_speedup(matrix_size, threads_speedup):
    threads = list(threads_speedup.keys())
    speedups = list(threads_speedup.values())
    
    plt.figure(figsize=(7, 7))
    plt.plot(threads, speedups, marker='o', label='Speedup Observado')
    
    # Extend linear line to y_max
    max_threads = max(threads)
    extended_threads = list(range(1, max_threads + 1)) + [int(y_max)]
    extended_linear_speedup = extended_threads
    
    plt.plot(extended_threads, extended_linear_speedup, linestyle='--', color='r', label='Speedup Linear')  # Linear reference line
    plt.title(f'Tamanho da Matriz: {matrix_size}')
    plt.xlabel('Threads')
    plt.ylabel('Speedup')
    plt.ylim(0, y_max)
    plt.xlim(0, y_max)
    plt.grid(True)
    
    # Add annotations for data points
    for i in range(len(threads)):
        plt.annotate(f'{speedups[i]:.3f}', (threads[i], speedups[i]), textcoords="offset points", xytext=(0,5), ha='center')
    
    plt.legend()
    plt.show()

# Plot for each matrix size
for matrix_size, threads_speedup in data.items():
    plot_speedup(matrix_size, threads_speedup)
