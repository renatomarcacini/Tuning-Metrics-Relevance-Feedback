//---------------------------------------------------------------------------
// city.h - Implementation of the User Layer
//
// This file implements the 2 classes required by the SlimTree Library User
// Layer.
//
// TCity is the object which will be indexed by a metric tree. It abstracts a
// city. Each city has a name, a latitude and a longitude. TCity defines an
// interface to manipulate its information and also implements the stObject
// interface.
//
// TCityDistanceEvaluator implements the stMetricEvaluator interface. It will
// mesure the distance between 2 TCity instances.
//
// With these classes, it is possible to define and instantiate any metric tree
// defined by the SlimTree Library.
//
// Authors: Marcos Rodrigues Vieira (mrvieira@icmc.sc.usp.br)
//          Fabio Jun Takada Chino (chino@icmc.sc.usp.br)
// Copyright (c) 2002 GBDI-ICMC-USP
//---------------------------------------------------------------------------
#ifndef cityH
#define cityH

#include <cstdlib>
#include <cstdint>
#include <math.h>
#include <string>
#include <time.h>
#include <ostream>
#include <iostream>
#include <vector>

using namespace std;

// Metric Tree includes
#include <arboretum/stUtil.h>

#include<hermes/DistanceFunction.h>

//---------------------------------------------------------------------------
// Class TCity
//---------------------------------------------------------------------------
/**
* This class abstracts a city in a map. Each city has a name and a pair
* latitude/longitude.
*
* <P>In addition to data manipulation methods (such as GetLatitude(), GetName()
* and others), this class implements the stObject interface. This interface
* qualifies this object to be indexed by a metric tree implemented by GBDI
* SlimTree Library.
*
* <P>This interface requires no inheritance (because of the use of class
* templates in the Structure Layer) but requires the following methods:
*     - TCity() - A default constructor.
*     - Clone() - Creates a clone of this object.
*     - IsEqual() - Checks if this instance is equal to another.
*     - GetSerializedSize() - Gets the size of the serialized version of this object.
*     - Serialize() - Gets the serialzied version of this object.
*     - Unserialize() - Restores a serialzied object.
*
* <P>Since the array which contains the serialized version of the object must be
* created and destroyed by each object instance, this class will hold this array
* as a buffer of the serialized version of this instance. This buffer will be
* created only if required and will be invalidated every time the object changes
* its values.
*
* <P>The serialized version of the object will be created as follows:<BR>
* <CODE>
* +----------+--------------+<BR>
* | Vector<double> | Name[] |<BR>
* +----------+--------------+<BR>
* </CODE>
*
* <P>Latitude and Logitude are stored as doubles (2 64-bit IEEE floating point
* value) and Name[] is an array of chars with no terminator. Since Name[] has
* a variable size (associated with the name of the city), the serialized form
* will also have a variable number of bytes.
*
* @version 1.0
* @author Fabio Jun Takada Chino
*/
class TImage{
   public:
      /**
      * Default constructor. It creates a city with no name and longitude and
      * latitude set to 0. This constructor is required by stObject interface.
      */
      TImage(){
         Name = "";
         Features.clear();

         // Invalidate Serialized buffer.
         Serialized = NULL;
      }//end TCity

      /**
      * Creates a new city.
      *
      * @param name The name of the city.
      * @param latitude Latitude.
      * @param longitude Longitude.
      */
      TImage(const string name, vector<double> features){
         Name = name;
         Features = features;   
         // Invalidate Serialized buffer.
         Serialized = NULL;
      }//end TCity

      /**
      * Destroys this instance and releases all associated resources.
      */
      ~TImage(){

         // Does Serialized exist ?
         if (Serialized != NULL){
            // Yes! Dispose it!
            delete [] Serialized;
         }//end if
      }//end TCity

      int GetSize() const{
         return (int)Size;
      }

      size_t size(){
         return Features.size();
      }

      /**
      * Gets the latitude of the city.
      */
      vector<double> GetFeatures(){
         return Features;
      }

      /**
      * Gets the name of the city.
      */
      const string & GetName(){
         return Name;
      }//end GetName

      // The following methods are required by the stObject interface.
      /**
      * Creates a perfect clone of this object. This method is required by
      * stObject interface.
      *
      * @return A new instance of TCity wich is a perfect clone of the original
      * instance.
      */
      TImage * Clone(){
         return new TImage(Name, Features);
      }//end Clone

      /**
      * Checks to see if this object is equal to other. This method is required
      * by  stObject interface.
      *
      * @param obj Another instance of TCity.
      * @return True if they are equal or false otherwise.
      */
      bool IsEqual(TImage *obj){
         for (int x = 0; x < Features.size(); x++)
            if (this->Features[x] != obj->GetFeatures()[x])
               return false;
         return true;
      }//end IsEqual

      /**
      * Returns the size of the serialized version of this object in bytes.
      * This method is required  by  stObject interface.
      */
      size_t GetSerializedSize(){

         return (sizeof(size_t) + (sizeof(double) * Features.size()) + Name.length());
      }//end GetSerializedSize

      /**
      * Returns the serialized version of this object.
      * This method is required  by  stObject interface.
      *
      * @warning If you don't know how to serialize an object, this methos may
      * be a good example.
      */
      const uint8_t * Serialize();

      /**
      * Rebuilds a serialized object.
      * This method is required  by  stObject interface.
      *
      * @param data The serialized object.
      * @param datasize The size of the serialized object in bytes.
      * @warning If you don't know how to serialize an object, this methos may
      * be a good example.
      */
      void Unserialize (const uint8_t *data, size_t datasize);
   private:
      /**
      * The name of the city.
      */
      double Size; 

      string Name;

      vector<double> Features;

      /**
      * Serialized version. If NULL, the serialized version is not created.
      */
      uint8_t * Serialized;
};//end TMapPoint

//---------------------------------------------------------------------------
// Class TCityDistanceEvaluator
//---------------------------------------------------------------------------
/**
* This class implements a metric evaluator for TCity instances. It calculates
* the distance between cities by performing a euclidean distance between city
* coordinates (I know it is not accurate but is is only a sample!!!).
*
* <P>It implements the stMetricEvaluator interface. As stObject interface, the
* stMetricEvaluator interface requires no inheritance and defines 2 methods:
*     - GetDistance() - Calculates the distance between 2 objects.
*     - GetDistance2()  - Calculates the distance between 2 objects raised by 2.
*
* <P>Both methods are defined due to optmization reasons. Since euclidean
* distance raised by 2 is easier to calculate, It will implement GetDistance2()
* and use it to calculate GetDistance() result.
*
* @version 1.0
* @author Fabio Jun Takada Chino
*/
class TImageDistanceEvaluator : public DistanceFunction <TImage> {
   public:
      TImageDistanceEvaluator(){
      }
      double weight;

      void SetWeights(double w){
         weight = w;
      }

      double GetWeights(){
         return weight;
      }

      /**
      * Returns the distance between 2 cities. This method is required by
      * stMetricEvaluator interface.
      *
      * @param obj1 Object 1.
      * @param obj2 Object 2.
      */
      double GetDistance(TImage& obj1, TImage& obj2){
         cout << "CHAMEI COM PESO: " << weight << endl;
         return sqrt(GetDistance2(obj1, obj2));
      }//end GetDistance

      double getDistance(TImage& obj1, TImage& obj2){
         cout << "CHAMEI COM PESO: " << weight << endl;

         return sqrt(GetDistance2(obj1, obj2));
      }

      /**
      * Returns the distance between 2 cities raised by the power of 2.
      * This method is required by stMetricEvaluator interface.
      *
      * @param obj1 Object 1.
      * @param obj2 Object 2.
      */
      double GetDistance2(TImage& obj1, TImage& obj2){
         double value = 0;
         updateDistanceCount(); // Update Statistics
         
         vector<double> f1 = obj1.GetFeatures();
         vector<double> f2 = obj2.GetFeatures();
         
         for(int i=0; i < obj1.GetFeatures().size(); i++){
            value += (f1[i] - f2[i]) * (f1[i] - f2[i]);
         }

         return value;
      }//end GetDistance2





};//end TCityDistanceEvaluator

//---------------------------------------------------------------------------
// Output operator
//---------------------------------------------------------------------------
/**
* This operator will write a string representation of a city to an outputstream.
*/
ostream & operator << (ostream & out, TImage & image);

#endif //end myobjectH
