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
* This file is the implementation of stGHTree methods.
* $Author: seraphim $
*
* @author Ives Renê Venturini Pola (ives@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/

//------------------------------------------------------------------------------
template <class ObjectType, class EvaluatorType>
stGHTree<ObjectType, EvaluatorType>::stGHTree(stPageManager * pageman):
      stMetricTree<ObjectType, EvaluatorType>(pageman){

   // Initialize fields
   Header = NULL;
   HeaderPage = NULL;
   // Load header.
   LoadHeader();

   // Will I create or load the tree ?
   if (this->myPageManager->IsEmpty()){
      DefaultHeader();
   }//end if
}//end stGHTree<ObjectType><EvaluatorType>::stGHTree

//------------------------------------------------------------------------------
template <class ObjectType, class EvaluatorType>
void stGHTree<ObjectType, EvaluatorType>::LoadHeader(){
   if (HeaderPage != NULL){
      this->myPageManager->ReleasePage(HeaderPage);
   }//end if

   // Load and set the header.
   HeaderPage = this->myPageManager->GetHeaderPage();
   if (HeaderPage->GetPageSize() < sizeof(stGHTreeHeader)){
      #ifdef __stDEBUG__
         throw std::logic_error("The page size is too small. Increase it!\n");
      #endif //__stDEBUG__
   }//end if

   this->Header = (stGHTreeHeader *) this->HeaderPage->GetData();
   this->HeaderUpdate = false;
}//end stGHTree<ObjectType, EvaluatorType>::LoadHeader

//------------------------------------------------------------------------------
template <class ObjectType, class EvaluatorType>
void stGHTree<ObjectType, EvaluatorType>::DefaultHeader(){
   // Clear header page.
   HeaderPage->Clear();
   // Default values
   Header->Magic[0] = 'G';
   Header->Magic[1] = 'H';
   Header->Magic[2] = '-';
   Header->Magic[3] = '3';
   Header->Height = 0;
   Header->ObjectCount = 0;
   Header->NodeCount = 0;
   this->SetRoot(0);
   // Notify modifications
   HeaderUpdate = true;
}//end stGHTree<ObjectType, EvaluatorType>::DefaultHeader

//------------------------------------------------------------------------------
template <class ObjectType, class EvaluatorType>
bool stGHTree<ObjectType, EvaluatorType>::Add(tObject * obj){
   // The object will be added in the page witch have room for it.
   // When the page is full, this method will create a new page and link
   // it as the clild of the nearest object in the current node.
   stPage * currPage;
   stGHNode * currNode;
   stGHNode * ParentNode;
   stPage * ParentPage;
   tObject * tmp;
   int insertIdx;
   u_int32_t height;
   u_int32_t nextPageID;
   u_int32_t subtree;
   bool stop;
   double distanceLeft, distanceRight;

   // Does it fit ?
   if (obj->GetSerializedSize() >
       this->myPageManager->GetMinimumPageSize() - sizeof(unsigned char) -
       sizeof(double) - sizeof(u_int32_t) - sizeof(u_int32_t)){
      return false;
   }//end if

   // There is a root node.
   // Allocate a new object.
   tmp = new tObject();
         
   // Store the parent.
   ParentPage = NULL;
   ParentNode = NULL;
   nextPageID = this->GetRoot();
   stop = false;
   height = 0;

   // Search process.
   while (!stop){
      // What happened?
      if (!nextPageID){
         // There is not a subtree.
         // Create a node and insert the new object.
         currPage = this->NewPage();
         currNode = new stGHNode(currPage, true);
         // Add the new object.
         // Does not test it, because there is room for it.
         currNode->AddEntry(obj->GetSerializedSize(),
                            obj->Serialize());
         // Set the subtree.
         if (ParentNode == NULL){
            // It is the root node.
            // Update the header statistics.
            this->SetRoot(currPage->GetPageID());
            height = 1;
         }else{
            // Link the two nodes.
            ParentNode->SetChildPageID(subtree, currPage->GetPageID());
            // Write and clean the parent.
            this->myPageManager->WritePage(ParentPage);
            delete ParentNode;
            this->myPageManager->ReleasePage(ParentPage);
         }//end if
         // Write and clean the current node.
         this->myPageManager->WritePage(currPage);
         delete currNode;
         this->myPageManager->ReleasePage(currPage);
         // Stop the process.
         stop = true;
      }else{
         // Get the next node.
         currPage = this->myPageManager->GetPage(nextPageID);
         currNode = new stGHNode(currPage);

         // Test if there is space to store in the current node.
         if (currNode->GetNumberOfEntries() == 1){
            // Yes. Add the new object.
            // Does not test it, because there is room for it.
            currNode->AddEntry(obj->GetSerializedSize(),
                               obj->Serialize());
            // Get the object object.
            tmp->Unserialize(currNode->GetObject(0),
                             currNode->GetObjectSize(0));
            // Calculate the distance.
            currNode->SetDistance(this->myMetricEvaluator->GetDistance(*tmp, *obj));
            // Write and clean.
            this->myPageManager->WritePage(currPage);
            delete currNode;
            this->myPageManager->ReleasePage(currPage);
            // Stop the process.
            stop = true;
         }else{
            // There is not space to store the new object.
            // Choose the best subtree.
            // Get the left object.
            tmp->Unserialize(currNode->GetObject(0),
                             currNode->GetObjectSize(0));
            // Calculate the distance.
            distanceLeft = this->myMetricEvaluator->GetDistance(*tmp, *obj);
            // Get the right object.
            tmp->Unserialize(currNode->GetObject(1),
                             currNode->GetObjectSize(1));
            // Calculate the distance.
            distanceRight = this->myMetricEvaluator->GetDistance(*tmp, *obj);
            // Choose the nearest subtree.
            if (distanceLeft <= distanceRight){
               // The left subtree is the best.
               nextPageID = currNode->GetChildPageID(0);
               subtree = 0;
            }else{
               // The right subtree is the best.
               nextPageID = currNode->GetChildPageID(1);
               subtree = 1;
            }//end if
            // Analize the subtree.
            height++;
            // Store the parent reference.
            ParentPage = currPage;
            ParentNode = currNode;
         }//end if
      }//end if
   }//end while
   // Clean the mess.
   delete tmp;
   // Test if the height increased.
   if (height > Header->Height){
      Header->Height = height;
   }//end if
   Header->ObjectCount++;
   // Notify modifications
   HeaderUpdate = true;
   return true;
}//end stGHTree<ObjectType><EvaluatorType>::Add

//------------------------------------------------------------------------------
template <class ObjectType, class EvaluatorType>
stResult<ObjectType> * stGHTree<ObjectType, EvaluatorType>::RangeQuery(
                              tObject * sample, double range){
   tResult * result = new tResult();  // Create result
   // Set the information.
   result->SetQueryInfo(sample->Clone(), RANGEQUERY, -1, range, false);

   // Let's search
   if (this->GetRoot() != 0){
      // Calls the routine to search the nodes recursively.
      this->RangeQuery(this->GetRoot(), result, sample, range);
   }//end if
   // Return the result set.
   return result;
}//end stGHTree<ObjectType><EvaluatorType>::RangeQuery

//------------------------------------------------------------------------------
template <class ObjectType, class EvaluatorType>
void stGHTree<ObjectType, EvaluatorType>::RangeQuery(u_int32_t pageID,
            tResult * result, ObjectType * sample, double range){
   stPage * currPage;
   stGHNode * currNode;
   double distanceLeft, distanceRight;
   tObject tmp;

   // Let's search
   if (pageID != 0){
      currPage = this->myPageManager->GetPage(pageID);
      currNode = new stGHNode(currPage);

      // Rebuild the left object.
      tmp.Unserialize(currNode->GetObject(0), currNode->GetObjectSize(0));
      // Evaluate the distance.
      distanceLeft = this->myMetricEvaluator->GetDistance(tmp, *sample);

      // Verify if the entry classify.
      if (distanceLeft <= range){
         // Yes, add it to the result.
         result->AddPair(tmp.Clone(), distanceLeft);
      }//end if

      // Analize its subtrees and the other entry.
      // Only in cases that there is two entries it has subtrees.
      if (currNode->GetNumberOfEntries() == 2){
         // It has two objects.
         // Get the second object.
         tmp.Unserialize(currNode->GetObject(1), currNode->GetObjectSize(1));
         // Evaluate the distance
         distanceRight = this->myMetricEvaluator->GetDistance(tmp, *sample);
         // Verify if the entry classify.
         if (distanceRight <= range){
            // Yes, add it to the result.
            result->AddPair(tmp.Clone(), distanceRight);
         }//end if

         // Verify the subtrees need to be visited.
         if (distanceLeft <= distanceRight){
            // First visit the left branch.
            RangeQuery(currNode->GetChildPageID(0), result, sample, range);

            // Try to prune the right branch.
            if (distanceLeft + range > (currNode->GetDistance()/2.0)){
               // The right subtree cannot be prunned.
               RangeQuery(currNode->GetChildPageID(1), result, sample, range);
            }//end if
         }else{
            // First visit the right branch.
            RangeQuery(currNode->GetChildPageID(1), result, sample, range);

            // Try to prune the left branch.
            if (distanceRight + range >= (currNode->GetDistance()/2.0)){
               // The left subtree cannot be prunned.
               RangeQuery(currNode->GetChildPageID(0), result, sample, range);
            }//end if
         }//end if
      }//end if

      //clean
      delete currNode;
      this->myPageManager->ReleasePage(currPage);
   }//end if
}//end stGHTree<ObjectType><EvaluatorType>::RangeQuery 

//------------------------------------------------------------------------------
template <class ObjectType, class EvaluatorType>
stResult<ObjectType> * stGHTree<ObjectType, EvaluatorType>::NearestQuery(
                     tObject * sample, u_int32_t k, bool tie){
   tResult * result = new tResult();  // Create result
   double rangeK = MAXDOUBLE;
   // Set the information.
   result->SetQueryInfo(sample->Clone(), KNEARESTQUERY, k, -1.0, tie);

   // Let's search
   if (this->GetRoot() != 0){
      //calls the routine to search the nodes recursively
      this->NearestQuery(this->GetRoot(), result, sample, rangeK, k);
   }//end if
   // Return the result set.
   return result;
}//end stGHTree<ObjectType, EvaluatorType>::NearestQuery

//------------------------------------------------------------------------------
template <class ObjectType, class EvaluatorType>
void stGHTree<ObjectType, EvaluatorType>::NearestQuery(
         u_int32_t pageID, tResult * result, ObjectType * sample,
         double & rangeK, u_int32_t k){

   stPage * currPage;
   stGHNode * currNode;
   double distanceLeft, distanceRight;
   tObject tmp;

   // Let's search
   if (pageID != 0){
      currPage = this->myPageManager->GetPage(pageID);
      currNode = new stGHNode(currPage);
      // Rebuild the left object
      tmp.Unserialize(currNode->GetObject(0), currNode->GetObjectSize(0));
      // Evaluate the distance
      distanceLeft = this->myMetricEvaluator->GetDistance(tmp, *sample);

      if (distanceLeft <= rangeK){
         // Add the object.
         result->AddPair(tmp.Clone(), distanceLeft);
         // There is more than k elements?
         if (result->GetNumOfEntries() >= k){
            // Cut if there is more than k elements.
            result->Cut(k);
            // May I use this for performance?
            rangeK = result->GetMaximumDistance();
         }//end if
      }//end if

      // Analize its subtrees and the other entry.
      // Only in cases that there is two entries it has subtrees.
      if (currNode->GetNumberOfEntries() == 2){
         // It has two objects.
         // Get the second object.
         tmp.Unserialize(currNode->GetObject(1), currNode->GetObjectSize(1));
         // Evaluate the distance
         distanceRight = this->myMetricEvaluator->GetDistance(tmp, *sample);
         // Verify if the entry classify.
         if (distanceRight <= rangeK){
            // Yes, add it to the result.
            result->AddPair(tmp.Clone(), distanceRight);
            // There is more than k elements?
            if (result->GetNumOfEntries() >= k){
               // Cut if there is more than k elements.
               result->Cut(k);
               // May I use this for performance?
               rangeK = result->GetMaximumDistance();
            }//end if
         }//end if

         // Verify the subtrees need to be visited.
         if (distanceLeft <= distanceRight){
            // First visit the left branch.
            NearestQuery(currNode->GetChildPageID(0), result, sample, rangeK, k);
            // Try to prune the right branch.
            if (distanceLeft + rangeK > (currNode->GetDistance()/2.0)){
               // The right subtree cannot be prunned.
               NearestQuery(currNode->GetChildPageID(1), result, sample, rangeK, k);
            }//end if
         }else{
            // First visit the right branch.
            NearestQuery(currNode->GetChildPageID(1), result, sample, rangeK, k);
            // Try to prune the left branch.
            if (distanceRight + rangeK >= (currNode->GetDistance()/2.0)){
               // The left subtree cannot be prunned.
               NearestQuery(currNode->GetChildPageID(0), result, sample, rangeK, k);
            }//end if
         }//end if
      }//end if

      // clean
      delete currNode;
      this->myPageManager->ReleasePage(currPage);
   }//end if
}//end stGHTree<ObjectType, EvaluatorType>::NearestQuery
