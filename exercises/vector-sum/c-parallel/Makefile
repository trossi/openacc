# make all |& tee make.log

# module load pgi/19.7

all: sum.exe sum-fast.exe sum-acc.exe sum-acc-fast.exe

sum.exe: sum.c
	pgcc -Minfo=all $^ -o $@

sum-fast.exe: sum.c
	pgcc -fast -Minfo=all $^ -o $@

sum-acc.exe: sum.c
	pgcc -acc -Minfo=all $^ -o $@

sum-acc-fast.exe: sum.c
	pgcc -acc -fast -Minfo=all $^ -o $@

clean:
	rm *.exe
