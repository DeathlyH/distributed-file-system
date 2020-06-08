# Simplified Harp File System

Implement the classic harp file system to better understand the paper.

There are 3 roles of servers:

* **primary** - Designated coordinator when it's alive.
* **backup** - Designated backup server. when primary server is unavailable, it will become the coordinator.
* **witness** - Only involve in view change. Backup logs if primary or backup server dead. Help the designated backup server or primary server to recover their operation logs.

The project has 1 primary server, 1 backup server and 1 witnes server.

## Prerequisites

The project used the rpclib for the rpc communication

Install [rpclib](github.com/rpclib/rpclib)

```
git clone https://github.com/rpclib/rpclib.git
cd rpclib
cmake -B build
cd build
make
make install
```
## Compile the project

In the top level folder, there's the Makefile.

To compile all the servers.

```
make
```

To remove the generated bin and .o files.

```
make clean
```

## Start the servers

1. Start the witness server.

```
cd server/witness
./witness_server
```

2. Start the primary server.

```
cd server/primary
./primary_server
```

3. Start the backup server.

```
cd server/backup
./backup_server
```

## Test

Under the client folder, there are two shell script.
1. **test_rd_wr_primary.sh**: This script is used to test read/write by communicating to primary server.
2. **test_rd_wr_backup.sh**: This script is used to test read/write by communicating to backup server.

This will request the primary server to do the read/write operation 5 times.

```
./test_rd_wr_primary.sh 5 

```

This will request the backup server to do the read/write operation 6 times.

```
./test_rd_wr_backup.sh 6 

```



To check the ports of the server

```
netstat -tulpn | grep LISTEN
```
