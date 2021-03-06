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


#include <deal.II/base/quadrature.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/tria_iterator.h>
#include <deal.II/dofs/dof_accessor.h>
#include <deal.II/fe/fe.h>
#include <deal.II/fe/mapping.h>
#include <deal.II/fe/fe_dgt.h>
#include <deal.II/fe/fe_values.h>

#include <sstream>

DEAL_II_NAMESPACE_OPEN

template <int dim, int spacedim>
FE_DGT<dim,spacedim>::FE_DGT (const unsigned int degree)
  :
  FiniteElement<dim,spacedim> (
    FiniteElementData<dim>(get_dpo_vector(degree), 1, degree,
                           FiniteElementData<dim>::L2),
    std::vector<bool>(
      FiniteElementData<dim>(get_dpo_vector(degree), 1, degree).dofs_per_cell,true),
    std::vector<ComponentMask>(
      FiniteElementData<dim>(get_dpo_vector(degree),1, degree).dofs_per_cell,
      std::vector<bool>(1,true))),
  polynomial_space (Polynomials::Monomial<double>::generate_complete_basis(degree))
{
  const unsigned int n_dofs = this->dofs_per_cell;
  for (unsigned int ref_case = RefinementCase<dim>::cut_x;
       ref_case<RefinementCase<dim>::isotropic_refinement+1; ++ref_case)
    {
      if (dim!=2 && ref_case!=RefinementCase<dim>::isotropic_refinement)
        // do nothing, as anisotropic
        // refinement is not
        // implemented so far
        continue;

      const unsigned int nc = GeometryInfo<dim>::n_children(RefinementCase<dim>(ref_case));
      for (unsigned int i=0; i<nc; ++i)
        {
          this->prolongation[ref_case-1][i].reinit (n_dofs, n_dofs);
          // Fill prolongation matrices with
          // embedding operators
          for (unsigned int j=0; j<n_dofs; ++j)
            this->prolongation[ref_case-1][i](j,j) = 1.;
        }
    }

  // restriction can be defined
  // through projection for
  // discontinuous elements, but is
  // presently not implemented for DGT
  // elements.
  //
  // if it were, then the following
  // snippet would be the right code
//    if ((degree < Matrices::n_projection_matrices) &&
//        (Matrices::projection_matrices[degree] != 0))
//      {
//        restriction[0].fill (Matrices::projection_matrices[degree]);
//      }
//    else
//                                   // matrix undefined, set size to zero
//      for (unsigned int i=0;i<GeometryInfo<dim>::max_children_per_cell;++i)
//        restriction[i].reinit(0, 0);
  // since not implemented, set to
  // "empty". however, that is done in the
  // default constructor already, so do nothing
//  for (unsigned int i=0;i<GeometryInfo<dim>::max_children_per_cell;++i)
//    this->restriction[i].reinit(0, 0);

  // note further, that these
  // elements have neither support
  // nor face-support points, so
  // leave these fields empty
}



template <int dim, int spacedim>
std::string
FE_DGT<dim,spacedim>::get_name () const
{
  // note that the
  // FETools::get_fe_by_name
  // function depends on the
  // particular format of the string
  // this function returns, so they
  // have to be kept in synch

  std::ostringstream namebuf;
  namebuf << "FE_DGT<"
		  << Utilities::dim_string(dim,spacedim)
		  << ">(" << this->degree << ")";

  return namebuf.str();
}



template <int dim, int spacedim>
FiniteElement<dim,spacedim> *
FE_DGT<dim,spacedim>::clone() const
{
  return new FE_DGT<dim,spacedim>(*this);
}


template <int dim, int spacedim>
double
FE_DGT<dim,spacedim>::shape_value (const unsigned int i,
                                   const Point<dim> &p) const
{
  Assert (false, ExcNotImplemented());
  return 0;
}



template <int dim, int spacedim>
double
FE_DGT<dim,spacedim>::shape_value (const typename Triangulation<dim,spacedim>::cell_iterator & cell,
                                   const unsigned int i,
                                   const Point<dim> &p) const

{
  Assert (i<this->dofs_per_cell, ExcIndexRange(i, 0, this->dofs_per_cell));

  const Point<dim> pp = (Point<dim>)(p-cell->center())/cell->diameter();

  return polynomial_space.compute_value(i, pp);
}


template <int dim, int spacedim>
void
FE_DGT<dim,spacedim>::shape_values (const typename Triangulation<dim,spacedim>::cell_iterator & cell,
                                    const std::vector< Point<dim> > &p,
                                    std::vector< std::vector<double> >& values) const
{
  Assert (values.size()==this->dofs_per_cell, 
          ExcDimensionMismatch(values.size(), this->dofs_per_cell));
  Assert (values[0].size()==p.size(), 
          ExcDimensionMismatch(values[0].size(), p.size()));

  for(unsigned int q=0; q<p.size(); ++q)
  {
      const Point<dim> pp = (Point<dim>)(p[q] - cell->center()) / cell->diameter();
      for(unsigned int i=0; i<this->dofs_per_cell; ++i)
         values[i][q] = polynomial_space.compute_value(i, pp);
  }
}



template <int dim, int spacedim>
double
FE_DGT<dim,spacedim>::shape_value_component (const unsigned int i,
                                             const Point<dim> &p,
                                             const unsigned int component) const
{
  Assert (false, ExcNotImplemented());
  return 0;
}

template <int dim, int spacedim>
double
FE_DGT<dim,spacedim>::shape_value_component 
   (const typename Triangulation<dim,spacedim>::cell_iterator & cell,
    const unsigned int i,
    const Point<dim> &p,
    const unsigned int component) const
{
  (void)component;
  Assert (i<this->dofs_per_cell, ExcIndexRange(i, 0, this->dofs_per_cell));
  Assert (component == 0, ExcIndexRange (component, 0, 1));
  const Point<dim> pp =(Point<dim>) (p - cell->center()) / cell->diameter();
  return polynomial_space.compute_value(i, pp);
}



template <int dim, int spacedim>
Tensor<1,dim>
FE_DGT<dim,spacedim>::shape_grad (const unsigned int i,
                                  const Point<dim> &p) const
{
  Assert (false, ExcNotImplemented());
  return Tensor<1,dim>();
}

template <int dim, int spacedim>
Tensor<1,dim>
FE_DGT<dim,spacedim>::shape_grad (const typename Triangulation<dim,spacedim>::cell_iterator & cell,
                                  const unsigned int i,
                                  const Point<dim> &p) const
{
  Assert (i<this->dofs_per_cell, ExcIndexRange(i, 0, this->dofs_per_cell));
  const Point<dim> pp = (Point<dim>)(p - cell->center()) / cell->diameter();
  return polynomial_space.compute_grad(i, pp) / cell->diameter();
}


template <int dim, int spacedim>
Tensor<1,dim>
FE_DGT<dim,spacedim>::shape_grad_component (const unsigned int i,
                                            const Point<dim> &p,
                                            const unsigned int component) const
{
  Assert (false, ExcNotImplemented());
  return Tensor<1,dim>();
}

template <int dim, int spacedim>
Tensor<1,dim>
FE_DGT<dim,spacedim>::shape_grad_component 
   (const typename Triangulation<dim,spacedim>::cell_iterator & cell,
    const unsigned int i,
    const Point<dim> &p,
    const unsigned int component) const
{
  Assert (i<this->dofs_per_cell, ExcIndexRange(i, 0, this->dofs_per_cell));
  Assert (component == 0, ExcIndexRange (component, 0, 1));
  const Point<dim> pp =(Point<dim>) (p - cell->center()) / cell->diameter();
  return polynomial_space.compute_grad(i, pp) / cell->diameter();
}



template <int dim, int spacedim>
Tensor<2,dim>
FE_DGT<dim,spacedim>::shape_grad_grad (const unsigned int i,
                                       const Point<dim> &p) const
{
  Assert (false, ExcNotImplemented());
  return Tensor<2,dim>();
}

template <int dim, int spacedim>
Tensor<2,dim>
FE_DGT<dim,spacedim>::shape_grad_grad 
   (const typename Triangulation<dim,spacedim>::cell_iterator & cell,
    const unsigned int i,
   const Point<dim> &p) const
{
  Assert (i<this->dofs_per_cell, ExcIndexRange(i, 0, this->dofs_per_cell));
  const Point<dim> pp = (Point<dim>)(p - cell->center()) / cell->diameter();
  return polynomial_space.compute_grad_grad(i, pp) / cell->diameter() / cell->diameter();
}


template <int dim, int spacedim>
Tensor<2,dim>
FE_DGT<dim,spacedim>::shape_grad_grad_component (const unsigned int i,
    const Point<dim> &p,
    const unsigned int component) const
{
  Assert (false, ExcNotImplemented());
  return Tensor<2,dim>();
}

template <int dim, int spacedim>
Tensor<2,dim>
FE_DGT<dim,spacedim>::shape_grad_grad_component 
   (const typename Triangulation<dim,spacedim>::cell_iterator & cell,
    const unsigned int i,
    const Point<dim> &p,
    const unsigned int component) const
{
  Assert (i<this->dofs_per_cell, ExcIndexRange(i, 0, this->dofs_per_cell));
  Assert (component == 0, ExcIndexRange (component, 0, 1));
  const Point<dim> pp = (Point<dim>)(p - cell->center()) / cell->diameter();
  return polynomial_space.compute_grad_grad(i, pp) / cell->diameter() / cell->diameter();
}


//---------------------------------------------------------------------------
// Auxiliary functions
//---------------------------------------------------------------------------


template <int dim, int spacedim>
std::vector<unsigned int>
FE_DGT<dim,spacedim>::get_dpo_vector (const unsigned int deg)
{
  std::vector<unsigned int> dpo(dim+1, static_cast<unsigned int>(0));
  dpo[dim] = deg+1;
  for (unsigned int i=1; i<dim; ++i)
    {
      dpo[dim] *= deg+1+i;
      dpo[dim] /= i+1;
    }
  return dpo;
}

template <int dim, int spacedim>
UpdateFlags
FE_DGT<dim,spacedim>::requires_update_flags (const UpdateFlags flags) const
{
   UpdateFlags out = flags;

  if (flags & (update_values | update_gradients | update_hessians))
    out |= update_quadrature_points ;

  return out;
}


//---------------------------------------------------------------------------
// Data field initialization
//---------------------------------------------------------------------------

template <int dim, int spacedim>
typename FiniteElement<dim,spacedim>::InternalDataBase *
FE_DGT<dim,spacedim>::
get_data (const UpdateFlags                                                    update_flags,
          const Mapping<dim,spacedim> &,
          const Quadrature<dim> &,
          dealii::internal::FEValues::FiniteElementRelatedData<dim, spacedim> & ) const
{
  // generate a new data object
  typename FiniteElement<dim,spacedim>::InternalDataBase *data
    = new typename FiniteElement<dim,spacedim>::InternalDataBase;
  data->update_each = requires_update_flags(update_flags);

  // other than that, there is nothing we can add here as discussed
  // in the general documentation of this class

  
  return data;
}



//---------------------------------------------------------------------------
// Fill data of FEValues
//---------------------------------------------------------------------------

template <int dim, int spacedim>
void
FE_DGT<dim,spacedim>::
fill_fe_values (const typename Triangulation<dim,spacedim>::cell_iterator & cell,
                const CellSimilarity::Similarity,
                const Quadrature<dim> &,
                const Mapping<dim,spacedim> &,
                const typename Mapping<dim,spacedim>::InternalDataBase &,
                const dealii::internal::FEValues::MappingRelatedData<dim, spacedim> &mapping_data,
                const typename FiniteElement<dim,spacedim>::InternalDataBase        &fe_internal,
                dealii::internal::FEValues::FiniteElementRelatedData<dim, spacedim> &output_data) const
{
  Assert (fe_internal.update_each & update_quadrature_points, ExcInternalError());

  const unsigned int n_q_points = mapping_data.quadrature_points.size();
  
  std::vector<double> values(fe_internal.update_each & update_values ? this->dofs_per_cell : 0);
  std::vector<Tensor<1,dim> > grads(fe_internal.update_each & update_gradients ? this->dofs_per_cell : 0);
  std::vector<Tensor<2,dim> > grad_grads(fe_internal.update_each & update_hessians ? this->dofs_per_cell : 0);
  std::vector<Tensor<3,dim> > empty_vector_of_3rd_order_tensors; //not used
  std::vector<Tensor<4,dim> > empty_vector_of_4th_order_tensors; 
  
  
  double h = cell->diameter();

  if (fe_internal.update_each & (update_values | update_gradients))
    for (unsigned int i=0; i<n_q_points; ++i)
      {
        const Point<dim> p = (Point<dim>)(mapping_data.quadrature_points[i] - cell->center())/h;
        polynomial_space.compute(p, //mapping_data.quadrature_points[i],
                                 values, grads, grad_grads,
                                 empty_vector_of_3rd_order_tensors,
                                 empty_vector_of_4th_order_tensors);
        if (fe_internal.update_each & update_values)
          for (unsigned int k=0; k<this->dofs_per_cell; ++k)
            output_data.shape_values[k][i] = values[k];

        if (fe_internal.update_each & update_gradients)
          for (unsigned int k=0; k<this->dofs_per_cell; ++k)
            output_data.shape_gradients[k][i] = grads[k]/h;

        if (fe_internal.update_each & update_hessians)
          for (unsigned int k=0; k<this->dofs_per_cell; ++k)
            output_data.shape_hessians[k][i] = grad_grads[k]/h/h;
      }
}



template <int dim, int spacedim>
void
FE_DGT<dim,spacedim>::
fill_fe_face_values (const typename Triangulation<dim,spacedim>::cell_iterator & cell,
                     const unsigned int                                                   ,
                     const Quadrature<dim-1>                                             &,
                     const Mapping<dim,spacedim> &,
                     const typename Mapping<dim,spacedim>::InternalDataBase &,
                     const dealii::internal::FEValues::MappingRelatedData<dim, spacedim> &mapping_data,
                     const typename FiniteElement<dim,spacedim>::InternalDataBase        &fe_internal,
                     dealii::internal::FEValues::FiniteElementRelatedData<dim, spacedim> &output_data) const
{
 



  Assert (fe_internal.update_each & update_quadrature_points, ExcInternalError());

  const unsigned int n_q_points = mapping_data.quadrature_points.size();
  
 std::vector<double> values(fe_internal.update_each & update_values ? this->dofs_per_cell : 0);
 std::vector<Tensor<1,dim> > grads(fe_internal.update_each & update_gradients ? this->dofs_per_cell : 0);
 std::vector<Tensor<2,dim> > grad_grads(fe_internal.update_each & update_hessians ? this->dofs_per_cell : 0);
 std::vector<Tensor<3,dim> > empty_vector_of_3rd_order_tensors; 
 std::vector<Tensor<4,dim> > empty_vector_of_4th_order_tensors;
 
  double h = cell->diameter();

  if (fe_internal.update_each & (update_values | update_gradients))
    for (unsigned int i=0; i<n_q_points; ++i)
      {
        const Point<dim> p =(Point<dim>) (mapping_data.quadrature_points[i] - cell->center())/h;
        polynomial_space.compute(p, //mapping_data.quadrature_points[i],
                                 values, grads, grad_grads,
                                 empty_vector_of_3rd_order_tensors,
                                 empty_vector_of_4th_order_tensors);
        if (fe_internal.update_each & update_values)
          for (unsigned int k=0; k<this->dofs_per_cell; ++k)
            output_data.shape_values[k][i] = values[k];

        if (fe_internal.update_each & update_gradients)
          for (unsigned int k=0; k<this->dofs_per_cell; ++k)
            output_data.shape_gradients[k][i] = grads[k]/h;

        if (fe_internal.update_each & update_hessians)
          for (unsigned int k=0; k<this->dofs_per_cell; ++k)
            output_data.shape_hessians[k][i] = grad_grads[k]/h/h;
      }
}



template <int dim, int spacedim>
void
FE_DGT<dim,spacedim>::
fill_fe_subface_values (const typename Triangulation<dim,spacedim>::cell_iterator & cell,
                        const unsigned int                                                   ,
                        const unsigned int                                                   ,
                        const Quadrature<dim-1>                                             &,
                        const Mapping<dim,spacedim> &,
                        const typename Mapping<dim,spacedim>::InternalDataBase &,
                        const dealii::internal::FEValues::MappingRelatedData<dim, spacedim> &mapping_data,
                        const typename FiniteElement<dim,spacedim>::InternalDataBase        &fe_internal,
                        dealii::internal::FEValues::FiniteElementRelatedData<dim, spacedim> &output_data) const
{
  Assert (fe_internal.update_each & update_quadrature_points, ExcInternalError());

  const unsigned int n_q_points = mapping_data.quadrature_points.size();

  std::vector<double> values(fe_internal.update_each & update_values ? this->dofs_per_cell : 0);
  std::vector<Tensor<1,dim> > grads(fe_internal.update_each & update_gradients ? this->dofs_per_cell : 0);
  std::vector<Tensor<2,dim> > grad_grads(fe_internal.update_each & update_hessians ? this->dofs_per_cell : 0);
  std::vector<Tensor<3,dim> > empty_vector_of_3rd_order_tensors;
  std::vector<Tensor<4,dim> > empty_vector_of_4th_order_tensors;
  
  double h = cell->diameter(); 

  if (fe_internal.update_each & (update_values | update_gradients))
    for (unsigned int i=0; i<n_q_points; ++i)
      {
        const Point<dim> p = (Point<dim>)(mapping_data.quadrature_points[i] - cell->center())/h;
        polynomial_space.compute(p, //mapping_data.quadrature_points[i],
                                 values, grads, grad_grads,
                                 empty_vector_of_3rd_order_tensors,
                                 empty_vector_of_4th_order_tensors);
        if (fe_internal.update_each & update_values)
          for (unsigned int k=0; k<this->dofs_per_cell; ++k)
            output_data.shape_values[k][i] = values[k];

        if (fe_internal.update_each & update_gradients)
          for (unsigned int k=0; k<this->dofs_per_cell; ++k)
            output_data.shape_gradients[k][i] = grads[k]/h;

        if (fe_internal.update_each & update_hessians)
          for (unsigned int k=0; k<this->dofs_per_cell; ++k)
            output_data.shape_hessians[k][i] = grad_grads[k]/h/h;//might want to add higher derivatives
      }
}



template <int dim, int spacedim>
void
FE_DGT<dim,spacedim>::
get_face_interpolation_matrix (const FiniteElement<dim,spacedim> &x_source_fe,
                               FullMatrix<double>       &interpolation_matrix) const
{
  // this is only implemented, if the source
  // FE is also a DGPNonparametric element. in that case,
  // both elements have no dofs on their
  // faces and the face interpolation matrix
  // is necessarily empty -- i.e. there isn't
  // much we need to do here.
  typedef              FiniteElement<dim,spacedim> FEE;
  AssertThrow ((x_source_fe.get_name().find ("FE_DGT<") == 0)
               ||
               (dynamic_cast<const FE_DGT<dim,spacedim>*>(&x_source_fe) != 0),
               typename FEE::
               ExcInterpolationNotImplemented());

  Assert (interpolation_matrix.m() == 0,
          ExcDimensionMismatch (interpolation_matrix.m(),
                                0));
  Assert (interpolation_matrix.n() == 0,
          ExcDimensionMismatch (interpolation_matrix.n(),
                                0));
}



template <int dim, int spacedim>
void
FE_DGT<dim,spacedim>::
get_subface_interpolation_matrix (const FiniteElement<dim,spacedim> &x_source_fe,
                                  const unsigned int ,
                                  FullMatrix<double>           &interpolation_matrix) const
{
  // this is only implemented, if the source
  // FE is also a DGPNonparametric element. in that case,
  // both elements have no dofs on their
  // faces and the face interpolation matrix
  // is necessarily empty -- i.e. there isn't
  // much we need to do here.
  typedef              FiniteElement<dim,spacedim> FEE;
  AssertThrow ((x_source_fe.get_name().find ("FE_DGT<") == 0)
               ||
               (dynamic_cast<const FE_DGT<dim,spacedim>*>(&x_source_fe) != 0),
               typename FEE::
               ExcInterpolationNotImplemented());

  Assert (interpolation_matrix.m() == 0,
          ExcDimensionMismatch (interpolation_matrix.m(),
                                0));
  Assert (interpolation_matrix.n() == 0,
          ExcDimensionMismatch (interpolation_matrix.n(),
                                0));
}



template <int dim, int spacedim>
bool
FE_DGT<dim,spacedim>::hp_constraints_are_implemented () const
{
  return true;
}



template <int dim, int spacedim>
std::vector<std::pair<unsigned int, unsigned int> >
FE_DGT<dim,spacedim>::
hp_vertex_dof_identities (const FiniteElement<dim,spacedim> &fe_other) const
{
  // there are no such constraints for DGPNonparametric
  // elements at all
  if (dynamic_cast<const FE_DGT<dim,spacedim>*>(&fe_other) != 0)
    return
      std::vector<std::pair<unsigned int, unsigned int> > ();
  else
    {
      Assert (false, ExcNotImplemented());
      return std::vector<std::pair<unsigned int, unsigned int> > ();
    }
}



template <int dim, int spacedim>
std::vector<std::pair<unsigned int, unsigned int> >
FE_DGT<dim,spacedim>::
hp_line_dof_identities (const FiniteElement<dim,spacedim> &fe_other) const
{
  // there are no such constraints for DGPNonparametric
  // elements at all
  if (dynamic_cast<const FE_DGT<dim,spacedim>*>(&fe_other) != 0)
    return
      std::vector<std::pair<unsigned int, unsigned int> > ();
  else
    {
      Assert (false, ExcNotImplemented());
      return std::vector<std::pair<unsigned int, unsigned int> > ();
    }
}



template <int dim, int spacedim>
std::vector<std::pair<unsigned int, unsigned int> >
FE_DGT<dim,spacedim>::
hp_quad_dof_identities (const FiniteElement<dim,spacedim>        &fe_other) const
{
  // there are no such constraints for DGPNonparametric
  // elements at all
  if (dynamic_cast<const FE_DGT<dim,spacedim>*>(&fe_other) != 0)
    return
      std::vector<std::pair<unsigned int, unsigned int> > ();
  else
    {
      Assert (false, ExcNotImplemented());
      return std::vector<std::pair<unsigned int, unsigned int> > ();
    }
}



template <int dim, int spacedim>
FiniteElementDomination::Domination
FE_DGT<dim,spacedim>::
compare_for_face_domination (const FiniteElement<dim,spacedim> &fe_other) const
{
  // check whether both are discontinuous
  // elements, see the description of
  // FiniteElementDomination::Domination
  if (dynamic_cast<const FE_DGT<dim,spacedim>*>(&fe_other) != 0)
    return FiniteElementDomination::no_requirements;

  Assert (false, ExcNotImplemented());
  return FiniteElementDomination::neither_element_dominates;
}



template <int dim, int spacedim>
bool
FE_DGT<dim,spacedim>::has_support_on_face (const unsigned int,
                                                        const unsigned int) const
{
  return true;
}



template <int dim, int spacedim>
std::size_t
FE_DGT<dim,spacedim>::memory_consumption () const
{
  Assert (false, ExcNotImplemented ());
  return 0;
}



template <int dim, int spacedim>
unsigned int
FE_DGT<dim,spacedim>::get_degree () const
{
  return this->degree;
}



// explicit instantiations
#include "fe_dgt.inst"


DEAL_II_NAMESPACE_CLOSE
