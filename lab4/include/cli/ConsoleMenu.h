#pragma once

namespace cli {

/// Runs the interactive sorting and directed graph demonstrations.
class ConsoleMenu {
public:
    /// Reads commands until the user selects Exit or the input stream ends.
    void run();
};

} // namespace cli
