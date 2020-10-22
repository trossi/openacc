#!/bin/bash
#SBATCH --account=project_2003514
#SBATCH --ntasks=1
#SBATCH --time=00:15:00
#SBATCH --gres=gpu:v100:1
##SBATCH --partition=gputest
#SBATCH --partition=gpu
#SBATCH --reservation=gpuhackathon


module load pgi/19.7
# module load pgi/19.7 cuda/10.1.168

echo "---- info"
pgaccelinfo -v
echo -e "---- end\n"

echo "---- hello c"
srun ./hello-c.exe
echo -e "---- end\n"

echo "---- hello c acc"
srun ./hello-c-acc.exe
echo -e "---- end\n"

echo "---- hello f"
srun ./hello-f.exe
echo -e "---- end\n"

echo "---- hello f acc"
srun ./hello-f-acc.exe
echo -e "---- end\n"


# Submit to the batch job queue with the command:
#  sbatch job.sh
