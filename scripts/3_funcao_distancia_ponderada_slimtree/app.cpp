using namespace std;
#include <iostream>
#pragma hdrstop
#include "app.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
// Class TApp
//------------------------------------------------------------------------------
void TApp::CreateTree(){
   // create for Slim-Tree
   SlimTree = new mySlimTree(PageManager);
}//end TApp::CreateTree

//------------------------------------------------------------------------------
void TApp::CreateDiskPageManager(){
   //for SlimTree
   PageManager = new stPlainDiskPageManager("SlimTree.dat", 512000);
}//end TApp::CreateDiskPageManager


void TApp::LoadSlimTree(){}

void TApp::ChangeWeightSlimTree(vector<double> weights){
   SlimTree->GetMetricEvaluator()->SetWeights(weights);
}

//------------------------------------------------------------------------------
void TApp::Run(){
   // Lets load the tree with a lot values from the file.
   cout << "\n\nAdding objects in the SlimTree";
   LoadTree(CITYFILE);
   PageManager->ResetStatistics();
   SlimTree->GetMetricEvaluator()->ResetStatistics();
}//end TApp::Run

//------------------------------------------------------------------------------
void TApp::Done(){

   if (this->SlimTree != NULL){
      delete this->SlimTree;
   }//end if
   if (this->PageManager != NULL){
      delete this->PageManager;
   }//end if

   // delete the vetor of queries.
   for (unsigned int i = 0; i < queryObjects.size(); i++){
      delete (queryObjects.at(i));
   }//end for
}//end TApp::Done

//------------------------------------------------------------------------------
void TApp::LoadTree(char * fileName){
    TImage * image;

    if(SlimTree != NULL){
        CSVToVector * csvReader = new CSVToVector();
        vector<vector<string>> data = csvReader->GetData(fileName);
        
         string name;
         vector<double> features;

        for(int i=0;i<data.size();i++)
        {   
            name = data[i][0];
            features.clear();
            for(int j=1;j<data[i].size();j++)
            {
                features.push_back(std::stod(data[i][j]));
            }
            cout<< "NOME:" << name << endl;

            TImage * img = new TImage(name, features);
            SlimTree->Add(img);
            delete img;
        }
        cout << " Added " << SlimTree->GetNumberOfObjects() << " objects ";
    }
    else{
        cout << "\n Zero object added!!";
    }
}//end TApp::LoadTree

//------------------------------------------------------------------------------
void TApp::LoadVectorFromFile(char * fileName){
    // clear before using.
    queryObjects.clear();
    CSVToVector * csvReader = new CSVToVector();
    vector<vector<string>> data = csvReader->GetData(fileName);
    for(int i=0;i<data.size();i++)
    {   
        string name = data[i][0];
        int id = i;
        vector<double> features;
        features.clear();
        for(int j=1;j<data[i].size();j++)
        {
            features.push_back(std::stod(data[i][j]));
        }
        this->queryObjects.insert(queryObjects.end(), new TImage(name, features));
    }
    cout << " Added " << queryObjects.size() << " query objects ";
}//end TApp::LoadVectorFromFile

//------------------------------------------------------------------------------
void TApp::PerformQueries(){
   if (SlimTree){
      cout << "\nStarting Statistics for Range Query with SlimTree.... ";
      PerformRangeQuery();
      cout << " Ok\n";

      cout << "\nStarting Statistics for Nearest Query with SlimTree.... ";
      PerformNearestQuery();
      cout << " Ok\n";
   }//end if
}//end TApp::PerformQuery

//------------------------------------------------------------------------------
void TApp::PerformRangeQuery(){

   myResult * result;
   double radius;
   //clock_t start, end;
   unsigned int size;
   unsigned int i;

   if (SlimTree){
      size = queryObjects.size();
      // reset the statistics
      PageManager->ResetStatistics();
      SlimTree->GetMetricEvaluator()->ResetStatistics();
      //start = clock();
      std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
      for (i = 0; i < size; i++){
         result = SlimTree->RangeQuery(queryObjects[i], 0.2);
         delete result;
      }//end for
      //end = clock();
      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
      //cout << "\nTotal Time: " << ((double )end-(double )start) / 1000.0 << "(s)";
      cout << "\nTotal Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()<<"[µs]";
      // is divided for queryObjects to get the everage
      cout << "\nAvg Disk Accesses: " << (double )PageManager->GetReadCount() / (double )size;
      // is divided for queryObjects to get the everage
      cout << "\nAvg Distance Calculations: " <<
         (double )SlimTree->GetMetricEvaluator()->GetDistanceCount() / (double )size;
   }//end if

}//end TApp::PerformRangeQuery

//------------------------------------------------------------------------------
void TApp::PerformNearestQuery(){

   myResult * result;
   clock_t start, end;
   unsigned int size;
   unsigned int i;

   if (SlimTree){
      size = queryObjects.size();
      PageManager->ResetStatistics();
      SlimTree->GetMetricEvaluator()->ResetStatistics();
      start = clock();
      for (i = 0; i < size; i++){
         result = SlimTree->NearestQuery(queryObjects[i], 15);
         delete result;
      }//end for
      end = clock();
      cout << "\nTotal Time: " << ((double )end-(double )start) / (CLOCKS_PER_SEC/1000) << "(mili second)";
      // is divided for queryObjects to get the everage
      cout << "\nAvg Disk Accesses: " << (double )PageManager->GetReadCount() / (double )size;
      // is divided for queryObjects to get the everage
      cout << "\nAvg Distance Calculations: " <<
         (double )SlimTree->GetMetricEvaluator()->GetDistanceCount() / (double )size;
   }//end if
}//end TApp::PerformNearestQuery

void TApp::KNNSearchImage(TImage * image){
   myResult * result;

   cout << "\nCONSULTA POR KNN";
   cout << "\nPESOS: " << SlimTree->GetMetricEvaluator()->GetWeights() << endl;
   result = SlimTree->NearestQuery(image, 30);
   cout << "\nQuantidade de entradas: " << result->GetNumOfEntries();
   cout << "\nObjeto de consulta: " << result->GetSample()->GetName() << endl;
      cout << "\nAvg Disk Accesses: " << (double )PageManager->GetReadCount(); 
      // is divided for queryObjects to get the everage
      cout << "\nAvg Distance Calculations: " <<
         (double )SlimTree->GetMetricEvaluator()->GetDistanceCount() << endl;
   unsigned int i;
   for(i=0; i < result->GetNumOfEntries(); i++){
      cout << "Objeto: " << result->GetPair(i)->GetObject()->GetName();
      cout << " Distância: " << result->GetPair(i)->GetDistance() << endl;
      /*
      cout << " Feature: ";

      for(int j=0; j < result->GetPair(i)->GetObject()->GetFeatures().size(); j++){
         cout << result->GetPair(i)->GetObject()->GetFeatures()[j] << " ";
      }
      cout << "" << endl;
      */
    }
   cout << "\n-------------------------";
}

vector<string> TApp::KNNSearchAndGetImage(TImage * image){
   myResult * result;

   cout << "\nCONSULTA POR KNN";
   cout << "\nPESOS: " << SlimTree->GetMetricEvaluator()->GetWeights() << endl;
   result = SlimTree->NearestQuery(image, 8);
   cout << "\nQuantidade de entradas: " << result->GetNumOfEntries();
   cout << "\nObjeto de consulta: " << result->GetSample()->GetName() << endl;

   vector<string> nameOfImages;
   nameOfImages.clear();
   unsigned int i;
   for(i=0; i < result->GetNumOfEntries(); i++){
      cout << "Objeto: " << result->GetPair(i)->GetObject()->GetName();
      cout << " Distância: " << result->GetPair(i)->GetDistance() << endl;
      nameOfImages.push_back(result->GetPair(i)->GetObject()->GetName());
   }
   cout << "\n-------------------------";
   return nameOfImages;
}

void TApp::RangeSearchImage(TImage * image){
   myResult * result;

   cout << "\nCONSULTA POR RANGE";

   result = SlimTree->RangeQuery(image, 0.4);
   cout << "\nQuantidade de entradas: " << result->GetNumOfEntries();
   cout << "\nObjeto de consulta: " << result->GetSample()->GetName();

   unsigned int i;
   for(i=0; i < result->GetNumOfEntries(); i++){
      cout << "Cidade: " << result->GetPair(i)->GetObject()->GetName() << endl;
      cout << "Latitude: " << result->GetPair(i)->GetObject()->GetFeatures()[0] << endl;
      cout << "Longitude: " << result->GetPair(i)->GetObject()->GetFeatures()[1] << endl;
   }
   cout << "\n-------------------------";
}