#pragma once

#include "sphere_operations.hh"

#include <memory>      // unique_ptr
#include <type_traits> // enable_if, is_const
#include <utility>     // forward

namespace wt {

class shape {
    // Shape's interface is implemented as hidden friends.

    template <typename S = shape>
    friend std::enable_if_t<not std::is_const_v<S>, tform4&> tform(shape& s) noexcept {
        return s.pimpl_->do_tform();
    }
    friend tform4 const& tform(shape const& s) noexcept { return s.pimpl_->do_tform(); }

    //    template <typename S = shape>
    //    friend std::enable_if_t<not std::is_const_v<S>, tform4&> inv_tform(shape& s) noexcept {
    //        return s.pimpl_->do_inv_tform();
    //    }
    friend tform4 const& inv_tform(shape const& s) noexcept { return s.pimpl_->do_inv_tform(); }

    template <typename S = shape>
    friend std::enable_if_t<not std::is_const_v<S>, material&> mater(shape& s) noexcept {
        return s.pimpl_->do_mater();
    }
    friend material const& mater(shape const& s) noexcept { return s.pimpl_->do_mater(); }

    friend void intersect(shape const& s, ray const& object_ray, unsigned shape_id,
                          std::vector<intersection>& world_isecs) noexcept {
        s.pimpl_->do_intersect(object_ray, std::move(shape_id), world_isecs);
    }

    class shape_concept {
      public:
        virtual ~shape_concept() = 0;

        // Shape's interface declaration. The real functions containing implementations are named
        // without the do_ prefix, but in order to facilitate ADL in finding the free functions
        // implementing the interface, the virtual interface functions must be named differently.

        //        virtual tform4& do_tform() = 0;
        [[nodiscard]] virtual tform4 const& do_tform() const noexcept = 0;

        //        virtual tform4& do_inv_tform() = 0;
        [[nodiscard]] virtual tform4 const& do_inv_tform() const noexcept = 0;

        //        virtual material& do_mater() = 0;
        [[nodiscard]] virtual material const& do_mater() const noexcept = 0;

        virtual void do_intersect(ray const& object_ray, unsigned shape_id,
                                  std::vector<intersection>& world_isecs) const noexcept = 0;

        // Make ShapeConcept's children copyable through ShapeConcept pointer.
        [[nodiscard]] virtual std::unique_ptr<shape_concept> clone() const = 0;

      protected:
        shape_concept() = default;
        shape_concept(shape_concept const&) = default;
        shape_concept(shape_concept&&) noexcept = default;
        shape_concept& operator=(shape_concept&&) noexcept = default;
        shape_concept& operator=(shape_concept const&) = default;
    };

    template <typename ConcreteShape> class shape_model final : public shape_concept {
      public:
        shape_model(ConcreteShape&& concrete_shape)
            // Copy-construct or move-construct the concrete type.
            : object_{std::forward<ConcreteShape>(concrete_shape)} {}

        template <typename C = ConcreteShape>
        std::enable_if_t<!std::is_const_v<C>, tform4&> do_tform() noexcept {
            return tform(object_);
        }
        [[nodiscard]] tform4 const& do_tform() const noexcept override { return tform(object_); }

        template <typename C = ConcreteShape>
        std::enable_if_t<!std::is_const_v<C>, tform4&> do_inv_tform() {
            return inv_tform(object_);
        }
        [[nodiscard]] tform4 const& do_inv_tform() const noexcept override {
            return inv_tform(object_);
        }

        template <typename C = ConcreteShape>
        std::enable_if_t<!std::is_const_v<C>, material&> do_mater() {
            return mater(object_);
        }
        [[nodiscard]] material const& do_mater() const noexcept override { return mater(object_); }

        void do_intersect(ray const& object_ray, unsigned shape_id,
                          std::vector<intersection>& world_isecs) const noexcept override {
            intersect(object_, object_ray, std::move(shape_id), world_isecs);
        }

        [[nodiscard]] std::unique_ptr<shape_concept> clone() const override {
            return std::make_unique<shape_model>(*this);
        }

      private:
        ConcreteShape object_;
    };

  public:
    template <typename ConcreteShape,
              // Make sure that templated forwarding constructor does not hide the copy constructor.
              // Another very important thing is to decay the ConcreteShape before comparison,
              // otherwise Shape& binds to this constructor.
              std::enable_if_t<!std::is_same_v<shape, std::decay_t<ConcreteShape>>, bool> = true>
    explicit shape(ConcreteShape&& concrete_shape)
        // Strip the reference, so that either a copy or a move occurs, but not reference binding.
        : pimpl_{std::make_unique<shape_model<std::remove_reference_t<ConcreteShape>>>(
              std::forward<std::remove_reference_t<ConcreteShape>>(concrete_shape))} {}

    // Deep copy of the concrete shape object.
    shape(shape const& other) : pimpl_{other.pimpl_->clone()} {}

    // Deep copy of the concrete shape object.
    shape& operator=(shape const& rhs) {
        if (&rhs != this) {
            pimpl_ = rhs.pimpl_->clone();
        }
        return *this;
    }

    // unique_ptr is movable by default.
    shape(shape&&) noexcept = default;
    shape& operator=(shape&&) noexcept = default;

    ~shape() = default;

  private:
    // Pointer to a ShapeModel, which has a member object of the concrete type.
    std::unique_ptr<shape_concept> pimpl_;
};

inline shape::shape_concept::~shape_concept() = default;

} // namespace wt
