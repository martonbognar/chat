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
  void parseCommand(std::string);
  void sendMessage(std::string);
  void listUsers();

public:
  Client();
  void connect();
  void login();
  std::thread messageParser();
  void parseString(std::string);
  void addUser(const char * name);
  void removeUser(const char * name);
  bool isQuit() { return this->quit; }
};

#endif
