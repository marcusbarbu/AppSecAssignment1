default: test

get-deps:
	# Assuming Debian or Ubuntu here
	sudo apt-get install -y build-essential check

dictionary.o: dictionary.c
	gcc -Wall -c dictionary.c dictionary.h

spell.o: spell.c
	gcc -Wall -ggdb -c spell.c

test.o: test_main.c
	gcc -Wall -c test_main.c

main.o: main.c
	gcc -Wall -c main.c
m_main.o: m_main.c
	gcc -Wall -c m_main.c

test: dictionary.o spell.o test_main.o
	gcc -Wall -o test_main test_main.o spell.o dictionary.o -lcheck -lm -lrt -lpthread -lsubunit
	./test_main

mtest: dictionary.o spell.o m_main.o
	gcc -Wall -ggdb -o m_main m_main.o spell.o dictionary.o -lcheck -lm -lrt -lpthread -lsubunit
	./m_main

prog: dictionary.o spell.o main.o
	gcc -Wall -o spell_check dictionary.o spell.o main.o

clean:
	rm dictionary.o spell.o main.o test_main.o check_spell.o

cleanall:clean
	rm spell_check
	
