all:	
	(cd server; make all)
	(cd client; make all)
	(cd test; make all)
	

clean:
	(cd server; make clean)
	(cd client; make clean)
	(cd test; make clean)
	(cd common; rm -rf *.gch)


