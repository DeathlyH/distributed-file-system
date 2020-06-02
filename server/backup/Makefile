CXXFLAGS =	-std=c++11 -O2 -g -Wall -fmessage-length=0

RPCLIB = /usr/local/lib/librpc.a

OBJS =	backup.o witnessFrontEnd.o common.o

LIBS = -lpthread -I /usr/local/include/rpc#-I ~/msgpack-c/include

TARGET = backup_server

$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(RPCLIB) $(LIBS)

backup.o:
	$(CXX) $(CXXFLAGS) -c backup.cc backup.h $(LIBS)

witnessFrontEnd.o: common.o
	$(CXX) $(CXXFLAGS) -c ../witness/witnessFrontEnd.cc ../witness/witnessFrontEnd.h $(LIBS)
	
common.o:
	$(CXX) $(CXXFLAGS) -c ../../common/common.cc ../../common/common.h $(LIBS)
	
all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) *.gch

