// #include "Bootstrap.h"
#include "UI.h"

using namespace std;
using namespace final;

int main() {
    vector<Stock> totalStock;
    StockGroup stockgroup;
  
    // stock data processing
    populateStock(totalStock);
    sortStock(totalStock);
    divideStock(totalStock, stockgroup); // stockgroup include three vectors

    executeUI(stockgroup, totalStock);

    cout << "Done" << endl;

    return 0;
}
