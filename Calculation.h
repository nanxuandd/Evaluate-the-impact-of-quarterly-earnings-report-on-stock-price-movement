#include"Extractdata.h"

namespace final{
     
Vec AAR(Matrix& ABReturnsMatrix);
Vec CAAR(Vec& Results_AAR);
Matrix Ave_AAR(BigMatrix& AAR_matrix); 
Matrix Ave_CAAR(BigMatrix& CAAR_matrix);
Matrix STD_AAR(BigMatrix& AAR_matrix, Matrix& Results_Ave_AAR);
Matrix STD_CAAR(BigMatrix& CAAR_matrix, Matrix& Results_Ave_CAAR);
BigMatrix Final(BigMatrix & arr_matrix,BigMatrix &caar_matrix);
}