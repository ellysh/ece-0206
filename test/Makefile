all: test_02061

tvk02061.o: library02061.c OZUtest.c  SI_test.c  SO_test.c  tvk02061.c tvk02061.h
	g++ -g -rdynamic -c tvk02061.c

test_02061: tvk02061.o
	g++ tvk02061.o -o test_02061
