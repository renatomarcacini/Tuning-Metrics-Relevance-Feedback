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
#ifndef BASICARRAYOBJECTWITHNAME_H
#define BASICARRAYOBJECTWITHNAME_H

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

/**
* This class implements a very important object for similarity search: just a single and simple
* feature vector that can make your life easier when talking about using arboretum resources. Be-
* cause of internal use on MAM's we must inherance from stObject class.
*
* For illustration, consider the feature vector as follows:
* +-----+------+------------------+--------+
* | OID | Size | Vector Data []   | Name[] |
* +-----+------+------------------+--------+
*
* @brief This class implements a generic feature vector
* @author Marcos Vinicius Naves Bedo
* @version 1.0
* @arg DType The data type stored by each position of the feature vector
*/
template <class DType>
class BasicArrayObjectWithName{


    private:
        //The internal variable that really stores the data
        std::vector<DType> data;
        //The OID that identifies the feature vector
        u_int32_t OID;
        //A previous directive that can allow store and retrieve
        //the feature vector from BLOB or FILE
        u_char *serialized;

        string Name;

    public:

        /**
        * Constructor Method.
        * Sets data and size to empty and 0, respectively.
        */
        BasicArrayObjectWithName(){
            Name = "";
            data.clear();
            serialized = NULL;
        }

        /**
        * Constructor Method.
        * Sets the values of the vector to current.
        */
        BasicArrayObjectWithName(const string name, const u_int32_t OID, const std::vector<DType> &data){
            Name = name;
            this->OID = OID;
            for(int x = 0; x < data.size(); x++)
                set(data[x]);
            serialized = NULL;
        }

        /**
        * Destructor.
        */
        ~BasicArrayObjectWithName(){

            data.clear();
            if (serialized != NULL)
                delete[] serialized;
        }

        /**
        * @deprecated
        * @copydoc setOID(u_int32_t OID).
        */
        void SetOID(u_int32_t OID){

            setOID(OID);
        }


        /**
        * Sets the feature vector OID.
        * @param OID The OID of the feature vector.
        * Caution: The OID is not checked as unique.
        */
        void setOID(u_int32_t OID){
            this->OID = OID;
        }

        /**
        * Gets the feature vector OID.
        * @return The feature vector OID.
        */
        u_int32_t getOID() const{
            return OID;
        }

        const string & GetName(){
            return getName();
        }//end GetName

        const string & getName(){
            return Name;
        }

        /**
        * @brief This method is deprecated, but still usefull for arboretum
        * template functions.
        * @deprecated This method is deprecated. Use getOID() instead.
        * @copydoc getOID().
        */
        u_int32_t GetOID() const{
            return getOID();
        }

        /**
        * Re-sizes a Basic Array Object.
        * All previous stored values are cleaned according to previous size.
        * @param size The new Basic Array size.
        */
        void resize(size_t size){

            DType aux2;
            std::vector<DType> aux;
            for (size_t x = 0; x < std::min(size, data.size()); x++){
                aux.push_back(data[x]);
            }
            data.clear();
            aux.clear();
            data = aux;
            for (size_t x = std::min(size, data.size()); x < size; x++){
                data.push_back(aux2);
            }
        }

        /**
        * Re-sizes a Basic Array Object.
        * All previous stored values are cleaned according to previous size.
        * New elements receives 'value' as the default value.
        * @param size The new Basic Array size.
        * @param value The default value to be set.
        */
        void resize(size_t size, DType value){

            std::vector<DType> aux;
            for (size_t x = 0; x < std::min(size, data.size()); x++){
                aux.push_back(data[x]);
            }
            data.clear();
            aux.clear();
            data = aux;
            for (size_t x = std::min(size, data.size()); x < size; x++){
                data.push_back(value);
            }
        }


        /**
        * @copydoc set(DType value).
        */
        void Set(DType value){
            set(value);
        }

        /**
        * Set the next (mandatory) position of the feature vector.
        * @param value The value to be pushed.
        */
        void set(DType value){
            data.push_back(value);
        }

        /**
        * @deprecated
        * @copydoc set(size_t pos, DType value).
        */
        void Set(size_t pos, DType value){

            set(pos, value);
        }

        /**
        * Sets a specific value in a specific position.
        * @todo Throw a out-of-bounds exception.
        * @param pos The position of the insertion.
        * @param value The value to be pushed.
        */
        void set(size_t pos, DType value){

            if (pos == data.size()){
                data.push_back(value);
            } else {
                if (pos > data.size()){
                    DType aux;
                    for (int x = 0; x < (pos-data.size()); x++)
                        data.push_back(aux);
                    data[pos] = value;
                } else {
                    data[pos] = value;
                }
            }
        }

        /**
        * Gets the entire stored data.
        * @return The entire stored data.
        */
        std::vector<DType> getData(){
            return data;
        }

        /**
        * Overloaded operator allowing modifications.
        * @param idx The index to be queried.
        */
        DType& operator[] (size_t idx) {

            return data[idx];
        }

        /**
        * Overloaded operator that not allows modifications.
        * @param idx The index to be queried.
        */
        const DType& operator[] (size_t idx)  const{

            return data[idx];
        }

        /**
        * Get a value in a specific position.
        * @todo Throw a out-of-bounds exception.
        * @param idx The position value to be retrieved.
        * @return The value of the position idx.
        */
        DType *get(size_t idx){

            return (&data[idx]);
        }

        /**
        * @brief This method is deprecated, but still necessary for arboretum
        * template functions.
        * @deprecated This method is deprecated. Use get(u_int32_t idx) instead.
        * @copydoc get(size_t idx).
        */
        DType *Get(size_t idx){

            return get(idx);
        }

        /**
        * Gets the number of elements in the feature vector.
        * @return The number of elements of the feature vector.
        */
        size_t getSize(){

            return data.size();
        }

        size_t size(){

            return getSize();
        }

        /**
        * @brief This method is deprecated, but still usefull for arboretum
        * template functions.
        * @deprecated This method is deprecated. Use getSize() instead.
        * @copydoc getSize().
        */
        size_t GetSize(){

            return getSize();
        }

        /**
        * @deprecated
        * @copydoc getObject().
        */
        BasicArrayObjectWithName<DType> GetObject(){

            return getObject();
        }

        /**
        * Return the instance of the current Basic Array Object.
        * @return The current instance of Basic Array Object.
        */
        BasicArrayObjectWithName<DType> getObject(){

            return this;
        }

        /**
        * Gets an instantied copy of the object.
        * @return A copy of the object.
        */
        BasicArrayObjectWithName<DType> *clone(){

            return new BasicArrayObjectWithName<DType>(getName(), getOID(), getData());
        }

        /**
        * @brief This method is deprecated, but still usefull for arboretum
        * template functions.
        * @deprecated This method is deprecated. Use clone() instead.
        * @copydoc clone().
        */
        BasicArrayObjectWithName<DType> *Clone(){

            return clone();
        }

        /**
        * Check if the obj is equal to the current object.
        * @param obj The object to be compared.
        * @return True if the objects are equal, else otherwise.
        */
        bool isEqual(BasicArrayObjectWithName<DType> *obj){

            if ((getOID() != obj->GetOID()) || (getSize() != obj->GetSize()))
                return false;

            for (int x = 0; x < getSize(); x++)
                if (*(this->get(x)) != *((DType*) (obj->Get(x))))
                    return false;

            return true;
        }

        /**
        * @brief This method is deprecated, but still usefull for arboretum
        * template functions.
        * @deprecated This method is deprecated. Use isEqual(stObject *obj) instead.
        * @copydoc isEqual(stObject *obj).
        */
        bool IsEqual(BasicArrayObjectWithName<DType> *obj){

            return isEqual(obj);
        }

        /**
        * Gets the size of the byte vector.
        * @return The size of the bytes vector.
        */
        u_int32_t getSerializedSize(){

            return (sizeof(u_int32_t) + sizeof(size_t) +  (sizeof(DType) * data.size()) + Name.length());
        }

        /**
        * @brief This method is deprecated, but still usefull for arboretum
        * template functions.
        * @deprecated This method is deprecated. Use getSerializedSize() instead.
        * @copydoc getSerializedSize().
        */
        u_int32_t GetSerializedSize(){

            return getSerializedSize();
        }

        /**
        * Gets the equivalent byte vector of the object.
        * @return The equivalent byte vector of the object.
        */
        const u_char *serialize(){

            if (serialized == NULL){
                serialized = new u_char[getSerializedSize()];
                size_t size = getSize();
                memcpy(serialized, &OID, sizeof(u_int32_t));
                memcpy(serialized + sizeof(u_int32_t), &size, sizeof(size_t));
                for (int x = 0; x < size; x++){
                    memcpy(serialized + sizeof(u_int32_t) + sizeof(size_t)+(sizeof(DType)*x), &data[x], sizeof(DType));
                }
                memcpy(serialized + sizeof(u_int32_t) + sizeof(size_t) +  (sizeof(DType) * data.size()) , Name.c_str(), Name.length());
            }
            return serialized;
        }

        /**
        * @brief This method is deprecated, but still usefull for arboretum
        * template functions.
        * @deprecated This method is deprecated. Use serialize() instead.
        * @copydoc serialize().
        */
        const u_char *Serialize(){

            return serialize();
        }

        /**
        * Gets the equivalent byte vector of the object.
        * @return The equivalent string byte vector of the object.
        */
        std::string serializeToString(){

            std::string answer;

            serialize();
            for (int x = 0; x < getSerializedSize(); x++){
                answer += serialized[x];
            }

            return answer;
        }

        /**
        * Transform a byte vector into an object.
        * @param dataIn The byte vector.
        * @param dataSize The byte vector size.
        */
        void unserialize(const u_char *dataIn, size_t dataSize = 0){

            DType *d = 0;
            size_t size_vector;
            size_t strl;

            // This is the reverse of Serialize(). So the steps are similar.
            // Remember, the format of the serizalized object is
            // +----------+-----------+--------+
            // | OID | Size | Vector Data []   |
            // +----------+-----------+--------+
            //Name.assign((char *)(dataIn), Name.length());


            memcpy(&OID, dataIn, sizeof(u_int32_t));

            if (dataSize != 0) {
                size_vector = (dataSize - sizeof(u_int32_t) - sizeof(size_t)) / sizeof(DType);
            } else {
                memcpy(&size_vector, dataIn + sizeof(u_int32_t), sizeof(size_t));
            }

            d = new DType[size_vector];

            for (int x = 0; x < size_vector; x++){
                memcpy(&d[x], dataIn + sizeof(u_int32_t) + sizeof(size_t) + (sizeof(DType)*x), sizeof(DType));
            }

            data.clear();

            for (int x = 0; x < size_vector; x++){
                data.push_back(d[x]);
            }

            strl = dataSize - sizeof(u_int32_t) - sizeof(size_t) -  (sizeof(DType) * data.size());
            Name.assign((char *)(dataIn + sizeof(u_int32_t) + sizeof(size_t) +  (sizeof(DType) * data.size())), strl);

            // Since we have changed the object contents, we must invalidate the old
            // serialized version if it exists. In fact we, may copy the given serialized
            // version of tbe new object to the buffer but we don't want to spend memory.
            if (serialized != NULL){
                delete [] serialized;
                serialized = NULL;
            }//end if
            delete[] d;
        }

        /**
        * @brief This method is deprecated, but still usefull for arboretum
        * template functions.
        * @deprecated This method is deprecated.
        * Use unserialize(const u_char *dataIn, u_int32_t dataSize) instead.
        * @copydoc unserialize(const u_char *dataIn, size_t dataSize).
        */
        void Unserialize(const u_char *dataIn, size_t dataSize = 0){

            unserialize(dataIn, dataSize);
        }

        /**
        * @copydoc unserialize(const u_char *dataIn, size_t dataSize).
        */
        void unserializeFromString(std::string dataIn){

            size_t size_vector;
            DType *d;

            // This is the reverse of Serialize(). So the steps are similar.
            // Remember, the format of the serizalized object is
            // +----------+-----------+--------+
            // | OID | Size | Vector Data []   |
            // +----------+-----------+--------+
            memcpy(&OID, dataIn.c_str(), sizeof(u_int32_t));
            memcpy(&size_vector, dataIn.c_str() + sizeof(u_int32_t), sizeof(size_t));
            d = new DType[size_vector];

            for (int x = 0; x < size_vector; x++){
                memcpy(&d[x], dataIn.c_str() + sizeof(u_int32_t) + sizeof(size_t) + (sizeof(DType)*x), sizeof(DType));
            }

            data.clear();

            for (int x = 0; x < size_vector; x++){
                data.push_back(d[x]);
            }

            //strl = dataSize - sizeof(u_int32_t) - sizeof(size_t) - (sizeof(DType) * data.size());
            //Name.assign((char *)(dataIn.c_str() + sizeof(u_int32_t) + sizeof(size_t) +  (sizeof(DType) * data.size())), size_vector);

            //Name.assign((char *)(dataIn.c_str() + sizeof(u_int32_t) + sizeof(size_t) +  (sizeof(DType) * data.size())), size_vector);

            // Since we have changed the object contents, we must invalidate the old
            // serialized version if it exists. In fact we, may copy the given serialized
            // version of tbe new object to the buffer but we don't want to waste memory.
            if (serialized != NULL){
                delete [] serialized;
                serialized = NULL;
            }//end if
        }

};

#endif
