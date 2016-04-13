CC      = g++
CFLAGS = -std=c++0x -g -Wall -O2 -pedantic -pthread
LIBS = -ldl

all: simulator

clean:
	-rm *.o *.so
	
#making the simulator 

simulator: robotic-cleaner-tester.o Simulator.o SingletonConfigReader.o SingletonHouseIOManager.o AlgorithmSingleRun.o AlgorithmFactory.o Sensor.o House.o Battery.o
	$(CC) -o $@ $^ $(LIBS)
	
robotic-cleaner-tester.o: robotic-cleaner-tester.cpp Direction.h
	$(CC) -c $(CFLAGS) $<

Simulator.o: Simulator.cpp Simulator.h Direction.h
	$(CC) -c $(CFLAGS) $<	

SingletonConfigReader.o: SingletonConfigReader.cpp SingletonConfigReader.h
	$(CC) -c $(CFLAGS) $<	

AlgorithmSingleRun.o: AlgorithmSingleRun.cpp AlgorithmSingleRun.h AbstractSensor.h Direction.h
	$(CC) -c $(CFLAGS) $<	
	
AlgorithmFactory.o: AlgorithmFactory.cpp AlgorithmFactory.h
	$(CC) -c $(CFLAGS) $< $(LIBS)	
	
Sensor.o: Sensor.cpp Sensor.h AbstractSensor.h House.h SensorInformation.h
	$(CC) -c $(CFLAGS) $<

SingletonHouseIOManager.o: SingletonHouseIOManager.cpp SingletonHouseIOManager.h House.h
	$(CC) -c $(CFLAGS) $<	
	
House.o: House.cpp SingletonHouseIOManager.h House.h Point.h Direction.h Matrix.h
	$(CC) -c $(CFLAGS) $<

Battery.o: Battery.cpp Battery.h 
	$(CC) -c $(CFLAGS) $<
