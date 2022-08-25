#pragma once

#include "checker2d_pattern_operations.hh"
#include "checker3d_pattern_operations.hh"
#include "color.hh"
#include "gradient_pattern_operations.hh"
#include "nested_checker2d_pattern_operations.hh"
#include "radial_gradient_pattern_operations.hh"
#include "ring_pattern_operations.hh"
#include "solid_pattern_operations.hh"
#include "stripe_pattern_operations.hh"

#include <memory>      // unique_ptr
#include <type_traits> // enable_if, is_const
#include <utility>     // forward

namespace wt {

class pattern {
    friend tform4& inv_tform(pattern& pattern) noexcept { return pattern.pimpl_->do_inv_tform(); }
    friend tform4 const& inv_tform(pattern const& pattern) noexcept {
        return pattern.pimpl_->do_inv_tform();
    }

    friend color pattern_at(pattern const& pattern, pnt3 const& world_point) noexcept {
        return pattern.pimpl_->do_pattern_at(world_point);
    }

    class pattern_concept {
      public:
        virtual ~pattern_concept() = 0;

        virtual tform4& do_inv_tform() noexcept = 0;
        virtual tform4 const& do_inv_tform() const noexcept = 0;

        virtual color do_pattern_at(pnt3 const& world_point) const noexcept = 0;

        virtual std::unique_ptr<pattern_concept> clone() const = 0;

      protected:
        pattern_concept() = default;
        pattern_concept(pattern_concept const&) = default;
        pattern_concept(pattern_concept&&) noexcept = default;
        pattern_concept& operator=(pattern_concept&&) noexcept = default;
        pattern_concept& operator=(pattern_concept const&) = default;
    };

    template <typename ConcretePattern> class pattern_model final : public pattern_concept {
      public:
        explicit pattern_model(ConcretePattern&& concrete_pattern)
            : object_{std::forward<ConcretePattern>(concrete_pattern)} {}

        tform4& do_inv_tform() noexcept override { return inv_tform(object_); }
        tform4 const& do_inv_tform() const noexcept override { return inv_tform(object_); }

        color do_pattern_at(pnt3 const& world_point) const noexcept override {
            return pattern_at(object_, world_point);
        }

        std::unique_ptr<pattern_concept> clone() const override {
            return std::make_unique<pattern_model>(*this);
        }

      private:
        ConcretePattern object_;
    };

  public:
    template <typename ConcretePattern>
    pattern(ConcretePattern concrete_pattern)
        : pimpl_{std::make_unique<pattern_model<ConcretePattern>>(std::move(concrete_pattern))} {}

    pattern(pattern const& other) : pimpl_{other.pimpl_->clone()} {}

    pattern& operator=(pattern const& rhs) {
        if (&rhs != this) {
            pimpl_ = rhs.pimpl_->clone();
        }
        return *this;
    }

    pattern(pattern&&) noexcept = default;
    pattern& operator=(pattern&&) noexcept = default;

    ~pattern() = default;

  private:
    std::unique_ptr<pattern_concept> pimpl_;
};

inline pattern::pattern_concept::~pattern_concept() = default;

} // namespace wt
