#include "gmock/gmock.h"

#include "bbt/bus/msg.h"
#include "bbt/bus/msg_packer.h"

namespace {

using bbt::bus::Msg;

static const char* json_data =
    "{\"id\":123,\"method\":\"foo\",\"param\":[{\"t\":\"key1\",\"v\":\"str1\"},"
    "{\"t\":\"key2\",\"v\":\"1001\"}]}";

TEST(MsgPacker, should_pack_by_json) {
  Msg msg;
  msg.set_id(123);
  msg.set_method("foo");
  msg.Set("key1", "str1");
  msg.Set("key2", "1001");

  bbt::bus::JsonPacker j;
  std::string s;

  j.Pack(msg, &s);
  ASSERT_EQ(s, json_data);
}

TEST(MsgPacker, should_unpack_by_json) {
  Msg msg;
  bbt::bus::JsonPacker j;

  auto st = j.Unpack(json_data, &msg);
  ASSERT_TRUE(st) << st.ToString();
  ASSERT_EQ(msg.id(), 123);
  ASSERT_EQ(msg.method(), "foo");
  ASSERT_EQ(msg.Get("key1"), "str1");
  ASSERT_EQ(msg.Get("key2"), "1001");
}

TEST(MsgPacker, should_catch_exception_with_unpack) {
  Msg msg;
  bbt::bus::JsonPacker j;

  ASSERT_FALSE(j.Unpack("{\"id\":\"abc\"}", &msg));
  ASSERT_FALSE(j.Unpack("{\"id\":123, \"method\": 123}", &msg));
  ASSERT_FALSE(j.Unpack(
      "{\"id\":123,\"method\":\"foo\",\"param\":[{\"t\":\"key1\"}]}", &msg));
  ASSERT_FALSE(j.Unpack(
      "{\"id\":123,\"method\":\"foo\",\"param\":[{\"v\":\"str1\"}]}", &msg));
  ASSERT_FALSE(j.Unpack(
      "{\"id\":123,\"method\":\"foo\",\"param\":[{\"nokey\":\"str1\"}]}",
      &msg));
}

}  // namespace