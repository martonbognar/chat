#ifndef client_h
#define client_h

#include <mutex>
#include <string>
#include <vector>
#include <thread>
#include "connector.hpp"

class Client {
private:
  Connector connector;
  std::vector<std::string> users;
  std::mutex cm;
  void parseMessages();
  bool parseCommand(std::string);
  void sendMessage(std::string);

public:
  bool connect();
  void login();
  std::thread messageParser();
  bool parseString(std::string);
  void listUsers();
};

#endif
