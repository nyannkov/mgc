/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_FEATURES_UPDATABLE_HPP
#define MGC_FEATURES_UPDATABLE_HPP

#include "mgc_cpp/internal/common.hpp"

namespace mgc {
namespace features {

template <typename UserContextT>
struct Updatable {

    virtual ~Updatable() = default;

    virtual void update(UserContextT& ctx) = 0;
};

}// namespace features
}// namespace mgc

#endif/*MGC_FEATURES_UPDATABLE_HPP*/
