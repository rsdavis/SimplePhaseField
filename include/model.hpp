
#ifndef SPF_MODEL_H
#define SPF_MODEL_H

#include <iostream>
#include <map>
#include "preprocessor.hpp"
#include "stencil.hpp"
#include "log.h"
#include "mpi.h"

#if (SPF_NDIMS==1)
#define for_loop_ijk(x) for (int i=SPF_NROWS-x;i<dims[0]-SPF_NROWS+x;i++)

#elif (SPF_NDIMS==2)
#define for_loop_ijk(x) for (int i=SPF_NROWS-x;i<dims[0]-SPF_NROWS+x;i++) \
                        for (int j=SPF_NROWS-x;j<dims[1]-SPF_NROWS+x;j++)

#elif (SPF_NDIMS==3)
#define for_loop_ijk(x) for (int i=SPF_NROWS-x;i<dims[0]-SPF_NROWS+x;i++) \
                        for (int j=SPF_NROWS-x;j<dims[1]-SPF_NROWS+x;j++) \
                        for (int k=SPF_NROWS-x;k<dims[2]-SPF_NROWS+x;k++)
#endif

#if (SPF_NDIMS==1)
#define calc_ijk_index() i

#elif (SPF_NDIMS==2)
#define calc_ijk_index() i*dims[1] + j

#elif (SPF_NDIMS==3)
#define calc_ijk_index() i*dims[1]*dims[2] + j*dims[2] + k
#endif

void preprocess(double ** phase, int * dims,
                std::map<std::string, std::string> params,
                std::map<std::string, int> name_index);

void kernel(double ** phase, double ** chem_pot, double ** mobility, int * dims);

void postprocess(double ** phase, double ** chem_pot, double ** mobility, int * dims);

template <typename T> // primary template
inline void unpack(std::map<std::string, std::string> params, std::string name, T &parameter)
{
    FILE_LOG(logERROR) << "Cant unpack parameter of this type: " << name;
    MPI_Abort(MPI_COMM_WORLD, 0);
}

template <> // explicit specialization for T = double
inline void unpack(std::map<std::string, std::string> hash, std::string name, double & parameter)
{
    std::map<std::string, std::string>::iterator it;
    it = hash.find(name);
    if (it==hash.end()) { // parameter not found
        FILE_LOG(logERROR) << "Parameter <" << name << "> not found"; 
        MPI_Abort(MPI_COMM_WORLD, 0);
    } else { // parameter found
        parameter = std::stod(it->second);
    }
}

template <> // explicit specialization for T = int
inline void unpack(std::map<std::string, std::string> hash, std::string name, int & parameter)
{
    std::map<std::string, std::string>::iterator it;
    it = hash.find(name);
    if (it==hash.end()) { // parameter not found
        FILE_LOG(logERROR) << "Parameter <" << name << "> not found"; 
        MPI_Abort(MPI_COMM_WORLD, 0);
    } else { // parameter found
        parameter = std::stoi(it->second);
    }
}

template <> // explicit specialization for T = float
inline void unpack(std::map<std::string, std::string> hash, std::string name, float & parameter)
{
    std::map<std::string, std::string>::iterator it;
    it = hash.find(name);
    if (it==hash.end()) { // parameter not found
        FILE_LOG(logERROR) << "Parameter <" << name << "> not found"; 
        MPI_Abort(MPI_COMM_WORLD, 0);
    } else { // parameter found
        parameter = std::stof(it->second);
    }
}

inline void unpack(std::map<std::string, int> name_index, std::string name, int &index)
{
    std::map<std::string, int>::iterator it;
    it = name_index.find(name);
    if (it==name_index.end()) { // parameter not found
        FILE_LOG(logERROR) << "Parameter <" << name << "> not found"; 
        MPI_Abort(MPI_COMM_WORLD, 0);
    } else { // parameter found
        index = it->second;
    }
}

#endif
