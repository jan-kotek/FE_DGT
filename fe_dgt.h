// ---------------------------------------------------------------------
//
// Copyright (C) 2002 - 2017 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE at
// the top level of the deal.II distribution.
//
// ---------------------------------------------------------------------

#ifndef dealii__fe_dgt_h
#define dealii__fe_dgt_h

#include <deal.II/base/config.h>
#include <deal.II/base/polynomial.h>
#include <deal.II/base/polynomial_space.h>
#include <deal.II/fe/fe.h>
#include <deal.II/fe/mapping.h>

DEAL_II_NAMESPACE_OPEN


/*!@addtogroup fe */
/*@{*/

/**
 *
 * Discontinuous finite elements evaluated at the mapped quadrature points.
 *
 * T - Taylor
 *
 * Warning: this class does not work properly, yet. Don't use it!
 *
 * This finite element implements complete polynomial spaces, that is,
 * $d$-dimensional polynomials of order $k$.
 *
 * The polynomials are not mapped. Therefore, they are constant,
 * linear, quadratic, etc. on any grid cell.
 *
 * Since the polynomials are evaluated at the quadrature points of the
 * actual grid cell, no grid transfer and interpolation matrices are
 * available.
 *
 * The purpose of this class is experimental, therefore the
 * implementation will remain incomplete.
 *
 * Besides, this class is not implemented for the codimension one case
 * (<tt>spacedim != dim</tt>).
 *
 * This is derived from FE_DGPNonparametric written by Guido Kanschat but using Monomial
 * instead of Legendre, and including a shift and scaling. The monomials are evaluated at
 *                 <tt> ( point - cell->center() ) / cell->diameter() </tt>
 *
 * @author Praveen. C, 2013
 */
template <int dim, int spacedim=dim>
class FE_DGT : public FiniteElement<dim,spacedim>
{
public:
  /**
   * Constructor for tensor product
   * polynomials of degree @p k.
   */
  FE_DGT (const unsigned int k);

  /**
   * Return a string that uniquely
   * identifies a finite
   * element. This class returns
   * <tt>FE_DGT<dim>(degree)</tt>,
   * with @p dim and @p degree
   * replaced by appropriate
   * values.
   */
  virtual std::string get_name () const;
  
    // for documentation, see the FiniteElement base class
  virtual
  UpdateFlags
  requires_update_flags (const UpdateFlags update_flags) const;

  /**
   * Return the value of the
   * @p ith shape function at the
   * point @p p. See the
   * FiniteElement base
   * class for more information
   * about the semantics of this
   * function. This function is not
   * implemented.
   */
  virtual double shape_value (const unsigned int i,
                              const Point<dim> &p) const;

  /**
   * Return the value of the
   * @p ith shape function at the
   * point @p p. See the
   * FiniteElement base
   * class for more information
   * about the semantics of this
   * function.
   */
  double shape_value 
     (const typename Triangulation<dim,spacedim>::cell_iterator & cell,
      const unsigned int i,
      const Point<dim> &p) const;


  /**
   * Compute all shape function values at the specified points @p p.
   * @p values[i][q] is the @p i th shape function value at point @p p[q].
   */
  void
  shape_values (const typename Triangulation<dim,spacedim>::cell_iterator & cell,
                const std::vector< Point<dim> > &p,
                std::vector< std::vector<double> >& values) const;

  /**
   * Return the value of the
   * @p componentth vector
   * component of the @p ith shape
   * function at the point
   * @p p. See the
   * FiniteElement base
   * class for more information
   * about the semantics of this
   * function.
   *
   * Since this element is scalar,
   * the returned value is the same
   * as if the function without the
   * @p _component suffix were
   * called, provided that the
   * specified component is zero.
   * This function is not implemented.
   */
  virtual double shape_value_component (const unsigned int i,
                                        const Point<dim> &p,
                                        const unsigned int component) const;

  /**
   * Return the value of the
   * @p componentth vector
   * component of the @p ith shape
   * function at the point
   * @p p. See the
   * FiniteElement base
   * class for more information
   * about the semantics of this
   * function.
   *
   * Since this element is scalar,
   * the returned value is the same
   * as if the function without the
   * @p _component suffix were
   * called, provided that the
   * specified component is zero.
   */
  double shape_value_component 
     (const typename Triangulation<dim,spacedim>::cell_iterator & cell,
      const unsigned int i,
      const Point<dim> &p,
      const unsigned int component) const;

  /**
   * Return the gradient of the
   * @p ith shape function at the
   * point @p p. See the
   * FiniteElement base
   * class for more information
   * about the semantics of this
   * function. This function is not
   * implemented.
   */
  virtual Tensor<1,dim> shape_grad (const unsigned int  i,
                                    const Point<dim>   &p) const;

  /**
   * Return the gradient of the
   * @p ith shape function at the
   * point @p p. See the
   * FiniteElement base
   * class for more information
   * about the semantics of this
   * function.
   */
  Tensor<1,dim> shape_grad 
     (const typename Triangulation<dim,spacedim>::cell_iterator & cell,
      const unsigned int  i,
      const Point<dim>   &p) const;

  /**
   * Return the gradient of the
   * @p componentth vector
   * component of the @p ith shape
   * function at the point
   * @p p. See the
   * FiniteElement base
   * class for more information
   * about the semantics of this
   * function.
   *
   * Since this element is scalar,
   * the returned value is the same
   * as if the function without the
   * @p _component suffix were
   * called, provided that the
   * specified component is zero.
   * This function is not implemented.
   */
  virtual Tensor<1,dim> shape_grad_component (const unsigned int i,
                                              const Point<dim> &p,
                                              const unsigned int component) const;

  /**
   * Return the gradient of the
   * @p componentth vector
   * component of the @p ith shape
   * function at the point
   * @p p. See the
   * FiniteElement base
   * class for more information
   * about the semantics of this
   * function.
   *
   * Since this element is scalar,
   * the returned value is the same
   * as if the function without the
   * @p _component suffix were
   * called, provided that the
   * specified component is zero.
   */
  Tensor<1,dim> shape_grad_component 
     (const typename Triangulation<dim,spacedim>::cell_iterator & cell,
      const unsigned int i,
      const Point<dim> &p,
      const unsigned int component) const;

  /**
   * X Return the tensor of second
   * X derivatives of the @p ith
   * X shape function at point @p p
   * X on the unit cell.  See the
   * X FiniteElement base
   * X class for more information
   * X about the semantics of this
   * X function. This function is not
   * X implemented.
   */
  virtual Tensor<2,dim> shape_grad_grad (const unsigned int  i,
                                         const Point<dim> &p) const;

  /**
   * Return the tensor of second
   * derivatives of the @p ith
   * shape function at point @p p
   * on the unit cell.  See the
   * FiniteElement base
   * class for more information
   * about the semantics of this
   * function.
   */
  Tensor<2,dim> shape_grad_grad 
     (const typename Triangulation<dim,spacedim>::cell_iterator & cell,
      const unsigned int  i,
      const Point<dim> &p) const;

  /**
   * Return the second derivative
   * of the @p componentth vector
   * component of the @p ith shape
   * function at the point
   * @p p. See the
   * FiniteElement base
   * class for more information
   * about the semantics of this
   * function.
   *
   * Since this element is scalar,
   * the returned value is the same
   * as if the function without the
   * @p _component suffix were
   * called, provided that the
   * specified component is zero.
   * This function is not implemented.
   */
  virtual Tensor<2,dim> shape_grad_grad_component (const unsigned int i,
                                                   const Point<dim> &p,
                                                   const unsigned int component) const;

  /**
   * Return the second derivative
   * of the @p componentth vector
   * component of the @p ith shape
   * function at the point
   * @p p. See the
   * FiniteElement base
   * class for more information
   * about the semantics of this
   * function.
   *
   * Since this element is scalar,
   * the returned value is the same
   * as if the function without the
   * @p _component suffix were
   * called, provided that the
   * specified component is zero.
   */
  Tensor<2,dim> shape_grad_grad_component 
     (const typename Triangulation<dim,spacedim>::cell_iterator & cell,
      const unsigned int i,
      const Point<dim> &p,
      const unsigned int component) const;

  /**
   * Return the polynomial degree
   * of this finite element,
   * i.e. the value passed to the
   * constructor.
   */
  unsigned int get_degree () const;

  /**
   * Return the matrix
   * interpolating from a face of
   * of one element to the face of
   * the neighboring element.
   * The size of the matrix is
   * then <tt>source.dofs_per_face</tt> times
   * <tt>this->dofs_per_face</tt>.
   *
   * Derived elements will have to
   * implement this function. They
   * may only provide interpolation
   * matrices for certain source
   * finite elements, for example
   * those from the same family. If
   * they don't implement
   * interpolation from a given
   * element, then they must throw
   * an exception of type
   * FiniteElement<dim,spacedim>::ExcInterpolationNotImplemented.
   */
  virtual void
  get_face_interpolation_matrix (const FiniteElement<dim,spacedim> &source,
                                 FullMatrix<double>       &matrix) const;

  /**
   * Return the matrix
   * interpolating from a face of
   * of one element to the face of
   * the neighboring element.
   * The size of the matrix is
   * then <tt>source.dofs_per_face</tt> times
   * <tt>this->dofs_per_face</tt>.
   *
   * Derived elements will have to
   * implement this function. They
   * may only provide interpolation
   * matrices for certain source
   * finite elements, for example
   * those from the same family. If
   * they don't implement
   * interpolation from a given
   * element, then they must throw
   * an exception of type
   * FiniteElement<dim,spacedim>::ExcInterpolationNotImplemented.
   */
  virtual void
  get_subface_interpolation_matrix (const FiniteElement<dim,spacedim> &source,
                                    const unsigned int        subface,
                                    FullMatrix<double>       &matrix) const;

  /**
   * @name Functions to support hp
   * @{
   */

  /**
   * If, on a vertex, several finite elements are active, the hp code
   * first assigns the degrees of freedom of each of these FEs
   * different global indices. It then calls this function to find out
   * which of them should get identical values, and consequently can
   * receive the same global DoF index. This function therefore
   * returns a list of identities between DoFs of the present finite
   * element object with the DoFs of @p fe_other, which is a reference
   * to a finite element object representing one of the other finite
   * elements active on this particular vertex. The function computes
   * which of the degrees of freedom of the two finite element objects
   * are equivalent, both numbered between zero and the corresponding
   * value of dofs_per_vertex of the two finite elements. The first
   * index of each pair denotes one of the vertex dofs of the present
   * element, whereas the second is the corresponding index of the
   * other finite element.
   *
   * This being a discontinuous element, the set of such constraints
   * is of course empty.
   */
  virtual
  std::vector<std::pair<unsigned int, unsigned int> >
  hp_vertex_dof_identities (const FiniteElement<dim,spacedim> &fe_other) const;

  /**
   * Same as hp_vertex_dof_indices(), except that the function treats
   * degrees of freedom on lines.
   *
   * This being a discontinuous element, the set of such constraints
   * is of course empty.
   */
  virtual
  std::vector<std::pair<unsigned int, unsigned int> >
  hp_line_dof_identities (const FiniteElement<dim,spacedim> &fe_other) const;

  /**
   * Same as hp_vertex_dof_indices(), except that the function treats
   * degrees of freedom on quads.
   *
   * This being a discontinuous element, the set of such constraints
   * is of course empty.
   */
  virtual
  std::vector<std::pair<unsigned int, unsigned int> >
  hp_quad_dof_identities (const FiniteElement<dim,spacedim> &fe_other) const;

  /**
   * Return whether this element
   * implements its hanging node
   * constraints in the new way,
   * which has to be used to make
   * elements "hp compatible".
   *
   * For the FE_DGT class the
   * result is always true (independent of
   * the degree of the element), as it has
   * no hanging nodes (being a
   * discontinuous element).
   */
  virtual bool hp_constraints_are_implemented () const;

  /**
   * Return whether this element dominates
   * the one given as argument when they
   * meet at a common face,
   * whether it is the other way around,
   * whether neither dominates, or if
   * either could dominate.
   *
   * For a definition of domination, see
   * FiniteElementBase::Domination and in
   * particular the @ref hp_paper "hp paper".
   */
  virtual
  FiniteElementDomination::Domination
  compare_for_face_domination (const FiniteElement<dim,spacedim> &fe_other) const;

  /**
   * @}
   */

  /**
   * Check for non-zero values on a face.
   *
   * This function returns
   * @p true, if the shape
   * function @p shape_index has
   * non-zero values on the face
   * @p face_index.
   *
   * Implementation of the
   * interface in
   * FiniteElement
   */
  virtual bool has_support_on_face (const unsigned int shape_index,
                                    const unsigned int face_index) const;

  /**
   * Determine an estimate for the
   * memory consumption (in bytes)
   * of this object.
   *
   * This function is made virtual,
   * since finite element objects
   * are usually accessed through
   * pointers to their base class,
   * rather than the class itself.
   */
  virtual std::size_t memory_consumption () const;


private:
  /**
   * Declare a nested class which
   * will hold static definitions of
   * various matrices such as
   * constraint and embedding
   * matrices. The definition of
   * the various static fields are
   * in the files <tt>fe_dgp_[123]d.cc</tt>
   * in the source directory.
   */
  struct Matrices
  {
    /**
     * Pointers to the embedding
     * matrices, one for each
     * polynomial degree starting
     * from constant elements
     */
    static const double *const embedding[][GeometryInfo<dim>::max_children_per_cell];

    /**
     * Number of elements (first
     * index) the above field
     * has. Equals the highest
     * polynomial degree plus one
     * for which the embedding
     * matrices have been
     * computed.
     */
    static const unsigned int n_embedding_matrices;

    /**
     * As @p embedding but for
     * projection matrices.
     */
    static const double *const projection_matrices[][GeometryInfo<dim>::max_children_per_cell];

    /**
     * As
     * @p n_embedding_matrices
     * but for projection
     * matrices.
     */
    static const unsigned int n_projection_matrices;
  };

protected:

  /**
   * @p clone function instead of
   * a copy constructor.
   *
   * This function is needed by the
   * constructors of @p FESystem.
   */
  virtual FiniteElement<dim,spacedim> *clone() const;

  /**
   * Prepare internal data
   * structures and fill in values
   * independent of the cell.
   */
  virtual
  typename FiniteElement<dim,spacedim>::InternalDataBase *
  get_data (const UpdateFlags                                                    update_flags,
            const Mapping<dim,spacedim>                                         &mapping,
            const Quadrature<dim>                                               &quadrature,
            dealii::internal::FEValues::FiniteElementRelatedData<dim, spacedim> &output_data) const;

  virtual
  void
  fill_fe_values (const typename Triangulation<dim,spacedim>::cell_iterator           &cell,
                  const CellSimilarity::Similarity                                     cell_similarity,
                  const Quadrature<dim>                                               &quadrature,
                  const Mapping<dim,spacedim>                                         &mapping,
                  const typename Mapping<dim,spacedim>::InternalDataBase              &mapping_internal,
                  const dealii::internal::FEValues::MappingRelatedData<dim, spacedim> &mapping_data,
                  const typename FiniteElement<dim,spacedim>::InternalDataBase        &fe_internal,
                  dealii::internal::FEValues::FiniteElementRelatedData<dim, spacedim> &output_data) const;

  virtual
  void
  fill_fe_face_values (const typename Triangulation<dim,spacedim>::cell_iterator           &cell,
                       const unsigned int                                                   face_no,
                       const Quadrature<dim-1>                                             &quadrature,
                       const Mapping<dim,spacedim>                                         &mapping,
                       const typename Mapping<dim,spacedim>::InternalDataBase              &mapping_internal,
                       const dealii::internal::FEValues::MappingRelatedData<dim, spacedim> &mapping_data,
                       const typename FiniteElement<dim,spacedim>::InternalDataBase        &fe_internal,
                       dealii::internal::FEValues::FiniteElementRelatedData<dim, spacedim> &output_data) const;

  virtual
  void
  fill_fe_subface_values (const typename Triangulation<dim,spacedim>::cell_iterator           &cell,
                          const unsigned int                                                   face_no,
                          const unsigned int                                                   sub_no,
                          const Quadrature<dim-1>                                             &quadrature,
                          const Mapping<dim,spacedim>                                         &mapping,
                          const typename Mapping<dim,spacedim>::InternalDataBase              &mapping_internal,
                          const dealii::internal::FEValues::MappingRelatedData<dim, spacedim> &mapping_data,
                          const typename FiniteElement<dim,spacedim>::InternalDataBase        &fe_internal,
                          dealii::internal::FEValues::FiniteElementRelatedData<dim, spacedim> &output_data) const;

private:

  /**
   * Only for internal use. Its
   * full name is
   * @p get_dofs_per_object_vector
   * function and it creates the
   * @p dofs_per_object vector that is
   * needed within the constructor to
   * be passed to the constructor of
   * @p FiniteElementData.
   */
  static 
  std::vector<unsigned int> 
  get_dpo_vector (const unsigned int degree);

 
  /**
   * Pointer to an object
   * representing the polynomial
   * space used here.
   */
  const PolynomialSpace<dim> polynomial_space;


  /**
   * Allow access from other dimensions.
   */
  template <int, int> friend class FE_DGT;



};

/*@}*/

#ifndef DOXYGEN

// declaration of explicit specializations of member variables, if the
// compiler allows us to do that (the standard says we must)
#ifndef DEAL_II_MEMBER_VAR_SPECIALIZATION_BUG
template <>
const double *const FE_DGT<1,1>::Matrices::embedding[][GeometryInfo<1>::max_children_per_cell];

template <>
const unsigned int FE_DGT<1,1>::Matrices::n_embedding_matrices;

template <>
const double *const FE_DGT<1,1>::Matrices::projection_matrices[][GeometryInfo<1>::max_children_per_cell];

template <>
const unsigned int FE_DGT<1,1>::Matrices::n_projection_matrices;

template <>
const double *const FE_DGT<2,2>::Matrices::embedding[][GeometryInfo<2>::max_children_per_cell];

template <>
const unsigned int FE_DGT<2,2>::Matrices::n_embedding_matrices;

template <>
const double *const FE_DGT<2,2>::Matrices::projection_matrices[][GeometryInfo<2>::max_children_per_cell];

template <>
const unsigned int FE_DGT<2,2>::Matrices::n_projection_matrices;

template <>
const double *const FE_DGT<3,3>::Matrices::embedding[][GeometryInfo<3>::max_children_per_cell];

template <>
const unsigned int FE_DGT<3,3>::Matrices::n_embedding_matrices;

template <>
const double *const FE_DGT<3,3>::Matrices::projection_matrices[][GeometryInfo<3>::max_children_per_cell];

template <>
const unsigned int FE_DGT<3,3>::Matrices::n_projection_matrices;
#endif

#endif // DOXYGEN

DEAL_II_NAMESPACE_CLOSE

#endif
