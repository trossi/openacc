# make all |& tee make.log

# module load pgi/19.7

all: main.exe main-fast.exe main-acc.exe main-acc-fast.exe main-managed.exe

main.exe: jacobi.c
	pgcc -Minfo=all $^ -o $@

main-fast.exe: jacobi.c
	pgcc -fast -Minfo=all $^ -o $@

main-acc.exe: jacobi.c
	pgcc -acc -Minfo=all $^ -o $@

main-acc-fast.exe: jacobi.c
	pgcc -acc -fast -Minfo=all $^ -o $@

main-managed.exe: jacobi.c
	pgcc -acc -fast -ta=nvidia:cc70,managed -Minfo=all $^ -o $@

clean:
	rm *.exe
