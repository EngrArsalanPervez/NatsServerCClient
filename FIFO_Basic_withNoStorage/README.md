# How to?

1. Install NatsServer
2. Run NATS Server in the backend without any configuration
3. Compile/Run Publiser and Subscriber

```bash
sudo apt install libnats-dev

gcc -o nats_sender nats_sender.c -lnats
./nats_sender

gcc -o nats_receiver nats_receiver.c -lnats
./nats_receiver
```
