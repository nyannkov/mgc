/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_FEATURES_VISIBLE_HPP
#define MGC_FEATURES_VISIBLE_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace features {

struct Visible {

    virtual ~Visible() = default;

    virtual bool is_visible() const = 0;
    virtual void set_visible(bool v) = 0;
};

}// namespace features
}// namespace mgc

#endif/*MGC_FEATURES_VISIBLE_HPP*/
