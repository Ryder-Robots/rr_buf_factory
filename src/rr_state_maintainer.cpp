#include "rr_buf_factory/rr_state_maintainer.hpp"

using namespace rrobot;

// gps setter/getter
void RrStateMaintainer::set_gps(const sensor_msgs::msg::NavSatFix gps)
{
    std::unique_lock<std::shared_mutex> lock(mutex_);
    feature_set_.has_gps = true;
    gps_ = gps;
}

bool RrStateMaintainer::has_gps()
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return feature_set_.has_gps;
}

const sensor_msgs::msg::NavSatFix RrStateMaintainer::get_gps()
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return gps_;
}

void RrStateMaintainer::set_joystick(const sensor_msgs::msg::Joy joystick)
{
    std::unique_lock<std::shared_mutex> lock(mutex_);
    feature_set_.has_joy = true;
    joystick_ = joystick;
}

bool RrStateMaintainer::has_joystick()
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return feature_set_.has_joy;
}

const sensor_msgs::msg::Joy RrStateMaintainer::get_joystick()
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return joystick_;
}

void RrStateMaintainer::set_batt_state(const sensor_msgs::msg::BatteryState batt_state)
{
    std::unique_lock<std::shared_mutex> lock(mutex_);
    feature_set_.has_batt_state = true;
    batt_state_ = batt_state;
}

bool RrStateMaintainer::has_batt_state()
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return feature_set_.has_batt_state;
}

const sensor_msgs::msg::BatteryState RrStateMaintainer::get_batt_state()
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return batt_state_;
}


void RrStateMaintainer::set_image(const sensor_msgs::msg::Image img)
{
    std::unique_lock<std::shared_mutex> lock(mutex_);
    feature_set_.has_img = true;
    img_ = img;
}

bool RrStateMaintainer::has_image()
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return feature_set_.has_img;
}

const sensor_msgs::msg::Image RrStateMaintainer::get_image()
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return img_;
}

void RrStateMaintainer::set_imu(const sensor_msgs::msg::Imu imu)
{
    std::unique_lock<std::shared_mutex> lock(mutex_);
    feature_set_.has_imu = true;
    imu_ = imu;
}

bool RrStateMaintainer::has_imu()
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return feature_set_.has_imu;
}

const sensor_msgs::msg::Imu RrStateMaintainer::get_imu()
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return imu_;
}

void RrStateMaintainer::set_ranges(const std::list<sensor_msgs::msg::Range> ranges)
{
    std::unique_lock<std::shared_mutex> lock(mutex_);
    feature_set_.has_ranges = true;
    ranges_ = ranges;
}

bool RrStateMaintainer::has_ranges()
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return feature_set_.has_ranges;
}

const std::list<sensor_msgs::msg::Range> RrStateMaintainer::get_ranges()
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return ranges_;
}

// default all feature sets to false to start with.
void RrStateMaintainer::init()
{
    feature_set_.has_batt_state = false;
    feature_set_.has_gps = false;
    feature_set_.has_img = false;
    feature_set_.has_imu = false;
    feature_set_.has_joy = false;
    feature_set_.has_ranges = false;
}