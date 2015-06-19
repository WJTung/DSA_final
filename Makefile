CFLAGS = -O2 -Wall
CPPFLAGS = -O2 -std=c++11 -Wall

%: %.c
	gcc $(CFLAGS) $< -o $@
%: %.cpp
	g++ $(CPPFLAGS) $< -o $@
%.o: %.cpp
	g++ $(CPPFLAGS) $< -c -o $@

all:main

main: main.o bank.o find_similar_strings.o md5.o match_string.o 
	g++ main.o bank.o find_similar_strings.o md5.o match_string.o -o final_project
main.o: main.cpp bank.h
	g++ $(CPPFLAGS) $< -c -o $@
bank.o: bank.cpp bank.h md5.h
	g++ $(CPPFLAGS) $< -c -o $@
find_similar_strings.o: find_similar_strings.cpp bank.h
	g++ $(CPPFLAGS) $< -c -o $@

clean:
	rm -f *.o final_project

debug: main
	rm -f test2.out
	./final_project < test2.in > test2.out
