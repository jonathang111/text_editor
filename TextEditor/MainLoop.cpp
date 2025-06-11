#include "TextFunctions.h"
Editor* Editor::instance = nullptr;
const auto debounceDelay = std::chrono::milliseconds(8);
std::atomic<std::chrono::steady_clock::time_point> Editor::lastResizeRequestTime;
std::atomic<int> Editor::delta = 0;

void Editor::start(){ //add method to change debounce delay, and frame sleep delay
    static const auto debounceDelay = std::chrono::milliseconds(8);
    signal(SIGWINCH, Editor::handle_sigwinch);
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(8));
        auto now = std::chrono::steady_clock::now();
        auto last = lastResizeRequestTime.load(std::memory_order_relaxed);
        int ifdelta = delta.load();
        if (now - last >= debounceDelay || ifdelta >= 1) {
            TerminalResize();
            lastResizeRequestTime.store(std::chrono::steady_clock::time_point::min(), std::memory_order_relaxed);
        }

        ReadInput();
        }
  
}