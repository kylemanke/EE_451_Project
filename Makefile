O_FILES = Parser.o ResultWriter.o

SerialTest: SerialTest.cpp $(O_FILES)
	g++ -o SerialDjikstra SerialDjikstra.cpp $(O_FILES)

Parser.o: Parser.cpp Parser.h
	g++ -c -o Parser.o Parser.cpp

ResultWriter.o: ResultWriter.cpp ResultWriter.h
	g++ -c -o ResultWriter.o ResultWriter.cpp

clean:
	rm -rf SerialDjikstra *.o