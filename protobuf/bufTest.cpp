#include <cassert>
#include <chrono>
#include <cstdint>
#include <future>
#include <iostream>
#include <iterator>
#include <thread>
#include <vector>

#include "schema.pb.h"
#include "zmq.hpp"
#include "zmq_addon.hpp"

void populateTstMsg(test::TestMsg *msg, uint32_t syncWord) {
   msg->set_syncword(syncWord);
   for (int i(0); i < 100; ++i) {
      msg->add_payload(i);
   }
}

void PublisherThread(zmq::context_t *ctx) {
   // Prepare publisher
   zmq::socket_t publisher(*ctx, zmq::socket_type::pub);
   publisher.bind("inproc://#1");

   test::TestMsg pbMsg;
   populateTstMsg(&pbMsg, 0x5500AAFF);

   // Give the subs a chance to connect, so they don't lose any messages
   std::this_thread::sleep_for(std::chrono::milliseconds(20));
   std::string msg;

   while (true) {
      // Write three messages, each with an envelope and context
      publisher.send(zmq::str_buffer("A"), zmq::send_flags::sndmore);
      publisher.send(zmq::str_buffer("Message in A envelope"));
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
   }
}  // End publisher thread function

void SubscriberThread1(zmq::context_t *ctx) {
   // Prepare subscriber
   zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
   subscriber.connect("inproc://#1");

   // Thread opens A and B envelopes
   subscriber.set(zmq::sockopt::subscribe, "A");
   subscriber.set(zmq::sockopt::subscribe, "B");

   while (true) {
      // Recieve all parts of the messages
      std::vector<zmq::message_t> recv_msgs;
      zmq::recv_result_t result =
            zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
      assert(result && "recv failed");
      assert(*result == 2);

      std::cout << "SubThread1: [" << recv_msgs[0].to_string() << "] "
                << recv_msgs[1].to_string() << std::endl;
   }
}

int main() {
   // No I/O threads are involved in passing messages using the
   // inproc transport. Therefore, if you are using a zmq context for
   // in-process messaging only you can initialize the context with
   // zero I/O threads.
   zmq::context_t ctx(0);
   auto pubThread = std::async(std::launch::async, PublisherThread, &ctx);

   // Give the publisher a chance to bind, since inproc requires it
   std::this_thread::sleep_for(std::chrono::milliseconds(10));

   auto subThread = std::async(std::launch::async, SubscriberThread1, &ctx);

   pubThread.wait();
   subThread.wait();
}
