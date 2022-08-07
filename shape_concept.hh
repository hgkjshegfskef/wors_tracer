//#pragma once
//
//#include "shape.hh"
//
//#include <array>
//#include <memory>
//
//namespace wt {
//
//struct intersection;
//struct ray;
//struct vec3;
//struct pnt3;
//
//class shape::shape_concept {
//  public:
//    virtual ~shape_concept() noexcept = 0;
//
//    // shape's interface declaration. The real functions containing implementations are named
//    // without the do_ prefix, but in order to facilitate ADL in finding the free functions
//    // implementing the interface, the virtual interface functions must be named differently.
//
//    virtual tform4& do_get_tform() noexcept = 0;
//    virtual tform4 const& do_get_tform() const noexcept = 0;
//
//    virtual material& do_get_material() noexcept = 0;
//    virtual material const& do_get_material() const noexcept = 0;
//
//    virtual vec3 do_normal_at(pnt3 const& world_point) const noexcept = 0;
//    virtual std::array<intersection, 2> do_intersect(ray const& world_ray) const noexcept = 0;
//
//    // Make shape_concept's children copyable through shape_concept pointer.
//    [[nodiscard]] virtual std::unique_ptr<shape_concept> clone() const noexcept = 0;
//
//  protected:
//    shape_concept() noexcept;
//    shape_concept(shape_concept const&) noexcept;
//    shape_concept(shape_concept&&) noexcept;
//    shape_concept& operator=(shape_concept const&) noexcept;
//    shape_concept& operator=(shape_concept&&) noexcept;
//};
//
//} // namespace wt
