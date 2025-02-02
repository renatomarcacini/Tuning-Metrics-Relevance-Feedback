#pragma hdrstop
#include "app.h"
#include <iostream>
#include<util/CSVToVector.h>
#include <opencv2/opencv.hpp>

#pragma argsused

using namespace cv;

#define IMAGES "../datastore-images/images/"


struct Data{
    private:
        string name;
        vector<double> feature;
    public:
        string GetName(){
            return name;
        }

        vector<double> GetFeature(){
            return feature;
        }

        void SetData(string name, vector<double> feature){
            this->name = name;
            this->feature = feature;
        }
};

vector<Data> GetDatasetCSV(string filename){
    CSVToVector * csvReader = new CSVToVector();
    vector<vector<string>> data = csvReader->GetData(filename);
    int nameColumn = data[0].size()-1;
    vector<Data> dataset;
    for(int i=0;i<data.size();i++)
    {   
        string name = data[i][nameColumn];
        
        int id = i;
        vector<double> features;
        features.clear();
        for(int j=0;j<data[i].size()-1;j++)
        {
            features.push_back(std::stod(data[i][j]));
        }

        Data data;
        data.SetData(name, features);
        dataset.push_back(data);
    }
    return dataset;
}

double ffRand()
{
    return ((double)rand() / RAND_MAX);
}

void Workflow(TApp app, int percent, bool weighted, string type){
    //CSVToVector * csvReader = new CSVToVector();
    //vector<vector<string>> data = csvReader->GetData(QUERYCITYFILE);
    vector<Data> dataset = GetDatasetCSV(CITYFILE);
    vector<Data> queries = GetDatasetCSV(QUERYCITYFILE);

    int total = dataset.size();
    int qtd = (percent * total)/100;

    double maxDistance = 4497;
    double radius = (percent * maxDistance)/100;


    for (int i = 0; i < 50; i++)
    {
        TImage * image = new TImage(queries[i].GetName(), queries[i].GetFeature());
        if(type == "KNN")
            app.KNNSearch(image, qtd, weighted);
        else if(type == "Range"){
            app.RangeSearch(image, radius, weighted);
        }
    }
}

int main(int argc, char* argv[]){
   TApp app;                                         

    // Init application.
    app.Init();
    // Run it.
    app.Run();
    vector<double> times(20, 0);
    for(int x = 0; x < 10; x++){
        cout << "\n-" << x;
        for (int i = 1; i <= 20; i++)
        {
            clock_t start, end;

            //cout << "\n---------------";
            //cout << "\nPORCENTAGEM:" << 5*i;
            start = clock();
            //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            Workflow(app, i*5, true, "Range");
            //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            end = clock();

            double time = ((double )end-(double )start) / (CLOCKS_PER_SEC/1000);
            times[i-1] += time;


            //cout << "\nCLock Total Time: " << ((double )end-(double )start) / (CLOCKS_PER_SEC/1000) << "(mili second)";

            //cout << "\nTime: " << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/1000000)<<"[µs]";
            //cout << "\nTotal Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()<<"[µs]";
            //cout << "\n---------------\n";
        }
    }
    
    for (int i = 0; i < times.size(); i++)
    {
        cout << "\nPERCENT:" << (i+1) * 5 << " - AVG_TIME:" << times[i] / 10;
    }
    
    app.Done();

   return 0;
}//end main


