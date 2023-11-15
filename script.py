import subprocess

# Specify the path to your hostfile.txt
HOSTFILE = "hostfile.txt"

# Specify the path to your compiled MPI program
MPI_PROGRAM = "./arraySum"

# Loop through batches from 1 to 1000
for BATCHES in range(1, 1001):
    # Construct the MPI command
    mpi_command = f"mpiexec -f {HOSTFILE} -n 3 {MPI_PROGRAM} {BATCHES}"

    # Execute the MPI command using subprocess
    subprocess.run(mpi_command, shell=True)