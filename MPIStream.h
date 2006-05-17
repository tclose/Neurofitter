//From : Simplifying Cluster (PVM and MPI) Application Programming Using Interface Classes: Part 2 

#ifndef MPISTREAM_H
#define MPISTREAM_H

#include <string>
#include <vector>
#include <iostream>
#include <mpi.h>

#include "InputChannel.h"
#include "OutputChannel.h"

using namespace MPI;
using namespace std;

class message_communicator{
public:
  //message_communicator(void);
  message_communicator(MPI_Comm X=MPI_COMM_WORLD);
  void setMessageRank(int X);
  int getMessageRank(void);
  int getRank(void);
  int getMessageId(void);
  void setMessageId(int X);
  int getSize(void);
protected:
  //Intracomm Communicator;
  int messageRank;
  int messageId;
  int size;
  int rank;
  MPI_Comm handle;

};

class impi_stream : public virtual message_communicator,  public InputChannel {
public:
  impi_stream(void);
  impi_stream(MPI_Comm X);
  impi_stream &operator>>(int &Data);
  impi_stream &operator>>(unsigned &Data);
  impi_stream &operator>>(string &Data);
  impi_stream &operator>>(float &Data);
  impi_stream &operator>>(double &Data);
private:
  MPI_Status status;
};


class ompi_stream : public virtual message_communicator, public OutputChannel {
public:
  ompi_stream(void);
  ompi_stream(MPI_Comm X);
  ompi_stream &operator<<(const int& Data);
  ompi_stream &operator<<(const unsigned& Data);
  ompi_stream &operator<<(const string& Data);
  ompi_stream &operator<<(const float& Data);
  ompi_stream &operator<<(const double& Data);
private:
  MPI_Status status;
};


class mpi_stream : public ompi_stream, public impi_stream{
public:
  mpi_stream(void);
  mpi_stream(MPI_Comm X);
};

#endif
