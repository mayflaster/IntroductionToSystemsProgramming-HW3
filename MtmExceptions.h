#ifndef EX3_MTMEXCEPTIONS_H
#define EX3_MTMEXCEPTIONS_H

#include <exception>
#include <string>
#include <iostream>
#include "Auxilaries.h"

using std::string;
namespace MtmMath {
    namespace MtmExceptions {
        class MtmExceptions : public std::exception {
        public:
            virtual ~MtmExceptions() throw() {}
        };

        /*
         * Exception for illegal initialization of an object, needs to output
         * "MtmError: Illegal initialization values" in what() class function
         */
        class IllegalInitialization : public MtmExceptions {
        public:
            const char * what () const throw ()
            {
                    return "MtmError: Illegal initialization values";
            }
        };

        /*
         * Exception for Memory allocation failure for an object, needs to output
         * "MtmError: Out of memory" in what() class function
         */
        class OutOfMemory : public MtmExceptions {
        public:
            const char * what () const throw ()
            {
                    return "MtmError: Out of memory";
            }
        };

        /*
         * Exception for dimension mismatch during a mathematical function, needs to output
         * "MtmError: Dimension mismatch: (<mat 1 row>,<mat 1 col>) (<mat 2 row>,<mat 2 col>)"
         * in what() class function
         */

        string createErrorString(string error,Dimensions mat1,string type,Dimensions mat2){
            string errorString= error+mat1.to_string()+type+mat2.to_string();
            return errorString;}

        class DimensionMismatch : public MtmExceptions {
            Dimensions mat1;
            Dimensions mat2;
            string s;
        public:
            DimensionMismatch(Dimensions mat1,Dimensions mat2): mat1(mat1) ,mat2(mat2),
                            s(createErrorString("MtmError: Dimension mismatch: ",mat1," ",mat2)){}
            const char * what () const throw ()
            {
                 return s.data();
            }
        };

        /*
         * Exception for error for changing matrix/vector shape in reshape and resize, needs to output
         * "MtmError: Change matrix shape failed from: (<mat row>,<mat col>) (<new mat row>,<new mat col>)"
         * in what() class function
         */
        class ChangeMatFail : public MtmExceptions {
            Dimensions mat1;
            Dimensions mat2;
            string s;
        public:
            ChangeMatFail(Dimensions mat1,Dimensions mat2): mat1(mat1) ,mat2(mat2),
                    s(createErrorString("MtmError: Change matrix shape failed from ",mat1," to ",mat2)){}
            const char * what () const throw ()
            {
                return s.data();
            }
        };

        /*
         * Exception for accessing an illegal element in matrix or vector, needs to output
         * "MtmError: Attempt access to illegal element" in what() class function
         */
        class AccessIllegalElement : public MtmExceptions {
        public:
            const char * what () const throw ()
            {
                return "MtmError: Attempt access to illegal element";
            }
        };
    }
}


#endif //EX3_MTMEXCEPTIONS_H
