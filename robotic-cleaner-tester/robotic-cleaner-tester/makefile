CC = g++-5
LIBS = -ldl
CFLAGS = -std=c++14 -g -Wall -O2 -pedantic -pthread


default:
	make all
	
#simulator part
	
simulator: DirectionExt.o GeneralizedPoint.o FilesListerWithSuffix.o FilesLister.o Battery.o House.o SingletonHouseIOManager.o Sensor.o AlgorithmSingleRun.o SubSimulation.o SingletonConfigReader.o AlgorithmRegistration.o AlgorithmFactory.o Simulator.o  Main.o
	$(CC) -rdynamic -o simulator $^ $(LIBS) -pthread
	
Main.o: Main.cpp
	$(CC) -c $(CFLAGS) $< $(LIBS)	

Simulator.o: Simulator.cpp Simulator.h Direction.h
	$(CC) -c $(CFLAGS) $<

AlgorithmFactory.o: AlgorithmFactory.cpp AlgorithmFactory.h $(LIBS)	
	$(CC) -c $(CFLAGS) $<

AlgorithmRegistration.o: AlgorithmRegistration.cpp	
	$(CC) -c $(CFLAGS) $^ $(LIBS)

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

	
FilesListerWithSuffix.o: FilesListerWithSuffix.cpp FilesListerWithSuffix.h FilesLister.h 
	$(CC) -c $(CFLAGS) $<
	
FilesLister.o: FilesLister.cpp FilesLister.h 
	$(CC) -c $(CFLAGS) $<

GeneralizedPoint.o: GeneralizedPoint.cpp GeneralizedPoint.h 
	$(CC) -c $(CFLAGS) $<

DirectionExt.o: DirectionExt.cpp DirectionExt.h 
	$(CC) -c $(CFLAGS) $<	
	
#dynamic algorithms part

Battery.o: Battery.cpp
	$(CC) -c -fPIC $(CFLAGS) $^
#	g++-5 -c -fPIC -std=c++14 -g -Wall -O2 -pedantic -pthread Battery.cpp

_039563838_D.so: Battery.o _039563838_D.o
	$(CC) -shared -Wl,-soname,$@ -o $@ $^

_039563838_D.o: _039563838_D.cpp
	$(CC) -c -fPIC $(CFLAGS) $^
	
#all: 
all: _039563838_D.so simulator

clean:
	rm -f *.so *.o simulator *~
