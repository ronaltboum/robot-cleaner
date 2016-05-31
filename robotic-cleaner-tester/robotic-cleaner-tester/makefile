CC = g++
LIBS = -ldl
CFLAGS = -std=c++11 -g -Wall -O2 -pedantic -pthread -g


default:
	make all
	
#simulator part
	
simulator: CommandLineInterpeter.o DirectionExt.o GeneralizedPoint.o FilesListerWithSuffix.o FilesLister.o Battery.o House.o SingletonHouseIOManager.o Sensor.o AlgorithmSingleRun.o SubSimulation.o SingletonConfigReader.o AlgorithmRegistration.o AlgorithmFactory.o ScoreRegistrar.o Simulator.o  Main.o
	$(CC) -rdynamic -o simulator $^ $(LIBS) -pthread
	
Main.o: Main.cpp
	$(CC) -c $(CFLAGS) $< $(LIBS)	

Simulator.o: Simulator.cpp Simulator.h Direction.h
	$(CC) -c $(CFLAGS) $<

ScoreRegistrar.o: ScoreRegistrar.cpp
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

CommandLineInterpeter.o: CommandLineInterpeter.cpp
	$(CC) -c $(CFLAGS) $<

	
	
#dynamic algorithms part

Battery.o: Battery.cpp
	$(CC) -c -fPIC $(CFLAGS) $^
#	g++ -c -fPIC -std=c++11 -g -Wall -O2 -pedantic -pthread Battery.cpp

DirectionExt.o: DirectionExt.cpp
	$(CC) -c -fPIC $(CFLAGS) $^

GeneralizedPoint.o: GeneralizedPoint.cpp
	$(CC) -c -fPIC $(CFLAGS) $^

DynamicPathFinder.o: DynamicPathFinder.cpp
	$(CC) -c -fPIC $(CFLAGS) $^

DynamicAlgo.o: DynamicAlgo.cpp
	$(CC) -c -fPIC $(CFLAGS) $^

_039563838_A.so: Battery.o DirectionExt.o GeneralizedPoint.o DynamicPathFinder.o DynamicAlgo.o _039563838_A.o
	$(CC) -shared -Wl,-soname,$@ -o $@ $^

_039563838_A.o: _039563838_A.cpp
	$(CC) -c -fPIC $(CFLAGS) $^

_039563838_B.so: Battery.o DirectionExt.o GeneralizedPoint.o DynamicPathFinder.o DynamicAlgo.o _039563838_B.o
	$(CC) -shared -Wl,-soname,$@ -o $@ $^

_039563838_B.o: _039563838_B.cpp
	$(CC) -c -fPIC $(CFLAGS) $^

_039563838_C.so: Battery.o DirectionExt.o GeneralizedPoint.o DynamicPathFinder.o DynamicAlgo.o _039563838_C.o
	$(CC) -shared -Wl,-soname,$@ -o $@ $^

_039563838_C.o: _039563838_C.cpp
	$(CC) -c -fPIC $(CFLAGS) $^

#dynamic default score formula
score_formula.so: score_formula.o
	$(CC) -shared -Wl,-soname,$@ -o $@ $^

score_formula.o: score_formula.cpp
	$(CC) -c -fPIC $(CFLAGS) $^
	
#all: 
all:  _039563838_A.so _039563838_B.so _039563838_C.so score_formula.so simulator

clean:
	rm -f *.so *.o simulator *~
