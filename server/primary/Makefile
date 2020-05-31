CXXFLAGS =	-std=c++11 -O2 -g -Wall -fmessage-length=0

RPCLIB = /usr/local/lib/librpc.a

OBJS =	primary.o backupServerDummy.o common.o

LIBS = -lpthread -I /usr/local/include/rpc#-I ~/msgpack-c/include

TARGET = primary_server

$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(RPCLIB) $(LIBS)

primary.o:
	$(CXX) $(CXXFLAGS) -c primary.cc primary.h $(LIBS)
	
backupServerDummy.o: common.o
	$(CXX) $(CXXFLAGS) -c backupServerDummy.cc backupServerDummy.h $(LIBS)
	
common.o:
	$(CXX) $(CXXFLAGS) -c ../../common/common.cc ../../common/common.h $(LIBS)
	
all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) *.gch


