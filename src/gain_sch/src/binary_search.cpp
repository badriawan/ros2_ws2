#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include <vector>
#include <algorithm>

using namespace std;
using std::placeholders::_1;

int binarySearch(const vector<float>& sensorArray,int size,double target){
    int left = 0;
    int right = size - 1;
    int mid;

    while(left<=right){
        mid = left + (right-left)/2;
    
    if(target==sensorArray[mid]){
        return mid;
    }
    else if(target<sensorArray[mid]){
        right = mid - 1;
    }
    else{
        left = mid + 1;
    }

    }

    return -1;
    }


class LaserScanNode : public rclcpp::Node
{
    public:
    LaserScanNode():Node("laser_scan_node"){

        _subscription = this->create_subscription<sensor_msgs::msg::LaserScan>(
            "/scan", 10, std::bind(&LaserScanNode::scan_callback, this, _1));

        RCLCPP_INFO(this->get_logger(), "Waiting for laser scan message");


    }
    private:
    void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
    {
        vector<float> sensorArray(msg->ranges.begin() + 340, msg->ranges.begin() + 359);
        sort(sensorArray.begin(), sensorArray.end());
        int index = binarySearch(sensorArray,sensorArray.size(),target);

        if (index != -1) {
            RCLCPP_INFO(this->get_logger(), "Target: %f ditemukan pada indeks %d", target, index);
        } else {
            RCLCPP_INFO(this->get_logger(), "Target: %f tidak ditemukan", target);
        }    
    
    }
    

    
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr _subscription; 
    double target = 0.5;



};


int main( int argc, char* argv[] ){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LaserScanNode>());
    rclcpp::shutdown();
    return 0;
}