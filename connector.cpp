#include <SFML/Network.hpp>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <exception>
#include "client.hpp"
#include "connector.hpp"

Connector::Connector(Client * client_) : client{client_} {}

void Connector::sendBytes(const char * data, size_t length) {
  if (socket.send(data, length) != sf::Socket::Done) {
    throw std::system_error{};
  }
}

void Connector::connect(const char * ip, unsigned int port) {
  if (isConnected) {
    disconnect();
  }

  sf::Socket::Status status = socket.connect(ip, port, sf::seconds(10));
  if (status != sf::Socket::Done) {
    throw std::system_error{};
  } else {
    sendHello();
    isConnected = true;
  }
}

void Connector::receiveMessage() {
  char data[256];
  size_t received;
  while (true) {
    if (socket.receive(data, 256, received) == sf::Socket::Done) {
      auto copy = std::make_unique<char[]>(256);
      strcpy(copy.get(), data);
      std::thread parser(&Connector::parseMessage, this, std::move(copy));
      parser.detach();
    }
  }
}

void Connector::parseMessage(std::unique_ptr<char[]> copy) {
  int i = 0;
  while (copy[i] != MESSAGE_TERMINATOR && i < 256) {
    ++i;
  }

  copy[i] = '\0';

  switch (copy[0]) {
    case MESSAGE_HELLO:
      client->printServerMessage(copy.get() + 1);
      break;
    case MESSAGE_SERVER:
      client->printServerMessage(copy.get() + 1);
      break;
    case MESSAGE_MESSAGE:
      client->printUserMessage(copy.get() + 1);
      break;
    case MESSAGE_PING:
      sendPong();
      client->printServerMessage("[ping-pong]");
      break;
    case MESSAGE_PONG:
      client->printServerMessage("[pong]");
      break;
    case MESSAGE_LOGIN:
      client->addUser(copy.get() + 1);
      break;
    case MESSAGE_LEAVE:
      client->removeUser(copy.get() + 1);
      break;
    default:
      client->printServerMessage("Unknown message received!");
  }
}

void Connector::sendHello() {
  char buf[2];
  buf[0] = MESSAGE_HELLO;
  buf[1] = MESSAGE_TERMINATOR;
  sendBytes(buf, 2);
}

void Connector::sendNeptun(const char * neptun) {
  char buf[8];
  buf[0] = MESSAGE_NEPTUN;
  for (int i = 1; i <= 6; ++i) {
    buf[i] = neptun[i - 1];
  }
  buf[7] = MESSAGE_TERMINATOR;
  sendBytes(buf, 8);
}

void Connector::sendPassword(const char * password) {
  char buf[8];
  buf[0] = MESSAGE_PASSWORD;
  for (int i = 1; i <= 6; ++i) {
    buf[i] = password[i - 1];
  }
  buf[7] = MESSAGE_TERMINATOR;
  sendBytes(buf, 8);
}

void Connector::sendMessage(std::string message) {
  int length = message.size();
  if (length > 200) {
    sendMessage(message.substr(0, 200));
    sendMessage(message.substr(200));
  } else {
    auto buf = std::make_unique<char[]>(length + 2);
    buf[0] = MESSAGE_MESSAGE;
    int i;
    for (i = 1; i <= length; ++i) {
      buf[i] = message[i - 1];
    }
    buf[i] = MESSAGE_TERMINATOR;
    sendBytes(buf.get(), length + 2);
  }
}

void Connector::sendPong() {
  char buf[2];
  buf[0] = MESSAGE_PONG;
  buf[1] = MESSAGE_TERMINATOR;
  sendBytes(buf, 2);
}

void Connector::sendPing() {
  char buf[2];
  buf[0] = MESSAGE_PING;
  buf[1] = MESSAGE_TERMINATOR;
  sendBytes(buf, 2);
}

void Connector::disconnect() {
  char buf[2];
  buf[0] = MESSAGE_LOGOUT;
  buf[1] = MESSAGE_TERMINATOR;
  sendBytes(buf, 2);
  socket.disconnect();
  isConnected = false;
}

Connector::~Connector() {
  if (isConnected) {
    disconnect();
  }
}
