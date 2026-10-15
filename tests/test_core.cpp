#include "ddr_core.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

static unsigned checks;
#define CHECK(test) do { ++checks; if (!(test)) { std::fprintf(stderr, "FAILED line %d: %s\n", __LINE__, #test); std::exit(1); } } while (false)
using namespace ddr;
int main() {
    const Identity one{10, 1}, two{10, 2}, other_fabric{11, 1}, fourth{10, 4};
    Registry registry;
    CHECK(registry.size() == 0 && !registry.active());
    CHECK(registry.add({10, 0}, "invalid") == Result::Invalid);
    CHECK(registry.add(one, "") == Result::Invalid);
    CHECK(registry.add(one, nullptr) == Result::Invalid);
    CHECK(registry.add(one, "line\nbreak") == Result::Invalid);
    CHECK(registry.add(one, "X40") == Result::Ok);
    CHECK(registry.add(one, "duplicate") == Result::Duplicate);
    CHECK(registry.add(two, "X50") == Result::Ok);
    CHECK(registry.add(other_fabric, "Same NodeId, different fabric") == Result::Ok);
    CHECK(registry.size() == 3);
    CHECK(registry.add(fourth, "fourth") == Result::Full);
    CHECK(registry.select(two) == Result::Ok);
    CHECK(registry.remove(one) == Result::Ok);
    CHECK(registry.active() && *registry.active() == two);
    CHECK(registry.find(other_fabric) != nullptr);
    CHECK(registry.select(one) == Result::Missing);
    CHECK(registry.rename(two, "Upstairs") == Result::Ok);
    CHECK(std::strcmp(registry.find(two)->name.data(), "Upstairs") == 0);
    CHECK(registry.rename(two, "0123456789012345678901234567890123456789012345678") == Result::Invalid);
    CHECK(registry.rename(one, "Missing") == Result::Missing);
    CHECK(registry.remove(two) == Result::Ok && !registry.active());
    CHECK(registry.remove(two) == Result::Missing);
    CHECK(registry.add(fourth, "Replacement") == Result::Ok);
    CHECK(registry.active() && *registry.active() == fourth);
    CHECK(registry.find(fourth)->observation.state == State::Unknown);
    CHECK(!registry.find(fourth)->observation.battery_percent);
    CHECK(!registry.find(fourth)->capabilities.actions[0]);

    // Synthetic protocol fixture; these numbers are not a Dreame capability claim.
    Capabilities caps;
    caps.actions[0] = Route{7, 0x54, 0, uint8_t{42}};
    caps.actions[1] = Route{9, 0x61, 0, std::nullopt};
    caps.actions[2] = Route{9, 0x61, 3, std::nullopt};
    caps.actions[3] = Route{9, 0x61, 0x80, std::nullopt};
    Observation idle{State::Idle, std::nullopt, 1, 1000};
    Command command;
    CHECK(command.begin(one, Action::Start, Connection::Offline, caps, idle, 1000) == Result::Offline);
    CHECK(command.begin(one, Action::Start, Connection::Online, {}, idle, 1000) == Result::Unsupported);
    CHECK(command.begin(one, Action::Count, Connection::Online, caps, idle, 1000) == Result::Invalid);
    CHECK(command.begin(one, Action::Start, Connection::Online, caps, idle, 16001) == Result::Stale);
    CHECK(command.begin(one, Action::Start, Connection::Online, caps, idle, 999) == Result::Stale);
    Observation unknown;
    CHECK(command.begin(one, Action::Start, Connection::Online, caps, unknown, 1000) == Result::Stale);
    CHECK(command.begin(one, Action::Start, Connection::Online, caps, idle, 1000) == Result::Ok);
    CHECK(command.route()->endpoint == 7 && command.route()->mode == 42);
    CHECK(command.phase() == Phase::Sending);
    const auto old_ticket = command.ticket();
    command.acknowledge(command.ticket());
    CHECK(command.phase() == Phase::AwaitingReadback);
    command.timeout(command.ticket());
    CHECK(command.phase() == Phase::NeedsReadback);
    command.reject(command.ticket()); // Late callback cannot unblock a timed-out command.
    CHECK(command.phase() == Phase::NeedsReadback);
    CHECK(command.begin(one, Action::Start, Connection::Online, caps, idle, 1000) == Result::Busy);
    Observation cleaning{State::Cleaning, uint8_t{0}, 2, 1001};
    CHECK(!command.reconcile(command.ticket(), two, cleaning));
    CHECK(!command.reconcile(command.ticket(), one, idle));
    Observation old = cleaning; old.received_ms = 999;
    CHECK(!command.reconcile(command.ticket(), one, old));
    CHECK(command.reconcile(command.ticket(), one, cleaning));
    CHECK(command.phase() == Phase::Confirmed);
    CHECK(cleaning.battery_percent && *cleaning.battery_percent == 0);
    CHECK(command.begin(one, Action::Start, Connection::Online, caps, cleaning, 1002) == Result::Invalid);
    CHECK(command.begin(one, Action::Pause, Connection::Online, caps, cleaning, 1002) == Result::Ok);
    CHECK(command.route()->endpoint == 9 && !command.route()->mode);
    command.reject(old_ticket);
    command.acknowledge(old_ticket);
    command.timeout(old_ticket);
    CHECK(!command.reconcile(old_ticket, one, cleaning));
    CHECK(command.phase() == Phase::Sending);
    command.timeout(command.ticket());
    Observation paused{State::Paused, std::nullopt, 3, 1003};
    CHECK(command.reconcile(command.ticket(), one, paused));
    CHECK(command.begin(one, Action::Resume, Connection::Online, caps, paused, 1004) == Result::Ok);
    Observation failed{State::Error, std::nullopt, 4, 1005};
    CHECK(!command.reconcile(command.ticket(), one, failed) && command.phase() == Phase::Failed);
    CHECK(command.begin(one, Action::Dock, Connection::Online, caps, paused, 1006) == Result::Ok);
    command.acknowledge(command.ticket());
    Observation docked{State::Docked, std::nullopt, 5, 1007};
    CHECK(command.reconcile(command.ticket(), one, docked));
    CHECK(command.begin(one, Action::Dock, Connection::Online, caps, docked, 1008) == Result::Invalid);

    auto *robot = registry.find(fourth);
    CHECK(robot->command.begin(fourth, Action::Start, Connection::Online, caps, idle, 1000) == Result::Ok);
    CHECK(registry.remove(fourth) == Result::Busy);
    robot->command.timeout(robot->command.ticket());
    CHECK(registry.remove(fourth) == Result::Busy);
    CHECK(robot->command.reconcile(robot->command.ticket(), fourth, cleaning));
    CHECK(registry.remove(fourth) == Result::Ok);
    CHECK(std::strcmp(state_text(State::Unknown), "Unknown") == 0);
    std::printf("%u core checks passed\n", checks);
}
