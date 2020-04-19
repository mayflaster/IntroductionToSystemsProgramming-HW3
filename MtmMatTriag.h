#ifndef EX3_MTMMATTRIAG_H
#define EX3_MTMMATTRIAG_H


#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "MtmMatSq.h"

using std::size_t;

namespace MtmMath {



    template <typename T>

    class MtmMatTriag: public MtmMatSq<T> {
        bool is_upper;

/*
 * this function gets a matrix after resizing and make it triangular (block
 * the Inaccessible areas in it.
 */
       void turnResizeMatToTriag(size_t old_dim){
           for (size_t i = 0; i < this->getCols() - old_dim; i++) {
               size_t current = old_dim + i;
               this->operator[](current).blockVectorFields(current, true);
           }
        }

        /*
         * this function gets a matrix and check if it is triangular.
         * return true if it is upper triangular matrix
         * false if it is lower,  throws IllegalInitialization error if it is
         * not a triangular matrix at all. if it both lower and upper, it
         * returns true by default
         */

        bool isTriag(const MtmMat<T>& m){

            bool is_upper_triag=true;
            bool is_lower_triag=true;
            for (size_t i = 0; i <m.getCols() ; ++i) {
                for (size_t j = 0; j <i ; ++j) {
                    if(m[i][j]!=0){
                        is_upper_triag=false;
                    }
                    if(m[j][i]!=0){
                        is_lower_triag=false;
                    }
                    if(!is_upper_triag&&!is_lower_triag){
                        throw MtmExceptions::IllegalInitialization();
                    }
                }
            }
            return is_upper_triag;
        }
    public:

        /*
         * Triangular Matrix constructor, m is the number of rows and columns in the matrix,
         * val is the initial value for the matrix elements and isUpper_ is whether it is upper
         * Rectangular matrix (true means it is)
         */
        MtmMatTriag (size_t m, const T& val=T(), bool isUpper_t=true):
        MtmMatSq<T>(m,val), is_upper(isUpper_t){
                for (size_t i = 0; i<m ; i++){
                    for (size_t j = 0; j<m ; j++){
                        if ((i>j && isUpper_t) || (i<j && !isUpper_t)){
                            this->operator[](i)[j] = 0;
                        }
                    }
                    this->operator[](i).blockVectorFields(i,isUpper_t);
                }
        }

        /*
        * this function get a regular matrix, checks if it is a triangular
        * matrix and do a convertion to MtmMatSq type and returns it.
        * it throws Illegal Initialization error if the matrix is not
         * triangular.
        */

        explicit MtmMatTriag(const MtmMat<T>& m):MtmMatSq<T>(m),is_upper(true){
            is_upper=isTriag(m);
            for (size_t i = 0; i<m.getRows(); i++){
                this->operator[](i).blockVectorFields(i,is_upper);
            }
        }

        MtmMatTriag(const MtmMatTriag& mat)= default;
        MtmMatTriag& operator=(const MtmMatTriag&)= default;





        /*
        * resizes a matrix to dimension dim, new elements gets the value val.
        */

        void resize(Dimensions dim, const T& val=T()) override{
            Dimensions oldDim(this->getMatDim());
            this->MtmMatSq<T>::resize(dim,val);
            if(dim.getRow()>oldDim.getRow()) {
                bool is_lower_triag=!is_upper;
                if (is_lower_triag) {
                    this->transpose();
                }
                this->turnResizeMatToTriag(oldDim.getCol());
                if (is_lower_triag) {
                    this->transpose();
                }
            }
        }

        /*
         * Performs transpose operation on triangular matrix
        */

        void transpose(){
            is_upper = !is_upper;
            this->unblockMat();
            MtmMat<T>::transpose();
            for (size_t i = 0; i<this->getRows(); i++){
                this->operator[](i).blockVectorFields(i,is_upper);
            }
        }





    };

}

#endif //EX3_MTMMATTRIAG_H
