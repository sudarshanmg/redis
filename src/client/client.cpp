#include <iostream>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;

int main() {
	try {
		boost::asio::io_context io_context;
		tcp::socket socket(io_context);

		tcp::resolver resolver(io_context);
		boost::asio::connect(socket, resolver.resolve("127.0.0.1", "6379"));
		cout << "Connected to server" << endl;
		
		string msg = "Hello form client";
		boost::asio::write(socket, boost::asio::buffer(msg));

		char buffer[128];

		size_t length = socket.read_some(boost::asio::buffer(buffer));
		cout << "Recvd from server: " << string(buffer, length) << endl;


	}
	catch(exception& e) {
		cerr << "Exception: " << e.what() << endl;
	}
}
