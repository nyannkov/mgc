#ifndef MGC_STATUS_DISPLAY_REQUEST_HPP
#define MGC_STATUS_DISPLAY_REQUEST_HPP

namespace app {

enum class StatusDisplayRequestType {
    None,
    Show,
    Hide
};

struct StatusDisplayRequest {

    StatusDisplayRequest() : request_(StatusDisplayRequestType::None) { }

    void request_show() { request_ = StatusDisplayRequestType::Show; }
    void request_hide() { request_ = StatusDisplayRequestType::Hide; }
    void clear_request() { request_ = StatusDisplayRequestType::None; }
    StatusDisplayRequestType request() const { return request_; }

private:
    StatusDisplayRequestType request_;
};

}// namespace app

#endif/*MGC_STATUS_DISPLAY_REQUEST_HPP*/

