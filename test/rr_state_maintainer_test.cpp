// have a look at https://medium.com/@junbs95/code-completion-and-debugging-for-ros2-in-vscode-a4ede900d979 for
// more information about debugging,  doesn't work unless on command prompt

// TODO: This test needs to be renamed to rr_state_maintainer_test

#include "rclcpp/rclcpp.hpp"

#include "rr_buf_factory/rr_state_maintainer.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sensor_msgs/image_encodings.hpp>

#define CTRL_AXIS_XL 0
#define CTRL_AXIS_YL 1

// Right stick X and Y axis
#define CTRL_AXIS_XR 2
#define CTRL_AXIS_YR 2

// buttons
#define CTRL_X_BUTTON 0
#define CTRL_SCROLL_DOWN 11
#define CTRL_SCROLL_UP 12
#define CTRL_SCROLL_LEFT 13
#define CTRL_SCROLL_RIGHT 14


// I could not find any frame_id standard for joysticks,  this may be because they do
// not reflect, spacial relations or coordinate systems in the traditional sense,  but
// it urks me not to have one,  so have named it joy-ps4
#define FRAME_ID_JOY_PS4 "joy-ps4"
#define TOPIC_JOY "/joy"

#define FRAME_ID_BATT_STATE "power_supply"
#define TOPIC_BATTERY_STATE "/battery_state"
#define POWER_SUPPLY_LOCATION "Robot-mounted"

using namespace rrobot;

class TestController : public testing::Test
{
  protected:
    TestController()
    {
    }

    ~TestController() override
    {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    rclcpp::Logger logger_ = rclcpp::get_logger("test_logger");
    RrStateMaintainer state_maintainer_ = RrStateMaintainer(logger_);
};


// Test setters and getters
TEST_F(TestController, gps)
{
    rclcpp::Clock clock;
    auto current_time = clock.now();

    sensor_msgs::msg::NavSatFix expected;
    expected.header.stamp = current_time;
    expected.header.frame_id = "gps_link";
    expected.status.status = sensor_msgs::msg::NavSatStatus::STATUS_FIX;
    expected.status.service = sensor_msgs::msg::NavSatStatus::SERVICE_GPS;

    // Set geographic coordinates (latitude, longitude, altitude)
    expected.latitude = -33.8688;  // Degrees, e.g., Sydney
    expected.longitude = 151.2093; // Degrees, e.g., Sydney
    expected.altitude = 58.0;      // In meters above WGS84 ellipsoid

    // Set position covariance (if known, otherwise leave as default zeros)
    std::fill(std::begin(expected.position_covariance), std::end(expected.position_covariance), 0.0);
    expected.position_covariance_type = sensor_msgs::msg::NavSatFix::COVARIANCE_TYPE_UNKNOWN;
    state_maintainer_.set_gps(expected);

    // Result
    sensor_msgs::msg::NavSatFix actual = state_maintainer_.get_gps();
    EXPECT_EQ(actual.header.stamp, current_time);
    EXPECT_EQ(actual.header.frame_id, "gps_link");
    EXPECT_EQ(actual.status.status, sensor_msgs::msg::NavSatStatus::STATUS_FIX);
    EXPECT_EQ(actual.status.service, sensor_msgs::msg::NavSatStatus::SERVICE_GPS);

    // allow a tolerance of around 1 meter.
    EXPECT_NEAR(actual.latitude, -33.8688, 0.000009);
    EXPECT_NEAR(actual.longitude, 151.2093, 0.000009);
    EXPECT_NEAR(actual.altitude, 58.0, 1);

    GTEST_EXPECT_TRUE(state_maintainer_.has_gps());
}

TEST_F(TestController, joystick)
{
    rclcpp::Clock clock;
    auto current_time = clock.now();
    sensor_msgs::msg::Joy joystick;

    joystick.header.frame_id = FRAME_ID_JOY_PS4;
    joystick.header.stamp = current_time;

    joystick.axes.resize(4);
    auto it = joystick.axes.begin();

    std::advance(it, CTRL_AXIS_XL);
    joystick.axes.insert(it, 0.1f);

    it = joystick.axes.begin();
    std::advance(it, CTRL_AXIS_YL);
    joystick.axes.insert(it, -0.8f);

    it = joystick.axes.begin();
    std::advance(it, CTRL_AXIS_XR);
    joystick.axes.insert(it, 0.0);

    it = joystick.axes.begin();
    std::advance(it, CTRL_AXIS_YR);
    joystick.axes.insert(it, 0.3f);

    // fill up buffer to avoid nulls
    joystick.buttons.resize(14);
    std::fill(joystick.buttons.begin(), joystick.buttons.end(), false);

    auto it2 = joystick.buttons.begin();
    std::advance(it2, CTRL_X_BUTTON);
    joystick.buttons.insert(it2, true);

    it2 = joystick.buttons.begin();
    std::advance(it2, CTRL_SCROLL_UP);
    joystick.buttons.insert(it2, true);

    state_maintainer_.set_joystick(joystick);

    // From experience found controllers will start to lose precision over time, especially
    // if you like first player shooters hahahahahahah, going with 0.0009 which is pretty arbitory
    sensor_msgs::msg::Joy actual = state_maintainer_.get_joystick();
    EXPECT_NEAR(actual.axes.at(CTRL_AXIS_XL), 0.1, 0.11);
    EXPECT_NEAR(actual.axes.at(CTRL_AXIS_YL), -0.8, 0.81);
    EXPECT_NEAR(actual.axes.at(CTRL_AXIS_YL), -0.8, 0.81);
    EXPECT_NEAR(actual.axes.at(CTRL_AXIS_YR), 0.3, 0.31);
    EXPECT_TRUE(actual.buttons.at(CTRL_X_BUTTON));
    EXPECT_TRUE(actual.buttons.at(CTRL_SCROLL_UP));

    EXPECT_TRUE(state_maintainer_.has_joystick());
}

TEST_F(TestController, batt_state)
{
    sensor_msgs::msg::BatteryState batt_state;
    rclcpp::Clock clock;
    auto current_time = clock.now();

    batt_state.header.stamp = current_time;
    batt_state.header.frame_id = FRAME_ID_BATT_STATE;

    // 14 volt battery
    batt_state.voltage = 14;

    // 20 degrees Celsius
    batt_state.temperature = 20;
    batt_state.charge = 8;
    batt_state.capacity = 4.2;
    batt_state.design_capacity = 4.2;
    batt_state.percentage = 0.8;
    batt_state.power_supply_status = sensor_msgs::msg::BatteryState::POWER_SUPPLY_STATUS_FULL;
    batt_state.power_supply_health = sensor_msgs::msg::BatteryState::POWER_SUPPLY_HEALTH_GOOD;
    batt_state.power_supply_technology = sensor_msgs::msg::BatteryState::POWER_SUPPLY_TECHNOLOGY_LIPO;
    batt_state.present = true;
    batt_state.cell_voltage.push_back(3.2);
    batt_state.cell_voltage.push_back(3.2);
    batt_state.cell_voltage.push_back(3.2);
    batt_state.cell_voltage.push_back(3.2);
    batt_state.cell_temperature.push_back(18);
    batt_state.cell_temperature.push_back(20);
    batt_state.cell_temperature.push_back(20);
    batt_state.cell_temperature.push_back(21);
    batt_state.location = POWER_SUPPLY_LOCATION;
    batt_state.serial_number = "";

    state_maintainer_.set_batt_state(batt_state);
    sensor_msgs::msg::BatteryState actual = state_maintainer_.get_batt_state();

    EXPECT_EQ(actual.header.stamp, current_time);
    EXPECT_EQ(actual.header.frame_id, FRAME_ID_BATT_STATE);

    EXPECT_NEAR(actual.voltage, 14, 0.1);
    EXPECT_NEAR(actual.temperature, 20, 0.1);
    EXPECT_NEAR(actual.charge, 8, 0.1);
    EXPECT_NEAR(actual.design_capacity, 4.2, 0.01);
    EXPECT_NEAR(actual.percentage, 0.8, 0.0001);
    EXPECT_EQ(actual.power_supply_status, sensor_msgs::msg::BatteryState::POWER_SUPPLY_STATUS_FULL);
    EXPECT_EQ(actual.power_supply_health, sensor_msgs::msg::BatteryState::POWER_SUPPLY_HEALTH_GOOD);
    EXPECT_EQ(actual.power_supply_technology, sensor_msgs::msg::BatteryState::POWER_SUPPLY_TECHNOLOGY_LIPO);
    EXPECT_TRUE(actual.present);

    for (auto it = actual.cell_voltage.begin(); it != actual.cell_voltage.end(); ++it) {
        EXPECT_NEAR(*it, 3.2, 0.01);
    }

    EXPECT_NEAR(actual.cell_temperature.at(0), 18, 0.1);
    EXPECT_NEAR(actual.cell_temperature.at(1), 20, 0.1);
    EXPECT_NEAR(actual.cell_temperature.at(2), 20, 0.1);
    EXPECT_NEAR(actual.cell_temperature.at(3), 21, 0.1);
    EXPECT_EQ(actual.location, POWER_SUPPLY_LOCATION);
    EXPECT_EQ(actual.serial_number, "");
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();
    return result;
}