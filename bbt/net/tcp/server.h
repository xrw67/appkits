#ifndef BBT_NET_TCP_SERVER_H_
#define BBT_NET_TCP_SERVER_H_

#include <string>

#include "bbt/net/callbacks.h"

namespace bbt {
namespace net {

class TcpConnectionManager;

class TcpServer {
 public:
  explicit TcpServer(asio::io_context& io);
  ~TcpServer();

  Status Listen(const std::string& address, const std::string& port);
  void Stop();

  void Boardcast(const void* data, size_t len) noexcept;

  void set_connection_callback(const ConnectionCallback& cb) {
    connection_callback_ = cb;
  }
  void set_receive_callback(const ReceiveCallback& cb) {
    receive_callback_ = cb;
  }

 private:
  /// Perform an asynchronous accept operation.
  void DoAccept();

  /// The io_context used to perform asynchronous operations.
  asio::io_context& io_context_;

  /// Acceptor used to listen for incoming connections.
  asio::ip::tcp::acceptor acceptor_;

  TcpConnectionManager* connection_manager_;

  ConnectionCallback connection_callback_;
  ReceiveCallback receive_callback_;
};

}  // namespace net
}  // namespace bbt
#endif  // BBT_NET_TCP_SERVER_H_