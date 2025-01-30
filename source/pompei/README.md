### Practicing On Manually Programming EAMxx Interfaces (POMPEI)

This folder contains a very small example for how to add a new
EAMxx atmosphere process. The process (called "POMPEI") simply
adds a new tracer called "ash", which is injected at a certain
elevation above the ruins of Pompei (Italy), mimicking (in a very
crude way) the eruption of the Vesuvius.

The ash is a passive tracer, that does not interact with the
rest of the atmosphere. However, we could easily modify that,
by making it impact the fields used by radiation, such as aero_g_sw,
aero_ssa_sw, aero_tau_sw, and aero_tay_lw, using some super-simple
(and probably completely unphysical) relation.
