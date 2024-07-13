# NATS Server C Client

## Introduction

NATS Server is like a PIPE/FIFO between two physical systems or applications. One application sends data to NATS server while the other application receives data from NATS server

### Basic NATS Server as a PIPE with no Data-Retention or Data-Deletion Policy

Go to FIFO_Basic_withNoStorage > README.md

### Basic NATS Server with Data-Retention and Data-Deletion Policy

Go to FIFO_Basic_withStorage > README.md

### Advanced NATS Server with Data-Retention and Data-Deletion Policy

Go to FIFO_Adv_withStorage > README.md

## Pre-requisites

1. Ubuntu
2. NATS Server
3. NATS JetStream
4. nats-server.conf
5. nats-cli
6. Define a new Stream
7. NATS C Client Library

## Installation

### NATS Server

```bash
sudo -s
cd /home/<USER>
curl -L https://github.com/nats-io/nats-server/releases/download/v2.10.17/nats-server-v2.10.17-linux-amd64.zip -o nats-server.zip
unzip nats-server.zip -d nats-server
sudo cp nats-server/nats-server-v2.10.17-linux-amd64/nats-server /usr/bin

# Running nats server without conf file
/usr/bin/nats-server

# Running nats server with conf file
/usr/bin/nats-server -c /home/<USER>/NATS/nats-server.conf

# Web Monitor
http://0.0.0.0:8222/

# Client Listen Port
0.0.0.0:4222
```

### NATS JetStream

By default NAT server act as a FIFO with no data-retention and data-deletion policy. One application application publish data while the other application subscribe to data. If there is no subscriber, NATS server will delete data.
To store data in NATS server until data is delivered to subscriber or until system memory/storage is full, we can use JETSTREAM
JetStream is highly customize able and we can define data-retention and data-delition policies using nats-cli


### nats-server.conf

```bash
mkdir /home/<USER>/NATS/
sudo chmod 777 /home/<USER>/NATS/
cd /home/<USER>/NATS/
nano nats-server.conf
```

```bash
# Server identification
server_name: my-nats-server

# Enable JetStream
jetstream: enabled

# JetStream storage configuration
jetstream: {
  store_dir: /home/<USER>/NATS
}

# General server configuration
listen: 4222
http: 8222
debug: true
trace: true
logtime: true
pid_file: "/home/<USER>/NATS/nats-server.pid"
```

### nats-cli

```bash
sudo -s
cd /usr/bin/
curl -sf https://binaries.nats.dev/nats-io/natscli/nats@latest | sh
/usr/bin/nats
```

#### nats-cli Utility
```bash
/usr/bin/nats -h
/usr/bin/nats stream list
/usr/bin/nats stream add
/usr/bin/nats stream delete

Monitor Stream
watch /usr/bin/nats stream list
```

## How to Use?

### Define a new Stream

Define a new Stream with data-retention and data-deletion policies
```bash
/usr/bin/nats stream add

# Stream Name: pace2
# Subjects: dpi
# Storage: file
# Replication (1): <ENTER>
# Retention Policy: Work Queue
# Discard Policy: Old
# Stream Msg Limit (-1): <ENTER> 
# Per Subject Msg Limit (-1): <ENTER>
# Total Stream Size: 10737418240 # 10GB
# Message TTL (-1): <ENTER>
# Max Message Size (-1): <ENTER>
# Duplicate tracking time window: <ENTER>
# Allow message Roll-ups: <ENTER>
# Allow message deletion (Y/n): <ENTER>
# Allow purging subjects or the entire stream (Y/n): <ENTER>

# Check Stream
/usr/bin/nats stream list

# Monitor Stream
watch /usr/bin/nats stream list

# Delete a Stream
/usr/bin/nats stream delete
```

### NATS C Client 

```bash
sudo apt install libnats-dev
sudo apt install libprotobuf-c-dev
```

#### Publisher
```bash
gcc -o js-pub js-pub.c -lnats
./js-pub -h
./js-pub -stream pace2 -subj dpi -count 100
```

#### Subscriber
```bash
gcc -o js-sub js-sub.c -lnats
./js-sub -h
./js-sub -stream pace2 -subj dpi -print
```

#### (Optional) NATS C Library

Library is very big with a lot of examples. We can compile the library, run the example and use the examples in our own projects
```bash
git clone https://github.com/nats-io/nats.c.git
cd nats.c
mkdir build
cd build
cmake ..
make
sudo make install
ldconfig
cd bin/
./js-pub -h
./js-sub -h
```

