whisper
=======

Status: In active development, not ready for general consumption.

Based on whisp this is a P2P domain level chat program that provides secure cryptography

Installation
=========

```
cmake .
make
sudo make install 
```

Additional Deps
=====

```
-lssl -lcrypto
```


Components
==========
```
session/
  Session Service
  Session Object
net/
  Discovery Service
  Multicast Service
  Secure/Non-Secure Socket interface

crypto/
  ~~Symmetrical key encrypt/decrypt~~ [DONE]
  ~~Asymmetrical key encrypt/decrypt~~ [DONE]
  ~~Integrity Service~~ [DONE]
usr/
  User command line I/O
```

Tests for each module are included
