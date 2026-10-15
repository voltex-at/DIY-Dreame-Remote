#include "ddr_core.hpp"
#include <cstring>

namespace ddr {
static bool valid_name(const char *name) {
    if (!name) return false;
    size_t size = 0;
    for (; size < 49 && name[size]; ++size) {
        if (static_cast<unsigned char>(name[size]) < 0x20) return false;
    }
    return size > 0 && size < 49;
}
Robot *Registry::find(Identity id) {
    for (auto &robot : robots_) if (robot && robot->identity == id) return &*robot;
    return nullptr;
}
const Robot *Registry::find(Identity id) const {
    for (const auto &robot : robots_) if (robot && robot->identity == id) return &*robot;
    return nullptr;
}
size_t Registry::size() const {
    size_t n = 0;
    for (const auto &robot : robots_) if (robot) ++n;
    return n;
}
Result Registry::add(Identity id, const char *name) {
    if (!id.valid() || !valid_name(name)) return Result::Invalid;
    if (find(id)) return Result::Duplicate;
    for (auto &slot : robots_) {
        if (slot) continue;
        slot.emplace();
        slot->identity = id;
        std::memcpy(slot->name.data(), name, std::strlen(name) + 1);
        if (!active_) active_ = id;
        return Result::Ok;
    }
    return Result::Full;
}
Result Registry::rename(Identity id, const char *name) {
    if (!valid_name(name)) return Result::Invalid;
    auto *robot = find(id);
    if (!robot) return Result::Missing;
    robot->name.fill(0);
    std::memcpy(robot->name.data(), name, std::strlen(name) + 1);
    return Result::Ok;
}
Result Registry::select(Identity id) {
    if (!find(id)) return Result::Missing;
    active_ = id;
    return Result::Ok;
}
Result Registry::remove(Identity id) {
    for (auto &robot : robots_) {
        if (!robot || !(robot->identity == id)) continue;
        if (robot->command.phase() == Phase::Sending ||
            robot->command.phase() == Phase::AwaitingReadback ||
            robot->command.phase() == Phase::NeedsReadback) return Result::Busy;
        robot.reset();
        if (active_ && *active_ == id) active_.reset();
        return Result::Ok;
    }
    return Result::Missing;
}
Result Command::begin(Identity target, Action action, Connection connection,
                      const Capabilities &caps, const Observation &state, uint64_t now) {
    const auto index = static_cast<size_t>(action);
    if (!target.valid() || index >= caps.actions.size()) return Result::Invalid;
    if (phase_ == Phase::Sending || phase_ == Phase::AwaitingReadback ||
        phase_ == Phase::NeedsReadback) return Result::Busy;
    if (connection != Connection::Online) return Result::Offline;
    if (!caps.actions[index]) return Result::Unsupported;
    if (state.revision == 0 || state.state == State::Unknown || now < state.received_ms ||
        now - state.received_ms > 15000) return Result::Stale;
    // Don't treat an already satisfied state as a successful command readback.
    const bool permitted =
        (action == Action::Start && (state.state == State::Idle || state.state == State::Docked)) ||
        (action == Action::Pause && state.state == State::Cleaning) ||
        (action == Action::Resume && state.state == State::Paused) ||
        (action == Action::Dock && (state.state == State::Idle || state.state == State::Cleaning || state.state == State::Paused));
    if (!permitted) return Result::Invalid;
    if (++ticket_ == 0) ++ticket_;
    target_ = target;
    action_ = action;
    baseline_revision_ = state.revision;
    started_ms_ = now;
    route_ = caps.actions[index];
    phase_ = Phase::Sending;
    return Result::Ok;
}
void Command::acknowledge(uint64_t ticket) {
    if (ticket != ticket_) return;
    if (phase_ == Phase::Sending) phase_ = Phase::AwaitingReadback;
}
void Command::timeout(uint64_t ticket) {
    if (ticket != ticket_) return;
    if (phase_ == Phase::Sending || phase_ == Phase::AwaitingReadback) phase_ = Phase::NeedsReadback;
}
void Command::reject(uint64_t ticket) {
    if (ticket != ticket_) return;
    // Rejection is valid only for a definitive protocol failure before timeout.
    if (phase_ == Phase::Sending || phase_ == Phase::AwaitingReadback) phase_ = Phase::Failed;
}
bool Command::reconcile(uint64_t ticket, Identity target, const Observation &fresh) {
    if (ticket != ticket_) return false;
    if (phase_ != Phase::Sending && phase_ != Phase::AwaitingReadback && phase_ != Phase::NeedsReadback) return false;
    if (!(target == target_) || fresh.revision <= baseline_revision_ ||
        fresh.received_ms < started_ms_ || fresh.state == State::Unknown) return false;
    const bool matched =
        ((action_ == Action::Start || action_ == Action::Resume) && fresh.state == State::Cleaning) ||
        (action_ == Action::Pause && fresh.state == State::Paused) ||
        (action_ == Action::Dock && (fresh.state == State::Returning || fresh.state == State::Docked));
    phase_ = matched ? Phase::Confirmed : Phase::Failed;
    return matched;
}
const char *state_text(State state) {
    switch (state) {
        case State::Idle: return "Idle";
        case State::Cleaning: return "Cleaning";
        case State::Paused: return "Paused";
        case State::Returning: return "Returning to dock";
        case State::Docked: return "Docked";
        case State::Error: return "Robot error";
        default: return "Unknown";
    }
}
} // namespace ddr
