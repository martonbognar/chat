#include <SFML/Network.hpp>
#include <iostream>
#include <memory>
#include <cstring>
#include "client.hpp"
#include "connector.hpp"

Connector::Connector(Client * client_) : client{client_} {}

void Connector::sendBytes(const char * data, size_t length) {
  if (socket.send(data, length) != sf::Socket::Done) {
    std::cout << "Error while sending!" << std::endl;
  }
}

bool Connector::connect(const char * ip, unsigned int port) {
  sf::Socket::Status status = socket.connect(ip, port, sf::seconds(10));
  if (status != sf::Socket::Done) {
    std::cout << "Error while connecting!" << std::endl;
    return false;
  } else {
    sendHello();
    return true;
  }
}

void Connector::receiveMessage() {
  char data[256];
  size_t received;
  while (true) {
    if (socket.receive(data, 256, received) != sf::Socket::Done) {
    } else {
      parseMessage(data);
    }
  }
}

void Connector::parseMessage(const char * input) {
  char copy[256];
  strcpy(copy, input);
  int i = 0;
  while (copy[i] != MESSAGE_TERMINATOR && i < 256) {
    ++i;
  }

  copy[i] = '\0';

  switch (input[0]) {
    case MESSAGE_HELLO:
      std::cout << "Hello: " << copy + 1 << std::endl;
      break;
    case MESSAGE_SERVER:
      std::cout << "Szerver uzenet: " << copy + 1 << std::endl;
      break;
    case MESSAGE_MESSAGE:
      std::cout << copy + 1 << std::endl;
      break;
    case MESSAGE_PING:
      sendPong();
      std::cout << "[ping-pong]" << std::endl;
      break;
    case MESSAGE_PONG:
      std::cout << "Pong!" << std::endl;
      break;
    case MESSAGE_LOGIN:
      client->addUser(copy + 1);
      std::cout << "Belepes: " << copy + 1 << std::endl;
      break;
    case MESSAGE_LEAVE:
      client->removeUser(copy + 1);
      std::cout << "Kilepes: " << copy + 1 << std::endl;
      break;
    default:
      std::cout << "Could not decipher" << std::endl;
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

void Connector::sendMessage(const char * message) {
  int length = strlen(message);
  if (length >= 220) {
    std::cout << "Message too long, did not send." << std::endl;
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

void Connector::logout() {
  char buf[2];
  buf[0] = MESSAGE_LOGOUT;
  buf[1] = MESSAGE_TERMINATOR;
  sendBytes(buf, 2);
}

Connector::~Connector() {
  // TODO: kapcsolat megszakitasa
}
