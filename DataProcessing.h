#include "Stock.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>  
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <stdexcept>

namespace final {
typedef vector<vector<Stock>> StockGroup;

void populateStock(vector<Stock>& totalStock); // Parse EPS information to stocks
void sortStock(vector<Stock>& totalStock); // Sort by surprisePercentage
void divideStock(vector<Stock>& totalStock, StockGroup& stockgroup);
bool myCmp(Stock& left, Stock& right); 
string formatDate(const string& date);
}
