O_FILES = Parser.o ResultWriter.o

SerialTest: SerialTest.cpp $(O_FILES)
	g++ -g -o SerialTest SerialTest.cpp $(O_FILES)

BellmanFord_Test: Parallel_BellmanFord_Test.cpp BellmanFord.o Parser.o
	g++ -fopenmp -o BellmanFord_Test Parallel_BellmanFord_Test.cpp BellmanFord.o Parser.o

BellmanFord.o: BellmanFord.h BellmanFord.cpp
	g++ -c -o BellmanFord.o BellmanFord.cpp

Parser.o: Parser.cpp Parser.h
	g++ -g -c -o Parser.o Parser.cpp

ResultWriter.o: ResultWriter.cpp ResultWriter.h
	g++ -g -c -o ResultWriter.o ResultWriter.cpp
	
clean:
	rm -rf SerialTest BellmanFord_Test *.o