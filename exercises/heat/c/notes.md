# Profiling

srun --account=project_2003514 --ntasks=1 --time=00:15:00 --gres=gpu:v100:1 --partition=gputest nvprof --cpu-profiling on --cpu-profiling-mode top-down ./heat.exe
