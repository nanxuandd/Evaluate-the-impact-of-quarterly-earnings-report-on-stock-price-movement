#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // For system() function

using namespace std;

// Assuming BigMatrix is a type alias for vector<vector<double>>
using BigMatrix = vector<vector<double>>;

namespace final {

    void writeDataToFile(const string& fileName, const vector<double>& xData, const vector<double>& yData) {
        ofstream tempDataFile(fileName);
        if (!tempDataFile) {
            cerr << "Cannot create temp data file..." << endl;
            return;
        }
        for (size_t i = 0; i < xData.size(); ++i) {
            tempDataFile << xData[i] << " " << yData[i] << endl;
        }
        tempDataFile.close();
    }

    void plotPictures(const vector<double>& xData, const vector<double>& yData1,
                      const vector<double>& yData2, const vector<double>& yData3) {
        const char* dataFiles[] = {"Missgroup.dat", "Meetgroup.dat", "Beatgroup.dat"};
        
        writeDataToFile(dataFiles[0], xData, yData1);
        writeDataToFile(dataFiles[1], xData, yData2);
        writeDataToFile(dataFiles[2], xData, yData3);

        FILE *gnuplotPipe = popen("gnuplot -persist", "w");
        if (!gnuplotPipe) {
            cerr << "Gnuplot not found..." << endl;
            return;
        }

        // Set up the plot
        fprintf(gnuplotPipe, "set grid\n");
        fprintf(gnuplotPipe, "set title 'average CAAR'\n");
        fprintf(gnuplotPipe, "set xlabel 'Date'\n");
        fprintf(gnuplotPipe, "set ylabel 'CAAR'\n");
        fprintf(gnuplotPipe, "set xrange [%lf:%lf]\n", xData.front(), xData.back());
        fprintf(gnuplotPipe, "set yrange [*:*]\n"); // Auto-scale y-axis
        fprintf(gnuplotPipe, "set arrow from 0, graph(0,0) to 0, graph(1,1) nohead lc rgb 'red'\n");
        fprintf(gnuplotPipe, "plot '%s' with lines title 'Miss' lc rgb 'red', \
                                   '%s' with lines title 'Meet' lc rgb 'green', \
                                   '%s' with lines title 'Beat' lc rgb 'blue'\n",
                dataFiles[0], dataFiles[1], dataFiles[2]);
        fflush(gnuplotPipe);
        pclose(gnuplotPipe);
    }

    void plotGnuplot(const BigMatrix& final_matrix) {
        size_t nIntervals = final_matrix[0][2].size();
        double xMin = -static_cast<double>(nIntervals) / 2;
        double xMax = static_cast<double>(nIntervals) / 2;

        // Adjust xMin for even number of intervals
        if (nIntervals % 2 == 0) {
            xMin += 0.5;
        }

        vector<double> xData(nIntervals);
        vector<double> yData1(nIntervals);
        vector<double> yData2(nIntervals);
        vector<double> yData3(nIntervals);

        for (size_t i = 0; i < nIntervals; ++i) {
            xData[i] = xMin + static_cast<double>(i);
            yData1[i] = final_matrix[0][2][i];
            yData2[i] = final_matrix[1][2][i];
            yData3[i] = final_matrix[2][2][i];
        }

        plotPictures(xData, yData1, yData2, yData3);
    }
}