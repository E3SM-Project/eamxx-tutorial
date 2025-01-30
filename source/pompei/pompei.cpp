#include "pompei.hpp"

#include <cmath>

namespace pompei
{

double ash_emission_rate (const double days_since_eruption)
{
  if (days_since_eruption<=0)
    return 0;

  double initial_emission = 1e4;
  double decay_rate = -2.3;
  return initial_emission*exp(days_since_eruption * decay_rate);
}

} // namespace pompei
