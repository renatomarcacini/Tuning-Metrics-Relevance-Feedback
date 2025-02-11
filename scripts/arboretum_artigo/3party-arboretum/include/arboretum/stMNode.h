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
* This file defines the MTree nodes.
*
* @version 1.0
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/

#ifndef __STMNODE_H
#define __STMNODE_H

#include <arboretum/stPage.h>

#include <stdexcept>

//-----------------------------------------------------------------------------
// Class stMNode
//-----------------------------------------------------------------------------
/**
* This abstract class is the basic MTree node. All classes that implement
* MTree nodes must extend this class.
*
* <p>The main function of this class is to provide a way to identify a disk node
* and create the required node instance to manipulate the node.
*
* <p>The structure of Index Node follows:
* @image html slimnode.png "M node structure"
*
* <p>The <b>Header</b> holds the information about the node itself.
*     - Type: Type of this node. It may be stMNode::INDEX or stMNode::LEAF.
*     - Occupation: Number of entries in this node.
*
* <p>The <b>Node Data</b> is the segment of the node which holds the particular information
* of each type of the node. This class does not know how this information is organized.
*
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @version 1.0
* @todo Documentation review.
* @see stMIndexNode
* @see stMLeafNode
* @ingroup slim
*/
class stMNode{
   public:
      /**
      * Node type.
      */
      enum stMNodeType{
         /**
         * ID of an index node.
         */
         INDEX = 0x4449, // In little endian "ID"

         /**
         * ID of a leaf node.
         */
         LEAF = 0x464C // In little endian "LF"
      };//end stMNodeType

      /**
      * This method will dispose this instance and all associated resources.
      */
      virtual ~stMNode(){
      }//end ~stMNode()

      /**
      * Returns the type of this MTree node (Leaf or Index).
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
      static stMNode * CreateNode(stPage * page);

      /**
      * Returns the number of entries in this node.
      *
      * @return the number of entries.
      * @see GetEntry()
      * @see GetObject()
      * @see GetObjectSize()
      */
      int GetNumberOfEntries(){
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
      virtual const unsigned char * GetObject(int id)=0;

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
      virtual u_int32_t GetObjectSize(int id)=0;

      /**
      * Returns the minimum radius of this node.
      */
      virtual double GetMinimumRadius()=0;

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
      * Returns the global overhead of a slim node (header size) in bytes.
      */
      static u_int32_t GetGlobalOverhead(){
         return sizeof(stMNodeHeader);
      }//end GetGlobalOverhead()
      
   protected:
      /**
      * This is the structure of the Header of a MTree node.
      */
      #pragma pack(1)
      typedef struct stMNodeHeader{
         /**
         * Node type.
         */
         u_int16_t Type;

         /**
         * Number of entries.
         */
         u_int32_t Occupation;
      } stMNodeHeader; //end stHeader
      #pragma pack()

      /**
      * Header of this page.
      */
      stMNodeHeader * Header;

      /**
      * The page related with this class.
      */
      stPage * Page;

      /**
      * Creates a new instance of this class.
      *
      * @param page An instance of stPage.
      */
      stMNode(stPage * page){
         this->Page = page;
         Header = (stMNodeHeader *)(this->Page->GetData());
      }//end stMNode
};//end stMNode

//-----------------------------------------------------------------------------
// Class stMIndexNode
//-----------------------------------------------------------------------------
/**
* This class implements the index node of the MTree.
*
* <P>The MTree index node...
*
* <P>The structure of Index Node follows:
* @image html indexnode.png "Index node structure"
*
* <P>The <b>Header</b> holds the information about the node itself.
*     - Type: Type of this node. It is always stMNode::INDEX (0x4449).
*     - Occupation: Number of entries in this node.
*
* <P>The <b>Entry</b> holds the information of the link to the other node.
*  - PageID: The identifier of the page which holds the root of the sub tree.
*       - Distance: The distance of this object from the representative object.
*       - Radius: Radius of the sub tree.
*       - Offset: The offset of the object in the page. DO NOT MODIFY ITS VALUE.
*
* <P>The <b>Object</b> is an array of bytes that holds the information required to rebuild
* the original object.
*
* @version 1.0
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @todo Documentation review.
* @see stMNode
* @see stMLeafNode
* @ingroup slim
*/
// +-----------------------------------------------------------------------------------------------------------------------------------+
// | Type | Occupation | PgID0 | Dist0 | Radius0 | OffSet0 |...|PgIDn | Distn | Radiusn | OffSetn | <-- blankspace --> |Objn |...|Obj0 |
// +-----------------------------------------------------------------------------------------------------------------------------------+
class stMIndexNode: public stMNode{
   public:
      /**
      * This type represents a slim tree index node entry.
      */
      #pragma pack(1)
      typedef struct stMIndexEntry{
         /**
         * ID of the page.
         */
         u_int32_t PageID;

         /**
         * Distance from the representative.
         */
         double Distance;

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
      } stMIndexEntry; //end stIndexEntry
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
      stMIndexNode(stPage * page, bool create = false);

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
      stMIndexEntry & GetIndexEntry(int id){
         #ifdef __stDEBUG__
         if ((id < 0) && (id >= GetNumberOfEntries())){
            throw std::logic_error("id value is out of range.");
         }//end if
         #endif //__stDEBUG__

         return Entries[id];
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
      const unsigned char * GetObject(int id);

      /**
      * Returns the size of the object. Use GetObject() to get the object data.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return The size of the serialized object.
      * @see GetObject()
      */
      u_int32_t GetObjectSize(int id);

      /**
      * Removes an entry from this object.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @see GetObjectSize()
      */
      void RemoveEntry(int id);

      /**
      * Returns the minimum radius of this node.
      */
      virtual double GetMinimumRadius();

      /**
      * Returns the overhead of each index node entry in bytes.
      */
      static u_int32_t GetIndexEntryOverhead(){
         return sizeof(stMIndexEntry);
      }//end GetIndexEntryOverhead()

   private:

      /**
      * Entry pointer
      */
      stMIndexEntry * Entries;

      /**
      * Returns the amount of the free space in this node.
      */
      u_int32_t GetFree();

};//end stMIndexPage

//-----------------------------------------------------------------------------
// Class stMLeafNode
//-----------------------------------------------------------------------------
/**
* This class implements the Leaf node of the MTree.
*
* <P>The MTree leaf node...
* The structure of Leaf Node follows:
* @image html leafnode.png "Leaf node structure"
*
* <P>The <b>Header</b> holds the information about the node itself.
*     - Type: Type of this node. It is always stMNode::LEAF (0x464C).
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
* @see stMNode
* @see stMIndexNode
* @ingroup slim
*/
// +--------------------------------------------------------------------------------------------+
// | Type | Occupation | Dist0 | OffSet0 | Distn | OffSetn | <-- blankspace --> |Objn |...|Obj0 |
// +--------------------------------------------------------------------------------------------+
class stMLeafNode: public stMNode{
   public:
      /**
      * This type represents a slim tree leaf node entry.
      */
      #pragma pack(1)
      typedef struct stMLeafEntry{
         /**
         * Distance from the representative.
         */
         double Distance;

         /**
         * Offset of the object.
         * @warning NEVER MODIFY THIS FIELD. YOU MAY DAMAGE THE STRUCTURE OF
         * THIS NODE.
         */
         u_int32_t Offset;
      } stMLeafEntry; //end stLeafEntry
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
      stMLeafNode(stPage * page, bool create = false);

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
      stMLeafEntry &GetLeafEntry(int id){
         #ifdef __stDEBUG__
         if ((id < 0) && (id >= GetNumberOfEntries())){
            throw std::logic_error("id value is out of range.");
         }//end if
         #endif //__stDEBUG__

         return Entries[id];
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
      const unsigned char * GetObject(int id);

      /**
      * Returns the size of the object. Use GetObject() to get the object data.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return The size of the serialized object.
      * @see GetObject()
      */
      u_int32_t GetObjectSize(int id);

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
      void RemoveEntry(int id);

      /**
      * Returns the minimum radius of this node.
      */
      virtual double GetMinimumRadius();

      /**
      * Returns the overhead of each leaf node entry in bytes.
      */
      static u_int32_t GetLeafEntryOverhead(){
         return sizeof(stMLeafEntry);
      }//end GetLeafEntryOverhead()
      
   private:
      /**
      * Entry pointer
      */
      stMLeafEntry * Entries;

      /**
      * Returns the amount of the free space in this node.
      */
      u_int32_t GetFree();
};//end stMLeafNode

#endif //__STMNODE_H
