#!/bin/bash
#SBATCH --account=project_2003514
#SBATCH --ntasks=1
#SBATCH --time=00:15:00
#SBATCH --gres=gpu:v100:1
##SBATCH --partition=gputest
#SBATCH --partition=gpu
#SBATCH --reservation=gpuhackathon
#SBATCH -o job.log

#module load pgi/19.7
module load pgi/19.7 cuda/10.1.168

function run {
    echo "---- $@"
    srun "$@"
    echo -e "---- end\n"
}

pgaccelinfo -v
echo

run ./sum.exe
run ./sum-fast.exe
run ./sum-acc.exe
run ./sum-acc-fast.exe
