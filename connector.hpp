#ifndef connector_h
#define connector_h

#include <SFML/Network.hpp>

class Connector {
private:
  sf::TcpSocket socket;
  void sendBytes(const char * input, size_t length);

public:
  bool connect(const char * ip, unsigned int port);
  void receiveMessage();
  void parseMessage(const char * input);
  void sendHello();
  void sendNeptun(const char * neptun);
  void sendPassword(const char * password);
  void sendMessage(const char * message);
  void sendPong();
  void logout();
};

#endif
