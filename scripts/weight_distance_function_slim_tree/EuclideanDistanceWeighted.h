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
