#include <cassert>
#include <chrono>
#include <future>
#include <iostream>
#include <iterator>
#include <thread>
#include <vector>

#include "zmq.hpp"
#include "zmq_addon.hpp"

void PublisherThread(zmq::context_t *ctx) {
   // Prepare publisher
   zmq::socket_t publisher(*ctx, zmq::socket_type::pub);
   publisher.bind("inproc://#1");

   // Give the subs a chance to connect, so they don't lose any messages
   std::this_thread::sleep_for(std::chrono::milliseconds(20));

   while (true) {
      // Write three messages, each with an envelope and context
      publisher.send(zmq::str_buffer("A"), zmq::send_flags::sndmore);
      publisher.send(zmq::str_buffer("Message in A envelope"));
      publisher.send(zmq::str_buffer("B"), zmq::send_flags::sndmore);
      publisher.send(zmq::str_buffer("Message in B envelope"));
      publisher.send(zmq::str_buffer("C"), zmq::send_flags::sndmore);
      publisher.send(zmq::str_buffer("Message in C envelope"));
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

void SubscriberThread2(zmq::context_t *ctx) {
   // Prepare subscriber
   zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
   subscriber.connect("inproc://#1");

   // Thread opens ALL envelopes
   subscriber.set(zmq::sockopt::subscribe, "");

   while (true) {
      // Recieve all parts of the message
      std::vector<zmq::message_t> recv_msgs;
      zmq::recv_result_t result =
            zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
      assert(result && "recv failed");
      assert(*result == 2);

      std::cout << "SubThread2: [" << recv_msgs[0].to_string() << "] "
                << recv_msgs[1].to_string() << std::endl;
   }
}  // end sub thread 2

int main() {
   // No I/O threads are involved in passing messages using the
   // inproc transport. Therefore, if you are using a zmq context for
   // in-process messaging only you can initialize the context with
   // zero I/O threads.
   zmq::context_t ctx(0);
   auto pubThread = std::async(std::launch::async, PublisherThread, &ctx);

   // Give the publisher a chance to bind, since inproc requires it
   std::this_thread::sleep_for(std::chrono::milliseconds(10));

   auto subThread1 = std::async(std::launch::async, SubscriberThread1, &ctx);
   auto subThread2 = std::async(std::launch::async, SubscriberThread2, &ctx);

   pubThread.wait();
   subThread1.wait();
   subThread2.wait();
}
