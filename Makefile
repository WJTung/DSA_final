CFLAGS = -O2 -Wall
CPPFLAGS = -O2 -std=c++11 -Wall

%: %.c
	gcc $(CFLAGS) $< -o $@
%: %.cpp
	g++ $(CPPFLAGS) $< -o $@
%.o: %.cpp
	g++ $(CPPFLAGS) $< -c $@

all:

main: main.o bank.o find_similar_strings.o 
	g++ main.o bank.o find_similar_strings.o -o main
main.o: main.cpp bank.h
	g++ $(CPPFLAGS) $< -c $@
bank.o: bank.cpp bank.h md5.h
	g++ $(CPPFLAGS) $< -c $@
find_similar_strings: find_similar_strings.cpp bank.h
	g++ $(CPPFLAGS) $< -c $@

