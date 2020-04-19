#ifndef EX3_MTMMAT_H
#define EX3_MTMMAT_H


#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "MtmVec.h"

using std::size_t;
using std::vector;
//using MtmMath::MtmVec;



namespace MtmMath {

/*
 * get's dimentions of 2 matrix or vectors and check if add or sub opertaions
 * are possible
 */
    const void checkDimensionForAddAndSub(Dimensions d1,Dimensions d2){
        if(d1!=d2) {
            throw MtmExceptions::DimensionMismatch(d1,d2);
        }
    }

    template <typename T>
    class MtmMat {
        size_t rows,cols;
        vector<MtmMath::MtmVec<T> > mat;

        /*
         * check if the index of the matrix is valid for read and write
         * with [] operator
         */
        const void validIndex(size_t index,const  MtmMat<T>& mat)const{
            if (mat.mat.size()<=index) {
                throw MtmExceptions::AccessIllegalElement();
            }
        }



    public:
        MtmMat<T>(const MtmMat<T>& mat)= default;
        MtmMat<T>& operator=(const MtmMat<T>&)= default;

        const void checkDimensionForMultyply(Dimensions d1,Dimensions d2){
            if(d1.getCol()!=d2.getRow()) {
                throw MtmExceptions::DimensionMismatch(d1,d2);
            }
        }

        const Dimensions getMatDim()const {
            Dimensions mat_dim (rows,cols);
            return mat_dim;
        }

        /*
         * Matrix constructor, dim_t is the dimension of the matrix and
         val is the initial value for the matrix elements
        */
        MtmMat<T>(Dimensions dim_t, const T& val=T()): rows(dim_t.getRow()),
                                                    cols(dim_t.getCol()){
            if (dim_t.getCol()==0 || dim_t.getRow() ==0){
                throw MtmExceptions::IllegalInitialization();
            }
            try{
                MtmVec<T> v1(cols,val);
                v1.transpose();
                mat.vector<MtmMath::MtmVec<T>>::resize(rows,v1);
            }
            catch(std::bad_alloc& e){
                throw MtmExceptions::OutOfMemory();
            }

        }

        MtmMat<T>(Dimensions dim_t, vector<MtmVec<T>> metrix): rows(dim_t.getRow()),
                                                    cols(dim_t.getCol()),mat(metrix){}

        vector<MtmVec<T> > getMatVec(){
            return mat;
        }
//        /operators/
        /*
         * add const scalar to matrix by using MtmVec + operator
         */
        MtmMat<T>&operator+= (const T& t){
            unblockMat();
            for (size_t i=0; i<mat.size(); ++i){
                mat[i] += t;  //mat[i] is MtmVec type
            }
            return *this;
        }

     /*   MtmMat<T>&operator-= (const T& t){
            return *this+= -t;
        }*/

        MtmMat<T>&operator+= (const MtmVec<T>& vec){
            unblockMat();
            if (!vec.is_col()){
                MtmVec<T> temp = vec+mat[0];
                for (size_t i=0;i<cols;i++){
                    this->mat[0][i]=temp[i];
                }
                return *this;
            }
                //case2 - (col v+mat(row,1))
            else {
                MtmVec<T> mat_vec(rows,0);
                for (size_t i=0; i<rows;i++){
                    mat_vec[i]=mat[i][0];
                }
                MtmVec<T> temp = vec+mat_vec;
                for (size_t i=0;i<rows;i++){
                    this->mat[i][0]=temp[i];
                }
                return *this;
            }
        }

        /*
         * add matrix scalar to matrix by using MtmVec + operator
         */
        MtmMat<T>&operator+= (const MtmMat<T>& mat1){
            unblockMat();
            checkDimensionForAddAndSub(getMatDim(), mat1.getMatDim());
            for (size_t i=0; i<mat.vector<MtmVec<T>>::size(); ++i){
                mat[i] += mat1[i]; //add vector to vector
            }
            return *this;
        }


        MtmMat<T> operator-(){
            unblockMat();
            MtmMat<T> new_mat=*this; // 0 is temp val
            for (size_t i=0; i<mat.size();i++){
                new_mat.mat[i] = -mat[i];
            }
            return new_mat;
        }


        /*
         * //sub const matrix
         */
        MtmMat<T>&operator-= (const MtmMat<T>& mat1){
            unblockMat();
            MtmMat<T> temp = mat1;
            return *this+= -temp;
        }

        /*
         * multiply matrix with const scalar
         */
        MtmMat<T>&operator*= (const T& t){
            unblockMat();
            for (size_t i=0; i<mat.size(); ++i){
                mat[i] *= t;
            }
            return *this;
        }

        /*
         * multiply with const vector
         */

        MtmMat<T>& operator*= (const MtmVec<T>& vec){
            checkDimensionForMultyply(getMatDim(),vec.getVectorDim(vec));
            Dimensions dim(rows,1);
            MtmMat<T> result_mat(dim,0);
            unblockMat();
            if (vec.is_col()){  //case1 - mat(rows*cols) * col vec
                for (size_t i=0;i<rows;i++){
                    for (size_t j=0; j<cols; j++){
                        result_mat[i][0] += vec[j]*mat[i][j];
                    }
                }
                this->resize(dim,0);
                *this=result_mat;
            }
            else {        //case 2 - mat(row,1) * row vec
                Dimensions dim2(this->rows,vec.vector_size());
                result_mat.resize(dim2,0);
                for (size_t i=0;i<result_mat.rows;i++){
                    for (size_t j=0;j<result_mat.cols;j++){
                        result_mat[i][j] = mat[i][0]*vec[j];
                    }
                }
                this->resize(dim2,0);
                *this=result_mat;
            }
            unblockMat();
            return *this;
        }

        MtmVec<T>& operator[] (size_t index){
            validIndex(index,*this);
            return mat[index];
        }

        const  MtmVec<T>& operator[] (size_t index) const {
            validIndex(index,*this);
            return mat[index];
        }

        /*
         * Function that get function object f and uses it's () operator on each element in the matrix columns.
         * It outputs a vector in the size of the matrix columns where each element is the final output
         * by the function object's * operator
        */
        template <typename Func>

        MtmVec<T> matFunc(Func& f) const{
            MtmVec<T> result_vec(cols,0);
            for (size_t i=0;i<cols;i++){
                Func f_copy=f;
                for (size_t j=0; j<rows; j++){
                    f_copy(mat[j][i]);
                }
                result_vec[i] = (*f_copy);
            }
            return result_vec;

        }

        /*
         * resizes a matrix to dimension dim, new elements gets the value val.
        */
        virtual void resize(Dimensions dim, const T& val=T()){
            if (dim.getCol()==0 || dim.getRow()==0){
                throw  MtmExceptions::ChangeMatFail(getMatDim(),dim);
            }
            try{
                Dimensions row_dim(1,dim.getCol());
                for (size_t i=0; i<rows;i++){
                    mat[i].resize(row_dim,val);
                }
                MtmVec<T> val_vec(dim.getCol(),val);
                val_vec.transpose();
                mat.resize(dim.getRow(),val_vec);
                //update cols and rows
                rows=dim.getRow();
                cols=dim.getCol();
            }
            catch(std::bad_alloc & e){
                throw MtmExceptions::OutOfMemory();
            }
        }


        /*
         * reshapes matrix so linear elements value are the same without changing num of elements.
        */
        virtual void reshape (Dimensions newDim){
            if (newDim.getRow()*newDim.getCol() != rows*cols){
                throw  MtmExceptions::ChangeMatFail(getMatDim(),newDim);
            }
            MtmMat<T> temp_mat(newDim,0);
            size_t r=0;
            size_t c=0;
            for (size_t j=0;j<cols;j++){
                for (size_t i=0;i<rows; i++){
                    temp_mat[r][c]= mat[i][j];
                    r++;
                    if (r>=newDim.getRow()){
                        c++;
                        r=0;
                    }
                }
            }
            this->resize(newDim,0);
            *this=temp_mat;

        }

        /*
         * Performs transpose operation on matrix
        */
        virtual void transpose(){
            MtmMath::Dimensions new_dim(cols,rows);
            MtmMat<T> temp_mat(new_dim,0);
            //copy to new matrix to save information
            for (size_t i=0;i<rows; i++){
                for (size_t j=0; j<cols; j++){
                    temp_mat[j][i] = mat[i][j];
                }
            }
            //change matrix size
            MtmVec<T> v(rows,0);
            v.transpose();
            mat.vector<MtmVec<T>>::resize(cols,v);
            MtmMath::Dimensions vec_dim(1,rows);
            for(size_t i=0;i<cols; i++){
                mat[i].resize(vec_dim,0);
                mat[i].transpose();
            }

            //copy temp mat to original mat
            *this=temp_mat;
        }

        const size_t getRows()const {
            return rows;
        }
        const size_t getCols() const {
            return cols;
        }


        /*
         *  releases the blocked fields in the matrix and allows opertaions
         */
        void unblockMat (){
            for (size_t i=0;i<mat.size(); i++){
                mat[i].unblockVectorFields();
            }
        }

        class iterator{
        protected:
            typename MtmVec<T>::iterator mat_iterator;
            size_t current_row;
            size_t current_col;
            vector<MtmVec<T>>& mat;
            size_t num_of_cols;
            size_t num_of_rows;
            void nextIndex(){
                if(current_row<num_of_rows-1) {
                    current_row++;
                }
                else{
                    current_col++;
                    current_row=0;
                }
                mat_iterator= mat[current_row].begin()+current_col;
                if(current_col==num_of_cols){
                    mat_iterator= mat[0].end();
                }
            }
        public:
            iterator(typename MtmVec<T>::iterator iterator1,
                    vector<MtmVec<T>>& matrix,size_t cols,size_t rows,
                    size_t col=0, size_t row=0):
            mat_iterator(iterator1),
                    current_row(row),current_col(col),mat(matrix),num_of_cols
                    (cols),num_of_rows(rows){};
            iterator& operator++(){
                nextIndex();
                return *this;
            }
            T& operator*(){
                return *mat_iterator;
            }

            friend bool operator==(const iterator& a, const iterator& b){
                bool d (a.mat_iterator==b.mat_iterator);
                return d;
            }

            friend bool operator!=(const iterator& a, const iterator& b){
                bool c= !(a.mat_iterator==b.mat_iterator);
                return c;
            }


        };

        iterator begin(){
            iterator iterator1(mat[0].begin(),mat,cols,rows);
            return iterator1;
        }

        iterator end(){
            iterator iterator1( mat[0].end(),mat,cols,rows);
            return iterator1;
        }

        class nonzero_iterator: public iterator{
        public:
            nonzero_iterator
            (typename MtmVec<T>::iterator iterator1, vector<MtmVec<T>>& matrix,
                    size_t cols,size_t rows,size_t col=0,size_t row=0):
                    iterator(iterator1,matrix, cols,rows,col,row){};
            iterator& operator++(){
                this->nextIndex();
                while(this->current_col!=this->num_of_cols&&
                *this->mat_iterator==0){
                    this->nextIndex();
                }
                return *this;
            }
        };
        nonzero_iterator nzbegin(){
            nonzero_iterator iterator1(mat[0].begin(),mat,cols,rows);
            if(*iterator1==0){
                ++iterator1;
            }
            return iterator1;
        }

        nonzero_iterator nzend(){
            nonzero_iterator iterator1( mat[0].end(),mat,cols,rows);
            return iterator1;
        }


    };



    template <typename T>
    /*
     * performs an add operation between 2 matrix
     */
    MtmMat<T> operator+(const MtmMat<T>& a, const MtmMat<T>& b){
        return MtmMat<T>(a) += b;
    }


    /*
    * performs an add operation between matrix and scalar
    */

    template <typename T>
    MtmMat<T> operator+(const MtmMat<T>& a, const T& t){
        return MtmMat<T>(a) += t;
    }


    /*
     * performs an add operation between matrix and scalar
    */

    template <typename T>
    MtmMat<T> operator+(const T& t,const MtmMat<T>& a){
        return MtmMat<T>(a) += t;
    }


    /*
    * performs a sub operation between 2 matrix
    */

    template <typename T>
    MtmMat<T> operator-(const MtmMat<T>& a, const MtmMat<T>& b){
        return MtmMat<T>(a) -= b;
    }

    /*
    * performs an add operation between matrix and vector
    */

    template <typename T>
    MtmMat<T> operator+(const MtmMat<T>& mat, const MtmVec<T>& v){
        checkDimensionForAddAndSub(mat.getMatDim(),v.getVectorDim(v));
        return MtmMat<T>(mat) += v;
    }



    /*
     * performs an add operation between matrix and vector
     */
    template <typename T>
    MtmMat<T> operator+(const MtmVec<T>& v,const MtmMat<T>& mat){
        checkDimensionForAddAndSub(v.getVectorDim(v),mat.getMatDim());
        return MtmMat<T>(mat) += v;
    }

    /*
* * performs a sub operation between matrix and vector and returns the
* reuslt matrix
*/


    template <typename T>
    MtmMat<T> operator-(const MtmMat<T>& mat, const MtmVec<T>& v){
        checkDimensionForAddAndSub(mat.getMatDim(),v.getVectorDim(v));
        return MtmMat<T>(mat) += -v;
    }

    /*
* * performs a sub operation between matrix and vector and returns the
* reuslt matrix
*/

    template <typename T>
    MtmMat<T> operator-(const MtmVec<T>& v,const MtmMat<T>& mat){
        checkDimensionForAddAndSub(v.getVectorDim(v),mat.getMatDim());
        return -MtmMat<T>(mat) += v;
    }

    /*
* * performs a sub operation between matrix and scalar and returns the
* reuslt matrix
*/

    template <typename T>
    MtmMat<T> operator-(const MtmMat<T>& a, const T& t){
        return MtmMat<T>(a) += -t;
    }

    /*
* * performs a sub operation between matrix and scalar and returns the
* reuslt matrix
*/
    template <typename T>
    MtmMat<T> operator-(const T& t,const MtmMat<T>& a){
        return -MtmMat<T>(a) += t;
    }

    /*
 * performs a multipication operation between matrix and scalar and returns the
* reuslt matrix
*/
    template <typename T>
    MtmMat<T> operator*(const MtmMat<T>& a, const T& t){
        return MtmMat<T>(a) *= t;
    }

    /*
     * * performs a multipication operation between matrix and scalar and returns
     * the reuslt matrix
 */

    template <typename T>
    MtmMat<T> operator*(const T& t,const MtmMat<T>& a){
        return MtmMat<T>(a) *= t;
    }

    /*
 * * performs a multipication between 2 vectors and returns the
 * reuslt matrix
 */

    template <typename T>
    MtmMat<T> operator*(const MtmVec<T>& v1, const MtmVec<T>& v2){
        Dimensions new_d(v1.vector_size(),v2.vector_size());
        MtmMat<T> result_mat(new_d,0);
        result_mat.checkDimensionForMultyply(v1.getVectorDim(v1), v2
        .getVectorDim(v2));
        //case 1 - row*col vectors
        if (!v1.is_col()){
            Dimensions new_d1(1,1);
            result_mat.resize(new_d1,0);
            for (size_t i = 0; i<v2.vector_size(); ++i){
                result_mat[0][0]+=v1[i]*v2[i];
            }
        }
            //case2-col*row vectors
        else{
            for (size_t i=0;i<v1.vector_size();i++){
                for (size_t j=0;j<v2.vector_size();j++){
                    result_mat[i][j] = v1[i]*v2[j];
                }
            }
        }
        return result_mat;
    }

    /*
 * * performs a multipication between matrix and vector and returns the
 * reuslt matrix
 */

    template <typename T>
    MtmMat<T> operator*(const MtmMat<T>& mat, const MtmVec<T>& v){
        return MtmMat<T>(mat) *= v;
    }

    /*
     * converts a regular row vector from a matrix to MtmVec type
     */
    template <typename T>
    static MtmVec<T> fromVectorToMtmVec(const MtmMat<T>& mat, size_t col){
        MtmVec<T> new_vec(mat.getRows(),0);
        for (size_t i=0; i<mat.getRows(); i++){
            new_vec[i]=mat[i][col];
        }
        return new_vec;
    }


    /*
     * * performs a multipication between vector and matrix and returns the
     * reuslt matrix
     */
    template <typename T>
    MtmMat<T> operator*(const MtmVec<T>& v, const MtmMat<T>& mat){
        Dimensions d_result(1,mat.getCols());
        MtmMat<T> result(d_result,0);
        result.checkDimensionForMultyply(v.getVectorDim(v), mat.getMatDim());
        MtmMat<T> tempMat = mat;
        tempMat.unblockMat();
        //case1 (col v * mat (1,col))
        if (v.is_col()){
            return v*mat[0];
        }
        //case2 - (row v*mat(row,1))
        else {
            for (size_t i=0; i<mat.getCols();i++){
                result[0][i]= (v*fromVectorToMtmVec(tempMat,i))[0][0];
            }
            //MtmVec mat_vec = fromVectorToMtmVec(mat,0);
            return result;
        }
    }



/*
 * performs a multipication operation between 2  matrixes and returns the reuslt
 * matrix
 */
    template <typename T>
    MtmMat<T> operator*(const MtmMat<T>& mat1, const MtmMat<T>& mat2){
        Dimensions new_dim(mat1.getRows(),mat2.getCols());
        MtmMat<T> result_mat(new_dim,0);
        result_mat.checkDimensionForMultyply(mat1.getMatDim(), mat2
        .getMatDim());
        MtmMat<T> tempmat1 = mat1;
        tempmat1.unblockMat();
        MtmMat<T> tempmat2 = mat2;
        tempmat2.unblockMat();
        Dimensions val_dim (1,1);
        MtmMat<T> val_mat(val_dim,0);
        for (size_t i=0; i<new_dim.getRow(); i++){
            for (size_t j=0; j<new_dim.getCol();j++){
                MtmVec<T> temp_row_vec = fromVectorToMtmVec(tempmat2,j);
                val_mat = tempmat1[i]*temp_row_vec;
                result_mat[i][j]= val_mat[0][0];
            }
        }
        return result_mat;
    }






}


#endif //EX3_MTMMAT_H