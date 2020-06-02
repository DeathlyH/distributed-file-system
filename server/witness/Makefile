CXXFLAGS =	-std=c++11 -O2 -g -Wall -fmessage-length=0

RPCLIB = /usr/local/lib/librpc.a

OBJS =	witness.o common.o

LIBS = -lpthread -I /usr/local/include/rpc#-I ~/msgpack-c/include

TARGET = witness_server

$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(RPCLIB) $(LIBS)

witness.o:
	$(CXX) $(CXXFLAGS) -c witness.cc witness.h $(LIBS)
	
common.o:
	$(CXX) $(CXXFLAGS) -c ../../common/common.cc ../../common/common.h $(LIBS)
	
all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) *.gch

