//---------------------------------------------------------------------------
// city.cpp - Implementation of the User Layer
//
// In this file we have the implementation of TCity::Serialize(),
// TCity::Unserialize() and an output operator for TCity (which is not required
// by user layer).
//
// Authors: Marcos Rodrigues Vieira (mrvieira@icmc.sc.usp.br)
//         Fabio Jun Takada Chino (chino@icmc.sc.usp.br)
// Copyright (c) 2003 GBDI-ICMC-USP
//---------------------------------------------------------------------------
#pragma hdrstop
#include "image.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
// Class TCity
//---------------------------------------------------------------------------
/**
* Returns the serialized version of this object.
* This method is required  by  stObject interface.
* @warning If you don't know how to serialize an object, this methos may
* be a good example.
*/
const uint8_t * TImage::Serialize(){
   double * d;

   if (Serialized == NULL){

      Serialized = new uint8_t[GetSerializedSize()];

      size_t size = GetFeatures().size();
      memcpy(Serialized, &size, sizeof(size_t));
      for (int x = 0; x < size; x++){
         memcpy(Serialized + sizeof(size_t)+(sizeof(double)*x), &Features[x], sizeof(double));
      }
      memcpy(Serialized + sizeof(size_t) + (sizeof(double) * Features.size()), Name.c_str(), Name.length());
   }//end if

   return Serialized;
}//end TCity::Serialize

/**
* Rebuilds a serialized object.
* This method is required  by  stObject interface.
*
* @param data The serialized object.
* @param datasize The size of the serialized object in bytes.
* @warning If you don't know how to serialize an object, this methos may
* be a good example.
*/
void TImage::Unserialize(const uint8_t *data, size_t datasize){
   size_t strl;
   size_t size;
   
   memcpy(&size, data, sizeof(size_t));

   double * d;
   d = new double[size];
   for (int x = 0; x < size; x++){
      memcpy(&d[x], data + sizeof(size_t) + (sizeof(double)*x), sizeof(double));
   }
   
   Features.clear();
   for (int x = 0; x < size; x++){
      Features.push_back(d[x]);
   }
   strl = datasize - sizeof(size_t) - (sizeof(double) * size);
   Name.assign((char *)(data + sizeof(size_t) + (sizeof(double) * size)), strl);

   if (Serialized != NULL){
      delete [] Serialized;
      Serialized = NULL;
   }//end if
}//end TCity::Unserialize

//---------------------------------------------------------------------------
// Output operator
//---------------------------------------------------------------------------
/**
* This operator will write a string representation of a city to an outputstream.
*/
ostream & operator << (ostream & out, TImage & image){

   out << "[image=" << image.GetName();
   return out;
}//end operator <<

