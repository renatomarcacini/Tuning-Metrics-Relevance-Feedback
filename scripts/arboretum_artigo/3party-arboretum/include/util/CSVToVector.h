#ifndef CSVTOVECTOR_H
#define CSVTOVECTOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
 
using namespace std;

class CSVToVector{

    private:
        vector<vector<string>> features;
    public:
        CSVToVector(){
            features.clear();
        }

        vector<vector<string>> GetData(string path){
            string fname = path;
            vector<vector<string>> content;
            vector<string> row;
            string line, word;

            string classe;
            vector<double> data;
        
            fstream file (fname, ios::in);
            if(file.is_open())
            {
                while(getline(file, line))
                {
                    row.clear();
        
                    stringstream str(line);
        
                    while(getline(str, word, ',')){
                        row.push_back(word);
                        //cout << word + " test" << endl;
                    }
                    content.push_back(row);
                }
            }
            else
                cout<<"Could not open the file\n";
            /*
            for(int i=0;i<content.size();i++)
            {   
                //cout<<content[i][0]<<endl;
                for(int j=0;j<content[i].size();j++)
                {
                    cout<<content[i][j]<<" " << endl;
                    //features.push_back(content[i][j]);
                }
                cout<<"fim \n";
            }
            */
            return content;
        }
};
#endif
