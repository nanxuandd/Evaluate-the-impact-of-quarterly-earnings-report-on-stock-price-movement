#include <iostream>
#include <vector>
#include <cmath>
#include "Calculation.h"
using namespace std;

namespace final{

// AAR
Vec AAR(Matrix& ABReturnsMatrix) { 
    int row = ABReturnsMatrix.size(); // number of assest
    int col = ABReturnsMatrix[0].size(); // number of days
    Vec Results_AAR(col, 0.0);
    
    // calculate the ave AAR for each day ana store it in Results_AAR
    for (int i = 0; i < col; i++) {
        double sum = 0.0;
        for (int j = 0; j < row; j++) {
            sum += ABReturnsMatrix[j][i];
        }
        Results_AAR[i] = sum / row;
    }
    return Results_AAR; // 1 X 2N+1
}

// CAAR 
Vec CAAR(Vec& Results_AAR) {
    int col = Results_AAR.size();
    Vec Results_CAAR(col, 0.0);
    double sum = 0.0;
    for (int i = 0; i < col; i++) {
        sum += Results_AAR[i];
        Results_CAAR[i] = sum;
    }
    return Results_CAAR; // 1 X 2N+1
}

// Ave AAR for 40 samples

Matrix Ave_AAR(BigMatrix& AAR_matrix) {
    int num_samples = AAR_matrix[0].size(); //40
    int num_days = AAR_matrix[0][0].size(); //2N
    
    Matrix Results_Ave_AAR;
    for (int i = 0; i < 3;i++){ //i is group number
        Vec results_Ave_AAR;
        for (int j = 0; j < num_days; j++){ // j is 2N days, it is the final postion
             double sum = 0.0;
             
             for (int k = 0; k < num_samples; k++){ // k is 40 samples, it is the second postion
                sum = sum + AAR_matrix[i][k][j];
             }
            results_Ave_AAR.push_back(sum / num_samples);
            
        }
        Results_Ave_AAR.push_back(results_Ave_AAR);
    }
    return Results_Ave_AAR;
}



// Ave CAAR for 40 
Matrix Ave_CAAR(BigMatrix& CAAR_matrix) {
    int num_samples = CAAR_matrix[0].size(); //40
    int num_days = CAAR_matrix[0][0].size(); //2N
    
    Matrix Results_Ave_CAAR;
    
    for (int i = 0; i < 3;i++){
        Vec results_Ave_CAAR;
        for (int j = 0; j < num_days; j++){
             double sum = 0.0;
             
             for (int k = 0; k < num_samples; k++){
                sum = sum + CAAR_matrix[i][k][j];
             }
            results_Ave_CAAR.push_back(sum / num_samples);
            
        }
        Results_Ave_CAAR.push_back(results_Ave_CAAR);
    }
    return Results_Ave_CAAR;
}

// STD AAR for 40 samples
Matrix STD_AAR(BigMatrix& AAR_matrix, Matrix& Results_Ave_AAR) {
    int num_samples = AAR_matrix[0].size(); //40
    int num_days = AAR_matrix[0][0].size(); //2N
    
    Matrix Results_Std_AAR;
    for (int i = 0; i < 3;i++){
        Vec results_Std_AAR;
        for (int j = 0; j < num_days; j++){
             double sqr_sum = 0.0;
             
             for (int k = 0; k < num_samples; k++){
                sqr_sum += pow(AAR_matrix[i][k][j]-Results_Ave_AAR[i][j],2);
             }
            results_Std_AAR.push_back(sqrt(sqr_sum / num_samples));
            
        }
        Results_Std_AAR.push_back(results_Std_AAR);
    }
    return Results_Std_AAR;
}


// STD CAAR for 40 

Matrix STD_CAAR(BigMatrix& CAAR_matrix, Matrix& Results_Ave_CAAR) {
    int num_samples = CAAR_matrix[0].size(); //40
    int num_days = CAAR_matrix[0][0].size(); //2N
    
    Matrix Results_Std_CAAR;
    for (int i = 0; i < 3;i++){
        Vec results_Std_CAAR;
        for (int j = 0; j < num_days; j++){
             double sqr_sum = 0.0;
             
             for (int k = 0; k < num_samples; k++){
                sqr_sum += pow(CAAR_matrix[i][k][j]-Results_Ave_CAAR[i][j],2);
             }
            results_Std_CAAR.push_back(sqrt(sqr_sum / num_samples));
            
        }
        Results_Std_CAAR.push_back(results_Std_CAAR);
    }
    return Results_Std_CAAR;
}


// Final Matrix
BigMatrix Final(BigMatrix & arr_matrix,BigMatrix &caar_matrix) {
    BigMatrix final_matrix(3);
    
    Matrix Results_Ave_AAR = Ave_AAR(arr_matrix);
    Matrix Results_Ave_CAAR = Ave_CAAR(caar_matrix);
    Matrix Results_Std_AAR = STD_AAR(arr_matrix, Results_Ave_AAR);
    Matrix Results_Std_CAAR = STD_CAAR(caar_matrix, Results_Ave_CAAR);
    
    for (int i = 0; i < 3; i++) {
        
        final_matrix[i].push_back(Results_Ave_AAR[i]);
        final_matrix[i].push_back(Results_Std_AAR[i]);
        final_matrix[i].push_back(Results_Ave_CAAR[i]);
        final_matrix[i].push_back(Results_Std_CAAR[i]);
        
    }

    return final_matrix;
}

}
