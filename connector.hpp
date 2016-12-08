#ifndef connector_h
#define connector_h

#include <SFML/Network.hpp>
#include <memory>
#include "client.hpp"

class Connector {
private:
  Client * client;
  sf::TcpSocket socket;
  bool isConnected = false;
  void sendBytes(const char * input, size_t length);

  enum MessageTypes {
    MESSAGE_HELLO = 1,
    MESSAGE_NEPTUN = 2,
    MESSAGE_PASSWORD = 3,
    MESSAGE_SERVER = 4,
    MESSAGE_MESSAGE = 5,
    MESSAGE_PING = 6,
    MESSAGE_PONG = 7,
    MESSAGE_LOGOUT = 8,
    MESSAGE_LOGIN = 9,
    MESSAGE_LEAVE = 10,
    MESSAGE_TERMINATOR = 127,
  };

public:
  Connector(Client * client);
  void connect(const char * ip, unsigned int port);
  void receiveMessage();
  void parseMessage(std::unique_ptr<char[]> copy);
  void sendHello();
  void sendNeptun(const char * neptun);
  void sendPassword(const char * password);
  void sendMessage(std::string message);
  void sendPing();
  void sendPong();
  void disconnect();
  ~Connector();
};

#endif
