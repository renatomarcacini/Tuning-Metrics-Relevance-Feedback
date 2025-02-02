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
template <class ObjectType>
LEditDistance<ObjectType>::LEditDistance(){
}

/**
* Destructor.
*/
template <class ObjectType>
LEditDistance<ObjectType>::~LEditDistance(){
}

/**
* @deprecated Use getDistance(ObjectType &obj1, ObjectType &obj2) instead.
*
* @copydoc getDistance(ObjectType &obj1, ObjectType &obj2) .
*/
template <class ObjectType>
double LEditDistance<ObjectType>::GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error){

    return getDistance(obj1, obj2);
}
/**
* Calculates the LEdit distance between two words.
* This calculus is based in the number of the letters changed.
*
* @param obj1: The first word.
* @param obj2: The second word.
* @return The LEdit distance between feature word 1 and word 2.
*/
template <class ObjectType>
double LEditDistance<ObjectType>::getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error){

    char aux1, aux2;
    unsigned int cost;
    int **diff;

    diff = new int*[obj2.size() + 1];
    for (int i = 0; i < obj2.size() + 1; i++)
        diff[i] = new int[obj1.size() + 1];

    // Initialize
    diff[0][0] = 0;
    for (size_t i = 1; i <= obj1.size(); i++){
        diff[i][0] = i;
    }//end for
    for (size_t j = 1; j <= obj2.size(); j++){
        diff[0][j] = j;
    }//end for

    for (size_t i = 1; i <= obj1.size(); i++){
        for (size_t j = 1; j <= obj2.size(); j++){
            // Cost
            aux1 = obj1[i - 1];
            aux2 = obj2[j - 1];
            if ((aux1) == (aux2))
                cost = 0;
            else
                cost = 1;

            diff[i][j] = std::min(std::min(diff[i - 1][j] + 1, diff[i][j - 1] + 1),diff[i - 1][j - 1] + (int32_t)cost);
        }//end for
    }//end for


    this->updateDistanceCount();

    return diff[obj1.size()][obj2.size()];
}
