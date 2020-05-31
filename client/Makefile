CXXFLAGS =	-std=c++11 -O2 -g -Wall -fmessage-length=0

RPCLIB = /usr/local/lib/librpc.a

OBJS =	harp_client_main.o 

LIBS = -lpthread

TARGET = harp_client

$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(RPCLIB) $(LIBS)

harp_client_main.o:
	$(CXX) $(CXXFLAGS) -c harp_client_main.cc 
#primary_server.o:
#	$(CXX) $(CXXFLAGS) -c primary_server.cpp
    
#client.o:
#	$(CXX) $(CXXFLAGS) -c harp_client.cpp harp_client.hpp
#
##backup.o:
##	$(CXX) $(CXXFLAGS) -c ../backup/backup.cc ../backup/backup.h
#communication_service.o:
#	$(CXX) $(CXXFLAGS) -c ../common/common.cc ../common/common.h
#common.o:
#	$(CXX) $(CXXFLAGS) -c ../common/common.cc ../common/common.h
	
all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
