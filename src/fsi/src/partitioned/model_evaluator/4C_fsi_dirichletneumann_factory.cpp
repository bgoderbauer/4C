// This file is part of 4C multiphysics licensed under the
// GNU Lesser General Public License v3.0 or later.
//
// See the LICENSE.md file in the top-level for license information.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "4C_fsi_dirichletneumann_factory.hpp"

#include "4C_fsi_dirichletneumann.hpp"
#include "4C_fsi_dirichletneumann_disp.hpp"
#include "4C_fsi_dirichletneumann_vel.hpp"
#include "4C_fsi_dirichletneumann_volcoupl.hpp"
#include "4C_fsi_dirichletneumannslideale.hpp"
#include "4C_global_data.hpp"
#include "4C_inpar_fsi.hpp"

#include <Teuchos_StandardParameterEntryValidators.hpp>

FOUR_C_NAMESPACE_OPEN

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
std::shared_ptr<FSI::DirichletNeumann> FSI::DirichletNeumannFactory::create_algorithm(
    MPI_Comm comm, const Teuchos::ParameterList& fsidyn)
{
  const Teuchos::ParameterList& fsipart = fsidyn.sublist("PARTITIONED SOLVER");
  auto method =
      Teuchos::getIntegralValue<Inpar::FSI::PartitionedCouplingMethod>(fsipart, "PARTITIONED");
  switch (method)
  {
    case Inpar::FSI::DirichletNeumannSlideale:
      switch (Global::Problem::instance()->get_problem_type())
      {
        case (Core::ProblemType::fsi):
        case (Core::ProblemType::fsi_redmodels):
          if (Teuchos::getIntegralValue<Inpar::FSI::CoupVarPart>(fsipart, "COUPVARIABLE") ==
              Inpar::FSI::CoupVarPart::vel)
          {
            FOUR_C_THROW(
                "Displacement coupling is not possible in this case! You are not handling any "
                "interface velocities. Check your problem type!");
          }
          else
            return std::shared_ptr<FSI::DirichletNeumann>(new FSI::DirichletNeumannSlideale(comm));
          break;
        default:
          FOUR_C_THROW("Your problem does not work with DirichletNeumann Slide ALE yet!!");
          break;
      }
      break;
    case Inpar::FSI::DirichletNeumannVolCoupl:
      switch (Global::Problem::instance()->get_problem_type())
      {
        case (Core::ProblemType::fsi):
        case (Core::ProblemType::fsi_redmodels):
          if (Teuchos::getIntegralValue<Inpar::FSI::CoupVarPart>(fsipart, "COUPVARIABLE") ==
              Inpar::FSI::CoupVarPart::vel)
          {
            FOUR_C_THROW(
                "Displacement coupling is not possible in this case! You are not handling any "
                "interface velocities. Check your problem type!");
          }
          else
            return std::shared_ptr<FSI::DirichletNeumann>(new FSI::DirichletNeumannVolCoupl(comm));
          break;
        default:
          FOUR_C_THROW("Your problem does not work with DirichletNeumann Volume Coupling yet!!");
          break;
      }
      break;
    case Inpar::FSI::DirichletNeumann:
      switch (Global::Problem::instance()->get_problem_type())
      {
        case (Core::ProblemType::fsi):
        case (Core::ProblemType::fsi_redmodels):
        case (Core::ProblemType::fsi_xfem):
          if (Teuchos::getIntegralValue<Inpar::FSI::CoupVarPart>(fsipart, "COUPVARIABLE") ==
              Inpar::FSI::CoupVarPart::vel)
          {
            FOUR_C_THROW(
                "Displacement coupling is not possible in this case! You are not handling any "
                "interface velocities. Check your problem type!");
          }
          else
            return std::shared_ptr<FSI::DirichletNeumann>(new FSI::DirichletNeumannDisp(comm));
          break;
        case (Core::ProblemType::fbi):
          if (Teuchos::getIntegralValue<Inpar::FSI::CoupVarPart>(fsipart, "COUPVARIABLE") ==
              Inpar::FSI::CoupVarPart::disp)
          {
            FOUR_C_THROW(
                "Displacement coupling is not possible in this case! You are not handling any "
                "interface displacements. Check your problem type!");
          }
          else
            return std::shared_ptr<FSI::DirichletNeumann>(new FSI::DirichletNeumannVel(comm));
          break;
        default:
          FOUR_C_THROW("Your problem does not work with DirichletNeumann yet!!");
          break;
      }
      break;
  }
  return nullptr;
}

FOUR_C_NAMESPACE_CLOSE
