#ifndef EX3_MTMVEC_H
#define EX3_MTMVEC_H
#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "Complex.h"

using std::size_t;
using std::vector;

namespace MtmMath {
    template <typename T>
    //typedef double T;
    class MtmVec {
        bool col_vec;
        vector<T> vec;
        vector<bool> is_blocked_index;


        /*
         * checking if we can add or sub vectors
         */
         const void checkDimensionForAddAndSub(const MtmVec<T>& v1 ,const
         MtmVec<T>& v2){
            if(v1.col_vec!=v2.col_vec||v2.vec.size()!=v1.vec.size()) {
                Dimensions v1_dim= getVectorDim(v1);
                Dimensions v2_dim= getVectorDim(v2);
                throw MtmExceptions::DimensionMismatch(v1_dim,v2_dim);
            }
        }

        /*
         * gets rows and cols and check if it is a vector
         */
        const bool dimensionsArntVector(size_t rows,size_t cols){
            return (rows==0||cols==0||(rows!=1&&cols!=1));

        }
        const bool dimensionsAreTranspose(Dimensions dim,bool v_is_col){
            return ((v_is_col&&dim.getCol()!=1)||(!v_is_col&&dim.getRow()!=1));
        }
        const void checkIfCanResize(Dimensions dim, MtmVec<T> v){
            if(dimensionsArntVector(dim.getRow(),dim.getCol())||
            dimensionsAreTranspose(dim,v.col_vec)) {
                throw MtmExceptions::ChangeMatFail(getVectorDim(v),dim);
            }

        }
        const void validIndex(size_t index, const MtmVec<T>& v)const{
            if (v.vec.size()<=index) {
                throw MtmExceptions::AccessIllegalElement();
            }
        }
    public:

        const Dimensions getVectorDim(const MtmVec<T>& v) const {
            Dimensions v_dim;
            if (v.col_vec) v_dim.setDimensions(1,v.vec.size());
            else v_dim.setDimensions(v.vec.size(),1);
            return v_dim;
        }

        /*
         * Vector constructor, m is the number of elements in it and val is the initial value for the matrix elements
         */
        MtmVec<T>(size_t m, const T& val=T()): col_vec(true){
            if(m==0){
                throw MtmExceptions::IllegalInitialization();
            }
            try {
                vec.resize(m, val);
                is_blocked_index.resize(m, false);
            }
            catch(std::bad_alloc& e){
                throw MtmExceptions::OutOfMemory();
            }
        }
        MtmVec<T>(const MtmVec<T>& v)= default;
        MtmVec<T>& operator=(const MtmVec<T>&)= default;

        class iterator{
        protected:
            typename vector<T>::iterator vector_iterator;
        public:
            iterator(typename vector<T>::iterator iterator1):
            vector_iterator(iterator1){};
            iterator& operator++(){
                ++vector_iterator;
                return *this;
            }
            T& operator*(){
                return *vector_iterator;
            }

            friend bool operator==(const iterator& a, const iterator& b){
                bool c (a.vector_iterator==b.vector_iterator);
                return c;
            }

            friend bool operator!=(const iterator& a, const iterator& b){
                bool c= !(a.vector_iterator==b.vector_iterator);
                return c;
            }
            typename std::vector<T>::iterator& operator+(int shift){
                vector_iterator+=shift;
                iterator it(vector_iterator);
                return vector_iterator;
            }/*
            iterator getVectorIterator(){
                return vector_iterator;
            }*/
        };

        iterator begin(){
            iterator iterator1(vec.begin());
            return iterator1;
        }

        iterator end(){
            iterator iterator1( vec.end());
            return iterator1;
        }

           class nonzero_iterator :public iterator{
            vector<T> vector2;
        public:
            friend class MtmVec<T>;
            nonzero_iterator(vector<T>& vec) : iterator(vec.begin()),
            vector2(vec){};
            typename vector<T>::iterator operator++(){
                ++this->vector_iterator;
                while (this->vector_iterator != vector2.end()
                &&*this->vector_iterator == 0){
                    ++this->vector_iterator;
                }
                return this->vector_iterator;
            }
        };
        nonzero_iterator nzbegin() {
            nonzero_iterator iterator1(vec);
            if (*iterator1 == 0) {
                ++iterator1;
            }
            return iterator1;
        }

        nonzero_iterator nzend() {
            nonzero_iterator iterator1(vec);
            iterator1.vector_iterator=vec.end();
            return iterator1;
        }

        /*
         * Function that get function object f and uses it's () operator on each element in the vectors.
         * It outputs the function object's * operator after iterating on all the vector's elements
         */
        template <typename Func>
        T vecFunc(Func& f) const {
            Func f_copy=f;
            for (size_t i = 0; i<vec.size(); i++){
                f_copy(vec[i]);
            }
            return (*f_copy);

        }

        /*
         * Resizes a vector to dimension dim, new elements gets the value val.
         * Notice vector cannot transpose through this method.
         */
        void resize(Dimensions dim, const T& val=T()){
            checkIfCanResize(dim,*this);
            try{
                if (col_vec){
                    vec.resize(dim.getRow(),val);
                    is_blocked_index.resize(vec.size(), false);
                }
                else{
                    vec.resize(dim.getCol(),val);
                    is_blocked_index.resize(vec.size(), false);
                }
            }
            catch(std::bad_alloc& e){
                throw MtmExceptions::OutOfMemory();
            }

        }

        /*
         * Performs transpose operation on matrix
         */
        void transpose(){
            col_vec= !col_vec;
        }
        /*
         * adding vector to vector
         */
        MtmVec<T>& operator+=(const MtmVec<T>& v){
            checkDimensionForAddAndSub (*this, v);
            for (size_t i = 0; i<vec.size(); ++i){
                vec[i] += v.vec[i];
            }
            return *this;
        }
        /*
        * adding scalar to vector
        */
        MtmVec<T>& operator+=(const T& t){
            for (size_t i = 0; i<vec.size(); ++i){
                vec[i] += t;
            }
            return *this;
        }
        MtmVec<T> operator-() const{
            MtmVec<T> new_vec =*this;
            for (size_t i = 0; i<vec.size(); i++){
                new_vec.vec[i] = -vec[i];
            }
            return new_vec;
        }
        MtmVec<T>& operator *=(const T& t){
            for (size_t i=0; i<vec.size();i++){
                vec[i]*=t;
            }
            return *this;
        }
        T& operator[](size_t index){
            validIndex(index,*this);
            if ( is_blocked_index[index] == 1){
                throw MtmExceptions::AccessIllegalElement();
            }
            return vec[index];
        }
        const  T& operator[](size_t index) const {
            validIndex(index,*this);
            return vec[index];
        }

        const size_t vector_size() const{
            return vec.size();
        }
        const bool is_col () const {
            return col_vec;
        }

        /*
         * gets rows vector of a triangular matrix and block the fields
         * according to the type of the matrix (upper or lower) and the
         * number of rows.
         */

        void blockVectorFields ( size_t i, bool is_upper){

            for (size_t j=0; j<vec.size(); j++){
                if (is_upper && j<i){
                    vec[j]=0;
                    is_blocked_index[j] = true;
                }
                else if (!is_upper && j>i){
                    vec[j]=0;
                    is_blocked_index[j] = true;
                }
            }
        }

        /*
        * gets vector and make its fields accessible
        */
        void unblockVectorFields (){
            for (size_t i=0; i<vector_size(); i++){
                is_blocked_index[i]= false;
            }
        }

    };

    template <typename T>
    MtmVec<T> operator+(const MtmVec<T>& a, const MtmVec<T>& b){
        return MtmVec<T>(a)+= b;
    }

    template <typename T>
    MtmVec<T> operator+(const MtmVec<T>& a, const T& t){
        return MtmVec<T>(a) += t;
    }

    template <typename T>
    MtmVec<T> operator+(const T& t,const MtmVec<T>& a){
        return MtmVec<T>(a) += t;
    }

    template <typename T>
    MtmVec<T> operator-(const MtmVec<T>& a, const MtmVec<T>& b){
        return MtmVec<T>(a) += -b;
    }

    template <typename T>
    MtmVec<T> operator-(const MtmVec<T>& a, const T& t){
        return MtmVec<T>(a) += -t;
    }

    template <typename T>
    MtmVec<T> operator-(const T& t,const MtmVec<T>& a){
        return -MtmVec<T>(a) += t;
    }

    template <typename T>
    MtmVec<T> operator*(const MtmVec<T>& a, const T& t){
        return MtmVec<T>(a) *= t;
    }

    template <typename T>
    MtmVec<T> operator*(const T& t,const MtmVec<T>& a){
        return MtmVec<T>(a) *= t;
    }


}

#endif //EX3_MTMVEC_H