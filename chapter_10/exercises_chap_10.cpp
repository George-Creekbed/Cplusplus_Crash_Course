#define CATCH_CONFIG_MAIN

#include "../catch/Catch2/catch.hpp"
#include <functional>
#include <stdexcept>

using namespace std;

struct SpeedUpdate {
  double velocity_mps;
};

struct CarDetected {
  double distance_m;
  double velocity_mps;
};

struct SpeedLimitDetected {
  unsigned short speed_mps;
};

struct BrakeCommand {
  double time_to_collision_s;
};

// typedef function<void(const SpeedUpdate&)> SpeedUpdateCallback;
// typedef function<void(const CarDetected&)> CarDetectedCallback;
using SpeedUpdateCallback = function<void(const SpeedUpdate&)>;
using CarDetectedCallback = function<void(const CarDetected&)>;
using SpeedLimitCallback = function<void(const SpeedLimitDetected&)>;

struct IServiceBus {
  virtual ~IServiceBus() = default;
  virtual void publish(const BrakeCommand&) = 0;
  virtual void subscribe(SpeedUpdateCallback) = 0;
  virtual void subscribe(CarDetectedCallback) = 0;
  virtual void subscribe(SpeedLimitCallback) = 0;
};

struct MockServiceBus : IServiceBus {
  void publish(const BrakeCommand& cmd) override {
    commands_published++;
    last_command = cmd;
  };
  void subscribe(SpeedUpdateCallback callback) override {
    speed_update_callback = callback;
  };
  void subscribe(CarDetectedCallback callback) override {
    car_detected_callback = callback;
  };
  void subscribe(SpeedLimitCallback callback) override {
    speed_limit_callback = callback;
  };
  BrakeCommand last_command{};
  int commands_published{};
  SpeedUpdateCallback speed_update_callback{};
  CarDetectedCallback car_detected_callback{};
  SpeedLimitCallback speed_limit_callback{};
};

struct AutoBrake {
  AutoBrake(IServiceBus& bus)
    : collision_threshold_s{ 5 }
    , speed_mps{}
    , speed_limit_mps{39} {
    bus.subscribe(
      [this, &bus](const SpeedUpdate& update) {
        speed_mps = update.velocity_mps; 
        if (speed_mps > speed_limit_mps)
          bus.publish(BrakeCommand{});
      }
    );
    bus.subscribe(
      [this, &bus](const CarDetected& cd) {
        auto relative_velocity_mps = speed_mps - cd.velocity_mps;
        auto time_to_collision_s = cd.distance_m / relative_velocity_mps;
        if(time_to_collision_s > 0 && time_to_collision_s <= collision_threshold_s) {
          bus.publish(BrakeCommand{ time_to_collision_s });
        }
      }
    );
    bus.subscribe(
      [this, &bus](const SpeedLimitDetected& sign) {
        speed_limit_mps = sign.speed_mps;
        if (speed_mps > speed_limit_mps)
          bus.publish(BrakeCommand{});
      }
    );
  }
  void set_collision_threshold_s(double x) {
    if(x < 1)
      throw runtime_error{ "Collision less than 1." };
    collision_threshold_s = x;
  }
  double get_collision_threshold_s() const {
    return collision_threshold_s;
  }
  double get_speed_mps() const {
    return speed_mps;
  }
  double get_speed_limit_mps() const {
    return speed_limit_mps;
  }

private:
  double collision_threshold_s;
  double speed_mps;
  double speed_limit_mps;
};

TEST_CASE("AutoBrake") {
  MockServiceBus bus{};
  AutoBrake auto_brake{ bus };

  SECTION("initializes speed to zero") {
    REQUIRE(auto_brake.get_speed_mps() == Approx(0));
  }

  SECTION("initializes sensitivity to five") {
    REQUIRE(auto_brake.get_collision_threshold_s() == Approx(5));
  }

  SECTION("throws when sensitivity less than one") {
    REQUIRE_THROWS(auto_brake.set_collision_threshold_s(0.5L));
  }

  SECTION("saves speed after update") {
    bus.speed_update_callback(SpeedUpdate{ 100L });
    REQUIRE(100L == auto_brake.get_speed_mps());
    bus.speed_update_callback(SpeedUpdate{ 50L });
    REQUIRE(50L == auto_brake.get_speed_mps());
    bus.speed_update_callback(SpeedUpdate{ 0L });
    REQUIRE(0L == auto_brake.get_speed_mps());
  }

  SECTION("no alert when not imminent") {
    auto_brake.set_collision_threshold_s(2L);
    bus.speed_limit_callback(SpeedLimitDetected{110}); // does not trigger auto brake 
    bus.speed_update_callback(SpeedUpdate{ 100L });
    bus.car_detected_callback(CarDetected{ 1000L, 50L });
    REQUIRE(bus.commands_published == 0);
  }

  SECTION("alert when imminent") {
    auto_brake.set_collision_threshold_s(10L);
    bus.speed_limit_callback(SpeedLimitDetected{110});  // does not trigger auto brake 
    bus.speed_update_callback(SpeedUpdate{ 100L });
    bus.car_detected_callback(CarDetected{ 100L, 0L });
    REQUIRE(bus.commands_published == 1);
    REQUIRE(bus.last_command.time_to_collision_s == Approx(1));
  }

  SECTION("last known speed limit initially set to thirty-nine") {
    REQUIRE(auto_brake.get_speed_limit_mps() == Approx(39.));
  }

  SECTION("saves speed limit after update") {
    bus.speed_limit_callback(SpeedLimitDetected{50});
    REQUIRE(auto_brake.get_speed_limit_mps() == 50);
    bus.speed_limit_callback(SpeedLimitDetected{80});
    REQUIRE(auto_brake.get_speed_limit_mps() == 80);
    bus.speed_limit_callback(SpeedLimitDetected{60});
    REQUIRE(auto_brake.get_speed_limit_mps() == 60);
  }

  SECTION("speeding lower than the limit") {
    bus.speed_update_callback(SpeedUpdate{34});
    bus.speed_limit_callback(SpeedLimitDetected{35});
    REQUIRE(bus.commands_published == 0);
  }

  SECTION("speeding over the limit") {
    bus.speed_limit_callback(SpeedLimitDetected{35});
    bus.speed_update_callback(SpeedUpdate{40});
    REQUIRE(bus.commands_published == 1);
    REQUIRE(bus.last_command.time_to_collision_s == 0);
  }

  SECTION("speeding lower then over the limit") {
    bus.speed_limit_callback(SpeedLimitDetected{35});
    bus.speed_update_callback(SpeedUpdate{30});
    REQUIRE(bus.commands_published == 0);
    bus.speed_limit_callback(SpeedLimitDetected{25});
    REQUIRE(bus.commands_published == 1);
    REQUIRE(bus.last_command.time_to_collision_s == 0);
  }
}
