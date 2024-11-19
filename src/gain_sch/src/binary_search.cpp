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

int binarySearchRange(const std::vector<float>& data, float lower_bound, float upper_bound)
{
    int left = 0;
    int right = data.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (data[mid] >= lower_bound && data[mid] <= upper_bound) {
            // Nilai dalam rentang ditemukan
            return mid;
        } else if (data[mid] < lower_bound) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -1; // Tidak ada nilai dalam rentang
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
        std::vector<float> sensorArray(msg->ranges.begin() + 340, msg->ranges.begin() + 359);

        // Menghapus nilai NaN atau infinit
        sensorArray.erase(std::remove_if(sensorArray.begin(), sensorArray.end(),
                                         [](float val){ return std::isnan(val) || std::isinf(val); }),
                          sensorArray.end());

        // Mengurutkan data
        std::sort(sensorArray.begin(), sensorArray.end());

        // Rentang nilai yang diinginkan
        float lower_bound = 0.3;
        float upper_bound = 0.7;

        // Menggunakan binary search untuk mencari nilai dalam rentang
        int index = binarySearchRange(sensorArray, lower_bound, upper_bound);

        if (index != -1) {
            RCLCPP_INFO(this->get_logger(), "Nilai dalam rentang %.2f - %.2f meter ditemukan dalam index %d .", lower_bound, upper_bound,index);
        } else {
            RCLCPP_INFO(this->get_logger(), "Nilai dalam rentang %.2f - %.2f meter tidak ditemukan.", lower_bound, upper_bound);
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