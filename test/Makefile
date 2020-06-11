CXXFLAGS =	-std=c++11 -O2 -g -Wall -fmessage-length=0

RPCLIB = /usr/local/lib/librpc.a

OBJS =	test_dfs.o primaryFrontEnd.o backupFrontEnd.o witnessFrontEnd.o common.o

LIBS = -lpthread -I /usr/local/include/rpc#-I ~/msgpack-c/include

TARGET = test_dfs

$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(RPCLIB) $(LIBS)


test_dfs.o:
	$(CXX) $(CXXFLAGS) -c test_dfs.cc $(LIBS)


primaryFrontEnd.o: 
	$(CXX) $(CXXFLAGS) -c ../server/primary/primaryFrontEnd.cc ../server/primary/primaryFrontEnd.h $(LIBS)
	
backupFrontEnd.o: common.o
	$(CXX) $(CXXFLAGS) -c ../server/backup/backupFrontEnd.cc ../server/backup/backupFrontEnd.h $(LIBS)

witnessFrontEnd.o: common.o
	$(CXX) $(CXXFLAGS) -c ../server/witness/witnessFrontEnd.cc ../server/witness/witnessFrontEnd.h $(LIBS)
	
common.o:
	$(CXX) $(CXXFLAGS) -c ../common/common.cc ../common/common.h $(LIBS)
	
all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) *.gch


