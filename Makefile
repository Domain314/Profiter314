profiter: main.o FileOrganizer.o Holder.o Profit.o TimeEngine.o Transfer.o
	g++ main.o Config.o FileOrganizer.o Holder.o Profit.o TimeEngine.o Transfer.o .o profiter

Config.o: Profiter/Misc/Config.cpp
	g++ -std=c++11 -c Profiter/Misc/Config.cpp

FileOrganizer.o: Profiter/FileOrganizer.cpp
	g++ -std=c++11 -c Profiter/FileOrganizer.cpp

TimeEngine.o: Profiter/TimeEngine.cpp
	g++ -std=c++11 -c Profiter/TimeEngine.cpp

Holder.o: Profiter/Holder.cpp
	g++ -std=c++11 -c Profiter/Holder.cpp

Profit.o: Profiter/Profit.cpp
	g++ -std=c++11 -c Profiter/Profit.cpp

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

Transfer.o: Profiter/Transfer.cpp
	g++ -std=c++11 -c Profiter/Transfer.cpp

clean:
	rm *.o profiter

run:
	./profiter