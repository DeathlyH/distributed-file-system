CXXFLAGS =	-std=c++11 -O2 -g -Wall -fmessage-length=0

RPCLIB = /usr/local/lib/librpc.a

OBJS =	client-main.o 

LIBS = -lpthread

TARGET = harp_client

$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(RPCLIB) $(LIBS)

client-main.o:
	$(CXX) $(CXXFLAGS) -c client-main.cc

	
all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
