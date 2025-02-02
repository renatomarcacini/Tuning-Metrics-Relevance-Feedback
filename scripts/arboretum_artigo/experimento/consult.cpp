#pragma hdrstop
#include "app.h"
#include <iostream>
#include<util/CSVToVector.h>


#pragma argsused
int main(int argc, char* argv[]){
   TApp app;                                         
    app.Init();
   CSVToVector * csvReader = new CSVToVector();
   vector<vector<string>> data = csvReader->GetData(CITYFILE);
        
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
      cout<< "CONSULT => NOME:" << name << endl;
      break;;
   }

   TImage * image = new TImage(name, features);

   app.KNNSearchImage(image);
   
   return 0;
}//end main
