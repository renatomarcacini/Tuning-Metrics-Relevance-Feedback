#ifndef appH
#define appH

#include <chrono>

// Metric Tree includes
#include <arboretum/stMetricTree.h>
#include <arboretum/stPlainDiskPageManager.h>
#include <arboretum/stDiskPageManager.h>
#include <arboretum/stMemoryPageManager.h>
#include <arboretum/stSlimTree.h>
#include <arboretum/stMetricTree.h>
#include<util/CSVToVector.h>
#include <hermes/EuclideanDistance.h>
#include <hermes/EuclideanDistanceWeighted.h>
// My object
#include "image.h"

#include <string.h>
#include <fstream>

#define CITYFILE "../datastore-images/features/dataset.csv"
#define QUERYCITYFILE "../datastore-images/features/perform_dataset.csv"
//---------------------------------------------------------------------------
// class TApp
//---------------------------------------------------------------------------
class TApp{
   public:
      /**
      * This is the type used by the result.
      */
      typedef stResult < TImage > myResult;

      typedef stMetricTree < TImage, EuclideanDistanceWeighted<TImage> > MetricTree;

      /**
      * This is the type of the Slim-Tree defined by TImage and
      * TImageDistanceEvaluator.
      */
      typedef stSlimTree < TImage, EuclideanDistanceWeighted<TImage> > mySlimTree;

      /**
      * Creates a new instance of this class.
      */
      TApp(){
         PageManager = NULL;
         SlimTree = NULL;
      }//end TApp

      /**
      * Initializes the application.
      *
      * @param pageSize
      * @param minOccup
      * @param quantidade
      * @param prefix
      */
      void Init(){
         // To create it in disk
         CreateDiskPageManager();
         // Creates the tree
         CreateTree();
      }//end Init

      void LoadSlimTree();
      void ChangeWeightSlimTree(vector<double> weights);

      /**
      * Runs the application.
      *
      * @param DataPath
      * @param DataQueryPath
      */
      void Run();

      /**
      * Deinitialize the application.
      */
      void Done();
      void KNNSearchImage(TImage * image);
      vector<string> KNNSearchAndGetImage(TImage * image);
      void RangeSearchImage(TImage * image);


   private:

      /**
      * The Page Manager for SlimTree.
      */
      stPlainDiskPageManager * PageManager;

      /**
      * The SlimTree.
      */
      MetricTree * SlimTree;

      /**
      * Vector for holding the query objects.
      */
      vector <TImage *> queryObjects;

      /**
      * Creates a disk page manager. It must be called before CreateTree().
      */
      void CreateDiskPageManager();

      /**
      * Creates a tree using the current PageManager.
      */
      void CreateTree();

      /**
      * Loads the tree from file with a set of cities.
      */
      void LoadTree(char * fileName);

      /**
      * Loads the vector for queries.
      */
      void LoadVectorFromFile(char * fileName);

      /**
      * Performs the queries and outputs its results.
      */
      void PerformQueries();

      void PerformNearestQuery();

      void PerformRangeQuery();

   
};//end TApp

#endif //end appH