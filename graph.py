import matplotlib.pyplot as plt

# read the results.txt file
with open('results.txt', 'r') as f:
    lines = f.readlines()

# extract the execution time and batch number from each line
execution_times = []
batch_numbers = []
for line in lines:
    parts = line.split()
    execution_time = float(parts[2])
    batch_number = int(parts[7])
    execution_times.append(execution_time)
    batch_numbers.append(batch_number)

# plot the execution time vs batch number
plt.plot(batch_numbers, execution_times)
plt.xlabel('Batches')
plt.ylabel('Execution Time (seconds)')
plt.title('Execution Time vs Batches')
plt.show()