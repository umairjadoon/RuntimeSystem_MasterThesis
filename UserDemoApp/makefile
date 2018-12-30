maekCXX= g++
CXXFLAGS= -Wall -Werror -g
SRC=$(wildcard *.cpp)
OBJ=$(patsubst %.cpp, %.o, $(SRC))
NAME= libuserapp
EXECNAME= userapp_exec
INCLUDE:=-I../../RuntimeSystem/lib/include/

.PHONY = all clean executable

all : $(OBJ) ../RuntimeSystem/bin/libruntime.a ../HAL-Scheduling/bin/librunner.a
	ar rcs $(NAME).a $(OBJ)
	#$(CXX) $^ -o $(NAME)
	
%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(INCLUDE)
	
executable :
	$(CXX) -o $(EXECNAME) -lrunner -lruntime -luserapp -L../HAL-Scheduling/bin -L../RuntimeSystem/bin -L. -lpthread

clean:
	rm -f *.o *.a userapp_exec $(NAME)

