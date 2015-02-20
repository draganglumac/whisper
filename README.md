[![Build Status](https://travis-ci.org/AlexsJones/whisper.svg?branch=master)](https://travis-ci.org/AlexsJones/whisper)
whisper
=======

Status: In active development, not ready for general consumption.

Based on whisp this is a P2P domain level chat program that provides secure cryptography

Installation
=========

```
cmake . -DRUN_TESTS=ON #Or ignore the flag for no tests
make
sudo make install 
```

Additional Deps
=====

```
-lssl -lcrypto -lprotobuf-c
```

Regenerate Protobuf files
=========================

```
./tools/generate_protobuf
```

This currently uses a wildcard `*.proto` with an assumption for now that there is one proto file in a directory
Components
==========
```
session/
  Session Service [WIP]
  Session Object [WIP]
net/
  Discovery Service [WIP]
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
