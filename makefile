####################################################################
#
# Makefile for CMPT 135 Spring 2021, SFU Surrey.
#
#####################################################################

# Set the C++ compiler options:
#   -std=c++17 compiles using the C++17 standard
#   -Wall turns on all warnings
#   -Wextra turns on even more warnings
#   -Werror causes warnings to be errors 
#   -Wfatal-errors stops the compiler after the first error
#   -Wno-sign-compare turns off warnings for comparing signed and 
#    unsigned numbers
#   -Wnon-virtual-dtor warns about non-virtual destructors
#   -g puts debugging info into the executables (makes them larger)
CPPFLAGS = -std=c++17 -Wall -Wextra -Werror -Wfatal-errors -Wno-sign-compare -Wnon-virtual-dtor -g

final_project_main_test: Database.o final_project_main.o Menu.o Team.o
	g++ -o final_project_main_test Database.o final_project_main.o Menu.o Team.o -lncurses

Database: Database.cpp
	g++ -c $(CPPFLAGS) Database.cpp

Database_main:	Database_main.cpp
	g++ -c $(CPPFLAGS) Database_main.cpp 

divisions: divisions.cpp
	g++ -c $(CPPFLAGS) divisions.cpp

former_divisions: former_divisions.cpp
	g++ -c $(CPPFLAGS) former_divisions.cpp

#requires -lncurses tag 
Menu: Menu.cpp
	g++ -c $(CPPFLAGS) -lncurses -o Menu.cpp

Menu_test: Menu.o 
	g++ -o Menu_test Menu.o

all: *.cpp
	g++ -c $(CPPFLAGS) *.cpp 

all_test: *.o
	g++ -o test *.o -lncurses

clean:
	rm -f final_project_main_test Database.o Team.o final_project_main.o Menu.o test