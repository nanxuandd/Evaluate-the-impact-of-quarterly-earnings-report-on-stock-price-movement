#include "Calculation.h"
using namespace std;

namespace final{
 class Bootstrap{
   private:
    StockGroup stockgroup;
    Bootstrap(){}
    public:
     Bootstrap(StockGroup group_):stockgroup(group_){}
     vector<Stock> randomStockSelection(vector<Stock> group); //randomly select 30 stocks from each group
     void getSampleGroup(StockGroup & samplegroups); //get one sampling
     BigMatrix getAbnormalReturn(StockMap &pricemaps); //get abnormal returns for a sampling
     void Samples(StockMap &pricemaps,BigMatrix & arr_matrix,BigMatrix & carr_matrix); //40 samplings
     ~ Bootstrap(){}
 };
}
