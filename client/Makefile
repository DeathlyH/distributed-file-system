CXXFLAGS =	-std=c++11 -O2 -g -Wall -fmessage-length=0

RPCLIB = /usr/local/lib/librpc.a

OBJS =	primaryFrontEnd.o backupFrontEnd.o witnessFrontEnd.o common.o

LIBS = -lpthread -I /usr/local/include/rpc#-I ~/msgpack-c/include



all: read_harp write_harp


read_harp: read.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o read_harp read.o $(OBJS) $(RPCLIB) $(LIBS)
	
write_harp: write.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o write_harp write.o $(OBJS) $(RPCLIB) $(LIBS)

read.o:
	$(CXX) $(CXXFLAGS) -c read.cc $(LIBS)
	
write.o:
	$(CXX) $(CXXFLAGS) -c write.cc $(LIBS)


primaryFrontEnd.o: 
	$(CXX) $(CXXFLAGS) -c ../server/primary/primaryFrontEnd.cc ../server/primary/primaryFrontEnd.h $(LIBS)
	
backupFrontEnd.o: common.o
	$(CXX) $(CXXFLAGS) -c ../server/backup/backupFrontEnd.cc ../server/backup/backupFrontEnd.h $(LIBS)

witnessFrontEnd.o: common.o
	$(CXX) $(CXXFLAGS) -c ../server/witness/witnessFrontEnd.cc ../server/witness/witnessFrontEnd.h $(LIBS)
	
common.o:
	$(CXX) $(CXXFLAGS) -c ../common/common.cc ../common/common.h $(LIBS)
	


clean:
	rm -f read_harp write_harp read.o write.o $(OBJS) $(TARGET) *.gch


