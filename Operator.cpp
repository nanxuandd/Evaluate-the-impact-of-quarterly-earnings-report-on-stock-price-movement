#include "Operator.h"
namespace final{
    
    Vec operator-(const Vec& X, const Vec& Y){
        int d = X.size();
        Vec U(d);
        for (int j = 0; j < d; j++) U[j] = X[j] - Y[j];
        return U;
    }


    Vec& operator+=(Vec & X,const Vec & Y){
        int d = Y.size();
        for(int j=0;j<d;j++){
            X[j]+=Y[j];
        }
        return X;
    }
    
    Vec& operator-=(Vec& X,const Vec& Y){
        int d=Y.size();
        for(int j=0;j<d;j++){
            X[j]-=Y[j];
        }
        return X;
    }
    
    std::ostream& operator<<(std::ostream& out,Vec& X){
        out << fixed << setprecision(3);
        for(Vec::iterator itr=X.begin();itr!=X.end();itr++){
            if(*itr>0){ out << *itr << "     ";}
            else{out << *itr << "    ";}
            size_t cur_index = distance(std::begin(X), itr); 
		    if(((int)cur_index+1)%10==0) out<<endl; 
        }
        out<<endl;
        return out;
    }
    
    std::ostream& operator<<(std::ostream& out,Matrix& Y){
        for(Matrix::iterator itr=Y.begin();itr!=Y.end();itr++){
           out << *itr;
        }
        out << endl;
        return out;
    }
             
    std::ostream& operator<<(std::ostream &out,BigMatrix &Z ){
        for(BigMatrix::iterator itr=Z.begin();itr!=Z.end();itr++){
            out << *itr;
        }
        out << endl;
        return out;
        }
        
    std::ostream& operator<<(std::ostream &out,vector<string> &U ){
        for(vector<string>:: iterator itr=U.begin();itr!=U.end();itr++){
            if(strlen((*itr).c_str())==2){
                out<<*itr<<"       ";
            }
            else if(strlen((*itr).c_str())==3){
                out<<*itr<<"   ";
            }
            else{
                out<<*itr<<"       ";
            }
            size_t cur_index = distance(std::begin(U), itr); 
		    if(((int)cur_index+1)%10==0) out<<endl; 
        }
        out << endl;
        return out;
    }

}