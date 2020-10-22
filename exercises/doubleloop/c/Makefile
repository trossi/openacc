# make all |& tee make.log

# module load pgi/19.7

all: main.exe main-fast.exe main-acc.exe main-acc-fast.exe

main.exe: doubleloop.c
	pgcc -Minfo=all $^ -o $@

main-fast.exe: doubleloop.c
	pgcc -fast -Minfo=all $^ -o $@

main-acc.exe: doubleloop.c
	pgcc -acc -Minfo=all $^ -o $@

main-acc-fast.exe: doubleloop.c
	pgcc -acc -fast -Minfo=all $^ -o $@

clean:
	rm *.exe
