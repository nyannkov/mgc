#include <stdio.h>
#include <chrono>
#include <iostream>
#include "mgc/mgc.h"
#include "mgc_cpp/mgc.hpp"
#include "./resources/generated/btree/test_btree.h"

struct TimerHost : mgc::platform::timer::Timer<TimerHost, uint32_t> {
    static timestamp_t now_ms_impl() {
        auto now = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        return ms;
    }
};

struct InputMock : mgc::platform::input::IButton {
    bool is_pressed(mgc::platform::input::Key key) const override {
        return is_pressed_;
    }

    bool was_pressed(mgc::platform::input::Key key) const override {
        return false;
    }

    uint16_t hold_counter(mgc::platform::input::Key key) const override {
        return 0;
    }

    void set_is_pressed(bool v) {
       is_pressed_ = v; 
    }

private:
    bool is_pressed_ = false;
};


int main() {

    InputMock input_mock;
   
    mgc::control::btree::BTreeController<TimerHost> btc;

    struct Listener : mgc::control::btree::IBTreeListener<mgc::control::btree::BTreeController<TimerHost>> {
        LeafResult on_proc_leaf(std::string_view id, const DurationT& duration, mgc_btree_tag_t tag) override {

            if ( id == "cond/timer/over_60s" ) {
                std::cout << id << std::endl;
                return LeafResult::Success;
            } else if ( id == "action/sleep" ) {
                std::cout << id << std::endl;
                return LeafResult::Failure;
            } else if ( id == "cond/timer/over_30s" ) {
                std::cout << id << std::endl;
                return LeafResult::Success;
            } else if ( id == "action/dance" ) {
                std::cout << id << std::endl;
                return LeafResult::Failure;
            } else if ( id == "idle_breathing" ) {
                std::cout << id << std::endl;
                return LeafResult::Success;

            } else { }
            
            //if ( duration.leaf_elapsed > 1000 ) {
            //    std::cout << id << std::endl;
            //    std::cout << duration.tree_elapsed << std::endl;
            //    std::cout << duration.composite_elapsed << std::endl;
            //    std::cout << duration.leaf_elapsed << std::endl;
            //    std::cout << duration.input_idle_time << std::endl;

            //    return LeafResult::Success;
            //}

            return LeafResult::Running;
        }
    } listener;

    btc.set_btree(test_btree);
    btc.bind_listener(listener);
    
    input_mock.set_is_pressed(true);

    while ( !btc.has_finished() ) {
        btc.proc(mgc::platform::input::is_any_button_pressed(input_mock));
    }

    return 0;
}

