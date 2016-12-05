#include <SFML/Network.hpp>
#include <iostream>
#include <memory>
#include <cstring>
#include "connector.hpp"

void Connector::sendBytes(const char * data, size_t length) {
  if (socket.send(data, length) != sf::Socket::Done) {
    std::cout << "Error while sending!" << std::endl;
  }
}

bool Connector::connect(const char * ip, unsigned int port) {
  sf::Socket::Status status = socket.connect(ip, port, sf::seconds(10));
  if (status != sf::Socket::Done) {
    std::cout << "Error while connecting!" << std::endl;
    return true;
  } else {
    sendHello();
    return false;
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
  while (copy[i] != 127 && i < 256) {
    ++i;
  }

  copy[i] = '\0';

  switch (input[0]) {
    case 1: std::cout << "Hello: " << copy + 1 << std::endl; break;
    case 2: std::cout << "Neptun: " << copy + 1 << std::endl; break;
    case 3: std::cout << "Jelszo: " << copy + 1 << std::endl; break;
    case 4: std::cout << "Szerver uzenet: " << copy + 1 << std::endl; break;
    case 5: std::cout << "Uzenet: " << copy + 1 << std::endl; break;
    case 6: std::cout << "Ping: " << copy + 1 << std::endl; break;
    case 7: std::cout << "Pong: " << copy + 1 << std::endl; break;
    case 8: std::cout << "Kijelentkezes: " << copy + 1 << std::endl; break;
    case 9: std::cout << "Belepes: " << copy + 1 << std::endl; break;
    case 10: std::cout << "Kilepes: " << copy + 1 << std::endl; break;
    default: std::cout << "Could not decipher" << std::endl;
  }
}

void Connector::sendHello() {
  char buf[2];
  buf[0] = 1;
  buf[1] = 127;
  sendBytes(buf, 2);
}

void Connector::sendNeptun(const char * neptun) {
  char buf[8];
  buf[0] = 2;
  for (int i = 1; i <= 6; ++i) {
    buf[i] = neptun[i - 1];
  }
  buf[7] = 127;
  sendBytes(buf, 8);
}

void Connector::sendPassword(const char * password) {
  char buf[8];
  buf[0] = 3;
  for (int i = 1; i <= 6; ++i) {
    buf[i] = password[i - 1];
  }
  buf[7] = 127;
  sendBytes(buf, 8);
}

void Connector::sendMessage(const char * message) {
  int length = strlen(message);
  auto buf = std::make_unique<char[]>(length + 2);
  buf[0] = 5;
  int i;
  for (i = 1; i <= length; ++i) {
    buf[i] = message[i - 1];
  }
  buf[i] = 127;
  sendBytes(buf.get(), length + 2);
}

void Connector::sendPong() {
  char buf[2];
  buf[0] = 7;
  buf[1] = 127;
  sendBytes(buf, 2);
}

void Connector::logout() {
  char buf[2];
  buf[0] = 8;
  buf[1] = 127;
  sendBytes(buf, 2);
}
