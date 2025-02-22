/*
 * First define this process intereface as a new header file
 */
#ifndef EAMXX_AP_TEMPLATE_PROCESS_INTERFACE_HPP
#define EAMXX_AP_TEMPLATE_PROCESS_INTERFACE_HPP
/*
 * Any include statements.  By default you want to include the atmosphere_process.hpp header
 */
#include "share/atm_process/atmosphere_process.hpp"

namespace scream
{

/*
 * Class definition for new process interface.  Change "AP_TEMPLATE" to a name that makes sense.
 */
class AP_TEMPLATE : public AtmosphereProcess
{
// Define public functions
public:

  AP_TEMPLATE (const ekat::Comm& comm, const ekat::ParameterList& params);

  AtmosphereProcessType type () const override { return AtmosphereProcessType::Physics; }

  std::string name () const override { return "SOME STRING DESCRIPTION"; }

  void set_grids (const std::shared_ptr<const GridsManager> grids_manager) override;
// Define the protected functions, usually at least initialize_impl, run_impl and finalize_impl,
// but others could be included.  See eamxx_template_process_interface.cpp for definitions of each of these.
protected:
  void initialize_impl (const RunType run_type) override;
  void run_impl        (const double dt) override;
  void finalize_impl   () override;

// Could also define here universal variables for the process interface.

};

} // namespace scream

#endif // EAMXX_AP_TEMPLATE_PROCESS_INTERFACE_HPP
