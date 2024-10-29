#include "Bootstrap.h"
#include "curl/curl.h"
#include <vector>
#include <random>
#include <algorithm> // for shuffle
namespace final{
    vector<Stock> Bootstrap:: randomStockSelection(vector<Stock> group) {    
        vector<Stock> randomSampling;
        random_device rd;  
        mt19937 gen(rd());
        shuffle(group.begin(), group.end(), gen);
        for(int i = 0; i < 30 && i < (int)group.size(); i++) {
            randomSampling.push_back(group[i]);
        }
        return randomSampling;
    }

    void Bootstrap:: getSampleGroup(StockGroup & samplegroups) {
        samplegroups.resize(3);
        samplegroups[0]=randomStockSelection(stockgroup[0]);
        samplegroups[1]=randomStockSelection(stockgroup[1]);
        samplegroups[2]=randomStockSelection(stockgroup[2]);
    }
   
    BigMatrix Bootstrap:: getAbnormalReturn(StockMap &pricemaps) {
         
        StockGroup samplegroups; 
        getSampleGroup(samplegroups);  // matrix 3x30
        string symbol;

        BigMatrix abnormalReturn;
        abnormalReturn.resize(3); // 3xmatrix
        
        for(int i=0;i<3;i++){
            for(vector<Stock>::iterator itr = samplegroups[i].begin();itr != samplegroups[i].end();itr++){ //loop from 1 to 30 stocks
                symbol = itr->getSymbol();
                if (pricemaps.find(symbol) != pricemaps.end()){
                    auto it = pricemaps.find(symbol); 
                    abnormalReturn[i].push_back((it->second)->getAbnormalPrices()); 
                }
            }
        }
        return abnormalReturn; // 3x30x2n+1
     }
     
     // AAR and CAAR matrix should be 3X matrix
    void Bootstrap::Samples(StockMap &pricemaps,BigMatrix & AAR_matrix,BigMatrix & CAAR_matrix){
        BigMatrix abmatrix;
        mt19937 gen;
        for(int n=0;n<40;n++){
            abmatrix = getAbnormalReturn(pricemaps); // for each loop sampling, get a abnormal return for 3 groups, 3X2NX30
            for(int i=0;i<3;i++) {
                Vec aar = AAR(abmatrix[i]);
                AAR_matrix[i].push_back(aar); // for each group, we add one AAR vector(2N), totally 40 AAR vector for each group
                CAAR_matrix[i].push_back(CAAR(aar)); // for each group, we add one CAAR vector(2N), totally 40 CAAR vector for each group
            }
        }
     }
     
}