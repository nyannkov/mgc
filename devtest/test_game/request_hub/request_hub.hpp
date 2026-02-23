#ifndef MGC_REQUEST_HUB_HPP
#define MGC_REQUEST_HUB_HPP

#include "status_display_request.hpp"

namespace app {

struct RequestHub {
    RequestHub() { }
    RequestHub(const RequestHub&) = delete;
    RequestHub& operator=(const RequestHub&) = delete;

    StatusDisplayRequest status_display_request{};
};

} // namespace app

#endif/*MGC_REQUEST_HUB_HPP*/

