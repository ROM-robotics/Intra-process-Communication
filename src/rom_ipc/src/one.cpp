#include <chrono>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

using namespace std::chrono_literals;

// ဒေတာ ထုတ်ပေးမည့် Node (ဥပမာ - LiDAR Sensor Node)
class Producer : public rclcpp::Node {
public:
  Producer(const rclcpp::NodeOptions & options)
  : Node("producer", options) {
    // IPC သုံးဖို့ဆိုရင် unique_ptr သုံးပြီး publish လုပ်ရပါမယ်
    pub_ = create_publisher<std_msgs::msg::string>("chatter", 10);
    
    timer_ = create_wall_timer(1s, [this]() {
      auto msg = std::make_unique<std_msgs::msg::String>(); // Unique pointer နဲ့ ဒေတာတည်ဆောက်ခြင်း
      msg->data = "Large Data from Sensor";
      
      RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", msg->data.c_str());
      
      // Pointer ကိုပဲ တစ်ခါတည်း လွှဲပေးလိုက်ခြင်း (Zero-copy)
      pub_->publish(std::move(msg));
    });
  }

private:
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
};

// ဒေတာ လက်ခံမည့် Node (ဥပမာ - Navigation Stack Node)
class Consumer : public rclcpp::Node {
public:
  Consumer(const rclcpp::NodeOptions & options)
  : Node("consumer", options) {
    // Unique pointer နဲ့ လက်ခံခြင်းဖြင့် ဒေတာကို ပွားစရာမလိုဘဲ တိုက်ရိုက်ယူသုံးခြင်း
    sub_ = create_subscription<std_msgs::msg::String>(
      "chatter", 10,
      [this](std_msgs::msg::String::UniquePtr msg) {
        RCLCPP_INFO(this->get_logger(), "Received via IPC: '%s'", msg->data.c_str());
      });
  }

private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_;
};

int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);

  // IPC ကို အသုံးပြုရန် Node Options ထဲတွင် true ပေးခြင်း
  auto options = rclcpp::NodeOptions().use_intra_process_comms(true);

  // Node နှစ်ခုလုံးကို Single Process (Executor) တစ်ခုတည်းအောက်မှာ Run ရပါမယ်
  rclcpp::executors::SingleThreadedExecutor executor;

  auto producer_node = std::make_shared<Producer>(options);
  auto consumer_node = std::make_shared<Consumer>(options);

  executor.add_node(producer_node);
  executor.add_node(consumer_node);

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Starting Intra-process communication example...");
  executor.spin();

  rclcpp::shutdown();
  return 0;
}