#include "eamxx_pompei_process_interface.cpp"

#include "pompei.hpp"

namespace scream
{

PompeiEruption (const ekat::Comm& comm, const ekat::ParameterList& params)
 : AtmosphereProcess(comm,params)
{
  m_eruption_start = util::str_to_time_stamp(params.get<std::string>("eruption_date"));
}

void PompeiEruption::
set_grids (const std::shared_ptr<const GridsManager> grids_manager)
{
  using namespace ekat::units;
  using namespace ShortFieldTagsNames;

  // Some constants
  constexpr Real deg2rad = Constants<Real>::pi / 180;
  constexpr Real r_earth = Constants<Real>::r_earth;

  // Declare the fields we need as input and/or output
  auto grid = grids_manager->get_grid("Physics");
  auto layout = grid->get_3d_scalar_layout(true);
  auto m3 = pow(m,3);
  add_field<Updated>("ash",layout,1/m3,grid->name());

  // Number of columns/levels on this MPI rank
  int ncols = layout.dim(COL);
  int nlevs = layout.dim(LEV);

  // We can create our helpe "mask" field. It will be 1 where the volcano injeciton
  // in the atmosphere happens, and 0 elsewhere.
  FieldIdentifier rate_fid("emission_mask",layout,1/(m3*s),grid->name());
  m_emission_mask = Field(rate_fid);
  m_emission_mask.allocate_view();
  m_emission_mask.deep_copy(0); // 0 means "not injecting here"

  // The grid already stores lat/lon, so go ahead and pre-compute the mask field
  auto lat = grid->get_geometry_data("lat"); // WATCH OUT: it's in degrees, NOT rad
  auto lon = grid->get_geometry_data("lon"); // WATCH OUT: it's in degrees, NOT rad

  // Target location of volcanic eruption
  auto volcano_lat = 40.8214 * deg2rad;
  auto volcano_lon = 14.4260 * deg2rad;
  auto radius = m_params.get<double>("plume_radius");
  int emission_lev = m_params.get<int>("emission_level");
  EKAT_REQUIRE_MSG (emission_lev>=0 and emission_lev<nlevs,
      "Error! Emission level out of bounds.\n"
      " - emission_lev: " << emission_lev << "\n"
      " - grid num lev: " << nlevs << "\n");

  // Extract (device) views from the fields
  auto emission_view = m_emission_mask.get_view<Real**>();
  auto lat_view = lat.get_view<const Real*>();
  auto lon_view = lon.get_view<const Real*>();

  // A lambda is just a function defined "on-the fly". KOKKOS_LAMBDA simply adds
  // some decoration for GPU execution, nothing to worry for now
  auto compute_mask = KOKKOS_LAMBDA (const int icol) {
    auto lat_rad = lat_view(icol) * deg2rad;
    auto lon_rad = lon_view(icol) * deg2rad;
    auto delta_lat = lat_rad - volcano_lat;
    auto delta_lon = lon_rad - volcano_lon;

    auto dist = r_earth * sqrt(delta_lat*delta_lat + delta_lon*delta_lon);

    if (dist<radius) {
      emission_view(icol,emission_lev) = 1;
    }
  };

  // A policy tells kokkos how to parallelize the loop. Here, we are doing
  // a single for loop over the range of indices [0,ncols)
  auto policy = Kokkos::RangePolicy<Field::device_t>(0,ncols);

  // Execute the lambda in parallel according to the execution policy
  Kokkos::parallel_for(policy,compute_mask);
}

void PompeiEruption::initialize_impl (const RunType /* run_type */)
{
  // If pompei internally requires to initialize data, do that now.
  // NOTE: run_type tells us if this is an initial or restarted run,
  //       but this parametrization doesn't care
}

void PomepeiEruption::run_impl (const double dt)
{
  // Compute current emission rate
  // timestamp returns time at the *beginning* of the atm step.
  auto t = timestamp() + dt;
  auto rate = compute_emission_rate (t.days_from(m_eruption_date));

  // Update the output field
  // y.update(x,a,b) means y = b*y + a*x
  auto ash = get_field_out("ash");
  ash.update(m_emission_mask,rate,1);
}

void PompeiEruption::finalize_impl ()
{
  // If pompei internally requires to cleanup some data, do that now.
}

} // namespace scream
