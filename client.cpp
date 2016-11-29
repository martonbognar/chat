#include "client.hpp"

bool Client::parseString(std::string input) {
  if (input[0] == '/') {
    parseCommand(input);
  } else {
    sendMessage(input);
  }
}
