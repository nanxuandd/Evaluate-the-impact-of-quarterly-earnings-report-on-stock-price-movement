//#pragma once
#include <cstring>
#include <string> 
#include <vector>
#ifndef _STOCK_H_
#define _STOCK_H_

 using namespace std;


    
class Stock{
private:
    string symbol;
    string earningAnnouncementDate;
    string periodEnding;
    double estimated;
    double reportedEarnings;
    double surprise;
    double surprisePercentage;
    
    string startDate;
    string endDate;
    
    int groupID; ///Which of the 3 groups it belongs to (beat:1,meet:2,miss:3)
    vector<double> prices; // 2N+1 daily prices
    vector<double> abnormalPrices;
    vector<double> cumulativereturn;

public:
    Stock(string symbol_, string earningAnnouncementDate_, string periodEnding_, double estimated_, double reportedEarnings_, double surprise_, double surprisePercentage_){
        symbol = symbol_;
        earningAnnouncementDate = earningAnnouncementDate_ ;
        periodEnding=periodEnding_;
        estimated=estimated_;
        reportedEarnings=reportedEarnings_;
        surprise=surprise_;
        surprisePercentage = surprisePercentage_;
    }
    
    string getSymbol(){
    	return symbol;
    }
    
    string getEarningAnnouncementDate(){
    	return earningAnnouncementDate;
    }
    
    double getSurprise(){
    	return  surprise;
    }
    string getPeriodEnding(){
        return periodEnding;
    }
    
    double getEstEPS(){
        return estimated;
    }
    
    double getActEPS(){
        return reportedEarnings;
    }
    
    
    double getSurprisePercentage(){
    	return surprisePercentage;
    }
    
    string getStartDate(){
        return startDate;
    }
    
    void setStartDate(string startDate_){
        startDate = startDate_;
    }
    
    string getEndDate(){
        return endDate;
    }
    
    void setEndDate(string endDate_){
        endDate = endDate_;
    }
    
    int getgroupID(){
        return groupID;
    }
    
    void setgroupID(int groupID_){
        groupID = groupID_;
    }
    
    string getgroupName(){
        int id = getgroupID();
        if(id==1){
            return "Miss";
        } 
        else if (id==2){
            return "Meet";
        } 
        else{
            return "Beat";
            
        }
    }
    vector<double> getPrices(){
        return prices;
    }
    
    void setPrices(vector<double> prices_){
        prices = prices_;
    }
    
    vector<double> getAbnormalPrices(){
        return abnormalPrices;
    }
    void setAbnormalPrices(vector<double> abnormalPrices_){
        abnormalPrices=abnormalPrices_;
    }
   void  setCummulativeReturn(vector<double> cumulativereturn_){
       cumulativereturn=cumulativereturn_;
   }
   vector<double> getCummulativeReturn(){
      return cumulativereturn;
   }
};
#endif

