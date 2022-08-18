#pragma once

#include "material.hh"
#include "tform4.hh"

#include <memory>      // unique_ptr
#include <type_traits> // is_same_v, decay_t, remove_reference_t
#include <vector>

namespace wt {

struct vec3;
struct pnt3;
struct intersection;
struct ray;
class tform4;
struct material;

class shape {
    class ShapeConcept {
      public:
        virtual ~ShapeConcept() noexcept = 0;

        // shape's interface declaration. The real functions containing implementations are named
        // without the do_ prefix, but in order to facilitate ADL in finding the free functions
        // implementing the interface, the virtual interface functions must be named differently.

        // XXX: cannot const propagate, that is, have non-const versions of functions

        //        virtual tform4& get_tform() noexcept = 0;
        //        virtual tform4& get_inv_tform() noexcept = 0;
        //        virtual material& get_material() noexcept = 0;

        virtual tform4 const& get_tform() const noexcept = 0;
        virtual tform4 const& get_inv_tform() const noexcept = 0;
        virtual material const& get_material() const noexcept = 0;

        //        virtual void set_tform(tform4 tform) noexcept = 0;
        //        virtual void set_inv_tform(tform4 inv_tform) noexcept = 0;
        //        virtual void set_material(material material) noexcept = 0;

        virtual void intersect(ray const& object_ray, unsigned shape_id,
                               std::vector<intersection>& world_isecs) const noexcept = 0;

        // Make shape_concept's children copyable through shape_concept pointer.
        [[nodiscard]] virtual std::unique_ptr<ShapeConcept> clone() noexcept = 0;

      protected:
        ShapeConcept() noexcept = default;
        ShapeConcept(ShapeConcept const&) noexcept = default;
        ShapeConcept(ShapeConcept&&) noexcept = default;
        ShapeConcept& operator=(ShapeConcept const&) noexcept = default;
        ShapeConcept& operator=(ShapeConcept&&) noexcept = default;
    };

    //    class ConstShapeConcept {
    //      public:
    //        virtual ~ConstShapeConcept() noexcept = 0;
    //
    //        // shape's interface declaration. The real functions containing implementations are
    //        named
    //        // without the do_ prefix, but in order to facilitate ADL in finding the free
    //        functions
    //        // implementing the interface, the virtual interface functions must be named
    //        differently.
    //
    //        // XXX: cannot const propagate, that is, have non-const versions of functions
    //
    //        virtual tform4 const& get_tform() const noexcept = 0;
    //        virtual tform4 const& get_inv_tform() const noexcept = 0;
    //        virtual material const& get_material() const noexcept = 0;
    //
    //        //        virtual void set_tform(tform4 tform) noexcept = 0;
    //        //        virtual void set_inv_tform(tform4 inv_tform) noexcept = 0;
    //        //        virtual void set_material(material material) noexcept = 0;
    //        //
    //        //        virtual void intersect(ray const& object_ray, unsigned shape_id,
    //        //                               std::vector<intersection>& world_isecs) const
    //        noexcept = 0;
    //
    //        //        // Make shape_concept's children copyable through shape_concept pointer.
    //        //        [[nodiscard]] virtual std::unique_ptr<ConstShapeConcept> clone() noexcept =
    //        0;
    //
    //      protected:
    //        ConstShapeConcept() noexcept = default;
    //        ConstShapeConcept(ConstShapeConcept const&) noexcept = default;
    //        ConstShapeConcept(ConstShapeConcept&&) noexcept = default;
    //        ConstShapeConcept& operator=(ConstShapeConcept const&) noexcept = default;
    //        ConstShapeConcept& operator=(ConstShapeConcept&&) noexcept = default;
    //    };

    // Adapt ConcreteShape to the Concept
    template <typename ConcreteShape> class Model final : public ShapeConcept {
      public:
        Model(ConcreteShape&& concrete_shape) noexcept
            // Copy-construct or move-construct the concrete type.
            : object_{std::forward<ConcreteShape>(concrete_shape)} {}

        tform4 const& get_tform() const noexcept override { return object_.get_tform(); }
        tform4 const& get_inv_tform() const noexcept override { return object_.get_inv_tform(); }
        material const& get_material() const noexcept override { return object_.get_material(); }

        template <typename CS = ConcreteShape>
        std::enable_if_t<!std::is_const_v<CS>, tform4&> get_tform() noexcept {
            return object_.get_tform();
        }

        template <typename CS = ConcreteShape>
        std::enable_if_t<!std::is_const_v<CS>, tform4&> get_inv_tform() noexcept {
            return object_.get_inv_tform();
        }
        template <typename CS = ConcreteShape>
        std::enable_if_t<!std::is_const_v<CS>, material&> get_material() noexcept {
            return object_.get_material();
        }

        //        void set_tform(tform4 tform) noexcept override {
        //        object_.set_tform(std::move(tform)); } void set_inv_tform(tform4 inv_tform)
        //        noexcept override {
        //            object_.set_inv_tform(std::move(inv_tform));
        //        }
        //        void set_material(material material) noexcept override {
        //            object_.set_material(std::move(material));
        //        }

        void intersect(ray const& object_ray, unsigned shape_id,
                       std::vector<intersection>& world_isecs) const noexcept override {
            return object_.intersect(object_ray, std::move(shape_id), world_isecs);
        }

        std::unique_ptr<ShapeConcept> clone() noexcept override {
            return std::make_unique<Model>(*this);
        }

      private:
        ConcreteShape object_;
    };

  public:
    tform4 const& get_tform() const noexcept { return pimpl_->get_tform(); }
    tform4 const& get_inv_tform() const noexcept { return pimpl_->get_inv_tform(); }
    material const& get_material() const noexcept { return pimpl_->get_material(); }

    template <typename T>
    std::enable_if_t<!std::is_const_v<T>, tform4&> get_tform() noexcept {
        return pimpl_->get_tform();
    }
    tform4& get_inv_tform() noexcept { return pimpl_->get_inv_tform(); }
    material& get_material() noexcept { return pimpl_->get_material(); }

    //    void set_tform(tform4 tform) noexcept { return pimpl_->set_tform(std::move(tform)); }
    //    void set_inv_tform(tform4 inv_tform) noexcept {
    //        return pimpl_->set_inv_tform(std::move(inv_tform));
    //    }
    //    void set_material(material material) noexcept {
    //        return pimpl_->set_material(std::move(material));
    //    }

    void intersect(ray const& object_ray, unsigned shape_id,
                   std::vector<intersection>& world_isecs) const noexcept {
        return pimpl_->intersect(object_ray, std::move(shape_id), world_isecs);
    }

    template <typename ConcreteShape,
              std::enable_if_t<not std::is_same_v<shape, std::decay_t<ConcreteShape>>, bool> = true>
    explicit shape(std::remove_cvref_t<ConcreteShape&&> concrete_shape)
        // Strip the reference, so that either a copy or a move occurs, but not reference binding.
        : pimpl_{std::make_unique<Model<std::remove_cvref_t<ConcreteShape>>>(
              std::forward<std::remove_cvref_t<ConcreteShape>>(concrete_shape))} {}

    // Deep copy of the concrete shape object.
    shape(const shape& other) noexcept : pimpl_{other.pimpl_->clone()} {}

    // Deep copy of the concrete shape object.
    shape& operator=(const shape& rhs) noexcept {
        if (&rhs != this) {
            pimpl_ = rhs.pimpl_->clone();
        }
        return *this;
    }

    // unique_ptr is movable by default.
    shape(shape&&) noexcept = default;
    shape& operator=(shape&&) noexcept = default;

    // rule of 5.
    ~shape() noexcept = default;

  private:
    std::unique_ptr<ShapeConcept> pimpl_;
};

inline shape::ShapeConcept::~ShapeConcept() noexcept = default;

} // namespace wt
