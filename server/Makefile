CXXFLAGS =	-std=c++11 -O2 -g -Wall -fmessage-length=0

RPCLIB = /usr/local/lib/librpc.a

OBJS =	primary_server.o primary.o backup.o common.o

LIBS = -lpthread

TARGET = primary_server

$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(RPCLIB) $(LIBS)

primary_server.o:
	$(CXX) $(CXXFLAGS) -c primary/primary_server.cc
    
primary.o:
	$(CXX) $(CXXFLAGS) -c primary/primary.cc primary/primary.h

backup.o:
	$(CXX) $(CXXFLAGS) -c backup/backup.cc backup/backup.h
	
common.o:
	$(CXX) $(CXXFLAGS) -c ../common/common.cc ../common/common.h
	
all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
