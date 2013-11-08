/*
Revision of last commit: $Rev: 234 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-22 14:20:25 +0900 (Mon, 22 Oct 2007) $
*/

//From : Simplifying Cluster (PVM and MPI) Application Programming Using Interface Classes: Part 2 

#ifndef NEUROFITTER_MPISTREAM_H
#define NEUROFITTER_MPISTREAM_H

#include <mpi.h>

#include <string>
#include <vector>
#include <iostream>

#include "InputChannel.h"
#include "OutputChannel.h"

using namespace MPI;
using namespace std;

/// A general class which has functionality the communicate messages using MPI
class MPICommunicator {
public:
  MPICommunicator(MPI_Comm X=MPI_COMM_WORLD);
  void setMessageRank(int X); ///< Set the message rank of the next message that will be sent
  int getMessageRank(void); ///< Get message rank of the last message
  int getRank(void); ///< Get the rank of this process
  int getMessageId(void); ///< Get the message id of the last message
  void setMessageId(int X); ///< Set the message id of the next message that will be sent
  int getSize(void); ///< Get the number of processes in the environment
protected:
  int messageRank; ///< Rank of the last message
  int messageId; ///< Id of the last message
  int size; ///< Number of processes in the environment
  int rank; ///< Rank of the current process
  MPI_Comm handle; ///< Handle to the MPI_comm object

};

/// A stream that reads from other processes using MPI 
class MPIInputStream : public virtual MPICommunicator,  public InputChannel {

public:
  MPIInputStream(void); ///< Default constructor of MPIInputStream
  MPIInputStream(MPI_Comm X); ///< Constructor of MPIInputStream using an MPI_Comm
  MPIInputStream &operator>>(bool &Data); ///< Read a bool from the MPI channel
  MPIInputStream &operator>>(int &Data); ///< Read an integer from the MPI channel
  MPIInputStream &operator>>(unsigned &Data); ///< Read an unsigned integer from the MPI channel
  MPIInputStream &operator>>(string &Data); ///< Read a string from the MPI channel
  MPIInputStream &operator>>(float &Data); ///< Read a float from the MPI channel
  MPIInputStream &operator>>(double &Data); ///< Read a double from the MPI channel

private:
  MPI_Status status; ///< The status value of the last received message

};

/// A stream that writes to other processes using MPI 
class MPIOutputStream : public virtual MPICommunicator, public OutputChannel {

public:
  MPIOutputStream(void); ///< Default constructor of MPIOutputStream
  MPIOutputStream(MPI_Comm X); ///< Constructor of MPIOutputStream that uses an MPI_Comm
  MPIOutputStream &operator<<(const bool& Data); ///< Write an bool to the MPI channel
  MPIOutputStream &operator<<(const int& Data); ///< Write an integer to the MPI channel
  MPIOutputStream &operator<<(const unsigned& Data); ///< Write an unsigned integer to the MPI channel
  MPIOutputStream &operator<<(const string& Data); ///< Write a string to the MPI channel
  MPIOutputStream &operator<<(const float& Data); ///< Write a float to the MPI channel
  MPIOutputStream &operator<<(const double& Data); ///< Write a double to the MPI channel

private:
  MPI_Status status; ///< The status value of the last sent message

};

/// A stream that reads and writes to other processses using MPI
class MPIStream : public MPIOutputStream, public MPIInputStream{
public:
  MPIStream(void); ///< Default constructor of MPIStream
  MPIStream(MPI_Comm X); ///< Constructor of MPIStream that uses an MPI_Comm
};

#endif
