#include "Extractdata.h"
#include <unistd.h>
#include <mutex>

namespace final{
    void* myrealloc(void* ptr, size_t size)
{
	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}
int write_data2(void* ptr, size_t size, size_t nmemb, void* data)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)data;
	mem->memory = (char*)myrealloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory) {
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	}
	return realsize;
}

// Day0 is defined from earning announcement date of each stock, find N days before and N days after
string findStartDate(string day0, int N, vector<string> DATE) {
    auto it = find(DATE.begin(), DATE.end(), day0);
    it -= N;
    return *it;
}

string findEndDate(const string& day0, int N, const vector<string>& DATE) {
    auto start = find(DATE.begin(), DATE.end(), day0);
    if (start != DATE.end() && distance(start, DATE.end()) > N) {
        advance(start, N);
        return *start;
    }
    // Return empty string if 'day0' not found or 'N' exceeds bounds
    return ""; 
}


// Extract specified time range of the IWV info for helper function
vector<double> extractIWV(vector<PAIR> &IWVData, string start_date, string end_date) {
    vector<double> IWV;
    bool startCollecting = false;

    for (auto& pair : IWVData) {
        if (pair.first == start_date) {
            startCollecting = true;
        }

        if (startCollecting) {
            IWV.push_back(pair.second);
            if (pair.first == end_date) {
                break;
            }
        }
    }
    return IWV;
}

// Extract dates from -N to N (UI) - Option(2)
vector<string> getDates(string start, string end, vector<PAIR> &IWVData) {
    vector<string> IWV;
    int i = -1;
    for (vector<PAIR>::iterator ite = IWVData.begin(); ite != IWVData.end(); ite++) {
        if (i == 0) {
            IWV.push_back(ite->first);
            if (ite->first == end) {
                break;
            }
        } else {
            if (ite->first == start) {
                IWV.push_back(ite->first);
                i++;
            }
        }
    }
    return IWV;
}


vector<PAIR> retrieveIWVData()
{
    vector<PAIR> IWVData; // Vector of <string,double>. Usered in Helper Function

	// declaration of an object CURL 
	CURL* handle;

	// set up the program environment that libcurl needs 
	curl_global_init(CURL_GLOBAL_ALL);

	// curl_easy_init() returns a CURL easy handle 
	handle = curl_easy_init();

	// if everything's all right with the easy handle... 
	if (handle)
	{
		string url_common = "https://eodhistoricaldata.com/api/eod/";
		string start_date = "2023-01-03";
		string end_date = "2023-12-02";
		string api_token = "65692759d89391.08273836";  // or 63842dba8ab842.02615564

		struct MemoryStruct data;
		data.memory = NULL;
		data.size = 0;

  string symbol="IWV";
		string url_request = url_common + symbol + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d";
		curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());

		// Adding a user agent
		curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
		curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
		// curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);

		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

		// Perform, then store the expected code in result
		curl_easy_perform(handle);

		stringstream sData;
		sData.str(data.memory);
		string sDate, sPrice; // String date, String price
		double dPrice=0;
		string line;
		while (getline(sData, line)) {
			size_t found = line.find('-');
			if (found != std::string::npos)
			{
				sDate = line.substr(0, line.find_first_of(','));
				line.erase(line.find_last_of(','));
				sPrice = line.substr(line.find_last_of(',') + 1);
				dPrice = strtod(sPrice.c_str(), NULL);
				IWVData.push_back(PAIR(sDate,dPrice));
			}
		}
		free(data.memory);
		data.size = 0;
	}
	curl_easy_cleanup(handle);
	curl_global_cleanup();

	return IWVData;
}

// Calculate Daily Return, Abnormal Return，and Cummulative return 
vector<double> CalculateDailyReturns(const vector<double> &priceSeries) {
    vector<double> dailyReturns;
    int n = priceSeries.size();
    for (int i = 1; i < n; i++) {
        double returns = log(priceSeries[i] / priceSeries[i - 1]);
        dailyReturns.push_back(returns);
    }
    return dailyReturns;
}

vector<double> CalculateCumuReturns(vector<double> &priceSeries) {
    vector<double> cumulativereturn;
    double Cumureturns;
    int n = priceSeries.size();
    for (int i = 0; i < n - 1; i++) {
        Cumureturns = (priceSeries[i + 1] - priceSeries[0]) / priceSeries[0];
        cumulativereturn.push_back(Cumureturns);
    }
    return cumulativereturn;
}

vector<double> calculateAbnormalPrices(vector<double> &stockPrices, vector<double> &IWVPrices,vector<double> &cumulativereturn)
{
	vector<double> stockDailyReturns=CalculateDailyReturns(stockPrices);
	vector<double> IWVDailyReturns=CalculateDailyReturns(IWVPrices);
	vector<double> abnormalPrices;
	
	cumulativereturn = CalculateCumuReturns(stockPrices);
    abnormalPrices=stockDailyReturns-IWVDailyReturns;
    
	return abnormalPrices;
}

StockMap retrieveStock_IWVData(StockGroup& stockgroup, vector<PAIR> IWVData, int N) {
    // Extract trading dates using a range-based for loop
    vector<string> tradingDates;
    for (const auto& data : IWVData) {
        tradingDates.push_back(data.first);
    }
    
    // Create and start threads while storing them in a vector for easy management.
    vector<thread> threads;
    vector<StockMap> stockMaps(stockgroup.size());

    for (size_t i = 0; i < stockgroup.size(); ++i) {
        threads.emplace_back(retrieveStock_Helper, threadParams{
            stockgroup[i],
            tradingDates,
            IWVData,
            N,
            static_cast<int>(i + 1), // Thread IDs starting from 1
            stockMaps[i],
        });
    }

    // Wait for all threads to complete
    for (auto& t : threads) {
        t.join();
    }

    // Merge all StockMaps into one
    StockMap mergedStockMap;
    for (auto& sm : stockMaps) {
        mergedStockMap.insert(sm.begin(), sm.end());
    }

    return mergedStockMap;
}

// Progress Bar
void printProgress(size_t cur_index, size_t last_index, const vector<Stock>& stocks_in_group) {
    static mutex m;
    lock_guard<mutex> lock(m);

    // Calculate progress
    double progress = 100.0 * cur_index / last_index;

    // Create progress bar string
    int progress_int = static_cast<int>(progress);
    string progress_bar(progress_int, '*');
    progress_bar += string(100 - progress_int, ' ');

    // Print progress
    cout << fixed << showpoint << setprecision(3);
    cout << "\r\033[F" // Move cursor to the beginning of the line and up one line
         << "progress: [" << progress << "%] [" << progress_bar << "]" << endl;
}

StockMap retrieveStock_Helper(threadParams params) {
	
    vector<Stock> &stocks_in_group = params.stocks_in_group;
    
    vector<string> &tradingDates = params.tradingDates;
    
    vector<PAIR> &IWVData = params.IWVData;

    int N = params.N;
    int id = params.ID;

	// StockMap map<string,Stock*> 
    StockMap& stockPriceMap = params.stockPriceMap;
	string symbol;
	string day0;
	string start_date;
	string end_date;
	vector<double> IWVPrices;
	
	CURL* handle;
		
	// set up the program environment that libcurl needs 
	curl_global_init(CURL_GLOBAL_ALL);
	
	// curl_easy_init() returns a CURL easy handle 
	handle = curl_easy_init();
	// if everything's all right with the easy handle... 
	if (handle){
	
	 string url_common = "https://eodhistoricaldata.com/api/eod/";
	 string api_token = "65692759d89391.08273836";
	 
	for(vector<Stock>:: iterator ite=stocks_in_group.begin();ite!=stocks_in_group.end();ite++)
	{   
		// convert itr to index
		size_t cur_index = distance(std::begin(stocks_in_group), ite);
		size_t last_index = distance(std::begin(stocks_in_group), std::end(stocks_in_group));
		printProgress(cur_index, last_index, stocks_in_group);

		// retrieve stock data    	
		vector<double> stockPrices;
		vector<double> IWVPrices;
		symbol=ite->getSymbol();
		day0=ite->getEarningAnnouncementDate();
		start_date=findStartDate(day0,N,tradingDates);
		end_date=findEndDate(day0,N,tradingDates);

		if(end_date=="")// Filter the stocks that do not have 2N+1 day data
		{
			continue;
		}
		
		
		ite->setStartDate(start_date);
		ite->setEndDate(end_date);
		IWVPrices=extractIWV(IWVData,start_date,end_date);
		
	
			struct MemoryStruct data;
			data.memory = NULL;
			data.size = 0;
	
			string url_request = url_common + symbol + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d";
			curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());
	
			//adding a user agent
			curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
		
			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);
			// perform
			curl_easy_perform(handle);
			
			stringstream sData;
			sData.str(data.memory);
			string sDate, sPrice;
			double dPrice;
			string line;
			while (getline(sData, line)) {
				size_t found = line.find('-');
				if (found != std::string::npos)
				{
					sDate = line.substr(0, line.find_first_of(','));
					line.erase(line.find_last_of(','));
					sPrice = line.substr(line.find_last_of(',') + 1);
					dPrice = strtod(sPrice.c_str(), NULL);
					stockPrices.push_back(dPrice);
				}
			}
			ite->setgroupID(id);
			ite->setPrices(stockPrices);

			vector<double> cumulativereturn;

			ite->setAbnormalPrices(calculateAbnormalPrices(stockPrices,IWVPrices,cumulativereturn));
			ite->setCummulativeReturn(cumulativereturn);
			stockPriceMap.insert({symbol,&(*ite)});
			
			free(data.memory);
			data.size = 0;
		}	
	
	}
	curl_easy_cleanup(handle);
	curl_global_cleanup();
	return stockPriceMap;
}


vector<string> getOutofMapStocks(vector<Stock> &totalStock,StockMap& pricemaps){
	vector<string> stocksymbols;
	string symbol;
	 for(vector<Stock>::iterator itr= totalStock.begin();itr!=totalStock.end();itr++){
         symbol=itr->getSymbol();
         if (pricemaps.find(symbol) == pricemaps.end()){ // Find the symbol of those stocks that do not have 2N+1 days prices
              stocksymbols.push_back(symbol);
           }
      }
   return stocksymbols;
}
}
