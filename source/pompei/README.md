### Practicing On Manually Programming EAMxx Interfaces (POMPEI)

This folder contains a very small example for how to add a new
EAMxx atmosphere process. The process (called "POMPEI") simply
adds a new tracer called "ash", which is injected at a certain
elevation above the ruins of Pompei (Italy), mimicking (in a very
crude way) the eruption of the Vesuvius.

Hands-on exercises:

1. The eruption location (lat/lon as well as height) is hard-coded.
Can you turn them into runtime parameters, following what's done
with "plume_radius"? NOTE: you may want to add some checks, to
ensure that the input parameters are valid.

2. The eruption initial rate and decay rate are prescribed in the
pompei.*pp files. Can you turn them into actual runtime parameters,
following the example of "plume_radius"? NOTE: you will have to
change the function signature in pompei.*pp

3. The ash is a passive tracer, that does not interact with the
rest of the atmosphere. Can you modify the process, by making it
update the fields used by radiation, such as aero_g_sw,
aero_ssa_sw, aero_tau_sw, and aero_tay_lw, using some super-simple
(and probably completely unphysical) relation?

4. The run_impl method dispatches 3 kernels, hidden inside each of
the 3 field manipulations (scale, update, scale_inv). Can you use
a single kokkos loop that performs the three operation in one sweep?
You will have to extract field views, create a lambda and a policy,
and dispatch the parallel for loop.
