# Profiling

## nvprof

module load pgi/19.7 cuda/10.1.168

srun --account=project_2003514 --ntasks=1 --time=00:15:00 --gres=gpu:v100:1 --partition=gputest nvprof ./main.exe 200 200 10000

# CPU only
srun --account=project_2003514 --ntasks=1 --time=00:15:00 --gres=gpu:v100:1 --partition=gputest nvprof --cpu-profiling on --cpu-profiling-mode top-down ./main.exe 200 200 10000

## nsys

# Load cuda11 (under development)

module purge
module unuse /appl/spack/modulefiles/linux-rhel7-x86_64/Core
module use /appl/test/new_pgi/modulefiles/linux-rhel7-x86_64/Core/
module load pgi cuda

srun --account=project_2003514 --ntasks=1 --time=00:15:00 --gres=gpu:v100:1 --partition=gputest nsys profile -t nvtx,openacc --stats=true -s cpu ./main.exe 200 200 10000

