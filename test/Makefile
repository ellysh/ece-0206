all: test_02061

test_02061.o: library02061.c OZUtest.c  SI_test.c  SO_test.c  tvk02061.c
	g++ -g -rdynamic -c library02061.c OZUtest.c  SI_test.c  SO_test.c  tvk02061.c 

test_02061: test_02061.o
	g++ test_02061.o -o test_02061
