#pragma once

#include "sphere_operations.hh"

#include <memory>      // unique_ptr
#include <type_traits> // enable_if, is_const
#include <utility>     // forward

namespace wt {

class shape {
    friend tform4& tform(shape& s) noexcept { return s.pimpl_->do_tform(); }
    friend tform4 const& tform(shape const& s) noexcept { return s.pimpl_->do_tform(); }

    friend tform4& inv_tform(shape& s) noexcept { return s.pimpl_->do_inv_tform(); }
    friend tform4 const& inv_tform(shape const& s) noexcept { return s.pimpl_->do_inv_tform(); }

    friend material& mater(shape& s) noexcept { return s.pimpl_->do_mater(); }
    friend material const& mater(shape const& s) noexcept { return s.pimpl_->do_mater(); }

    friend void intersect(shape const& s, ray const& object_ray, unsigned shape_id,
                          std::vector<intersection>& world_isecs) noexcept {
        s.pimpl_->do_intersect(object_ray, std::move(shape_id), world_isecs);
    }

    class shape_concept {
      public:
        virtual ~shape_concept() = 0;

        virtual tform4& do_tform() noexcept = 0;
        virtual tform4 const& do_tform() const noexcept = 0;

        virtual tform4& do_inv_tform() noexcept = 0;
        virtual tform4 const& do_inv_tform() const noexcept = 0;

        virtual material& do_mater() noexcept = 0;
        virtual material const& do_mater() const noexcept = 0;

        virtual void do_intersect(ray const& object_ray, unsigned shape_id,
                                  std::vector<intersection>& world_isecs) const noexcept = 0;

        virtual std::unique_ptr<shape_concept> clone() const = 0;

      protected:
        shape_concept() = default;
        shape_concept(shape_concept const&) = default;
        shape_concept(shape_concept&&) noexcept = default;
        shape_concept& operator=(shape_concept&&) noexcept = default;
        shape_concept& operator=(shape_concept const&) = default;
    };

    template <typename ConcreteShape> class shape_model final : public shape_concept {
      public:
        explicit shape_model(ConcreteShape&& concrete_shape)
            : object_{std::forward<ConcreteShape>(concrete_shape)} {}

        tform4& do_tform() noexcept override { return tform(object_); }
        tform4 const& do_tform() const noexcept override { return tform(object_); }

        tform4& do_inv_tform() noexcept override { return inv_tform(object_); }
        tform4 const& do_inv_tform() const noexcept override { return inv_tform(object_); }

        material& do_mater() noexcept override { return mater(object_); }
        material const& do_mater() const noexcept override { return mater(object_); }

        void do_intersect(ray const& object_ray, unsigned shape_id,
                          std::vector<intersection>& world_isecs) const noexcept override {
            intersect(object_, object_ray, std::move(shape_id), world_isecs);
        }

        std::unique_ptr<shape_concept> clone() const override {
            return std::make_unique<shape_model>(*this);
        }

      private:
        ConcreteShape object_;
    };

  public:
    template <typename ConcreteShape>
    shape(ConcreteShape concrete_shape)
        : pimpl_{std::make_unique<shape_model<ConcreteShape>>(std::move(concrete_shape))} {}

    shape(shape const& other) : pimpl_{other.pimpl_->clone()} {}

    shape& operator=(shape const& rhs) {
        if (&rhs != this) {
            pimpl_ = rhs.pimpl_->clone();
        }
        return *this;
    }

    shape(shape&&) noexcept = default;
    shape& operator=(shape&&) noexcept = default;

    ~shape() = default;

  private:
    std::unique_ptr<shape_concept> pimpl_;
};

inline shape::shape_concept::~shape_concept() = default;

} // namespace wt
