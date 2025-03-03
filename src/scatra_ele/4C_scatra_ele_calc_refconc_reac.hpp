// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef FOUR_C_SCATRA_ELE_CALC_REFCONC_REAC_HPP
#define FOUR_C_SCATRA_ELE_CALC_REFCONC_REAC_HPP

#include "4C_config.hpp"

#include "4C_scatra_ele_calc_advanced_reaction.hpp"

FOUR_C_NAMESPACE_OPEN


namespace Discret
{
  namespace Elements
  {
    template <Core::FE::CellType distype>
    class ScaTraEleCalcRefConcReac : public ScaTraEleCalcAdvReac<distype>
    {
     private:
      /// private constructor, since we are a Singleton.
      ScaTraEleCalcRefConcReac(
          const int numdofpernode, const int numscal, const std::string& disname);

      typedef ScaTraEleCalc<distype> my;
      typedef ScaTraEleCalcAdvReac<distype> advreac;
      using my::nen_;
      using my::nsd_;

     public:
      /// Singleton access method
      static ScaTraEleCalcRefConcReac<distype>* instance(
          const int numdofpernode, const int numscal, const std::string& disname);

     protected:
      //! Set reac. body force, reaction coefficient and derivatives
      void set_advanced_reaction_terms(const int k,                  //!< index of current scalar
          const std::shared_ptr<Mat::MatListReactions> matreaclist,  //!< index of current scalar
          const double* gpcoord  //!< current Gauss-point coordinates
          ) override;

      //! calculation of convective element matrix: add conservative contributions
      void calc_mat_conv_add_cons(
          Core::LinAlg::SerialDenseMatrix& emat,  //!< element matrix to be filled
          const int k,                            //!< index of current scalar
          const double timefacfac,  //!< domain-integration factor times time-integration factor
          const double vdiv,        //!< velocity divergence
          const double densnp       //!< density at time_(n+1)
          ) override;

      //! set internal variables
      void set_internal_variables_for_mat_and_rhs() override;

      //! calculation of diffusive element matrix
      void calc_mat_diff(Core::LinAlg::SerialDenseMatrix& emat,  //!< element matrix to be filled
          const int k,                                           //!< index of current scalar
          const double timefacfac  //!< domain-integration factor times time-integration factor
          ) override;

      //! calculate the Laplacian (weak form)
      void get_laplacian_weak_form(double& val,                //!< ?
          const Core::LinAlg::Matrix<nsd_, nsd_>& difftensor,  //!< ?
          const int vi,                                        //!< ?
          const int ui                                         //!< ?
      )
      {
        val = 0.0;
        for (unsigned j = 0; j < nsd_; j++)
        {
          for (unsigned i = 0; i < nsd_; i++)
          {
            val += my::derxy_(j, vi) * difftensor(j, i) * my::derxy_(i, ui);
          }
        }
        return;
      };

      //! standard Galerkin diffusive term on right hand side
      void calc_rhs_diff(Core::LinAlg::SerialDenseVector& erhs,  //!< element vector to be filled
          const int k,                                           //!< index of current scalar
          const double rhsfac  //!< time-integration factor for rhs times domain-integration factor
          ) override;

      //! calculate the Laplacian (weak form)
      void get_laplacian_weak_form_rhs(double& val,            //!< ?
          const Core::LinAlg::Matrix<nsd_, nsd_>& difftensor,  //!< ?
          const Core::LinAlg::Matrix<nsd_, 1>& gradphi,        //!< ?
          const int vi                                         //!< ?
      )
      {
        val = 0.0;
        for (unsigned j = 0; j < nsd_; j++)
        {
          for (unsigned i = 0; i < nsd_; i++)
          {
            val += my::derxy_(j, vi) * difftensor(j, i) * gradphi(i);
          }
        }
        return;
      };

      // add nodal displacements to point coordinates
      void update_node_coordinates() override
      { /*nothing to to since we want reference coordinates*/ return; };

     private:
      /// determinante of deformation gradient
      double j_;

      /// inverse of cauchy-green deformation gradient
      Core::LinAlg::Matrix<nsd_, nsd_> c_inv_;

      /// derivative dJ/dX by finite differences
      Core::LinAlg::Matrix<nsd_, 1> d_jd_x_;

    };  // end ScaTraEleCalcRefConcReac


  }  // namespace Elements

}  // namespace Discret


FOUR_C_NAMESPACE_CLOSE

#endif
