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

final_project_main_test: final_project.o final_project_main.o 
	g++ -o final_project_main_test final_project.o final_project_main.o 

final_project:
	g++ -c $(CPPFLAGS) final_project.cpp

final_project_main:
	g++ -c $(CPPFLAGS) final_project_main.cpp

clean:
	rm -f final_project_main_test final_project.o final_project_main.o 