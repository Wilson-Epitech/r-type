
---

# Binary Protocol

This document describes our implementaiton of binary protocol adapted to our needs

## Introduction

In our R-Type project, we use a binary protocol to manage communications between client and server. This ensures fast data exchange, which is essential for real-time gaming. By structuring messages compactly, we reduce latency and optimize bandwidth utilization. Furthermore, this protocol imposes a strict structure, preventing the sending of non-compliant messages and limiting the risk of intrusion. In short, the binary protocol is a cornerstone of our network architecture, guaranteeing both performance and security.

## Message structure

The message structure used to communicate between server and client is designed to be both compact and complete for our needs, guaranteeing efficient and secure communication between client and server. Each message consists of the following elements:
<br>
<br>
> Magic Number | Message Type | Message ID | Message Size | Message
<br>

> [!NOTE]
> Magic Number
- The magic number is a unique identifier in **hexadecimal** to ensure that the message received belongs to our protocol. This differentiates valid messages from messages from external sources or potentially corrupted messages.

```cpp

const uint16_t _magicNumber = 0x9B3D; // 2 bytes

```
> [!IMPORTANT]
> 2 bytes (16 bits) -> Can store values from 0 to 65 535.
<br>


> [!NOTE]
> Message Type
- The message type is represented by a unique identifier that indicates the nature of the message. This can include categories such as errors, information transmissions, or other specific types of communication. This enables the various project parties to treat each message appropriately according to its type.
```cpp

uint8_t _messageType // 1 byte

```
> [!IMPORTANT]
> 1 byte (8 bits) -> Can store values from 0 to 255, to define up to 256 different message types.
<br>


> [!NOTE]
> Message ID
- The message identifier is a unique number assigned to each message to ensure its precise identification. This allows duplicate messages to be detected and avoided, ensuring that each message is processed only once.
```cpp

uint8_t _messageId // 1 byte

```
> [!IMPORTANT]
> 1 byte (8 bits) -> Can store values from 0 à 255, sufficient to define up to 256 unique messages in a short interval
<br>


> [!NOTE]
> Message Size
- Message size is a value indicating the total length of the message in bytes. It is essential for checking that the message has been received completely and without corruption.
```cpp

uint16_t _messageSize // 2 bytes

```
> [!IMPORTANT]
> 2 bytes (16 bits) -> Can represent message sizes of up to 65 535 bytes, which is useful for managing large messages.
<br>


> [!NOTE]
> Message
- Message content
```cpp

std::string message = "New connection";

message.data();

```
<br>
<br>

This well-defined structure ensures that each message is processed in an optimal and consistent way, maximizing security.

<br>

## Message types

| Type          | Id                            | Description                        |
|---------------|-------------------------------|------------------------------------|
| Connection    | $\textsf{\color{#5491f1}{1}}$ | New client connexion               |
| Disconnection | $\textsf{\color{#5491f1}{2}}$ | Client Disconnection               |
| Communication | $\textsf{\color{#5491f1}{3}}$ | Send of data paquet                |
| Repeat        | $\textsf{\color{#5491f1}{4}}$ | Send of reapeat paquet             |
| Error         | $\textsf{\color{#5491f1}{5}}$ | Send of Error                      |
<br>

<!--
Truc qu'on peut rajouter
| Call          | 2   | Un appel de fonction               |
| Reply         | 3   | Une réponse à un appel             |
| Event         | 3   | Un événement déclenché par un service |
-->

## Error Management

> [!NOTE]
> Magic number check
- When a message is received in one of the project areas, the first step is to check the magic number. This ensures that the message conforms to our protocol:
```cpp
    uint16_t _receivedMagicNbr;

    std::memcpy(&_receivedMagicNbr, _receiveBuffer.data(), sizeof(_receivedMagicNbr));

    if (_receivedMagicNbr != _magicNumber) {
        std::cerr << "Invalid magic number" << std::endl;
        return;
    }
```
<br>

> [!NOTE]
> Message size check
- Next, we check whether the message has been fully received. This ensures that all expected data is present:
```cpp
    std::size_t byteTransfered // variable passed as parameter to function

    uint8_t _messageType;
    uint8_t _messageId;
    uint16_t _messageLen;

    std::memcpy(&_messageType, _receiveBuffer.data() + sizeof(_receivedMagicNbr), sizeof(_messageType));
    std::memcpy(&_messageId, _receiveBuffer.data() + sizeof(_receivedMagicNbr) + sizeof(_messageType), sizeof(_messageId));
    std::memcpy(&_messageLen, _receiveBuffer.data() + sizeof(_receivedMagicNbr) + sizeof(_messageType) + sizeof(_messageId), sizeof(_messageLen));

    if (byteTransfered != sizeof(_receivedMagicNbr) + sizeof(_messageType) + sizeof(_messageId) + sizeof(_messageLen) + _messageLen) {
        std::cerr << "Incomplete message received" << std::endl;
        return;
    }
```
<br>

## Security

> [!IMPORTANT]
> Security point:
- This protocol is secured by the **magic number**. This unique number verifies the origin of the message. If a message does not respect our protocol, it will be automatically rejected, ensuring that only legitimate communications are accepted.


- $\textsf{\textbf{\color{#8957E5}{Starting the server :}}}$
```bash
    ╭─ ~/root
    ╰─ ./server 127.0.0.1 4242
```

- $\textsf{\textbf{\color{#8957E5}{Attempt to connect a client not conforming to the protocol :}}}$
```bash
    ╭─ ~/root
    ╰─ nc -u 127.0.0.1 4242
```

- $\textsf{\textbf{\color{#8957E5}{Server response:}}}$
```bash
    ╭─ ~/root
    |   ./server 127.0.0.1 4242
    ╰─  Invalid magic number
```


## Performance and Optimization

Performance optimization has been crucial in ensuring a smooth and responsive gaming experience. Our binary protocol has been designed to be lightweight and efficient, which is essential when handling large numbers of packets in a real-time gaming environment. Compared with a conventional binary protocol, our implementation stands out for its compactness and efficiency.

### Efficiency

Compared with standard communication protocols, our message structure is optimized to minimize the size of the data transmitted. For example, by using compact data types such as:
```cpp
uint8_t
uint16_t
```
we save space compared with a conventional protocol that might use
```cpp
uint32_t
```
which doubles the size needed to store the same type of information

> [!IMPORTANT]
> Reminder
> - Here are the differences in storage capacity between the different **uint_t** types: <br><br>
> ```uint8_t (8 bits)``` : Can store values from 0 to 255 bytes <br><br>
> ```uint16_t (16 bits)``` : Can store values from 0 to 65 535 bytes <br><br>
> ```uint32_t (32 bits)``` : Can store values from 0 to 4 294 967 295 bytes (typically used)<br>

### Advantages
> [!NOTE]
> Reduced message size
- Our message header uses just 6 bytes, which is significantly smaller than the 12-20 byte headers often found in conventional protocols. This reduces communication overhead and improves transmission speed.
<br>

> [!NOTE]
> Increased speed
- By reducing message size, latency is reduced, enabling faster, more responsive data exchanges - crucial for real-time gaming.
<br>

> [!NOTE]
> Bandwidth optimization
- By minimizing message size, we optimize bandwidth utilization, enabling us to handle a greater number of simultaneous connections without compromising performance.
<br>
