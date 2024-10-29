CC = g++
CFLAGS = -Wall -ggdb3 -std=c++11 -lcurl -pthread

Evaluate_impact: Evaluate_impact.o Bootstrap.o Calculation.o Extractdata.o DataProcessing.o Operator.o 
	$(CC) $(CFLAGS) -o  Evaluate_impact  Evaluate_impact.o  Bootstrap.o Calculation.o Extractdata.o DataProcessing.o Operator.o  

Evaluate_impact.o: Evaluate_impact.cpp  UI.h 
	$(CC) $(CFLAGS) -c  Evaluate_impact.cpp
 
Bootstrap.o: Calculation.h Bootstrap.h Bootstrap.cpp
	$(CC) $(CFLAGS) -c Bootstrap.cpp


Calculation.o: Extractdata.h Calculation.h Calculation.cpp
	$(CC) $(CFLAGS) -c Calculation.cpp
     
Extractdata.o:Operator.h DataProcessing.h Extractdata.h Extractdata.cpp
	$(CC) $(CFLAGS) -c Extractdata.cpp

DataProcessing.o: Stock.h DataProcessing.h DataProcessing.cpp
	$(CC) $(CFLAGS) -c DataProcessing.cpp
   
Operator.o: Operator.h Operator.cpp
	$(CC) $(CFLAGS) -c Operator.cpp
 



clean:
	rm -rf Evaluate_impact  *.o
