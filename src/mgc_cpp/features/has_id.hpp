/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_FEATURES_HAS_ID_HPP
#define MGC_FEATURES_HAS_ID_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace features {

struct HasId {

    virtual ~HasId() = default;

    virtual void set_id(mgc_id_t id) = 0;
    virtual mgc_id_t get_id() const = 0;
};

}// namespace features
}// namespace mgc

#endif/*MGC_FEATURES_HAS_ID_HPP*/
