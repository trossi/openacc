# make all |& tee make.log

# module load pgi/19.7

CC=pgc++
CFLAGS=-Minfo=accel

all: main.exe main-fast.exe main-acc.exe main-acc-fast.exe main-managed.exe

main.exe: main.cpp
	${CC} ${CFLAGS} $^ -o $@

main-fast.exe: main.cpp
	${CC} -fast ${CFLAGS} $^ -o $@

main-acc.exe: main.cpp
	${CC} -acc ${CFLAGS} $^ -o $@

main-acc-fast.exe: main.cpp
	${CC} -acc -fast ${CFLAGS} $^ -o $@

main-managed.exe: main.cpp
	${CC} -acc -fast -ta=nvidia:cc70,managed ${CFLAGS} $^ -o $@

clean:
	rm *.exe
