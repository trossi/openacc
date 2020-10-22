#!/bin/bash

module load pgi/19.7
# module load pgi/19.7 cuda/10.1.168

pgcc hello.c -o hello-c.exe
pgcc -acc hello.c -o hello-c-acc.exe

pgf90 hello.F90 -o hello-f.exe
pgf90 -acc hello.F90 -o hello-f-acc.exe
