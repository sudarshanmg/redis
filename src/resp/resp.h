#ifndef RESP_H
#define RESP_H

#include <string>
#include <vector>

namespace resp {

// Parsing functions
std::string parseSimpleString(const std::string& message);
std::string parseError(const std::string& message);
std::string parseBulkString(const std::string& message);
std::vector<std::string> parseArray(const std::string& message);

// Encoding functions
std::string encodeSimpleString(const std::string& value);
std::string encodeError(const std::string& error);
std::string encodeBulkString(const std::string& data);
std::string encodeArray(const std::vector<std::string>& elements);

} // namespace resp

#endif // RESP_H

