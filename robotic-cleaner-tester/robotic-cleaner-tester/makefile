CC = g++
LIBS = -ldl
CFLAGS = -std=c++0x -g -Wall -O2 -pedantic -pthread

default:
	make all
	
#simulator part
	
simulator: Battery.o House.o SingletonHouseIOManager.o Sensor.o AlgorithmSingleRun.o SingletonConfigReader.o Simulator.o AlgorithmFactory.o Main.o
	$(CC) -rdynamic -o simulator $^ $(LIBS)
	
Main.o: Main.cpp
	$(CC) -c $(CFLAGS) $< $(LIBS)	

AlgorithmFactory.o: AlgorithmFactory.cpp AlgorithmFactory.h $(LIBS)	
	$(CC) -c $(CFLAGS) $<
	
Simulator.o: Simulator.cpp Simulator.h Direction.h
	$(CC) -c $(CFLAGS) $<

SingletonConfigReader.o: SingletonConfigReader.cpp SingletonConfigReader.h	
	$(CC) -c $(CFLAGS) $<

AlgorithmSingleRun.o: AlgorithmSingleRun.cpp AlgorithmSingleRun.h AbstractAlgorithm.h AbstractSensor.h Direction.h	House.h Point.h Battery.h
	$(CC) -c $(CFLAGS) $<
	
Sensor.o: Sensor.cpp Sensor.h AbstractSensor.h House.h SensorInformation.h
	$(CC) -c $(CFLAGS) $<
	
SingletonHouseIOManager.o: SingletonHouseIOManager.cpp SingletonHouseIOManager.h House.h
	$(CC) -c $(CFLAGS) $<
	
House.o: House.cpp SingletonHouseIOManager.h House.h Matrix.h Point.h Direction.h 
	$(CC) -c $(CFLAGS) $<

Battery.o: Battery.cpp Battery.h 
	$(CC) -c $(CFLAGS) $<
	
	
	
#dynamic algorithms part

libAlgorithm1.so: Algorithm1.o
	g++ -shared -Wl,-soname,libAlgorithm1.so -o libAlgorithm1.so Algorithm1.o

libAlgorithm2.so: Algorithm2.o
	g++ -shared -Wl,-soname,libAlgorithm2.so -o libAlgorithm2.so Algorithm2.o

Algorithm1.o: Algorithm1.cpp Algorithm1.h AbstractAlgorithm.h
	$(CC) -c -fPIC $(CFLAGS) $<
	
Algorithm2.o: Algorithm2.cpp Algorithm2.h AbstractAlgorithm.h
	$(CC) -c -fPIC $(CFLAGS) $<

all: simulator libAlgorithm1.so libAlgorithm2.so 

clean:
	rm -f *.so *.o simulator
