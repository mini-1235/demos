// Copyright 2015 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include <cinttypes>
#include <cstdio>
#include <memory>
#include <string>
#include "rosgraph_msgs/msg/clock.hpp"
#include <utility>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

using namespace std::chrono_literals;

// Node that produces messages.
struct Producer : public rclcpp::Node
{
  Producer(const std::string & name, const std::string & output)
  : Node(name, rclcpp::NodeOptions().use_intra_process_comms(false))
  {
    // Create a publisher on the output topic.
    pub_ = this->create_publisher<std_msgs::msg::Int32>(output, 10);
    // Create a timer which publishes on the output topic at ~1Hz.
    auto callback = [this]() -> void {
        static int32_t count = 0;
        auto msg = std::make_unique<std_msgs::msg::Int32>();
        // std_msgs::msg::Int32::SharedPtr msg = std::make_shared<std_msgs::msg::Int32>();
        msg->data = count++;
        printf(
          "Published message with value: %d, and address: 0x%" PRIXPTR "\n", msg->data,
          reinterpret_cast<std::uintptr_t>(msg.get()));
        pub_->publish(std::move(msg));
        // pub_->publish(*msg);
      };
    timer_ = this->create_wall_timer(1s, callback);
  }

  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
};

// Node that consumes messages.
struct Consumer : public rclcpp::Node
{
  Consumer(const std::string & name, const std::string & input)
  : Node(name, rclcpp::NodeOptions().use_intra_process_comms(false))
  {
    sub_ = this->create_subscription<std_msgs::msg::Int32>(
      input,
      10,
      [](const std_msgs::msg::Int32::SharedPtr msg) {
        printf(
          " [SUB1]Received message with value: %d, and address: 0x%" PRIXPTR "\n", msg->data,
          reinterpret_cast<std::uintptr_t>(msg.get()));
      });
    sub2_ = this->create_subscription<std_msgs::msg::Int32>(
      input,
      10,
      [](const std_msgs::msg::Int32::SharedPtr msg) {
        printf(
          " [SUB2]Received message with value: %d, and address: 0x%" PRIXPTR "\n", msg->data,
          reinterpret_cast<std::uintptr_t>(msg.get()));
      });
    sub3_ = this->create_subscription<std_msgs::msg::Int32>(
      input,
      10,
      [](const std_msgs::msg::Int32::SharedPtr msg) {
        printf(
          " [SUB3]Received message with value: %d, and address: 0x%" PRIXPTR "\n", msg->data,
          reinterpret_cast<std::uintptr_t>(msg.get()));
      });
  }

  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub_;
  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub2_;
  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub3_;
  rclcpp::Subscription<rosgraph_msgs::msg::Clock>::SharedPtr clock_subscription_;
};


// Node that consumes messages.
struct Consumer2 : public rclcpp::Node
{
  Consumer2(const std::string & name, const std::string & input)
  : Node(name, rclcpp::NodeOptions().use_intra_process_comms(false))
  {
    sub_ = this->create_subscription<std_msgs::msg::Int32>(
      input,
      10,
      [](const std_msgs::msg::Int32::SharedPtr msg) {
        printf(
          " [SUB4]Received message with value: %d, and address: 0x%" PRIXPTR "\n", msg->data,
          reinterpret_cast<std::uintptr_t>(msg.get()));
      });
    sub2_ = this->create_subscription<std_msgs::msg::Int32>(
      input,
      10,
      [](const std_msgs::msg::Int32::SharedPtr msg) {
        printf(
          " [SUB5]Received message with value: %d, and address: 0x%" PRIXPTR "\n", msg->data,
          reinterpret_cast<std::uintptr_t>(msg.get()));
      });
    sub3_ = this->create_subscription<std_msgs::msg::Int32>(
      input,
      10,
      [](const std_msgs::msg::Int32::SharedPtr msg) {
        printf(
          " [SUB6]Received message with value: %d, and address: 0x%" PRIXPTR "\n", msg->data,
          reinterpret_cast<std::uintptr_t>(msg.get()));
      });
  }

  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub_;
  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub2_;
  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub3_;
  rclcpp::Subscription<rosgraph_msgs::msg::Clock>::SharedPtr clock_subscription_;
};


int main(int argc, char * argv[])
{
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);
  rclcpp::init(argc, argv);
  rclcpp::executors::SingleThreadedExecutor executor;

  auto producer = std::make_shared<Producer>("producer", "number");
  auto consumer = std::make_shared<Consumer>("consumer", "number");
  auto consumer2 = std::make_shared<Consumer2>("consumer2", "number");

  executor.add_node(producer);
  executor.add_node(consumer);
  executor.add_node(consumer2);
  executor.spin();

  rclcpp::shutdown();

  return 0;
}
