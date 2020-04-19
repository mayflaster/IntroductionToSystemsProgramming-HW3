#ifndef EX3_MTMMATREC_H
#define EX3_MTMMATREC_H

#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "MtmMat.h"

using std::size_t;

namespace MtmMath {

    template<typename T>
    class MtmMatSq : public MtmMat<T> {

       /*
        * this function gets number of rows and cols of a matrix and check if
        * the matrix is square.
        */
        void isMatSq(size_t row,size_t col){
            if(row!=col){
                throw MtmExceptions::IllegalInitialization();
            }
        }
    public:


        /**
         *
         * gets the number of row and column and return a Dimentions object
         * with this information
         */
        const Dimensions createDim(size_t m) const {
            Dimensions d(m,m);
            return d;
        }
        /*
         * Rectangular Matrix constructor, m is the number of rows and columns
         *
         * in the matrix
         * and val is the initial value for the matrix elements.
         */
        MtmMatSq (size_t m, const T& val=T()): MtmMat<T>(createDim(m),val){}

        /*
         * this function get a regular matrix, checks if it is a triangular
         * matrix and do a convertion to MtmMatSq type and returns it.
        * it throws Illegal Initialization error if the matrix is not square.

         */
        explicit MtmMatSq (MtmMat<T> m): MtmMat<T>(Dimensions(m.getRows(),
                m.getCols()), m.getMatVec()){
            isMatSq(m.getRows(),m.getCols());
        }

        MtmMatSq(const MtmMatSq& mat)= default;
        MtmMatSq& operator=(const MtmMatSq&)= default;

        /*
        *  reshapes matrix so linear elements value are the same without
         *  changing num of elements.
        */
        void reshape(Dimensions newDim) override{
            throw MtmExceptions::ChangeMatFail(this->getMatDim() ,newDim);
        }

        /*
        * resizes a matrix to dimension dim, new elements gets the value val.
        */
        void resize(Dimensions dim, const T& val=T()) override{
            if (dim.getCol() != dim.getRow()){
                throw MtmExceptions::ChangeMatFail(this->getMatDim(),dim);
            }
            this->MtmMat<T>::resize(dim,val);

        }

    };

}

#endif //EX3_MTMMATREC_H
