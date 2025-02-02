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
* Constructor.
*/
#include <vector>
using namespace std;

template <class ObjectType>
EuclideanDistanceWeighted<ObjectType>::EuclideanDistanceWeighted(){
}

/**
* Destructor.
*/
template <class ObjectType>
EuclideanDistanceWeighted<ObjectType>::~EuclideanDistanceWeighted(){
}

/**
* @deprecated Use getDistance(ObjectType &obj1, ObjectType &obj2) instead.
*
* @copydoc getDistance(ObjectType &obj1, ObjectType &obj2) .
*/
template <class ObjectType>
double EuclideanDistanceWeighted<ObjectType>::GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error){

    return getDistance(obj1, obj2);
}

/**
* Calculates the Euclidean distance between two feature vectors.
* This calculus is based on the math form sqrt((feature_1[i] - feature_2[i])^2).
* To make this computations both feature vectors should have the same size().
*
* @param obj1: The first feature vector.
* @param obj2: The second feature vector.
* @throw Exception If the computation is not possible.
* @return The Euclidean distance between feature vector 1 and feature vector 2.
*/
template <class ObjectType>
double EuclideanDistanceWeighted<ObjectType>::getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error){

    weights = GetWeights();
    if(weights.empty() && obj1.size() > 0){
        weights.clear();
        for (size_t i = 0; i < obj1.size(); i++)
        {
            weights.push_back(1);
        }   
    }

    if (obj1.size() != obj2.size() && obj1.size() != weights.size()){
        throw std::length_error("The feature vectors do not have the same size.");
    }

    double d = 0;
    double tmp = 0;

    for (size_t i = 0; i < obj1.size(); i++){
        //tmp = obj1[i] - obj2[i];
        tmp = obj1.GetFeatures()[i] - obj2.GetFeatures()[i];
        //cout << "W: " << weights[i] << " ";

        d = d + ((tmp * tmp) * weights[i]);

    }
    //cout << "" << endl;

    // Statistic support
    this->updateDistanceCount();

    return sqrt(d);
}

template <class ObjectType>
void EuclideanDistanceWeighted<ObjectType>::SetWeights(vector<double> weights) throw (std::length_error){
    if(weights.size() == 0 || weights.empty())
        throw std::length_error("The weight vectors cant be zero");

    this->weights = weights; 
}

template <class ObjectType>
vector<double> EuclideanDistanceWeighted<ObjectType>::GetWeights() throw (std::length_error){
    return weights;
}

