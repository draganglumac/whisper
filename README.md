
> Imagine being able to work in an environment where your conversations arent monitored continuously.
  Take that idea and apply it to your everyday workplace where having an outspoken opinion remains locked away due to fear of leaving a digital footprint.
  Whisper aims to help provide a platform for inter-domain level chat.
  Using dynamically generated dual encrypted one time sessions; we're giving you the tools to speak your mind without   worrying about who is listening in.
  
  
  Example of session initiator:
  [Sender](http://showterm.io/84a6e4faaf26cacf26b70)
  
  Example of receiver of session:
  [Receiver](http://showterm.io/d5b18c422e701d2b8a441)

---
##For Developers

[![Join the chat at https://gitter.im/AlexsJones/whisper](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/AlexsJones/whisper?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

[![Build Status](https://travis-ci.org/AlexsJones/whisper.svg?branch=master)](https://travis-ci.org/AlexsJones/whisper)

Status: In active development, not ready for general consumption.

Whisper uses a combination of asymmetric 2048bit key RSA with a DES cipher for establishing sessions.
It has multi-protocol finding ability which includes multicasting upto top tier router domain and drop n' replace cyptography solutions.

The aim is to finish the kernel in C and bring it to other platforms with a variety of GUI and language hooks.

### Table of Contents
  * [Installation](#installation)
  * [Additional Deps](#Deps)
  * [Protobuf](#protobuf)
  * [Components](#components)



#### Installation <a id="installation"></a>


```
cmake . -DRUN_TESTS=ON #Or ignore the flag for no tests
make
sudo make install 
```

#### Additional Deps <a id="Deps"></a>

```
-lssl -lcrypto -lprotobuf-c
```

####  Regenerate Protobuf files <a id="protobuf"></a>

```
./tools/generate_protobuf
```

This currently uses a wildcard `*.proto` with an assumption for now that there is one proto file in a directory

#### Components <a id="components"></a>

| Component | Status |
| ---------:| -----------:|
|Session Service | DONE |
|Session Object  | DONE |
|Discovery Service| DONE|
|Multicast Service| DONE|
|Secure Sockets|DONE|
|Key encrypt/decrpt|DONE|
|Integrity Service|DONE|
|Commandline IO|DONE|
|Basic GUI|DONE|


