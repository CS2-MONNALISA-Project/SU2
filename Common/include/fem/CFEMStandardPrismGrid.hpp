/*!
 * \file CFEMStandardPrismGrid.hpp
 * \brief Class for the FEM prism standard element for the grid.
 *        The functions are in the <i>CFEMStandardPrismGrid.cpp</i> file.
 * \author E. van der Weide
 * \version 7.0.8 "Blackbird"
 *
 * SU2 Project Website: https://su2code.github.io
 *
 * The SU2 Project is maintained by the SU2 Foundation
 * (http://su2foundation.org)
 *
 * Copyright 2012-2020, SU2 Contributors (cf. AUTHORS.md)
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once 

#include "CFEMStandardPrism.hpp"

/*!
 * \class CFEMStandardPrismGrid
 * \brief Class which defines the variables and methods for the
 *        prism standard element for the grid.
 * \author E. van der Weide
 * \version 7.0.8 "Blackbird"
 */
class CFEMStandardPrismGrid final: public CFEMStandardPrism {

public:
  /*!
   * \brief Default constructor of the class, deleted to make sure the
   *        overloaded constructor is always used.
   */
  CFEMStandardPrismGrid() = delete;

  /*!
   * \overload
   * \param[in] val_nPoly      - Polynomial degree of the grid for this element.
   * \param[in] val_orderExact - Polynomial order that must be integrated exactly
   *                             by the integration rule.
   */
  CFEMStandardPrismGrid(const unsigned short val_nPoly,
                        const unsigned short val_orderExact);

  /*!
   * \overload
   * \param[in] val_nPolyGrid   - Polynomial degree of the grid for this element.
   * \param[in] val_nPolyGrid   - Polynomial degree of the solution for this element.
   * \param[in] val_orderExact  - Polynomial order that must be integrated exactly
   *                              by the integration rule.
   * \param[in] val_locGridDOFs - Location of the grid DOFS, either LGL or equidistant.
   */
  CFEMStandardPrismGrid(const unsigned short val_nPolyGrid,
                        const unsigned short val_nPolySol,
                        const unsigned short val_orderExact,
                        const unsigned short val_locGridDOFs);

  /*!
   * \brief Destructor.
   */
  ~CFEMStandardPrismGrid();

  /*!
   * \brief Function, which computes the coordinates in the integration points.
   * \param[in]  LGLDistribution - Whether or not the LGL node distribution must be used.
   * \param[in]  matCoorDOF - Matrix that contains the coordinates of the grid DOFs.
   * \param[out] matCoorInt - Matrix that contains the coordinates of the integration
   *                          points.
   */
  void CoorIntPoints(const bool                LGLDistribution,
                     ColMajorMatrix<su2double> &matCoorDOF,
                     ColMajorMatrix<su2double> &matCoorInt) override;

  /*!
   * \brief Function, which computes the derivatives of the coordinates in the
   *        integration points.
   * \param[in]  LGLDistribution - Whether or not the LGL node distribution must be used.
   * \param[in]  matCoor         - Matrix that contains the coordinates of the grid DOFs.
   * \param[out] matDerCoor      - Vector of matrices to store the derivatives of the coordinates.
   */
  void DerivativesCoorIntPoints(const bool                         LGLDistribution,
                                ColMajorMatrix<su2double>          &matCoor,
                                vector<ColMajorMatrix<su2double> > &matDerCoor) override;

  /*!
   * \brief Function, which computes the derivatives of the coordinates in the
   *        solution DOFs.
   * \param[in]  matCoor    - Matrix that contains the coordinates of the grid DOFs.
   * \param[out] matDerCoor - Vector of matrices to store the derivatives of the coordinates.
   */
  void DerivativesCoorSolDOFs(ColMajorMatrix<su2double>          &matCoor,
                              vector<ColMajorMatrix<su2double> > &matDerCoor) override;

  /*!
   * \brief Function, that returns the number of different face types
   *        occuring in this volume element.
   * \return The number of different face types of the volume element.
   */
  unsigned short GetnFaceTypes(void) const override {return 2;}

  /*!
   * \brief Function, that returns the number of solution DOFs.
   * \return The number of solution DOFs of the volume element.
   */
  unsigned short GetNSolDOFs(void) const override {return rLineSolDOFs.size()*rTriangleSolDOFs.size();}

  /*!
   * \brief Function that returns the VTK type for the given face type index.
   * \param[in] ind - Index of the face type for which the VTK type must be returned.
   * \return The VTK type of the given face type.
   */
  unsigned short GetVTK_TypeFace(unsigned short ind) const override {
    if(ind == 0) return TRIANGLE;
    else         return QUADRILATERAL;
  }

  /*!
   * \brief Function, which estimates the amount of work for a volume prism.
   *        This information is used to determine a well balanced partition.
   *        The work of the surface integral in DG is not included.
   * \param[in] config - Object, which contains the input parameters.
   * \return The work estimate for the volume for this type of element.
   */
  passivedouble WorkEstimateVolume(CConfig *config) override;

private:

  vector<passivedouble> rLineSolDOFs;     /*!< \brief 1D parametric coordinates of the nodal solution DOFs. */
  vector<passivedouble> rTriangleSolDOFs; /*!< \brief Parametric r-coordinates of the triangle nodal
                                                      solution DOFs. */
  vector<passivedouble> sTriangleSolDOFs; /*!< \brief Parametric s-coordinates of the triangle nodal
                                                      solution DOFs. */


  ColMajorMatrix<passivedouble> lagBasisIntEqui; /*!< \brief The values of the Lagrangian basis functions
                                                             in the integration points for the equidistant
                                                             point distribution. */
  ColMajorMatrix<passivedouble> lagBasisIntLGL;  /*!< \brief The values of the Lagrangian basis functions
                                                             in the integration points for the LGL
                                                             point distribution. */

  vector<ColMajorMatrix<passivedouble> > derLagBasisIntEqui; /*!< \brief The values of the derivatives of the Lagrangian
                                                                         basis functions in the integration points for the
                                                                         equidistant point distribution. It is a vector,
                                                                         because there are derivatives in three directions. */
  vector<ColMajorMatrix<passivedouble> > derLagBasisIntLGL;  /*!< \brief The values of the derivatives of the Lagrangian
                                                                         basis functions in the integration points for the
                                                                         LGL point distribution. It is a vector, because
                                                                         there are derivatives in three directions. */

  vector<ColMajorMatrix<passivedouble> > hesLagBasisInt;     /*!< \brief The values of the 2nd derivatives, Hessian, of the
                                                                         Lagrangian basis functions in the integration points
                                                                         for the point distribution in use. It is a vector,
                                                                         because there are 6 2nd derivatives in 3D. */

  ColMajorMatrix<passivedouble> lagBasisSolDOFs;             /*!< \brief The values of the Lagrangian basis functions
                                                                         in the nodal solution DOFs. */
  vector<ColMajorMatrix<passivedouble> > derLagBasisSolDOFs; /*!< \brief The values of the derivatives of the Lagrangian
                                                                         basis functions in the nodal solution DOFs.
                                                                         It is a vector, because there are derivatives
                                                                         in three directions. */

  void *jitterSolDOFs = nullptr;     /*!< \brief Pointer to the data for the jitted gemm function. */
  dgemm_jit_kernel_t dgemmSolDOFs;   /*!< \brief Pointer to the function to carry out the jitted gemm call. */

  /*!
   * \brief Function, which computes the values of the derivatives of the Lagrangian
   *        basis functions of a prism in the integration points for the given
   *        location of the DOFs.
   * \param[in]  rTriangleDOFs - Vector, which contains the parametric r-locations of the DOFs
   *                             of the base triangle.
   * \param[in]  sTriangleDOFs - Vector, which contains the parametric s-locations of the DOFs
   *                             of the base triangle.
   * \param[in]  rLineDOFs     - Vector, which contains the parametric r-locations of the DOFs
   *                             of the line normal to the triangles.
   * \param[out] derLag        - Matrix, which contains the values of derivatives of all the
   *                             Lagrangian basis functions in all the integration points.
   */
  void DerLagBasisIntPointsPrism(const vector<passivedouble>            &rTriangleDOFs,
                                 const vector<passivedouble>            &sTriangleDOFs,
                                 const vector<passivedouble>            &rLineDOFs,
                                 vector<ColMajorMatrix<passivedouble> > &derLag);

  /*!
   * \brief Function, which computes the values of the 2nd derivatives, Hessian,
   *        of the Lagrangian basis functions of a prism in the integration
   *        points for the given location of the DOFs.
   * \param[in]  rTriangleDOFs - Vector, which contains the parametric r-locations of the DOFs
   *                             of the base triangle.
   * \param[in]  sTriangleDOFs - Vector, which contains the parametric s-locations of the DOFs
   *                             of the base triangle.
   * \param[in]  rLineDOFs     - Vector, which contains the parametric r-locations of the DOFs
   *                             of the line normal to the triangles.
   */
  void HesLagBasisIntPointsPrism(const vector<passivedouble> &rTriangleDOFs,
                                 const vector<passivedouble> &sTriangleDOFs,
                                 const vector<passivedouble> &rLineDOFs);

  /*!
   * \brief Function, which computes the values of the Lagrangian basis functions
   *        of a prism in the integration points for the given location of the DOFs.
   * \param[in]  rTriangleDOFs - Vector, which contains the parametric r-locations of the DOFs
   *                             of the base triangle.
   * \param[in]  sTriangleDOFs - Vector, which contains the parametric s-locations of the DOFs
   *                             of the base triangle.
   * \param[in]  rLineDOFs     - Vector, which contains the parametric r-locations of the DOFs
   *                             of the line normal to the triangles.
   * \param[out] lag           - Matrix, which contains the values of all the Lagrangian
   *                             basis functions in all the integration points.
   */
  void LagBasisIntPointsPrism(const vector<passivedouble>   &rTriangleDOFs,
                              const vector<passivedouble>   &sTriangleDOFs,
                              const vector<passivedouble>   &rLineDOFs,
                              ColMajorMatrix<passivedouble> &lag);

  /*!
   * \brief Function, which computes the values of the Lagrangian basis functions
   *        of a prism and its derivatives in the nodal solution DOFs for the
   *        given location of the grid DOFs.
   * \param[in]  rTriangleDOFs - Vector, which contains the parametric r-locations of the DOFs
   *                             of the base triangle.
   * \param[in]  sTriangleDOFs - Vector, which contains the parametric s-locations of the DOFs
   *                             of the base triangle.
   * \param[in]  rLineDOFs     - Vector, which contains the parametric r-locations of the DOFs
   *                             of the line normal to the triangles.
   */
  void LagBasisAndDerSolDOFsPrism(const vector<passivedouble> &rTriangleDOFs,
                                  const vector<passivedouble> &sTriangleDOFs,
                                  const vector<passivedouble> &rLineDOFs);

  /*!
   * \brief Function, which computes the location of all the DOFs of the prim
   *        from the given coordinates of the triangle and line.
   * \param[in]  rTriangleDOFs - Vector, which contains the parametric r-locations of the DOFs
   *                             of the base triangle.
   * \param[in]  sTriangleDOFs - Vector, which contains the parametric s-locations of the DOFs
   *                             of the base triangle.
   * \param[in]  rLineDOFs     - Vector, which contains the parametric r-locations of the DOFs
   *                             of the line normal to the triangles.
   * \param[out] rDOFs         - Parametric r-coordinates of the DOFs of the prism.
   * \param[out] sDOFs         - Parametric s-coordinates of the DOFs of the prism.
   * \param[out] tDOFs         - Parametric t-coordinates of the DOFs of the prism.
   */
  void LocationAllDOFsPrism(const vector<passivedouble>   &rTriangleDOFs,
                            const vector<passivedouble>   &sTriangleDOFs,
                            const vector<passivedouble>   &rLineDOFs,
                            vector<passivedouble>         &rDOFs,
                            vector<passivedouble>         &sDOFs,
                            vector<passivedouble>         &tDOFs);

  /*!
   * \brief Function, which computes the gradients of the Vandermonde matrix for a standard prism.
   * \param[in]  r   - Parametric r-coordinates for which the Vandermonde matrix must be computed.
   * \param[in]  s   - Parametric s-coordinates for which the Vandermonde matrix must be computed
   * \param[in]  t   - Parametric t-coordinates for which the Vandermonde matrix must be computed
   * \param[out] VDr - Matrix to store the derivative in r-direction of the Vandermonde matrix
   *                   in all r,s,t-locations.
   * \param[out] VDs - Matrix to store the derivative in s-direction of the Vandermonde matrix
   *                   in all r,s,t-locations.
   * \param[out] VDt - Matrix to store the derivative in t-direction of the Vandermonde matrix
   *                   in all r,s,t-locations.
   */
  void GradVandermondePrism(const vector<passivedouble>   &r,
                            const vector<passivedouble>   &s,
                            const vector<passivedouble>   &t,
                            ColMajorMatrix<passivedouble> &VDr,
                            ColMajorMatrix<passivedouble> &VDs,
                            ColMajorMatrix<passivedouble> &VDt);

  /*!
   * \brief Function, which computes the Hessian of the Vandermonde matrix for a standard prism.
   * \param[in]  r   - Parametric r-coordinates for which the Vandermonde matrix must be computed.
   * \param[in]  s   - Parametric s-coordinates for which the Vandermonde matrix must be computed
   * \param[in]  t   - Parametric t-coordinates for which the Vandermonde matrix must be computed
   * \param[out] VDr2 - Matrix to store the 2nd derivative in r-direction of the Vandermonde matrix
   *                    in all r,s,t-locations.
   * \param[out] VDs2 - Matrix to store the 2nd derivative in s-direction of the Vandermonde matrix
   *                    in all r,s,t-locations.
   * \param[out] VDt2 - Matrix to store the 2nd derivative in t-direction of the Vandermonde matrix
   *                    in all r,s,t-locations.
   * \param[out] VDrs - Matrix to store the cross derivative in rs-direction of the Vandermonde matrix
   *                    in all r,s,t-locations.
   * \param[out] VDrt - Matrix to store the cross derivative in rt-direction of the Vandermonde matrix
   *                    in all r,s,t-locations.
   * \param[out] VDst - Matrix to store the cross derivative in st-direction of the Vandermonde matrix
   *                    in all r,s,t-locations.
   */
  void HesVandermondePrism(const vector<passivedouble>   &r,
                           const vector<passivedouble>   &s,
                           const vector<passivedouble>   &t,
                           ColMajorMatrix<passivedouble> &VDr2,
                           ColMajorMatrix<passivedouble> &VDs2,
                           ColMajorMatrix<passivedouble> &VDt2,
                           ColMajorMatrix<passivedouble> &VDrs,
                           ColMajorMatrix<passivedouble> &VDrt,
                           ColMajorMatrix<passivedouble> &VDst);

  /*!
   * \brief Function, which computes the Vandermonde matrix for a standard prism.
   * \param[in]  r  - Parametric r-coordinates for which the Vandermonde matrix must be computed.
   * \param[in]  s  - Parametric s-coordinates for which the Vandermonde matrix must be computed
   * \param[in]  t  - Parametric t-coordinates for which the Vandermonde matrix must be computed
   * \param[out] V  - Matrix to store the Vandermonde matrix in all r,s,t-locations.
   */
  void VandermondePrism(const vector<passivedouble>   &r,
                        const vector<passivedouble>   &s,
                        const vector<passivedouble>   &t,
                        ColMajorMatrix<passivedouble> &V);

  /*!
   * \brief Function, which creates the local grid connectivities of the faces
   *        of the volume element.
   */
  void LocalGridConnFaces(void);

  /*!
   * \brief Function, which creates the connectivity of the linear sub-elements when the
   *        high order element is split in such elements.
   */
  void SubConnLinearElements(void);
};