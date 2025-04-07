#include <jsoncpp/json/json.h>

#include <cassert>
#include <chrono>
#include <future>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "zmq.hpp"
#include "zmq_addon.hpp"

void subThreadA(zmq::context_t *ctx);
void subThreadB(zmq::context_t *ctx);
void subThreadC(zmq::context_t *ctx);
void subThreadD(zmq::context_t *ctx);
void subThreadE(zmq::context_t *ctx);

int main() {
   zmq::context_t ctx(1);
   auto threadA = std::async(std::launch::async, subThreadA, &ctx);
   auto threadB = std::async(std::launch::async, subThreadB, &ctx);
   auto threadC = std::async(std::launch::async, subThreadC, &ctx);
   auto threadD = std::async(std::launch::async, subThreadD, &ctx);
   auto threadE = std::async(std::launch::async, subThreadE, &ctx);

   threadA.wait();
   threadB.wait();
   threadC.wait();
   threadD.wait();
   threadE.wait();
}

void subThreadA(zmq::context_t *ctx) {
   zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
   std::cout << "connecting subscriber A" << std::endl;
   subscriber.set(zmq::sockopt::subscribe, "Topic1");
   subscriber.connect("tcp://localhost:5555");

   JSONCPP_STRING err;
   Json::Value root;
   Json::CharReaderBuilder builder;
   const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

   // Receive all parts of messages
   std::vector<zmq::message_t> recv_msgs;

   while (true) {
      zmq::recv_result_t result =
            zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
      assert(result && "recv failed");
      assert(*result == 2);

      // recv_msgs[0] should have been the topic
      std::string recStr(recv_msgs[1].to_string());
      std::istringstream iss(recStr);

      if (!Json::parseFromStream(builder, iss, &root, &err)) {
         std::cout << err << std::endl;
         break;
      }

      std::cout << "Subscriber A has read: \n" << recStr << std::endl;
      std::cout << root["key1"].toStyledString() << std::endl;
      recv_msgs.clear();
   }
}

void subThreadB(zmq::context_t *ctx) {
   zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
   std::cout << "connecting subscriber B" << std::endl;
   subscriber.set(zmq::sockopt::subscribe, "Topic2");
   subscriber.connect("tcp://localhost:5555");

   JSONCPP_STRING err;
   Json::Value root;
   Json::CharReaderBuilder builder;
   const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

   // Receive all parts of messages
   std::vector<zmq::message_t> recv_msgs;

   while (true) {
      zmq::recv_result_t result =
            zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
      assert(result && "recv failed");
      assert(*result == 2);

      // recv_msgs[0] should have been the topic
      std::string recStr(recv_msgs[1].to_string());
      std::istringstream iss(recStr);

      if (!Json::parseFromStream(builder, iss, &root, &err)) {
         std::cout << err << std::endl;
         break;
      }

      std::cout << "Subscriber B has read: \n" << recStr << std::endl;
      std::cout << root["key2"].toStyledString() << std::endl;
      recv_msgs.clear();
   }
}

void subThreadC(zmq::context_t *ctx) {
   zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
   std::cout << "connecting subscriber C" << std::endl;
   subscriber.set(zmq::sockopt::subscribe, "Topic3");
   subscriber.connect("tcp://localhost:5555");

   JSONCPP_STRING err;
   Json::Value root;
   Json::CharReaderBuilder builder;
   const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

   // Receive all parts of messages
   std::vector<zmq::message_t> recv_msgs;

   while (true) {
      zmq::recv_result_t result =
            zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
      assert(result && "recv failed");
      assert(*result == 2);

      // recv_msgs[0] should have been the topic
      std::string recStr(recv_msgs[1].to_string());
      std::istringstream iss(recStr);

      if (!Json::parseFromStream(builder, iss, &root, &err)) {
         std::cout << err << std::endl;
         break;
      }

      std::cout << "Subscriber C has read: \n" << recStr << std::endl;
      std::cout << root["key3"].toStyledString() << std::endl;
      recv_msgs.clear();
   }
}

void subThreadD(zmq::context_t *ctx) {
   zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
   std::cout << "connecting subscriber D" << std::endl;
   subscriber.set(zmq::sockopt::subscribe, "Topic4");
   subscriber.connect("tcp://localhost:5555");

   JSONCPP_STRING err;
   Json::Value root;
   Json::CharReaderBuilder builder;
   const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

   // Receive all parts of messages
   std::vector<zmq::message_t> recv_msgs;

   while (true) {
      zmq::recv_result_t result =
            zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
      assert(result && "recv failed");
      assert(*result == 2);

      // recv_msgs[0] should have been the topic
      std::string recStr(recv_msgs[1].to_string());
      std::istringstream iss(recStr);

      if (!Json::parseFromStream(builder, iss, &root, &err)) {
         std::cout << err << std::endl;
         break;
      }

      std::cout << "Subscriber D has read: \n" << recStr << std::endl;
      std::cout << root["key4"].toStyledString() << std::endl;
      recv_msgs.clear();
   }
}

void subThreadE(zmq::context_t *ctx) {
   zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
   std::cout << "connecting subscriber E" << std::endl;
   subscriber.set(zmq::sockopt::subscribe, "Topic5");
   subscriber.connect("tcp://localhost:5555");

   JSONCPP_STRING err;
   Json::Value root;
   Json::CharReaderBuilder builder;
   const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

   // Receive all parts of messages
   std::vector<zmq::message_t> recv_msgs;

   while (true) {
      zmq::recv_result_t result =
            zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
      assert(result && "recv failed");
      assert(*result == 2);

      // recv_msgs[0] should have been the topic
      std::string recStr(recv_msgs[1].to_string());
      std::istringstream iss(recStr);

      if (!Json::parseFromStream(builder, iss, &root, &err)) {
         std::cout << err << std::endl;
         break;
      }

      std::cout << "Subscriber E has read: \n" << recStr << std::endl;
      std::cout << root["key5"].toStyledString() << std::endl;
      recv_msgs.clear();
   }
}
