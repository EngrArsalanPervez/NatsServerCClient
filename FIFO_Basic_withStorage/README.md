# How to?

1. Install NatsServer
2. Run NATS Server in the backend with JetStream Configuration
3. Create a new Stream with policies
3. Compile/Run Publiser and Subscriber

```bash
sudo apt install libnats-dev

gcc -o js-pub js-pub.c -lnats
./js-pub

gcc -o js-sub js-sub.c -lnats
./js-sub -stream pace2 -subj dpi -print
```
