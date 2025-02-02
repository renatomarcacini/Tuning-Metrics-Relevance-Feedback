#include<util/CSVToVector.h>
#define FILENAME "../datastore-images/features/dataset_cor.csv"
using namespace std;


int main(){ 
 
    CSVToVector * csvReader = new CSVToVector();
    vector<vector<string>> data = csvReader->GetData(FILENAME);
    
    string name;
    vector<double> features;

    for(int i=0;i<data.size();i++)
    {   
        name = data[i][0];
        cout<< "NOME:" << name << " ";
        cout << "F: ";  
        for(int j=1;j<data[i].size();j++)
        {
            cout << data[i][j] << " ";
        }
        cout << "" << endl;
    }
    return 0;
}