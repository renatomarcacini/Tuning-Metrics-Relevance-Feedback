/**
        * Constructor.
        */
template <class ObjectType>
ManhattanDistance<ObjectType>::ManhattanDistance(){
}

/**
        * Destructor.
        */
template <class ObjectType>
ManhattanDistance<ObjectType>::~ManhattanDistance(){
}

/**
        * @deprecated Use getDistance(ObjectType &obj1, ObjectType &obj2) instead.
        *
        * @copydoc getDistance(ObjectType &obj1, ObjectType &obj2) .
        */
template <class ObjectType>
double ManhattanDistance<ObjectType>::GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error){

    return getDistance(obj1, obj2);
}

/**
        * Calculates the Manhattan distance between two feature vectors.
        * This calculus is based on the math form sum(feature_1[i] - feature_2[i]).
        * To make this computations both feature vectors should have the same size().
        *
        * @param obj1: The first feature vector.
        * @param obj2: The second feature vector.
        * @throw Exception If the computation is not possible.
        * @return The Manhattan distance between feature vector 1 and feature vector 2.
        */
template <class ObjectType>
double ManhattanDistance<ObjectType>::getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error){

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
    double tmp;

    for (size_t i = 0; i < obj1.size(); i++){
        tmp = fabs(obj1[i] - obj2[i]);
        d = d + tmp * weights[i];
    }

    // Statistic support
    this->updateDistanceCount();

    return d;
}

template <class ObjectType>
void ManhattanDistanceWeighted<ObjectType>::SetWeights(vector<double> weights) throw (std::length_error){
    if(weights.size() == 0 || weights.empty())
        throw std::length_error("The weight vectors cant be zero");

    this->weights = weights; 
}

template <class ObjectType>
vector<double> ManhattanDistanceWeighted<ObjectType>::GetWeights() throw (std::length_error){
    return weights;
}