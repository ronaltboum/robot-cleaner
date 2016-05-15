CC = g++
LIBS = -ldl
CFLAGS = -std=c++0x -g -Wall -O2 -pedantic -pthread

default:
	make all
	
#simulator part
	
simulator: FilesListerWithSuffix.o FilesLister.o Battery.o House.o SingletonHouseIOManager.o Sensor.o AlgorithmSingleRun.o SubSimulation.o SingletonConfigReader.o Simulator.o AlgorithmFactory.o Main.o
	$(CC) -rdynamic -o simulator $^ $(LIBS) -pthread
	
Main.o: Main.cpp
	$(CC) -c $(CFLAGS) $< $(LIBS)	

AlgorithmFactory.o: AlgorithmFactory.cpp AlgorithmFactory.h $(LIBS)	
	$(CC) -c $(CFLAGS) $<
	
Simulator.o: Simulator.cpp Simulator.h Direction.h
	$(CC) -c $(CFLAGS) $<

SingletonConfigReader.o: SingletonConfigReader.cpp SingletonConfigReader.h	
	$(CC) -c $(CFLAGS) $<
	
SubSimulation.o: SubSimulation.cpp SubSimulation.h
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
	
FilesListerWithSuffix.o: FilesListerWithSuffix.cpp FilesListerWithSuffix.h FilesLister.h 
	$(CC) -c $(CFLAGS) $<
	
FilesLister.o: FilesLister.cpp FilesLister.h 
	$(CC) -c $(CFLAGS) $<	
	
#dynamic algorithms part


039563838_A_.so: _039563838_A.o
	g++ -shared -Wl,-soname,039563838_A_.so -o 039563838_A_.so _039563838_A.o

_039563838_A.o: _039563838_A.cpp _039563838_A.h AbstractAlgorithm.h FactoryDefinition.h AlgorithmRegistration.h
	$(CC) -c -fPIC $(CFLAGS) $<
	
039563838_C_.so: _039563838_C.o
	g++ -shared -Wl,-soname,039563838_C_.so -o 039563838_C_.so _039563838_C.o

_039563838_C.o: _039563838_C.cpp _039563838_C.h AbstractAlgorithm.h FactoryDefinition.h AlgorithmRegistration.h
	$(CC) -c -fPIC $(CFLAGS) $<
	
039563838_B_.so: _039563838_B.o
	g++ -shared -Wl,-soname,039563838_B_.so -o 039563838_B_.so _039563838_B.o

_039563838_B.o: _039563838_B.cpp _039563838_B.h AbstractAlgorithm.h FactoryDefinition.h AlgorithmRegistration.h
	$(CC) -c -fPIC $(CFLAGS) $<
	
	
#all: 
all: 039563838_A_.so 039563838_C_.so 039563838_B_.so  simulator

clean:
	rm -f *.so *.o simulator
