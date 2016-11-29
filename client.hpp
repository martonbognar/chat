#ifndef client_h
#define client_h

#include "connector/connector.hpp"

class Client {
private:
  Connector* connector;

public:
  bool parseString(std::string);
};

#endif
