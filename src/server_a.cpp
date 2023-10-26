#include "rclcpp/rclcpp.hpp"
#include "tutorial_interfaces/srv/shutdown.hpp"
#include "tutorial_interfaces/msg/service.hpp"


#include <memory>

void shutdown(const std::shared_ptr<tutorial_interfaces::srv::Shutdown::Request> request,     // CHANGE
          std::shared_ptr<tutorial_interfaces::srv::Shutdown::Response> response)  // CHANGE
{
   RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Incoming request\na: %ld",   // CHANGE
                request->req);
  std::this_thread::sleep_for(std::chrono::seconds(4));
   auto service_response = tutorial_interfaces::msg::Service();
  if(request->req==0)
  {   
   response->resp.resp=0;   
   response->resp.service_name="service_a";   
   rclcpp::shutdown();                                                            
   RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Shutdown of node b success!!: [%ld]", (long int)response->resp.resp);
   rclcpp::shutdown();    
  }
  else{ 
   response->resp.resp=1;   
   response->resp.service_name="service_a";         
   RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Shutdown of node b failure!!: [%ld]", (long int)response->resp.resp);
   
  }
 
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("server_a");  // CHANGE

  rclcpp::Service<tutorial_interfaces::srv::Shutdown>::SharedPtr service =                 // CHANGE
    node->create_service<tutorial_interfaces::srv::Shutdown>("service_a",  &shutdown);     // CHANGE

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to shutdown node a....");      // CHANGE

  rclcpp::spin(node);
  rclcpp::shutdown();
}