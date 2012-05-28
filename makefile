# vim: noet

racing_line_analyser: main.o gui.o graphics.o racing_line.o
	g++ -o racing_line_analyser main.o gui.o graphics.o racing_line.o -lGL -lGLU -lglut

main.o: gui.h racing_line.h graphics.h
	g++ -c -Wall main.cpp

gui.o: gui.h graphics.h racing_line.h
	g++ -c -Wall gui.cpp

racing_line.o: racing_line.h graphics.h
	g++ -c -Wall racing_line.cpp

graphics.o: graphics.h
	g++ -c -Wall graphics.cpp

