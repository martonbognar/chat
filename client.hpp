#ifndef client_h
#define client_h

#include <mutex>
#include <string>
#include <vector>
#include <thread>

class Client;

#include "connector.hpp"

class Client {
private:
  std::unique_ptr<Connector> connector = nullptr;
  std::vector<std::string> users;
  std::mutex cm;
  bool isConnected = false;
  bool quit = false;

  void connect(const char * ip, unsigned int port);
  void parseCommand(std::string);
  void sendMessage(std::string);
  void sendNeptun(std::string);
  void sendPassword(std::string);
  void listUsers();
  void printServerMessage(std::string);
  void printClientMessage(std::string);
  void printUserMessage(std::string);
  void printHelp();
  void printWelcome();
  bool checkStringStart(std::string haystack, std::string needle);

public:
  Client();
  std::thread messageParser();
  void parseString(std::string);
  void addUser(const char * name);
  void removeUser(const char * name);
  bool isQuit() { return this->quit; }
};

#endif
