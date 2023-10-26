#include "rclcpp/rclcpp.hpp"
#include "tutorial_interfaces/srv/shutdown.hpp"
#include "tutorial_interfaces/msg/service.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>

using namespace std::chrono_literals;
std::vector<rclcpp::Client<tutorial_interfaces::srv::Shutdown>::FutureAndRequestId> futures;

class Client : public rclcpp::Node
{
public:
rclcpp::TimerBase::SharedPtr timer_;
size_t count_;
Client()
  : Node("client"), count_(0)
  {
    client =  this->create_client<tutorial_interfaces::srv::Shutdown>("service_a"); 
    client2 = this->create_client<tutorial_interfaces::srv::Shutdown>("service_b");   
    auto request = std::make_shared<tutorial_interfaces::srv::Shutdown::Request>();               // CHANGE
    request->req = 0;
     timer_ = this->create_wall_timer(
       1s, std::bind(&Client::timer_callback, this));
    
  }

 private:
 rclcpp::Client<tutorial_interfaces::srv::Shutdown>::SharedPtr  client;
 rclcpp::Client<tutorial_interfaces::srv::Shutdown>::SharedPtr  client2;
  void timer_callback()
  {
    auto request = std::make_shared<tutorial_interfaces::srv::Shutdown::Request>();               // CHANGE
    request->req = 0;// CHANGE
    auto result = client->async_send_request(request);
    auto result2 = client2->async_send_request(request);
    futures.push_back(std::move(result)); 
    futures.push_back(std::move(result2));
    timer_ = this->create_wall_timer(1s, std::bind(&Client::timer_callback, this)); 
     for (size_t i = 0; i < futures.size(); ++i) 
     {
        // Check if the future completed successfully
        if (futures[i].valid() && futures[i].wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            auto response = futures[i].get();
            // Handle the response
            RCLCPP_INFO(this->get_logger(), "Service response for  %s: %s", response->resp.service_name.c_str(), response->resp.resp ? "Failure":"Success"  );
        } else {
            RCLCPP_ERROR(this->get_logger(), "Service request %zu failed", i);
        }
    }
  }
};
int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<Client>();
  // Wait for all the futures to complete
    rclcpp::spin(node);  
  rclcpp::shutdown();
  return 0;
}