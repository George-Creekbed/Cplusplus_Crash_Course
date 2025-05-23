#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <functional>
#include <stdexcept>
#include <iostream>

using namespace std;

struct SpeedUpdate {
  double velocity_mps;
};

struct CarDetected {
  double distance_m;
  double velocity_mps;
};

struct BreakCommand {
  double time_to_collision_s;
};

typedef function<void(const SpeedUpdate&)> SpeedUpdateCallback;
typedef function<void(const CarDetected&)> CarDetectedCallback;

struct IServiceBus {
  virtual ~IServiceBus() = default;
  virtual void publish(const BreakCommand&) = 0;
  virtual void subscribe(SpeedUpdateCallback) = 0;
  virtual void subscribe(CarDetectedCallback) = 0;
};

struct MockServiceBus : IServiceBus {
  void publish(const BreakCommand& cmd) override {
    commands_published++;
    last_command = cmd;
  };
  void subscribe(SpeedUpdateCallback callback) override {
    speed_update_callback = callback;
  };
  void subscribe(CarDetectedCallback callback) override {
    car_detected_callback = callback;
  };
  BreakCommand last_command{};
  int commands_published{};
  SpeedUpdateCallback speed_update_callback{};
  CarDetectedCallback car_detected_callback{};
};

struct AutoBrake {
  AutoBrake(IServiceBus& bus)
      : collision_threshold_s{ 5 }
      , speed_mps{} {
    bus.subscribe([this](const SpeedUpdate& update) { speed_mps = update.velocity_mps; });
    bus.subscribe([this, &bus](const CarDetected& cd) {
      auto relative_velocity_mps = speed_mps - cd.velocity_mps;
      auto time_to_collision_s = cd.distance_m / relative_velocity_mps;
      if(time_to_collision_s > 0 && time_to_collision_s <= collision_threshold_s) {
        bus.publish(BreakCommand{ time_to_collision_s });
      }
    });
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

  private:
  double collision_threshold_s;
  double speed_mps;
};

ostream& operator<<(ostream& os, const AutoBrake& brake) {
  os << "Autobreak:\n" << "speed=" << brake.get_speed_mps() << " m/s";
  os << "\ncollision threshold=" << brake.get_collision_threshold_s() << " s\n" << endl;

  return os;
}

TEST_CASE("AutoBrake") {
  MockServiceBus bus{};
  AutoBrake auto_break{ bus };

  SECTION("initializes speed to zero") {
    cout << "Test: initializes speed to zero\n"; 
    cout << auto_break;
    REQUIRE(auto_break.get_speed_mps() == Approx(0));
  }

  SECTION("initializes sensitivity to five") {
    cout << "Test: initializes sensitivity to five\n"; 
    cout << auto_break;
    REQUIRE(auto_break.get_collision_threshold_s() == Approx(5));
  }

  SECTION("throws when sensitivity less than one") {
    cout << "Test: throws when sensitivity less than one\n"; 
    REQUIRE_THROWS(auto_break.set_collision_threshold_s(0.5L));
    cout << auto_break;
  }

  SECTION("saves speed after update") {
    cout << "Test: saves speed after update\n"; 
    bus.speed_update_callback(SpeedUpdate{ 100L });
    cout << auto_break;
    REQUIRE(100L == auto_break.get_speed_mps());
    bus.speed_update_callback(SpeedUpdate{ 50L });
    cout << auto_break;
    REQUIRE(50L == auto_break.get_speed_mps());
    bus.speed_update_callback(SpeedUpdate{ 0L });
    cout << auto_break;
    REQUIRE(0L == auto_break.get_speed_mps());
  }

  SECTION("no alert when not imminent") {
    cout << "Test: no alert when not imminent\n"; 
    auto_break.set_collision_threshold_s(2L);
    bus.speed_update_callback(SpeedUpdate{ 100L });
    bus.car_detected_callback(CarDetected{ 1000L, 50L });
    cout << auto_break;
    REQUIRE(bus.commands_published == 0);
  }

  SECTION("alert when imminent") {
    cout << "Test: alert when imminent\n"; 
    auto_break.set_collision_threshold_s(10L);
    bus.speed_update_callback(SpeedUpdate{ 100L });
    bus.car_detected_callback(CarDetected{ 100L, 0L });
    cout << auto_break;
    assert(bus.commands_published == 1);
    assert(bus.last_command.time_to_collision_s == Approx(1));
  }
}
