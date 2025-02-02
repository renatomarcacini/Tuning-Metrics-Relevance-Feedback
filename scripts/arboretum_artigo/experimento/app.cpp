//---------------------------------------------------------------------------
// app.cpp - Implementation of the application.
//
// To change the behavior of this application, comment and uncomment lines at
// TApp::Init() and TApp::Query().
//
// Authors: Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
// Copyright (c) 2003 GBDI-ICMC-USP
//---------------------------------------------------------------------------
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
   //
   SlimTree = new mySlimTree(PageManager);
}//end TApp::CreateTree

//------------------------------------------------------------------------------
void TApp::CreateDiskPageManager(){
   //for SlimTree
   PageManager = new stPlainDiskPageManager("SlimTree.dat", 256*4);
}//end TApp::CreateDiskPageManager


void TApp::LoadSlimTree(){

}

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
   cout << "\n\nLoading the query file";
   LoadVectorFromFile(QUERYCITYFILE);

   if (queryObjects.size() > 0){
      // Do 500 queries.
      PerformQueries();
   }//end if
   // Hold the screen.
   cout << "\n\nFinished the whole test!";
   
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

int sizeDataset = 5000;
//------------------------------------------------------------------------------
void TApp::LoadTree(char * fileName){
    TImage * image;

    if(SlimTree != NULL){
        CSVToVector * csvReader = new CSVToVector();
        vector<vector<string>> data = csvReader->GetData(fileName);
        
         string name;
         vector<double> features;
         std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
         cout << "\n PATH: " << CITYFILE;

         cout << "\n TAMANHO DATASET: " << data.size();

        for(int i=0;i<data.size();i++)
        {   
            name = data[i][data[i].size()-1];
            features.clear();
            for(int j=0;j<data[i].size()-1;j++)
            {
                features.push_back(std::stod(data[i][j]));
            }
            //cout<< "Index:" << name << endl;
            //cout<< "Feature:" << features.size() << endl;
            TImage * img = new TImage(name, features);
            SlimTree->Add(img);
            if(i % 5000 == 0){
               std::chrono::steady_clock::time_point middle = std::chrono::steady_clock::now();

               cout << "\n Index:" << i << " Time: " << (std::chrono::duration_cast<std::chrono::microseconds>(middle - begin).count()/1000000)<<"[µs]";
            }
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
   string name;
   vector<double> features;

    CSVToVector * csvReader = new CSVToVector();
    vector<vector<string>> data = csvReader->GetData(fileName);
    for(int i=0;i<data.size();i++)
    {   
         name = data[i][data[i].size()-1];
        features.clear();
        for(int j=0;j<data[i].size()-1;j++)
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

double fRand()
{
    return ((double)rand() / RAND_MAX);
}

int sizePerfom = 1000;

//------------------------------------------------------------------------------
void TApp::PerformRangeQuery(){

   //vector<double> avg_disk;
   //vector<double> avg_calc;
   for (int x = 0; x < 1; x++)
   {
      bool enableWeight = true;
      myResult * result;
      double radius;
      //clock_t start, end;
      unsigned int size;
      unsigned int i;

      int counter = 1;
      if (SlimTree){
         size = queryObjects.size();
         // reset the statistics
         PageManager->ResetStatistics();
         SlimTree->GetMetricEvaluator()->ResetStatistics();
         //start = clock();
         std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
         vector<double> weights;
         for (i = 0; i < sizePerfom; i++){

            if(enableWeight == true){
               for(int i=0; i < 50; i++){
                  weights.push_back(fRand());
               }
               ChangeWeightSlimTree(weights);
            }

            result = SlimTree->RangeQuery( queryObjects[i] , 0.2);
            weights.clear();
            delete result;
            
            if(counter % 100 == 0){
               cout << "\nQTD:" << (counter);
               cout << "\nTotal Disk Accesses: " << (double )PageManager->GetReadCount();
               cout << "\nTotal Distance Calculations: " <<
               (double )SlimTree->GetMetricEvaluator()->GetDistanceCount();
            }
            
            counter++;
         }//end for
         //end = clock();
         std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
         //cout << "\nTotal Time: " << ((double )end-(double )start) / 1000.0 << "(s)";
         cout << "\nTotal Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()<<"[µs]";
         // is divided for queryObjects to get the everage
         cout << "\nTotal Disk Accesses: " << (double )PageManager->GetReadCount();
         cout << "\nAvg Disk Accesses: " << (double )PageManager->GetReadCount() / (double )sizePerfom;
         // is divided for queryObjects to get the everage
         // is divided for queryObjects to get the everage
         cout << "\nTotal Distance Calculations: " <<
            (double )SlimTree->GetMetricEvaluator()->GetDistanceCount();
         cout << "\nAvg Distance Calculations: " <<
            (double )SlimTree->GetMetricEvaluator()->GetDistanceCount() / (double )sizePerfom;
      }//end if
   }
   
   

}//end TApp::PerformRangeQuery

//------------------------------------------------------------------------------
void TApp::PerformNearestQuery(){

   myResult * result;
   clock_t start, end;
   unsigned int size;
   unsigned int i;
   bool enableWeight = true;

   if (SlimTree){
      size = queryObjects.size();
      PageManager->ResetStatistics();
      SlimTree->GetMetricEvaluator()->ResetStatistics();
      start = clock();
      vector<double> weights;

      for (i = 0; i < sizePerfom; i++){
         if(enableWeight == true){
            for(int i=0; i < 50; i++){
               weights.push_back(fRand());
            }

            ChangeWeightSlimTree(weights);
         }
         result = SlimTree->NearestQuery(queryObjects[i], 15);
         weights.clear();
         delete result;
         //test
         if(i > 0 && i % 100 == 0){
            cout << "\nQTD:" << i;
            cout << "\nTotal Disk Accesses: " << (double )PageManager->GetReadCount();
            cout << "\nTotal Distance Calculations: " <<
            (double )SlimTree->GetMetricEvaluator()->GetDistanceCount();
         }
         /*
         else if(i == sizePerfom-1){
            cout << "\nQTD:" << i;
            cout << "\nTotal Disk Accesses: " << (double )PageManager->GetReadCount();
            cout << "\nTotal Distance Calculations: " <<
            (double )SlimTree->GetMetricEvaluator()->GetDistanceCount();
         }
         */
         
      }//end for
      end = clock();
      cout << "\nTotal Time: " << ((double )end-(double )start) / (CLOCKS_PER_SEC/1000) << "(mili second)";
      // is divided for queryObjects to get the everage
      cout << "\nTotal Disk Accesses: " << (double )PageManager->GetReadCount();
      cout << "\nAvg Disk Accesses: " << (double )PageManager->GetReadCount() / (double )sizePerfom;
      // is divided for queryObjects to get the everage
      cout << "\nTotal Distance Calculations: " <<
         (double )SlimTree->GetMetricEvaluator()->GetDistanceCount();
      cout << "\nAvg Distance Calculations: " <<
         (double )SlimTree->GetMetricEvaluator()->GetDistanceCount() / (double )sizePerfom;
   }//end if
}//end TApp::PerformNearestQuery

void TApp::KNNSearch(TImage * image, int k, bool  weighted){
   myResult * result;
   vector<double> weights;
   if(weighted == true){
      for(int i=0; i < image->GetFeatures().size(); i++){
         weights.push_back(fRand());
      }
      ChangeWeightSlimTree(weights);
   }

   result = SlimTree->NearestQuery(image, k);
   weights.clear();
   delete result;
   delete image;
}

void TApp::RangeSearch(TImage * image, double radius, bool  weighted){
   myResult * result;
   vector<double> weights;
   if(weighted == true){
      for(int i=0; i < image->GetFeatures().size(); i++){
         weights.push_back(fRand());
      }
      ChangeWeightSlimTree(weights);
   }
   result = SlimTree->RangeQuery(image, radius);
   weights.clear();
   delete result;
   delete image;
}


void TApp::TimerNearestQuery(){
   /*
   myResult * result;
   TImage * image = new TImage(queries[i].GetName(), queries[i].GetFeature());

   result = SlimTree->NearestQuery(image, 30);
   */
}

void TApp::TimerRangeQuery(){

}

void TApp::KNNSearchImage(TImage * image){
   myResult * result;

   cout << "\nCONSULTA POR KNN";

   //cout << "\nPESOS: " << SlimTree->GetMetricEvaluator()->GetWeights() << endl;
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

   //cout << "\nPESOS: " << SlimTree->GetMetricEvaluator()->GetWeights() << endl;

   result = SlimTree->NearestQuery(image, 15);
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