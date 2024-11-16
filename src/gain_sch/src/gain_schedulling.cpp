// gain_scheduling_distance_control.cpp

#include <chrono>
#include <memory>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

using namespace std::chrono_literals;

class GainSchedulingDistanceControl : public rclcpp::Node
{
public:
    GainSchedulingDistanceControl()
    : Node("gain_scheduling_distance_control")
    {
        cmd_vel_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
        laser_subscriber_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
            "scan", 10, std::bind(&GainSchedulingDistanceControl::laser_callback, this, std::placeholders::_1));

        desired_distance_ = 1.0; // Jarak yang diinginkan (meter)
        kp_min_ = 0.1; // Gain minimum
        kp_max_ = 1.0; // Gain maksimum
    }

private:
    void laser_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
    {
        // Mendapatkan jarak terdekat di depan robot
        float distance = std::numeric_limits<float>::infinity();
        // float distance = 0.0;
        //int index_center = msg->ranges.size() / 2;
        int index_center = 0;


        if (!std::isnan(msg->ranges[index_center]) && !std::isinf(msg->ranges[index_center])) {
            distance = msg->ranges[index_center];
        }

        distance = *std::min_element(msg->ranges.begin() + 340, msg->ranges.begin() + 360);

        // Menghitung error jarak
        float error = distance - desired_distance_;

        // Menghitung gain berdasarkan error (gain scheduling non-linear)
        float kp = compute_gain(error);

        auto cmd_msg = geometry_msgs::msg::Twist();
        // Kontrol proporsional dengan gain yang disesuaikan
        cmd_msg.linear.x = kp * error;

        // Batasi kecepatan maksimum
        if (cmd_msg.linear.x > max_speed_) {
            cmd_msg.linear.x = max_speed_;
        } else if (cmd_msg.linear.x < -max_speed_) {
            cmd_msg.linear.x = -max_speed_;
        }

        cmd_vel_publisher_->publish(cmd_msg);
    }

    float compute_gain(float error)
    {
        // Fungsi gain scheduling non-linear
        // Gain meningkat seiring dengan besar error
        float gain = kp_min_ + (kp_max_ - kp_min_) * (std::fabs(error) / max_error_);
        RCLCPP_INFO(this->get_logger(), "Error: %.2f, Gain: %.2f", error, gain);

        // Batasi gain ke nilai maksimum
        if (gain > kp_max_) {
            gain = kp_max_;
        } else if (gain < kp_min_) {
            gain = kp_min_;
        }

        return gain;
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_publisher_;
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_subscriber_;

    float desired_distance_;
    float kp_min_;
    float kp_max_;
    const float max_speed_ = 0.2; // Kecepatan maksimum (m/s)
    const float max_error_ = 1.0; // Error maksimum yang diharapkan
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GainSchedulingDistanceControl>());
    rclcpp::shutdown();
    return 0;
}