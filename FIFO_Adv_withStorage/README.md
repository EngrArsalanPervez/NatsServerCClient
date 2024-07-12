# How to?

1. Install NatsServer
2. Run NATS Server in the backend without any configuration
3. Compile/Run Publiser and Subscriber

```bash
sudo apt install libnats-dev

gcc -o js-pub js-pub.c -lnats
./js-pub -h
./js-pub -stream pace2 -subj dpi -count 100

gcc -o js-sub js-sub.c -lnats
./js-sub -h
./js-sub -stream pace2 -subj dpi -print
```
