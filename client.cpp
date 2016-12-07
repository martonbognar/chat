#include <iostream>
#include <mutex>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include "client.hpp"

Client::Client() : connector{std::make_unique<Connector>(this)} {
  printWelcome();
}

void Client::printWelcome() {
  printUserMessage(Messages::WELCOME);
}

void Client::connect(const char * ip, unsigned int port) {
  try {
    connector->connect(ip, port);
    this->isConnected = true;
    std::thread parser = messageParser();
    parser.detach();
    users.push_back("(You)");
  } catch (std::system_error & e) {
    printClientMessage(Messages::CONNECTION_FAILED);
  }
}

std::thread Client::messageParser() {
  return std::thread{[=] { connector->receiveMessage(); }};
}

void Client::printServerMessage(std::string input) {
  std::lock_guard<std::mutex> guard{cm};
  std::cout << "[server] " << input << std::endl;
}

void Client::printClientMessage(std::string input) {
  std::lock_guard<std::mutex> guard{cm};
  std::cout << "[client] " << input << std::endl;
}

void Client::printUserMessage(std::string input) {
  std::lock_guard<std::mutex> guard{cm};
  std::cout << input << std::endl;
}

void Client::sendMessage(std::string input) {
  if (!isConnected) {
    printClientMessage(Messages::CONNECTION_REQUIRED);
    return;
  }

  try {
    connector->sendMessage(input.c_str());
  } catch (std::system_error & e) {
    printClientMessage(Messages::SENDING_FAILED);
  }
}

void Client::sendNeptun(std::string input) {
  if (!isConnected) {
    printClientMessage(Messages::CONNECTION_REQUIRED);
    return;
  }

  try {
    connector->sendNeptun(input.c_str());
  } catch (std::system_error & e) {
    printClientMessage(Messages::SENDING_FAILED);
  }
}

void Client::sendPassword(std::string input) {
  if (!isConnected) {
    printClientMessage(Messages::CONNECTION_REQUIRED);
    return;
  }

  try {
    connector->sendPassword(input.c_str());
  } catch (std::system_error & e) {
    printClientMessage(Messages::SENDING_FAILED);
  }
}

void Client::parseString(std::string input) {
  if (input[0] == '/') {
    parseCommand(input.substr(1));
  } else {
    sendMessage(input);
  }
}

bool Client::checkStringStart(std::string haystack, std::string needle) {
  return haystack.size() >= needle.size() && haystack.substr(0, needle.size()).compare(needle) == 0;
}

void Client::parseCommand(std::string input) {
  if (checkStringStart(input, "quit")) {
    connector->disconnect();
    this->isConnected = false;
    this->quit = true;
    return;
  }

  if (checkStringStart(input, "disconnect")) {
    connector->disconnect();
    this->isConnected = false;
    return;
  }

  if (checkStringStart(input, "ping")) {
    if (!isConnected) {
      printClientMessage(Messages::CONNECTION_REQUIRED);
      return;
    }

    connector->sendPing();
    return;
  }

  if (checkStringStart(input, "users")) {
    if (!isConnected) {
      printClientMessage(Messages::CONNECTION_REQUIRED);
      return;
    }

    listUsers();
    return;
  }

  if (checkStringStart(input, "connect")) {
    char ip[256];
    unsigned int port;
    sscanf(input.c_str(), "%*s %[^:]:%u", ip, &port);
    connect(ip, port);
    return;
  }

  if (checkStringStart(input, "neptun")) {
    char neptun[7];
    sscanf(input.c_str(), "%*s %s", neptun);
    sendNeptun(neptun);
    return;
  }

  if (checkStringStart(input, "password")) {
    char password[7];
    sscanf(input.c_str(), "%*s %s", password);
    sendPassword(password);
    return;
  }

  if (checkStringStart(input, "help")) {
    printHelp();
    return;
  }

  printClientMessage(Messages::INVALID_COMMAND);
}

void Client::listUsers() {
  if (!isConnected) {
    printClientMessage(Messages::CONNECTION_REQUIRED);
    return;
  }

  printClientMessage("Currently online users:");
  for (auto & user : users) {
    printClientMessage(user);
  }
}

void Client::printHelp() {
  printClientMessage("Messages starting with a / are treated as commands, others as messages.");
  printClientMessage("Available commands:");
  printClientMessage("/connect address:port");
  printClientMessage("/disconnect");
  printClientMessage("/help");
  printClientMessage("/neptun NEPTUN");
  printClientMessage("/password NUTPEN");
  printClientMessage("/ping");
  printClientMessage("/quit");
  printClientMessage("/users");
}

void Client::addUser(const char * name) {
  users.push_back(name);
}

void Client::removeUser(const char * name) {
  for (size_t i = 0; i < users.size(); ++i) {
    if (users[i].compare(name) == 0) {
      users.erase(users.begin() + i);
      return;
    }
  }
}
