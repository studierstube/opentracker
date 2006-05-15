/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/**
 * @file   OTQtMath.cxx
 * @author Christian Pirchheim
 *
 * @brief  Implementation of class \c OTQtMath
 *
 * $Id: OpenTracker.h 900 2006-01-19 16:47:43Z spiczak $
 */

#include <dllinclude.h>
#if USE_OTQT

#include "OTQtMath.h"
#include "OTQtLog.h"

namespace ot {

//--------------------------------------------------------------------------------
float *
OTQtMath::rotateVectorFromCSToCS(State const & cs_from, State const & cs_to,
                                 float const vec_local_from[3], float vec_local_to[3])
{
  // Compute orig / target CS orientation difference
  float cs_from_orient_conj[4];
  MathUtils::invertQuaternion(cs_from.orientation, cs_from_orient_conj);
  float cs_from_to_orient_diff[4];
  MathUtils::multiplyQuaternion(cs_to.orientation, cs_from_orient_conj, cs_from_to_orient_diff);
  // Rotate vector
  MathUtils::rotateVector(cs_from_to_orient_diff, vec_local_from, vec_local_to);
  return vec_local_to;
}

//--------------------------------------------------------------------------------
State &
OTQtMath::transformVectorFromCSToCS(State const & cs_from, State const & cs_to,
                                    State const & vec_global, State & vec_global_to)
{
  // Convert global vector to local vector relative to original CS
  float vec_local_from[3];
  for (int i = 0; i < 3; i++) {
    vec_local_from[i] = vec_global.position[i] - cs_from.position[i];
  }
  float vec_local_to[3];
  // Rotate local vector about the orientation (from - to) difference of given CS
  rotateVectorFromCSToCS(cs_from, cs_to, vec_local_from, vec_local_to);
  // Convert local vector to global vector relative to target CS
  for (int i = 0; i < 3; i++) {
    vec_global_to.position[i] = cs_to.position[i] + vec_local_to[i];
  }
  for (int i = 0; i < 4; i++) {
    vec_global_to.orientation[i] = cs_to.orientation[i];
  }
  return vec_global_to;
}

//--------------------------------------------------------------------------------
float
OTQtMath::distance(float const * v1, float const * v2, int const dim)
{
  RowVector vec1(dim);
  vec1 << v1;
  RowVector vec2(dim);
  vec2 << v2;
  RowVector diff = vec1 - vec2;
  return (float)(diff.NormFrobenius());
}

//--------------------------------------------------------------------------------
RowVector
OTQtMath::crossProductR3(RowVector const & x, RowVector const & y)
{
  if (x.Ncols() != 3 || y.Ncols() != 3) {
    throw Exception("OTQtMath::crossProductR3(): One of given RowVectors is not a triple.");
  }

  RowVector z(3);
  z(1) = x(2)*y(3) - x(3)*y(2);
  z(2) = x(3)*y(1) - x(1)*y(3);
  z(3) = x(1)*y(2) - x(2)*y(1);

  return z;
}

//--------------------------------------------------------------------------------
bool
OTQtMath::isWithinPlaneBorders(RowVector const & mpos,
                               RowVector const & normal, RowVector const & point_on_plane,
                               RowVector & distances)
{
  OTQT_DEBUG("OTQtMath::isWithinPlaneBorders(): *** START. normal = %f %f %f\n",
             normal(1), normal(2), normal(3));

  double normal_length = normal.NormFrobenius();
  double tmp2 = (normal * mpos.t()).AsScalar();
  double b;

  b = (normal * point_on_plane.t()).AsScalar();
  distances(1) = fabs(tmp2 - b) / normal_length;

  b = (normal * (point_on_plane + normal).t()).AsScalar();
  distances(2) = fabs(tmp2 - b) / normal_length;

  // compare with max distance
  double max_distance = normal_length;
  OTQT_DEBUG("OTQtMath::isWithinPlaneBorders(): max_distance = %f\n",
             max_distance);
  OTQT_DEBUG("OTQtMath::isWithinPlaneBorders(): *** END. distances = %f %f\n",
             distances(1), distances(2));
  return (distances(1) <= max_distance && distances(2) <= max_distance);
}

} // namespace ot

#endif // USE_OTQT
