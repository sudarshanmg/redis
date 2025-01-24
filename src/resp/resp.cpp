#include "resp.h"
#include <sstream>
#include <stdexcept>

using namespace std;

namespace resp {

	// Helper func to split a string by delimiter
	vector<string> split(const string& str, char delimiter) {
		vector<string> tokens;
		string token;
		istringstream tokenStream(str);
		while(getline(tokenStream, token, delimiter)) {
			tokens.push_back(token);
		}
		return tokens;
	}

	// parses a simple string (+<string>\r\n)
	string parseSimpleString(const string& message) {
		if(message.size() < 3 || message[0] != '+') {
			throw invalid_argument("Invalid Simple String Format");
		}
		return message.substr(1, message.size() - 3); // remove + and \r\n
	}

	// parses an error (-<error>\r\n)
	string parseError(const string& message) {
		if(message.size() < 3 || message[0] != '-') {
			throw invalid_argument("Invalid Error Format");
		}
		return message.substr(1, message.size() - 3); // remove - and \r\n
	}

	// parses a bulk string ($<length>\r\n<data>\r\n)
	string parseBulkString(const string& message) {
		if(message.size() < 5 || message[0] != '$') {
			throw invalid_argument("Invalid Bulk String Format");
		}

		size_t crlf_pos = message.find("\r\n", 1);
		if(crlf_pos == string::npos) {
			throw invalid_argument("Malformed Bulk String");
		}

		int length = stoi(message.substr(1, crlf_pos - 1));
		if(length == -1) {
			return ""; // null bulk string
		}

		size_t data_start = crlf_pos + 2;
		size_t data_end = data_start + length;
		if(data_end + 2 > message.size() || message.substr(data_end, 2) != "\r\n") {
			throw invalid_argument("Incomple Bulk String");
		}

		return message.substr(data_start, length);
	}

	// parses an array (*<count>\r\n<elements>)
	vector<std::string> parseArray(const std::string& message) {
		if (message.size() < 3 || message[0] != '*') {
			throw std::invalid_argument("Invalid Array format");
		}

		size_t crlf_pos = message.find("\r\n");
		if (crlf_pos == std::string::npos) {
			throw std::invalid_argument("Malformed Array");
		}

		int count = std::stoi(message.substr(1, crlf_pos - 1));
		if (count < 0) {
			return {}; // Null array
		}

		std::vector<std::string> elements;
		size_t current_pos = crlf_pos + 2;
		for (int i = 0; i < count; ++i) {
			if (current_pos >= message.size()) {
				throw std::invalid_argument("Incomplete Array");
			}

			char type = message[current_pos];
			size_t next_crlf = message.find("\r\n", current_pos);

			if (type == '+') {
				elements.push_back(parseSimpleString(message.substr(current_pos, next_crlf - current_pos + 2)));
			} else if (type == '-') {
				elements.push_back(parseError(message.substr(current_pos, next_crlf - current_pos + 2)));
			} else if (type == '$') {
				size_t bulk_end = message.find("\r\n", next_crlf + 2);
				elements.push_back(parseBulkString(message.substr(current_pos, bulk_end - current_pos + 2)));
			}
			current_pos = next_crlf + 2;
		}

		return elements;
	}

	// Encodes a simple string (+<string>\r\n)
	std::string encodeSimpleString(const std::string& value) {
		return "+" + value + "\r\n";
	}

	// Encodes an error (-<error>\r\n)
	std::string encodeError(const std::string& error) {
		return "-" + error + "\r\n";
	}

	// Encodes a bulk string ($<length>\r\n<data>\r\n)
	std::string encodeBulkString(const std::string& data) {
		if (data.empty()) {
			return "$-1\r\n"; // Null bulk string
		}
		return "$" + std::to_string(data.size()) + "\r\n" + data + "\r\n";
	}

	// Encodes an array (*<count>\r\n<elements>)
	std::string encodeArray(const std::vector<std::string>& elements) {
		if (elements.empty()) {
			return "*-1\r\n"; // Null array
		}

		std::string result = "*" + std::to_string(elements.size()) + "\r\n";
		for (const auto& element : elements) {
			result += encodeBulkString(element);
		}
		return result;

	}
 
}	// namespace resp
