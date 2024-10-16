#include "utils.h"

namespace p2t {

Orientation Orient2d(const Point& pa, const Point& pb, const Point& pc)
{
  double detleft = (pa.x - pc.x) * (pb.y - pc.y);
  double detright = (pa.y - pc.y) * (pb.x - pc.x);
  double val = detleft - detright;

// Using a tolerance here fails on concave-by-subepsilon boundaries
//   if (val > -EPSILON && val < EPSILON) {
// Using == on double makes -Wfloat-equal warnings yell at us
  if (std::fpclassify(val) == FP_ZERO) {
    return COLLINEAR;
  } else if (val > 0) {
    return CCW;
  }
  return CW;
}

bool InScanArea(const Point& pa, const Point& pb, const Point& pc, const Point& pd) {
  double oadb = (pa.x - pb.x)*(pd.y - pb.y) - (pd.x - pb.x)*(pa.y - pb.y);
  if (oadb >= -EPSILON) {
    return false;
  }

  double oadc = (pa.x - pc.x)*(pd.y - pc.y) - (pd.x - pc.x)*(pa.y - pc.y);
  if (oadc <= EPSILON) {
    return false;
  }
  return true;
}
} // namespace p2t