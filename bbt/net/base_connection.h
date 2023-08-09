#ifndef BBT_NET_BASE_CONNECTION_H_
#define BBT_NET_BASE_CONNECTION_H_

#include <memory>
#include <functional>
#include "buffer.h"

namespace bbt {
namespace net {

class BaseConnection;
typedef std::shared_ptr<BaseConnection> BaseConnectionPtr;

typedef std::function<void(const BaseConnectionPtr&)> ConnCallback;
typedef std::function<void(const BaseConnectionPtr&, Buffer*)> ReadCallback;

class BaseContext {
 public:
  virtual ~BaseContext() {}
};

class BaseConnection : public std::enable_shared_from_this<BaseConnection> {
 public:
  enum State { kDisconnected, kConnected };

  BaseConnection() : state_(kDisconnected), context_(nullptr) {}

  State state() const { return state_; }
  BaseContext* context() const { return context_; }
  void set_context(BaseContext* c) { context_ = c; }

  void set_conn_callback(const ConnCallback& cb) { conn_callback_ = cb; }
  void set_read_callback(const ReadCallback& cb) { read_callback_ = cb; }

  virtual void Start() {
    state_ = kConnected;
    conn_callback_(shared_from_this());
  }

  virtual void Stop() {
    state_ = kDisconnected;
    conn_callback_(shared_from_this());
  }

  virtual void Send(const void* data, size_t len) = 0;

 protected:
  virtual ~BaseConnection() { delete context_; }

  State state_;
  BaseContext* context_;
  ConnCallback conn_callback_;
  ReadCallback read_callback_;
};

}  // namespace net
}  // namespace bbt
#endif  // BBT_NET_BASE_CONNECTION_H_