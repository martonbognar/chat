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
  void printHelp();
  bool checkStringStart(std::string haystack, std::string needle);
  void printWelcome();

  struct Messages {
    static constexpr auto CONNECTION_FAILED = "Could not connect to the server!";
    static constexpr auto CONNECTION_REQUIRED = "You need to connect to a server before using this command.";
    static constexpr auto INVALID_COMMAND = "Invalid command! Type /help for help.";
    static constexpr auto SENDING_FAILED = "Could not send the message!";
    static constexpr auto WELCOME = "Welcome to the client! Type /help for help.";
  };

public:
  Client();
  std::thread messageParser();
  void parseString(std::string);
  void printServerMessage(std::string);
  void printClientMessage(std::string);
  void printUserMessage(std::string);
  void addUser(const char * name);
  void removeUser(const char * name);
  bool isQuit() { return this->quit; }
};

#endif
