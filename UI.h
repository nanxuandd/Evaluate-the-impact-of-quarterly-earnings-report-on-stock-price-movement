#include <iostream>
#include <string>
#include <map>
#include <unistd.h>
#include "Bootstrap.h"
#include "Gnuplot.h"

using namespace std;

namespace final {

// Function to display menu options
void displayMenuOptions() {
    cout << endl;
    cout << "1 - Retrieve 2N+1 days of historical price data for all stocks " << endl;
    cout << "2 - Pull information for one stock from one group" << endl;
    cout << "3 - Show AAR, AAR-STD, CAAR and CAAR-STD for one group" << endl;
    cout << "4 - Show the gnuplot graph with CAAR for all 3 groups" << endl;
    cout << "5 - Exit" << endl;
}


void handleOption1(vector<Stock> &allStocks, StockGroup &stockGroup, vector<PAIR> &IWV_Data, StockMap &priceMaps, BigMatrix &finalMatrix) {
    int N;
    cout << "Enter N (N >= 60 and N <= 90): ";
    
    while (true) {
        cin >> N;

        if (N >= 60 && N <= 90) {
            break; // N is valid, exit the loop
        } else {
        cout << "We only support N from 60 to 90! Plz select another one :-(: ";
        }
    }
    
    
    IWV_Data = final::retrieveIWVData();
    
    vector<PAIR> IWV_Copy;
    for (int i = 0; i < (int)IWV_Data.size(); i++) {
        IWV_Copy.push_back(IWV_Data[i]);
    }

    cout << "Wait....I trying to find stock data..." << endl;
    priceMaps = final::retrieveStock_IWVData(stockGroup, IWV_Copy, N);

    vector<string> stocksWithoutEnoughData = getOutofMapStocks(allStocks, priceMaps);
    cout << endl << "The following stocks do not have enough " << 2 * N + 1 << " days of prices:" << endl;

    for (const string &symbol : stocksWithoutEnoughData) {
        cout << symbol << " ";
    }
    cout << endl;

    Bootstrap model(stockGroup);
    BigMatrix aarMatrix(3), caarMatrix(3);
    model.Samples(priceMaps, aarMatrix, caarMatrix);
    finalMatrix = Final(aarMatrix, caarMatrix);
}

// Function to handle option 2
void handleOption2(StockMap pricemaps, vector<PAIR> IWVData) {
    string symbol;
    cout << "Alright! Which one? Enter a symbol: ";
    
    while (true) {
        cin >> symbol;

        if (pricemaps.find(symbol) != pricemaps.end()) {
            break; // Valid symbol found, exit loop
        } else {
        cout << "Sorry 5555~ There is no data available for the that symbol! Please select another one: ";
        }
    }
    
    cout << "Daily Prices: " << endl;
    vector<double> prices = pricemaps[symbol]->getPrices();
    cout << prices << endl;
    
    cout << "Cumulative Daily Returns: " << endl;
    vector<double> Cumu_Return = pricemaps[symbol]->getCummulativeReturn();
    cout << Cumu_Return << endl;


    cout << "Stock Information:             "                   << endl;
    cout << "Group:                         "                   << pricemaps[symbol]->getgroupName() << endl;
    cout << "Ticker:                        "                   << pricemaps[symbol]->getSymbol() << endl;
    cout << "Earnings Announcement Date:    "                   << pricemaps[symbol]->getEarningAnnouncementDate() << endl;
    cout << "Period Ending:                 "                   << pricemaps[symbol]->getPeriodEnding() << endl;
    cout << "Reported Earnings:             "                   << pricemaps[symbol]->getActEPS() << endl;
    cout << "Estimated Earnings:            "                   << pricemaps[symbol]->getEstEPS() << endl;
    cout << "Surprise:                      "                   << pricemaps[symbol]->getSurprise() << endl;
    cout << "Surprise%:                     "                   << pricemaps[symbol]->getSurprisePercentage() << endl;
    
}


// Function to handle option 3
void handleOption3(StockGroup &stockGroup, StockMap &priceMaps, const BigMatrix &finalMatrix) {
    string groupName;
    map<string, int> groupIndexMap = {{"Miss", 0}, {"Meet", 1}, {"Beat", 2}};
    cout << "Almost!! Which group (Beat, Meet, Miss)?: ";
    
    while (true) {
        cin >> groupName;

        if (groupIndexMap.find(groupName) != groupIndexMap.end()) {
            break; // Valid group name entered, exit loop
        } else {
            cout << "We only have 'Beat', 'Meet', or 'Miss' :-( Plz Select from Them: ";
        }
    }

    vector<string> datainfo = {"Avg AAR", "AAR STD", "Avg CAAR", "CAAR STD"};
    
    for(int i=0; i<=3; i++){ 
        cout<<endl;
        cout<< datainfo[i] << " for Group "<<groupName<<endl;
        vector<double> data_vec;
        data_vec = finalMatrix[(int)groupIndexMap[groupName]][i]; 
        cout<<data_vec<<endl;
    }
}

// Function to handle option 4
void handleOption4(BigMatrix &finalMatrix) {
    plotGnuplot(finalMatrix);
}

// Function to execute the user interface
void executeUI(StockGroup stockGroup, vector<Stock> &allStocks) {
    StockMap priceMaps;
    vector<PAIR> IWVData;
    BigMatrix finalMatrix;
    bool dataRetrieved = false;
    string userInput;

    do {
        displayMenuOptions();
        cout << endl << "Boss, Please Enter the Number: ";
        cin >> userInput;

        if (userInput == "1") {
            handleOption1(allStocks, stockGroup, IWVData, priceMaps, finalMatrix);
            dataRetrieved = true;
        } else if (userInput == "2") {
            if (!dataRetrieved) {
                cout << "ooops, Enter 1 First :-( !" << endl;
            } else {
                handleOption2(priceMaps, IWVData);
            }
        } else if (userInput == "3") {
            if (!dataRetrieved) {
                cout << "ooops, Enter 1 First :-( !" << endl;
            } else {
                handleOption3(stockGroup, priceMaps, finalMatrix);
            }
        } else if (userInput == "4") {
            if (!dataRetrieved) {
                cout << "ooops, Enter 1 First :-( !" << endl;
            } else {
                handleOption4(finalMatrix);
                usleep(2000000);
            }
        } else if (userInput != "5") {
            cout << endl << "ooops, Enter 1 First :-( !" << endl;
        }
    } while (userInput != "5");
}
}
