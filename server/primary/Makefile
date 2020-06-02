CXXFLAGS =	-std=c++11 -O2 -g -Wall -fmessage-length=0

RPCLIB = /usr/local/lib/librpc.a

OBJS =	primary.o backupFrontEnd.o witnessFrontEnd.o common.o

LIBS = -lpthread -I /usr/local/include/rpc#-I ~/msgpack-c/include

TARGET = primary_server

$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(RPCLIB) $(LIBS)

primary.o:
	$(CXX) $(CXXFLAGS) -c primary.cc primary.h $(LIBS)
	
backupFrontEnd.o: common.o
	$(CXX) $(CXXFLAGS) -c ../backup/backupFrontEnd.cc ../backup/backupFrontEnd.h $(LIBS)

witnessFrontEnd.o: common.o
	$(CXX) $(CXXFLAGS) -c ../witness/witnessFrontEnd.cc ../witness/witnessFrontEnd.h $(LIBS)
	
common.o:
	$(CXX) $(CXXFLAGS) -c ../../common/common.cc ../../common/common.h $(LIBS)
	
all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) *.gch


