//===========================================================================
//
// File: GeometryHelpers.hpp
//
// Created: Mon Jun 22 13:43:23 2009
//
// Author(s): Atgeirr F Rasmussen <atgeirr@sintef.no>
//            Halvor M Nilsen     <hnil@sintef.no>
//            Bjørn Spjelkavik    <bsp@sintef.no>
//
// $Date$
//
// $Revision$
//
//===========================================================================

/*
  Copyright 2009, 2010 SINTEF ICT, Applied Mathematics.
  Copyright 2009, 2010 Statoil ASA.

  This file is part of The Open Porous Media project  (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OPM_GEOMETRYHELPERS_HEADER
#define OPM_GEOMETRYHELPERS_HEADER

#include "Volumes.hpp"

#include <cassert>
#include <cmath>

namespace Dune
{

    namespace GeometryHelpers
    {
        /// @brief
        /// @todo Doc me!
        /// @tparam
        /// @param
        /// @return
        template <class Point, template <class> class Vector>
        Point average(const Vector<Point>& points)
        {
            int num_points = points.size();
            assert(num_points > 0);
            Point pt = points[0];
            for (int i = 1; i < num_points; ++i) {
                pt += points[i];
            }
            pt /= double(num_points);
            return pt;
        }

        /// @brief
        /// @todo Doc me!
        /// @tparam
        /// @param
        /// @return
        template <class Point, template <class> class Vector>
        double polygonArea(const Vector<Point>& points,
                           const Point& centroid)
        {
            double tot_area = 0.0;
            int num_points = points.size();
            for (int i = 0; i < num_points; ++i) {
                Point tri[3] = { centroid, points[i], points[(i+1)%num_points] };
                tot_area += area(tri);
            }
            return tot_area;
        }


        /// @brief
        /// @todo Doc me!
        /// @tparam
        /// @param
        /// @return
        template <class Point, template <class> class Vector>
        Point polygonCentroid(const Vector<Point>& points,
                              const Point& inpoint)
        {
            double tot_area = 0.0;
            Point tot_centroid(0.0);
            int num_points = points.size();
            for (int i = 0; i < num_points; ++i) {
                Point tri[3] = { inpoint, points[i], points[(i+1)%num_points] };
                double tri_area = area(tri);
                Point tri_w_mid = (tri[0] + tri[1] + tri[2]);
                tri_w_mid *= tri_area/3.0;
                tot_area += tri_area;
                tot_centroid += tri_w_mid;
            }

            if (std::abs(tot_area) > 0.0) {
                tot_centroid /= tot_area;
            }
            else {
                tot_centroid = inpoint;
            }

            return tot_centroid;
        }


        /// @brief
        /// @todo Doc me!
        /// @tparam
        /// @param
        /// @return
        template <class Point, template <class> class Vector>
        Point polygonNormal(const Vector<Point>& points,
                            const Point& centroid)
        {
            Point tot_normal(0.0);
            int num_points = points.size();
            for (int i = 0; i < num_points; ++i) {
                Point tri[3] = { centroid, points[i], points[(i+1)%num_points] };
                Point d0 = tri[1] - tri[0];
                Point d1 = tri[2] - tri[0];
                Point w_normal = cross(d0, d1);
                w_normal *= area(tri);
                tot_normal += w_normal;
            }

            if (const auto length = tot_normal.two_norm(); length > 0.0) {
                tot_normal /= length;
            }

            return tot_normal;
        }


        /// @brief
        /// @todo Doc me!
        /// @tparam
        /// @param
        /// @return
        template <class Point, template <class> class Vector>
        double polygonCellVolume(const Vector<Point>& points,
                                 const Point& face_centroid,
                                 const Point& cell_centroid)
        {
            double tot_volume = 0.0;
            int num_points = points.size();
            for (int i = 0; i < num_points; ++i) {
                const Point tet[4] = { cell_centroid, face_centroid, points[i], points[(i+1)%num_points] };
                double small_volume = std::fabs(simplex_volume(tet));
                assert(small_volume >= 0);
                tot_volume += small_volume;
            }
            assert(tot_volume>=0);
            return tot_volume;
        }


        /// @brief
        /// @todo Doc me!
        /// @tparam
        /// @param
        /// @return
        template <class Point, template <class> class Vector>
        Point polygonCellCentroid(const Vector<Point>& points,
                                  const Point& face_centroid,
                                  const Point& cell_centroid)
        {
            Point centroid(0.0);
            double tot_volume = 0.0;
            int num_points = points.size();
            for (int i = 0; i < num_points; ++i) {
                const Point tet[4] = { cell_centroid, face_centroid, points[i], points[(i+1)%num_points] };
                double small_volume = std::fabs(simplex_volume(tet));
                assert(small_volume >= 0);
                Point small_centroid = tet[0];
                for(int j = 1; j < 4; ++j){
                    small_centroid += tet[j];
                }
                small_centroid *= small_volume/4.0;
                centroid += small_centroid;
                tot_volume += small_volume;
            }
            centroid /= tot_volume;
            assert(tot_volume>=0);
            return centroid;
        }


    } // namespace GeometryHelpers

} // namespace Dune


#endif // OPM_GEOMETRYHELPERS_HEADER
