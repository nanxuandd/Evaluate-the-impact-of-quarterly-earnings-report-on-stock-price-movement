#include "DataProcessing.h"
#include <iomanip>

namespace final {

    void populateStock(vector<Stock>& stock) {
        const char* Russell3000EarningFile = "Russell3000EarningsAnnouncements.csv";
        ifstream fin;
        fin.open(Russell3000EarningFile, ios::in);
        string line, symbol, earningAnnouncementDate, periodEnding, estimated, reportedEarnings, surprise, surprisePercentage, other;
        getline(fin, line);
        while (!fin.eof()) {
            getline(fin, line);
            stringstream sin(line);
            getline(sin, symbol, ',');
            getline(sin, earningAnnouncementDate, ',');
            earningAnnouncementDate = formatDate(earningAnnouncementDate);
            getline(sin, periodEnding, ',');
            getline(sin, estimated, ',');
            getline(sin, reportedEarnings, ',');
            getline(sin, surprise, ',');
            getline(sin, surprisePercentage, ',');

            vector<double> values;
            for (const string& field : {estimated, reportedEarnings, surprise, surprisePercentage}) {
                stringstream ss(field);
                double value;
                ss >> value;
                values.push_back(value);
            }

            Stock s(symbol, earningAnnouncementDate, periodEnding, values[0], values[1], values[2], values[3]);
            stock.push_back(s);
        }
    }

    bool myCmp(Stock& left, Stock& right) {
        return left.getSurprisePercentage() < right.getSurprisePercentage();
    }

    void sortStock(vector<Stock>& stock) {
        sort(stock.begin(), stock.end(), myCmp);
    }


    
void divideStock(vector<Stock>& totalStock, StockGroup& stockgroup) {
    // Calculate the size of each group
    int totalSize = totalStock.size();
    int groupSize = totalSize / 3;
    int remainder = totalSize % 3;

    // Resize stockgroup to contain three groups
    stockgroup.resize(3);

    // Distribute the stocks into three groups
    int index = 0;
    for (int groupIndex = 0; groupIndex < 3; ++groupIndex) {
        // Calculate how many stocks this groupIndex should take
        int count = groupSize + (groupIndex < remainder ? 1 : 0);

        // Take 'count' stocks for this groupIndex from the 'totalStock' vector
        for (int i = 0; i < count; ++i) {
            stockgroup[groupIndex].push_back(totalStock[index++]);
        }
    }
}


string formatDate(const string& date) {
    // Initialize to all zeros
    tm timeinfo = {}; 
    
    // Parse the date. Note: strptime is not standard C++ and may not be available on your system
    if (strptime(date.c_str(), "%d-%b-%y", &timeinfo) == nullptr) {
        
        // Handle error: input date string does not match format
        return "";
    }

    char buf[20];
    
    // Format the date
    strftime(buf, sizeof(buf), "%Y-%m-%d", &timeinfo);
    return string(buf);
}
}
