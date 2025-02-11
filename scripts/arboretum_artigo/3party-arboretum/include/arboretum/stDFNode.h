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
* This file defines the DFTree nodes.
*
* @version 1.0
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @author Joselene Marques (joselene@icmc.usp.br)
* @todo Review of documentation.
*/

#ifndef __STDFNODE_H
#define __STDFNODE_H

#include <arboretum/stPage.h>

#include <stdexcept>

// used to set GR Vector
#define STFOCUS 3

//-----------------------------------------------------------------------------
// Class stDFNode
//-----------------------------------------------------------------------------
/**
* This abstract class is the basic DFTree node. All classes that implement
* DFTree nodes must extend this class.
*
* <p>The main function of this class is to provide a way to identify a disk node
* and create the required node instance to manipulate the node.
*
* <p>The structure of Index Node follows:
* @image html DFnode.png "DF node structure"
*
* <p>The <b>Header</b> holds the information about the node itself.
*     - Type: Type of this node. It may be stDFNode::INDEX or stDFNode::LEAF.
*     - Occupation: Number of entries in this node.
*
* <p>The <b>Node Data</b> is the segment of the node which holds the particular information
* of each type of the node. This class does not know how this information is organized.
*
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @version 1.0
* @todo Documentation review.
* @see stDFIndexNode
* @see stDFLeafNode
* @ingroup DF
*/
class stDFNode{

   public:

      /**
      * Node type.
      */
      enum stDFNodeType{
         /**
         * ID of an index node.
         */
         INDEX = 0x4449, // In little endian "ID"

         /**
         * ID of a leaf node.
         */
         LEAF = 0x464C // In little endian "LF"
      };//end stDFNodeType

      /**
      * This method will dispose this instance and all associated resources.
      */
      virtual ~stDFNode(){
      }//end ~stDFNode()

      /**
      * Returns the type of this DFTree node (Leaf or Index).
      *
      * @return the type of node.
      * @see stNodeType
      */
      u_int16_t GetNodeType(){
         return Header->Type;
      }//end GetNodeType

      /**
      * Returns the associated page.
      *
      * @return The associated page.
      */
      stPage * GetPage(){
         return Page;
      }//end GetPage

      /**
      * Returns the ID of the associated page.
      *
      * @return The ID of the associated page.
      */
      u_int32_t GetPageID(){
         return Page->GetPageID();
      }//end GetPage

      /**
      * This is a virtual method that defines a interface for the instantiate
      * the correct specialization of this class.
      *
      * @param page The instance of stPage.
      */
      static stDFNode * CreateNode(stPage * page);

      /**
      * Returns the number of entries in this node.
      *
      * @return the number of entries.
      * @see GetEntry()
      * @see GetObject()
      * @see GetObjectSize()
      */
      u_int32_t GetNumberOfEntries(){
         return this->Header->Occupation;
      }//end GetNumberOfEntries

      /**
      * This is a virtual method that defines a interface in the insertion of a
      * new Object in a Node.
      *
      * @param size The size of the object in bytes.
      * @param object The object data.
      * @warning The parameter size is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return The position in the vector Entries.
      * @see RemoveObject()
      */
      virtual int AddEntry(u_int32_t size, const unsigned char * object) = 0;

      /**
      * Gets the serialized object. Use GetObjectSize to determine the size of
      * the object.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return A pointer to the serialized object.
      * @see GetObjectSize()
      */
      virtual const unsigned char * GetObject(u_int32_t idx) = 0;

      /**
      * Returns the size of the object in bytes. Use GetObject() to get the
      * object data.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return The size of the serialized object.
      * @see GetObject()
      */
      virtual u_int32_t GetObjectSize(u_int32_t idx) = 0;

      /**
      * Returns the minimum radius of this node.
      */
      virtual double GetMinimumRadius() = 0;

      /**
      * Remove All entries.
      */
      void RemoveAll(){

         #ifdef __stDEBUG__
         u_int16_t type;
         type = Header->Type;
         Page->Clear();
         Header->Type = type;
         #else
         this->Header->Occupation = 0;
         #endif //__stDEBUG__
      }//end RemoveAll

      /**
      * Returns the total number of objects in the subtree.
      */
      virtual u_int32_t GetTotalObjectCount() = 0;

      /**
      * Returns the global overhead of a DF node (header size) in bytes.
      */
      static u_int32_t GetGlobalOverhead(){
         return sizeof(stDFNodeHeader);
      }//end GetGlobalOverhead()
      
   protected:
      /**
      * This is the structure of the Header of a DFTree node.
      */
      #pragma pack(1)
      typedef struct stDFNodeHeader{
         /**
         * Node type.
         */
         u_int16_t Type;

         /**
         * Number of entries.
         */
         u_int32_t Occupation;
      } stDFNodeHeader; //end stHeader
      #pragma pack()

      /**
      * Header of this page.
      */
      stDFNodeHeader * Header;

      /**
      * The page related with this class.
      */
      stPage * Page;

      /**
      * Creates a new instance of this class.
      *
      * @param page An instance of stPage.
      */
      stDFNode(stPage * page){
         this->Page = page;
         Header = (stDFNodeHeader *)(this->Page->GetData());
      }//end stDFNode
};//end stDFNode

//-----------------------------------------------------------------------------
// Class stDFIndexNode
//-----------------------------------------------------------------------------
/**
* This class implements the index node of the DFTree.
*
* <P>The DFTree index node...
*
* <P>The structure of Index Node follows:
* @image html indexnode.png "Index node structure"
*
* <P>The <b>Header</b> holds the information about the node itself.
*     - Type: Type of this node. It is always stDFNode::INDEX (0x4449).
*     - Occupation: Number of entries in this node.
*
* <P>The <b>Entry</b> holds the information of the link to the other node.
*  - PageID: The identifier of the page which holds the root of the sub tree.
*       - Distance: The distance of this object from the representative object.
*       - NEntries: Number of objects in the sub tree.
*       - Radius: Radius of the sub tree.
*       - Offset: The offset of the object in the page. DO NOT MODIFY ITS VALUE.
*
* <P>The <b>Object</b> is an array of bytes that holds the information required
* to rebuild the original object.
*
* @version 1.0
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @todo Documentation review.
* @see stDFNode
* @see stDFLeafNode
* @ingroup DF
*/
// +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
// | Type | Occupation | PgID0 | Dist0 | GRDist0 | NEnt0 | Radius0 | OffSet0 |...|PgIDn | Distn |  GRDistn | NEntn | Radiusn | OffSetn | <-- blankspace --> |Objn |...|Obj0 |
// +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
class stDFIndexNode: public stDFNode{

   public:

      /**
      * This type represents a DF tree index node entry.
      */

      #pragma pack(1)
      typedef struct stDFIndexEntry{
         /**
         * ID of the page.
         */
         u_int32_t PageID;

         /**
         * Distance from the representative.
         */
         double Distance;

         /**
         ** Distance from the global representatives
         **/
         double * FieldDistance;

         /**
         * Number of entries in the sub-tree.
         */
         u_int32_t NEntries;

         /**
         * Radius of the sub-tree.
         */
         double Radius;

         /**
         * Offset of the object.
         * @warning NEVER MODIFY THIS FIELD. YOU MAY DAMAGE THE STRUCTURE OF
         * THIS NODE.
         */
         u_int32_t Offset;
      } stDFIndexEntry; //end stIndexEntry
      #pragma pack()

      /**
      * Creates a new instance of this class. The parameter <i>page</i> is an
      * instance of stPage that hold the node data.
      *
      * <P>The parameter <i>create</i> tells to stIndexPage what operation will
      * be performed. True means that the page will be initialized and false
      * means that the page will be used as it is. The default value is false.
      *
      * @param page The page that hold the data of this node.
      * @param create The operation to be performed.
      */
      stDFIndexNode(stPage * page, bool create = false);

      /**
      * Returns the reference of the desired entry. You may use this method to
      * read and modify the entry information.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return stIndexEntry the reference of the desired entry.
      * @see stLeafEntry
      * @see GetNumberOfEntries()
      * @see GetObject()
      * @see GetObjectSize()
      */
      stDFIndexEntry & GetIndexEntry(u_int32_t idx){
         #ifdef __stDEBUG__
         if (idx >= GetNumberOfEntries()){
            throw std::logic_error("idx value is out of range.");
         }//end if
         #endif //__stDEBUG__

         return Entries[idx];
      }//end GetIndexEntry

      /**
      * Adds a new entry to this node. This method will return the id of the new
      * node or a negative value for failure.
      *
      * <P>This method will fail if there is not enough space to hold the
      * new object.
      *
      * <P>If you have added a new entry successfully, you may edit the entry
      * fields using the method GetEntry().
      *
      * @param size The size of the object in bytes.
      * @param object The object data.
      * @warning The parameter size is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return The position in the vector Entries.
      * @see RemoveEntry()
      * @see GetEntry()
      */
      virtual int AddEntry(u_int32_t size, const unsigned char * object);

      /**
      * Returns the entry id that hold the representaive object.
      */
      int GetRepresentativeEntry();

      /**
      * Gets the serialized object. Use GetObjectSize to determine the size of
      * the object.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation.
      * unless __stDEBUG__ is defined at compile time.
      * @return A pointer to the serialized object.
      * @see GetObjectSize()
      */
      const unsigned char * GetObject(u_int32_t idx);

      /**
      * Returns the size of the object. Use GetObject() to get the object data.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return The size of the serialized object.
      * @see GetObject()
      */
      u_int32_t GetObjectSize(u_int32_t idx);

      /**
      * Removes an entry from this object.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @see GetObjectSize()
      */
      void RemoveEntry(u_int32_t idx);

      /**
      * Returns the minimum radius of this node.
      */
      virtual double GetMinimumRadius();

      /**
      * Returns the total number of objects in the subtree.
      */
      virtual u_int32_t GetTotalObjectCount();

      /**
      * Returns the overhead of each index node entry in bytes.
      */
      static u_int32_t GetIndexEntryOverhead(){
         return sizeof(stDFIndexEntry);
      }//end GetIndexEntryOverhead()

   private:

      /**
      * Entry pointer
      */
      stDFIndexEntry * Entries;

      /**
      * Returns the amount of the free space in this node.
      */
      u_int32_t GetFree();

};//end stDFIndexPage

//-----------------------------------------------------------------------------
// Class stDFLeafNode
//-----------------------------------------------------------------------------
/**
* This class implements the Leaf node of the DFTree.
*
* <P>The DFTree leaf node...
* The structure of Leaf Node follows:
* @image html leafnode.png "Leaf node structure"
*
* <P>The <b>Header</b> holds the information about the node itself.
*     - Type: Type of this node. It is always stDFNode::LEAF (0x464C).
*     - Occupation: Number of entries in this node.
*
* <P>The <b>Entry</b> holds the information of the link to the other node.
*       - Distance: The distance of this object from the representative object.
*       - Offset: The offset of the object in the page. DO NOT MODIFY ITS VALUE.
*
* <P>The <b>Object</b> is an array of bytes that holds the information required
* to rebuild the original object.
*
* @version 1.0
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @todo Documentation review.
* @see stDFNode
* @see stDFIndexNode
* @see stDFMemLeafNode
* @ingroup DF
*/
// +--------------------------------------------------------------------------------------------------------------------+
// | Type | Occupation | Dist0 | GRDist0 | OffSet0 | Distn | GRDistn | OffSetn | <-- blankspace --> |Objn |...|Obj0 |
// +--------------------------------------------------------------------------------------------------------------------+
class stDFLeafNode: public stDFNode{

   public:

      /**
      * This type represents a DF tree leaf node entry.
      */
      #pragma pack(1)
      typedef struct stDFLeafEntry{
         /**
         * Distance from the representative.
         */
         double Distance;

         /**
         ** Distance from the global representatives
         **/
         double * FieldDistance;

         /**
         * Offset of the object.
         * @warning NEVER MODIFY THIS FIELD. YOU MAY DAMAGE THE STRUCTURE OF
         * THIS NODE.
         */
         u_int32_t Offset;
      } stDFLeafEntry; //end stLeafEntry
      #pragma pack()

      /**
      * Creates a new instance of this class. The paramenter <i>page</i> is an
      * instance of stPage that hold the node data.
      *
      * <P>The parameter <i>create</i> tells to stLeafPage what operation will
      * be performed. True means that the page will be initialized and false
      * means that the page will be used as it is. The default value is false.
      *
      * @param page The page that hold the data of this node.
      * @param create The operation to be performed.
      */
      stDFLeafNode(stPage * page, bool create = false);

      /**
      * Returns the reference of the desired leaf entry. You may use this method to
      * read and modify the leaf entry information.
      *
      * @param id The id of the leaf entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return stLeafEntry the reference of the desired entry.
      * @see stLeafEntry
      * @see GetNumberOfEntries()
      * @see GetObject()
      * @see GetObjectSize()
      */
      stDFLeafEntry &GetLeafEntry(u_int32_t idx){
         #ifdef __stDEBUG__
         if (idx >= GetNumberOfEntries()){
            throw std::logic_error("idx value is out of range.");
         }//end if
         #endif //__stDEBUG__

         return Entries[idx];
      }//end GetLeafEntry

      /**
      * Adds a new entry to this node. This method will return the id of the new
      * node or a negative value for failure.
      *
      * <P>This method will fail if there is not enough space to hold the
      * new object.
      *
      * <P>If you have added a new entry successfully, you may edit the entry
      * fields using the method GetEntry().
      *
      * @param size The size of the object in bytes.
      * @param object The object data.
      * @warning The parameter size is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return The position in the vector Entries.
      * @see RemoveEntry()
      * @see GetEntry()
      */
      virtual int AddEntry(u_int32_t size, const unsigned char * object);

      /**
      * Returns the entry id that hold the representative object.
      */
      int GetRepresentativeEntry();

      /**
      * Gets the serialized object. Use GetObjectSize() to determine the size of
      * the object.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return A pointer to the serialized object.
      * @see GetObjectSize()
      */
      const unsigned char * GetObject(u_int32_t idx);

      /**
      * Returns the size of the object. Use GetObject() to get the object data.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return The size of the serialized object.
      * @see GetObject()
      */
      u_int32_t GetObjectSize(u_int32_t idx);

      /**
      * Removes an entry from this object.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return A pointer to the serialized object.
      * @see GetObjectSize()
      * @todo This method is not implemented yet.
      */
      void RemoveEntry(u_int32_t idx);

      /**
      * Returns the minimum radius of this node.
      */
      virtual double GetMinimumRadius();

      /**
      * Returns the total number of objects in the subtree.
      */
      virtual u_int32_t GetTotalObjectCount(){
         return GetNumberOfEntries();
      }//end GetTotalObjectCount()

      /**
      * Returns the overhead of each leaf node entry in bytes.
      */
      static u_int32_t GetLeafEntryOverhead(){
         return sizeof(stDFLeafEntry);
      }//end GetLeafEntryOverhead()

      /**
      * Returns the GR distance of a given entry.
      *
      * @param idx The object index.
      * @return The GR Vector
      */
      double * GetFieldDistance(u_int32_t idx){
         return Entries[idx].FieldDistance;
      }//end GetFieldDistance

   private:

      /**
      * Entry pointer
      */
      stDFLeafEntry * Entries;

      /**
      * Returns the amount of the free space in this node.
      */
      u_int32_t GetFree();
      
};//end stDFLeafNode
                   
//-----------------------------------------------------------------------------
// Class stDFMemLeafNode
//-----------------------------------------------------------------------------
/**
* This class template implements a memory shell for a stDFLeafNode instance.
* It implements a memory optimized leaf node wich simulates the physical
* occupation of the original node.
*
* <p>Since all objects are stored using their implementation (not the serialized
* form), this class is very useful to optimize heavy object manipulations such
* as SlimDown and MinMax algorithms.
*
* <p>By the other side, this class will unserialize all objects of the node
* without exceptions. Due to that, the use of this class is not recommended
* for procedures with low rate of object unserializations/serializations.
* Another disadvantage of this class is the potential high memory usage.
*
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @see stDFLeafNode
* @ingroup DF
*/
template < class ObjectType >
class stDFMemLeafNode{

   public:

      /**
      * Creates a new stDFMemLeafNode instance from a stDFLeafNode.
      *
      * @param leafNode The leafNode to be insert.
      * @warning This method will remove entries from leafNode.
      */
      stDFMemLeafNode(stDFLeafNode * leafNode);

      /**
      * Release the resources of this class template and construct a new
      * DFLeafNode that has all entries in this instance.
      *
      * @note The resulting node will have all objects sorted according to
      * their distance from the representative object (in ascendent order).
      * @return A new instance of stDFLeafNode.
      */
      stDFLeafNode * ReleaseNode();

      /**
      * Adds an object to this node.
      *
      * <p>All entries are sorted in crescent order of distance to allow complex
      * object manipulation such as SlimDown.
      *
      * @param obj An object to be insert.
      * @param distance A correspondent distance of the new entry.
      * @return True for success or false otherwise.
      */
      bool Add(ObjectType * obj, double distance);

      /**
      * Returns the number of entries.
      */
      u_int32_t GetNumberOfEntries(){
         return this->NumEntries;
      }//end GetNumberOfObjects

      /**
      * Returns the object at position idx.
      *
      * @param idx Index of the object.
      * @return An object at position idx.
      * @warning Do not modify/dispose the object using this method.
      */
      ObjectType * ObjectAt(u_int32_t idx){
         #ifdef __stDEBUG__
            if (idx >= this->numEntries){
               throw std::logic_error("idx value is out of range.");
            }//end if
         #endif //__stDEBUG__
         return Entries[idx].Object;
      }//end ObjectAt

      /**
      * Returns a pointer to the last object of the node. This object is special
      * because it has the largest distance from the representative.
      *
      * @return A pointer to the last object of this node.
      */
      ObjectType * LastObject(){
         return Entries[this->numEntries-1].Object;
      }//end LastObject

      /**
      * Returns a pointer to the representative object of the node.
      *
      * @return A pointer to the representative object of this node.
      */
      ObjectType * RepObject(){
         return Entries[0].Object;
      }//end RepObject

      /**
      * Returns the distance associated with a given entry.
      *
      * @param idx Index of the object.
      * @return The distance.
      */
      double DistanceAt(u_int32_t idx){
         #ifdef __stDEBUG__
            if (idx >= this->numEntries){
               throw std::logic_error("idx value is out of range.");
            }//end if
         #endif //__stDEBUG__

         return Entries[idx].Distance;
      }//end DistanceAt

      /**
      * Returns the distance of the last object. It is a synonym of
      * GetMinimumRadius().
      */
      double Ladouble(){
         return Entries[this->numEntries-1].Distance;
      }//end Ladouble

      /**
      * Removes an entry in idx from this node.
      *
      * @param idx Index of the object.
      * @return The removed object.
      */
      ObjectType * Remove(u_int32_t idx);

      /**
      * Remove the last object from this tree.
      */
      ObjectType * PopObject();

      /**
      * Returns true if there is enough free space to add the given object.
      *
      * @return True for success or false otherwise.
      */
      bool CanAdd(ObjectType * obj){
         // Does it fit ?
         if (obj->GetSerializedSize() + stDFLeafNode::GetLeafEntryOverhead() +
             this->UsedSize > this->MaximumSize){
            // No, it doesn't.
            return false;
         }//end if
         // yes, it does.
         return true;
      }//end CanAdd

      /**
      * Returns the minimum radius of this node.
      *
      * @return The radius.
      */
      double GetMinimumRadius(){
         return Entries[this->numEntries-1].Distance;
      }//end GetMinimumRadius                    

      /**
      * Returns the free space of this node.
      *
      * @return The free space.
      */
      u_int32_t GetFreeSize(){
         return (this->maximumSize - this->usedSize);
      }//end GetFreeSize

   private:
   
      /**
      * This struct holds all information required to store an leaf node entry.
      */
      struct stDFMemNodeEntry{
         /**
         * Object.
         */
         ObjectType * Object;

         /**
         * Distance from representative.
         */
         double Distance;
      };

      /**
      * Number of entries in this node.
      */
      u_int32_t NumEntries;
      
      /**
      * Current capacity of this node.
      */
      u_int32_t Capacity;
      
      /**
      * Entries of this node.
      */
      stDFMemNodeEntry * Entries;
      
      /**
      * Maximum size of the node in bytes.
      */
      u_int32_t MaximumSize;
      
      /**
      * Used size of the node in bytes.
      */
      u_int32_t UsedSize;
      
      /**
      * Point to the source leafNode.
      */
      stDFLeafNode * SrcLeafNode; 

      /**
      * Returns the insert position for a given distance.
      *
      * @param distance The distance of the object to be insert.
      * @return The position that will be insert.
      * @todo This code needs optimizations. I suggest a binary search
      * implementation.
      */
      int InsertPosition(double distance); 
      
      /**
      * Resizes the entries vector to hold more entries. It will at 16
      * positions to the capacity for each call.
      */
      void Resize(u_int32_t incSize = 16);
      
};//end stDFMemLeafNode

// Include implementation
#include "stDFNode-inl.h"

#endif //__STDFNODE_H
