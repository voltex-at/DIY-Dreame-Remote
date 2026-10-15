#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace ddr {
// Compressed Fabric ID incorporates the root public key and Fabric ID.
// This is a public lookup key, never a substitute for CASE authentication.
struct Identity {
    uint64_t compressed_fabric_id = 0;
    uint64_t node_id = 0;
    bool operator==(const Identity &other) const {
        return compressed_fabric_id == other.compressed_fabric_id && node_id == other.node_id;
    }
    bool valid() const { return node_id != 0; }
};
enum class State { Unknown, Idle, Cleaning, Paused, Returning, Docked, Error };
enum class Action : size_t { Start, Pause, Resume, Dock, Count };
enum class Connection { Offline, Connecting, Online };
// Filled by discovery of the actual endpoint, AcceptedCommandList and mode tags.
// No model-name-based route or implicit endpoint/mode number is supplied.
struct Route {
    uint16_t endpoint;
    uint32_t cluster;
    uint32_t command;
    std::optional<uint8_t> mode;
};
struct Capabilities {
    std::array<std::optional<Route>, static_cast<size_t>(Action::Count)> actions{};
    bool service_area = false;
};
enum class Phase { Idle, Sending, AwaitingReadback, NeedsReadback, Confirmed, Failed };
enum class Result { Ok, Invalid, Full, Duplicate, Missing, Offline, Unsupported, Stale, Busy };
struct Observation {
    State state = State::Unknown;
    std::optional<uint8_t> battery_percent;
    uint64_t revision = 0;
    uint64_t received_ms = 0;
};
// One instance per robot. Timeout never permits replay until fresh readback.
class Command {
public:
    Result begin(Identity target, Action action, Connection connection,
                 const Capabilities &caps, const Observation &state, uint64_t now_ms);
    void acknowledge(uint64_t ticket);
    void timeout(uint64_t ticket);
    void reject(uint64_t ticket);
    bool reconcile(uint64_t ticket, Identity target, const Observation &fresh);
    Phase phase() const { return phase_; }
    uint64_t ticket() const { return ticket_; }
    std::optional<Route> route() const { return route_; }
private:
    Phase phase_ = Phase::Idle;
    uint64_t ticket_ = 0;
    Identity target_{};
    Action action_ = Action::Start;
    uint64_t baseline_revision_ = 0;
    uint64_t started_ms_ = 0;
    std::optional<Route> route_;
};
struct Robot {
    Identity identity{};
    std::array<char, 49> name{};
    Connection connection = Connection::Offline;
    Capabilities capabilities{};
    Observation observation{};
    Command command{};
};
class Registry {
public:
    static constexpr size_t capacity = 3;
    Result add(Identity identity, const char *name);
    Result remove(Identity identity);
    Result rename(Identity identity, const char *name);
    Result select(Identity identity);
    Robot *find(Identity identity);
    const Robot *find(Identity identity) const;
    std::optional<Identity> active() const { return active_; }
    size_t size() const;
private:
    std::array<std::optional<Robot>, capacity> robots_{};
    std::optional<Identity> active_;
};
const char *state_text(State state);
} // namespace ddr
