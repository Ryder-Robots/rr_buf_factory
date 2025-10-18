# RR BUF FACTORY

Provides plugin interface and state maintainer. Used for plugins to that provide
and interface to the buffer service for subscribers.

## Example Of Code

```c++
#include <rr_buf_factory/rr_buf_factory.hpp>
#include <rr_buf_factory/rr_state_maintainer.hpp>
#include <rr_subscribers/img_subscriber.hpp>
#include <rr_subscribers/imu_subscriber.hpp>

namespace rrobot {
    class RrUltraSonicBot : public RrBufFactory {
        public:
            void initialize(rclcpp::Node::SharedPtr ctl, RrStateMaintainer state_manager) override 
            {
                ctl_ = ctl;
                RCLCPP_INFO(ctl_->get_logger(), "creating callback group(s)");
                auto cg = cnt_->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
                rclcpp::SubscriptionOptions options;

                RCLCPP_INFO(ctl_->get_logger(), "getting image subscriber params");
                ctl_->declare_parameter(image_subscriber_.getTopicParam(), image_subscriber_.getTopicDefault());
                ctl_->declare_parameter(image_subscriber_.getQueueSzParam(), image_subscriber_.getQueueSzDefault());
                image_subscriber_.set_logger(cnt_->get_logger());

                RCLCPP_INFO(ctl_->get_logger(), "getting imu subscriber params");
                ctl_->declare_parameter(imu_subscriber.getTopicParam(), imu_subscriber.getTopicDefault());
                ctl_->declare_parameter(imu_subscriber.getQueueSzParam(), imu_subscriber.getQueueSzDefault());                
                imu_subscriber.set_logger(cnt_->get_logger());

                auto img_callback =  std::bind(&RrImageSubscriber::callback, image_subscriber_, std::placeholders::_1);
                auto img_topic_str = cnt_->get_parameter(image_subscriber_.getTopicParam()).as_string();
                img_subscription_ = cnt_->create_subscription<sensor_msgs::msg::Image>(img_topic_str, rclcpp::SensorDataQoS(), img_callback, options);

                auto imu_callback =  std::bind(&RrImuSubscriber::callback, imu_subscriber_, std::placeholders::_1);
                auto imu_topic_str = cnt_->get_parameter(image_subscriber_.getTopicParam()).as_string();
                imu_subscription_ = cnt_->create_subscription<sensor_msgs::msg::Imu>(imu_topic_str, rclcpp::SensorDataQoS(), imu_callback, options);
            }

            void tear_down() @override 
            {
                
            }
        protected:
            rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr img_subscription_;
            rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_subscription_;

            RrImageSubscriber image_subscriber_;
            RrImuSubscriber imu_subscriber_; 
    }
}
```
