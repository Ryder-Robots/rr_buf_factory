#ifndef RR_STATE_MAINTAINER_HPP
#define RR_STATE_MAINTAINER_HPP

#include "rclcpp/logger.hpp"
#include "rr_interfaces/msg/buffer_response.hpp"
#include "rr_interfaces/msg/feature_set.hpp"
#include <list>
#include <mutex>
#include <shared_mutex>

namespace rrobot
{
    /**
     * @class RrStateMaintainer
     * @brief Shared object that maintains state variables accross different subscribers, and publisers.
     * 
     * when setter is activated, state remains until the setter is written to again.
     * 
     * Supported feature are:
     *   - GPS
     *   - Peripheral controllers (joysticks)
     *   - battery state (mangager) such BMS
     *   - camera streams
     *   - range detection devices, such as ultra sonic
     *   - IMU
     * 
     * State is used to send to normalization layer.
     * 
     * For image encoding refer to https://docs.ros.org/en/noetic/api/sensor_msgs/html/image__encodings_8h.html
     * Documenation on images can be found at https://docs.ros.org/en/noetic/api/sensor_msgs/html/msg/Image.html
     */
    class RrStateMaintainer
    {
      public:
        RrStateMaintainer(rclcpp::Logger logger) : logger_(logger) { init(); }
        ~RrStateMaintainer() = default;

        /**
         * @fn set_gps has_gps get_gps
         * @brief setters and getters relating to gps
         */
        void set_gps(const sensor_msgs::msg::NavSatFix gps);
        bool has_gps();
        const sensor_msgs::msg::NavSatFix get_gps();

        /**
         * @fn set_joystick has_joystick get_joystick
         * @brief setters and getters relating to joystick
         */
        void set_joystick(const sensor_msgs::msg::Joy joystick);
        bool has_joystick();
        const sensor_msgs::msg::Joy get_joystick();

        /**
         * @fn set_batt_state get_batt_state has_batt_state
         * @brief setters and getters relating to battery state
         */
        void set_batt_state(const sensor_msgs::msg::BatteryState);
        bool has_batt_state();
        const sensor_msgs::msg::BatteryState get_batt_state();

        /**
         * @fn set_image has_image get_image
         * @brief setters and getters relating to images (video stream)
         */
        void set_image(const sensor_msgs::msg::Image);
        bool has_image();
        const sensor_msgs::msg::Image get_image();

        /**
         * @fn set_imu has_imu get_imu
         * @brief setters and getters relating to imu (video stream)
         */
        void set_imu(const sensor_msgs::msg::Imu);
        bool has_imu();
        const sensor_msgs::msg::Imu get_imu();

        /**
         * @fn set_imu has_imu get_imu
         * @brief setters and getters relating to imu (video stream)
         */
        void set_ranges(const std::list<sensor_msgs::msg::Range>);
        bool has_ranges();
        const std::list<sensor_msgs::msg::Range> get_ranges();

        const rr_interfaces::msg::FeatureSet get_feature_set();


      private:
        void init();

        // variables
        sensor_msgs::msg::NavSatFix gps_;            // GPS
        sensor_msgs::msg::Joy joystick_;             // last command recieved by controller peripheral device
        sensor_msgs::msg::BatteryState batt_state_;  // current battery state
        sensor_msgs::msg::Image img_;                // current image
        sensor_msgs::msg::Imu imu_;                  // IMU
        std::list<sensor_msgs::msg::Range> ranges_;  // ranges given by ultra-sonic, or other range detecting device
        rr_interfaces::msg::FeatureSet feature_set_; // indicates that feature is present, or has been present

        // shared mutex to allow multiple readers or one writer
        std::shared_mutex mutex_;
        rclcpp::Logger logger_;
    };
} // namespace rrobot

#endif