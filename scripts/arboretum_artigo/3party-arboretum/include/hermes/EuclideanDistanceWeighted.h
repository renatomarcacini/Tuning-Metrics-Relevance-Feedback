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
*
* This file defines the Euclidean distance weighted.
*
* @version 1.0
* @date 10-29-2014
*/

#ifndef EUCLIDEANDISTANCEWEIGHTED_H
#define EUCLIDEANDISTANCEWEIGHTED_H

#include "DistanceFunction.h"
#include <cmath>
#include <vector>
#include <stdexcept>
using namespace std;

/**
* Class to obtain the Euclidean (or geometric) Distance
*
* @brief L2 distance class.
* @author 006.
* @version 1.0.
*/
template <class ObjectType>
class EuclideanDistanceWeighted : public DistanceFunction <ObjectType>{

    public:
        vector<double> weights;
        EuclideanDistanceWeighted();
        virtual ~EuclideanDistanceWeighted();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        void SetWeights(vector<double> weights) throw (std::length_error);
        vector<double> GetWeights() throw (std::length_error);
};

#include "EuclideanDistanceWeighted-inl.h"
#endif // EUCLIDEANDISTANCEWEIGHTED_H
