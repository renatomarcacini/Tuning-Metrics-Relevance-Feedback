/* Copyright 2003-2017 GBDI-ICMC-USP <caetano@icmc.usp.br>
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*   http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
/**
* @file
* This file defines the class stLineFitting.
*
* @version 1.0
* @author Caetano Traina Jr (caetano@icmc.usp.br)
* @author Agma Juci Machado Traina (agma@icmc.usp.br)
* @author Christos Faloutsos (christos@cs.cmu.edu)
* @author Elaine Parros Machado de Sousa (parros@icmc.usp.br)
* @author Rodrigo Nishihara Adï¿½o (adao@grad.icmc.usp.br)
* @author Ana Carolina Riekstin (anacarol@grad.icmc.usp.br)
*/

#ifndef __STFITTING_H
#define __STFITTING_H

#include <arboretum/stPointSet.h>
#include <arboretum/stCommon.h>
#include <stdio.h>
#include <math.h>

//----------------------------------------------------------------------------
// template class stLineFitting
//----------------------------------------------------------------------------
/**
* This class is used to do the line fitting, adjusting the results obtained in
* other methods.
*
* @version 1.0
* @author Caetano Traina Jr (caetano@icmc.usp.br)
* @author Agma Juci Machado Traina (agma@icmc.usp.br)
* @author Christos Faloutsos (christos@cs.cmu.edu)
* @author Elaine Parros Machado de Sousa (parros@icmc.usp.br)
* @author Ana Carolina Riekstin (anacarol@grad.icmc.usp.br)
* @ingroup fastmap
*/
//---------------------------------------------------------------------------
template <class DataType> class stLineFitting{
   public:
      /**
      * Constructor method.
      */
      stLineFitting() {
         Alpha = -1.0;
         Beta = 0;
         MinimumLeastSquaredError = 0.015;
         MinimumNumberOfPoints = 5;
      }//end stLineFitting

      /**
      * Calculate the Fractal Dimension and returns it in two flavors:
      * the minumum value obtained with a minimum of minimumNumberOfPoints
      * points: MinimumAlpha and MinimumBeta, and the first value that lie
      * in the Minimum LeastSquaredError requested: FirstAlpha and FirstBeta.
      * Alpha and Beta stands for y=Alpha*x + Beta in the resultant line.
      *
      * @param numberOfPoints Number of points.
      * @param logR R are the values of r to plot the graph to find out de intrinsic dimension (r is the lenght of the side of a grid cell).
      * @param logSqr SqR is the sum of squared occupancy for the grid of cell side r.
      * @param minimumLeastSquaredError Minimum least squared error acceptable.
      * @param minimumNumberOfPoints Minimum number of points acceptable.
      */
      void stLeastSquaredFitting(int numberOfPoints, DataType * logR, DataType * logSqR,
                                 DataType minimumLeastSquaredError, int minimumNumberOfPoints);

      /**
      * Calculates the Fractal Dimension and returns the values of Alpha and Beta
      * (y=Alpha*x + Beta in the resulting line) and the error obtained for <Alpha,Beta>
      *
      * @param numberOfPoints Number of points.
      * @param logR R are the values of r to plot the graph to find out de intrinsic dimension (r is the lenght of the side of a grid cell).
      * @param logSqr SqR is the sum of squared occupancy for the grid of cell side r.
      * @param minimumLeastSquaredError Minimum least squared error acceptable.
      *
      */
      void stRegionBasedLineFitting (int numberOfPoints, DataType * logR,
                                     DataType *logSqR, DataType minimumLeastSquaredError);

      /**
      * Method to get the MinimumAlpha value.
      *
      * @return MinimumAlpha The minumum value of Alpha obtained with a minimum
      * of minimumNumberOfPoints points in the stLeastSquaredFitting algorithm.
      */
      DataType GetMinimumAlpha(){
         return MinimumAlpha;
      }//end GetMinimumAlpha()

      /**
      * Method to get the MinimumBeta value.
      *
      * @return MinimumBeta The minumum value of Beta obtained with a minimum
      * of minimumNumberOfPoints points in the stLeastSquaredFitting algorithm.
      */
      DataType GetMinimumBeta(){
         return MinimumBeta;
      }//end GetMinimumBeta

      /**
      * Method to get the FirstAlpha value.
      *
      * @return FirstAlpha The first value of Alpha that lie in the
      * minimumLeastSquaredError requested in the stLeastSquaredFitting algorithm.
      */
      DataType GetFirstAlpha(){
         return FirstAlpha;
      }//end GetFirstAlpha

      /**
      * Method to get the FirstBeta value.
      *
      * @return FirstBeta The first value of Beta that lie in the
      * minimumLeastSquaredError requested in the stLeastSquaredFitting algorithm.
      */
      DataType GetFirstBeta(){
         return FirstBeta;
      }//end GetBFirst

      /**
      * Method to get the error value.
      *
      * @return Error The error obtained for <Alpha,Beta> in the
      * stRegionBasedLineFitting algorithm or stLeastSquaredFitting algorithm.
      */

      DataType GetError(){
         return Error;
      };//end GetError

      /**
      * Method to get Alpha value.
      *
      * @return Alpha The value of Alpha that lie in the minimumLeastSquaredError
      * requested in the stRegionBasedLineFitting algorithm.
      */
      DataType GetAlpha(){
         return Alpha;
      }//end GetAlpha

      /**
      * Method to get Beta value.
      *
      * @return Beta The value of Beta that lie in the minimumLeastSquaredError
      * requested in the stRegionBasedLineFitting algorithm.
      */
      DataType GetBeta(){
         return Beta;
      }//end GetBeta

   private:
   
      /**
      * Computes the linear regression and the mean squared error.
      *
      * @param X x-points (in)
      * @param Y y-points(in)
      * @param numberOfPoints Number of points.
      *
      * @return Slope Slope of the line (out)
      * @return YIncrement y-increment (out)
      */
      DataType linerror(DataType * x, DataType * y, int numberOfPoints,
                        DataType & slope, DataType & yIncrement);

      /**
      * Minimum least squared error acceptable.
      */
      DataType MinimumLeastSquaredError;

      /**
      * Minimum number os points acceptable.
      */
      DataType MinimumNumberOfPoints;

      /**
      * The slope of the adjusted line.
      */
      DataType Alpha;

      /**
      * The increment in the Y-axis of the adjusted line.
      */
      DataType Beta;

      /**
      * The first value of Alpha that lie in the minimumLeastSquaredError requested
      * in the stLeastSquaredFitting algorithm.
      */
      DataType FirstAlpha;

      /**
      * The first value of Beta that lie in the minimumLeastSquaredError requested
      * in the stLeastSquaredFitting algorithm.
      */
      DataType FirstBeta;

      /**
      * The minumum value of Alpha obtained with a minimum of minimumNumberOfPoints
      * points in the stLeastSquaredFitting algorithm.
      */
      DataType MinimumAlpha;

      /**
      * The minumum value of Beta obtained with a minimum of minimumNumberOfPoints
      * points in the stLeastSquaredFitting algorithm.
      */
      DataType MinimumBeta;

      /**
      * The error obtained for <Alpha,Beta> in the stRegionBasedLineFitting
      * algorithm or stLeastSquaredFitting algorithm.
      */
      DataType Error;
};

// Include Template source
#include <arboretum/stLineFitting-inl.h>

#endif __STFITTING_H
