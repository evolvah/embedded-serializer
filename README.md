# embedded-serializer
Light-weight serialized for embedded applications

# Sample run
```
$ g++ -o 1 test.cpp && ./1
Serializing  : x1: 1, r1: 'blah-blah!' x2: 2, x3: 3, x4: 0.123467
ok: true, capacity: 30 , position: 26
Deserializing: x1: 1, r1: 'blah-blah!' x2: 2, x3: 3, x4: 0.123467
Testing overflow
ok: false
Serializing LogRecord...
LogRecord: this: 0x7fff5ce559b8 schedulerClocksFromStart: 1, level: 1, source: 2, code: 3, textToStore: 'Test string!'
Deserializing LogRecord...
LogRecord: this: 0x7fff5ce55970 schedulerClocksFromStart: 1, level: 1, source: 2, code: 3, textToStore: 'Test string!'
```
