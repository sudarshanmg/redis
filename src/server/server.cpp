#include <iostream>
#include <boost/asio.hpp>
#include <memory> // to use enable_shared_from_this

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
	public:
		// transfer the ownership of the socket to the Session object
		explicit Session(tcp::socket socket) : socket_(std::move(socket)) {};
		void start() {
			read();
		}
	
	private:
		void read() {
			auto self(shared_from_this());
			socket_.async_read_some(boost::asio::buffer(buffer_),
					[this, self](const boost::system::error_code& ec, std::size_t bytes) {
						if(!ec) {
							std::cout << "Received: " << std::string(buffer_, bytes) << std::endl;
							write("Hello from server");
						}
					});
		}

		void write(const std::string& msg) {
			auto self(shared_from_this());
			boost::asio::async_write(socket_, boost::asio::buffer(msg),
					[this, self](const boost::system::error_code& ec, std::size_t bytes) {
						if(!ec) {
						std::cout << "Message sent to the client" << std::endl;
						}
					});
		}

		tcp::socket socket_;
		char buffer_[128];
};

class Server {
	public:
		Server(boost::asio::io_context& io_context, short port) 
			: acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
				accept();
			};
	
	private:
		void accept() {
			acceptor_.async_accept(
					[this](const boost::system::error_code& ec, tcp::socket socket) {
						if(!ec) {
							std::cout << "Client connected" << std::endl;
							std::make_shared<Session>(std::move(socket))->start();
						}
						accept();
					}
			);
		}
		
		tcp::acceptor acceptor_;
};

int main() {
	try {
		boost::asio::io_context io_context;
		Server server(io_context, 6379);
		std::cout << "Server running on port 6379...\n";
		io_context.run();
	} catch(std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	return 0;
}

