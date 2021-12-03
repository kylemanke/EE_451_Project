O_FILES = Parser.o ResultWriter.o

SerialTest: SerialTest.cpp $(O_FILES)
	g++ -g -o SerialTest SerialTest.cpp $(O_FILES)

Parser.o: Parser.cpp Parser.h
	g++ -g -c -o Parser.o Parser.cpp

ResultWriter.o: ResultWriter.cpp ResultWriter.h
	g++ -g -c -o ResultWriter.o ResultWriter.cpp

clean:
	rm -rf SerialTest *.o