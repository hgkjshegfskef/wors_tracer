#pragma once

// #include "material.hh"
// #include "tform4.hh"

#include <array>       // array
#include <memory>      // unique_ptr
#include <type_traits> // is_same_v, decay_t, remove_reference_t

//    tform4 tform;
//    material mat;
//
//    virtual ~shape() noexcept = 0;
//    shape() noexcept;
//    shape(tform4 tform) noexcept;
//    shape(tform4 tform, struct material material) noexcept;

namespace wt {

struct vec3;
struct pnt3;
struct intersection;
struct ray;
class tform4;
struct material;
struct sphere;

class shape {
    friend tform4& get_tform(shape& shape) noexcept;
    friend tform4 const& get_tform(shape const& shape) noexcept;

    friend material& get_mat(shape& shape) noexcept;
    friend material const& get_mat(shape const& shape) noexcept;

    friend vec3 normal_at(shape const& shape, pnt3 const& world_point) noexcept;
    friend std::array<intersection, 2> intersect(shape const& shape, ray const& world_ray) noexcept;

    class shape_concept;
    template <typename ConcreteShape> class shape_model;

  public:
    template <typename ConcreteShape,
              // Make sure that templated forwarding constructor does not hide the copy constructor.
              // Another very important thing is to decay the ConcreteShape before comparison,
              // otherwise shape& binds to this constructor.
              std::enable_if_t<!std::is_same_v<shape, std::decay_t<ConcreteShape>>, bool> = true>
    explicit shape(ConcreteShape&& concrete_shape);

    // Deep copy of the concrete shape object.
    shape(shape const& other) noexcept;

    // Deep copy of the concrete shape object.
    shape& operator=(shape const& rhs) noexcept;

    // unique_ptr is movable by default.
    shape(shape&&) noexcept;
    shape& operator=(shape&&) noexcept;

    // rule of 5.
    ~shape() noexcept;

  private:
    std::unique_ptr<shape_concept> pimpl_;
};

// extern template shape::shape(sphere&);
extern template shape::shape(sphere&&);

} // namespace wt
