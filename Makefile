CXXFLAG = -O3 -Iinclude -std=c++11
LDFLAG = -lopengl32 -lglu32 -lglut32
CXXSHELL = g++ -o $@ $< -c $(CXXFLAG) 
LDSHELL = g++ -o $@ $^ $(LDFLAG)

all: main.exe
clean:
	del main.exe
	del src\*.o
src/main.o: src/main.cpp src/game.hpp src/event.hpp src/label.hpp src/button.hpp src/widget.hpp
	$(CXXSHELL)
src/game.o: src/game.cpp src/game.hpp src/event.hpp src/label.hpp src/button.hpp src/widget.hpp
	$(CXXSHELL)
src/label.o: src/label.cpp src/event.hpp src/label.hpp src/widget.hpp
	$(CXXSHELL)
src/button.o: src/button.cpp src/event.hpp src/label.hpp src/button.hpp src/widget.hpp
	$(CXXSHELL)
src/event.o: src/event.cpp src/event.hpp
	$(CXXSHELL)
src/widget.o: src/widget.cpp src/event.hpp src/widget.hpp
	$(CXXSHELL)
main.exe: src/main.o src/game.o src/label.o src/button.o src/event.o src/widget.o
	$(LDSHELL)
