# R-Type - The Network

This documentation has for purpose to inform on the way the server and client work. Both will be detailled in separate paragraph.

# The Server

## UDP Protocole

The server is a UDP protocole, as our game favors fluidity over accuracy and consistency of information. Our server does not wait for an answer from the client to send more data packet.

## Heartbeat System

However, our server must still retrieve important informations and not miss it. Thus we implemented a heartbeat system. <br>
This system listens, all time, to every client. If after 5 seconds the client has not sent a proof of aliveness, the server consider that this perticular client is deconnected.

```c++
if (std::chrono::duration_cast<std::chrono::seconds>(now - it->second).count() > 5) {
    server.sendToAllClient("Disconnected: " + std::to_string(it->first), 4);
    disconnectedPlayerEntity = it->first;
    it = lastSeenClient.erase(it);
}
```

## Maximum User

Our server only accepts four players at the same time. Each time a client is considered disconnected, a new slots opens up to allow a new connection.

```c++
if (_connectedClients.size() >= NB_CLIENTS_AUTHORIZED) {
    std::string messageToSend = "Could not connect";
```

## Importance of the Server

Even though many actions are taken inside the client, the server remains the sole commandant. Indeed, the server handles most of the game aspect, from the creation of ennemy to the display of everything on the screen. Only the keys are handled in each separate client, however, what those keys do is decided by the server.

## Connection && Disconnection

The server is in charge of every client that its related to. The client does not have knowledge on other clients state or actions, it only knows what the server decide to send it. In the light of this, The server must inform every client of the connection and disconnection of a client.

```c++
_idClient++;
_connectedClients[_idClient] = endpoint;
sendToAllClient("New connection: " + std::to_string(_idClient), 1);
```

# The Client

The client does not handle much in our game, as the server is the decisionnary of every impactful decision.

## Heartbeat System

The client is in charge of proving its aliveness to the server. As such, it sends every seconds an "Alive" message with its ID.

