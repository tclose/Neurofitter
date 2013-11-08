/*
Revision of last commit: $Rev: 255 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-24 10:03:24 +0900 (Wed, 24 Oct 2007) $
*/

#include "../MPIStream.h"

/// Get the number of processes in the environment
int MPICommunicator::getSize(void) {
  return size;
}

/// Get the rank of this process
int MPICommunicator::getRank(void) {
  return rank;
}


/// Set the message id of the next message that will be sent
void MPICommunicator::setMessageId(int X) {
  messageId = X;
}

/// Get the message id of the last message
int MPICommunicator::getMessageId(void) {
 return messageId;
}

/// Set the message rank of the next message that will be sent
void MPICommunicator::setMessageRank(int X) {
  messageRank = X;
}

/// Get message rank of the last message
int MPICommunicator::getMessageRank(void) {
  return(messageRank);
}

/// Constructor of MPICommunicator
MPICommunicator::MPICommunicator(MPI_Comm X) : handle(X) {
	MPI_Comm_size(handle, &size);
	MPI_Comm_rank(handle, &rank);
}

/// Default constructor of MPIInputStream
MPIInputStream::MPIInputStream(void) : MPICommunicator()  {}

/// Constructor of MPIInputStream using an MPI_Comm
MPIInputStream::MPIInputStream(MPI_Comm X) : MPICommunicator(X)  {}

/// Read a bool from the MPI channel
MPIInputStream &MPIInputStream::operator>>(bool &Data) {
  int truth = 0;
  MPI_Recv(&truth,1,MPI_INT,messageRank,messageId,handle,&status);
  if (truth == 0) {
		Data = false;
  }
  else {
		Data = true;
  }
  messageRank = status.MPI_SOURCE;
  messageId = status.MPI_TAG;
  return(*this);
}

/// Read an integer from the MPI channel
MPIInputStream &MPIInputStream::operator>>(int &Data) {
  MPI_Recv(&Data,1,MPI_INT,messageRank,messageId,handle,&status);
  messageRank = status.MPI_SOURCE;
  messageId = status.MPI_TAG;
  return(*this);
}

/// Read an unsigned integer from the MPI channel
MPIInputStream &MPIInputStream::operator>>(unsigned &Data) {
  MPI_Recv(&Data,1,MPI_UNSIGNED,messageRank,messageId,handle,&status);
  messageRank = status.MPI_SOURCE;
  messageId = status.MPI_TAG;
  return(*this);
}

/// Read a float from the MPI channel
MPIInputStream &MPIInputStream::operator>>(float &Data) {
  MPI_Recv(&Data,1,MPI_FLOAT,messageRank,messageId,handle,&status);
  messageRank = status.MPI_SOURCE;
  messageId = status.MPI_TAG;
  return(*this);
}

/// Read a double from the MPI channel
MPIInputStream &MPIInputStream::operator>>(double &Data) {
  MPI_Recv(&Data,1,MPI_DOUBLE,messageRank,messageId,handle,&status);
  messageRank = status.MPI_SOURCE;
  messageId = status.MPI_TAG;
  return(*this);
}

/// Read a string from the MPI channel
MPIInputStream &MPIInputStream::operator>>(string &Data) {
  char *value;
  int stringLength;
  MPI_Recv(&stringLength,1,MPI_INT,messageRank,messageId,handle,&status);
  messageRank = status.MPI_SOURCE;
  messageId = status.MPI_TAG;
  value = new char[stringLength];
  MPI_Recv(value,stringLength,MPI_CHAR,messageRank,messageId,handle,&status);
  Data.assign(value);
  delete value;
  return(*this);
}

/// Default constructor of MPIOutputStream
MPIOutputStream::MPIOutputStream(void) : MPICommunicator() {}

/// Constructor of MPIOutputStream that uses an MPI_Comm
MPIOutputStream::MPIOutputStream(MPI_Comm X): MPICommunicator(X) {}

/// Write a bool to the MPI channel
MPIOutputStream &MPIOutputStream::operator<<(const bool & Data) {
	int truth = 0;
	if (Data) {
		truth = 1;
	}
	else {
		truth = 0;
	}
	MPI_Send(&truth,1,MPI_INT,messageRank,messageId,handle);
	return(*this);
}

/// Write an integer to the MPI channel
MPIOutputStream &MPIOutputStream::operator<<(const int & Data) {
	int dat = Data;
	MPI_Send(&dat,1,MPI_INT,messageRank,messageId,handle);
	return(*this);
}

/// Write an unsigned integer to the MPI channel
MPIOutputStream &MPIOutputStream::operator<<(const unsigned & Data) {
  unsigned dat = Data;
  MPI_Send(&dat,1,MPI_UNSIGNED,messageRank,messageId,handle);
  return(*this);
}

/// Write a float to the MPI channel
MPIOutputStream &MPIOutputStream::operator<<(const float & Data) {
  float dat = Data;
  MPI_Send(&dat,1,MPI_FLOAT,messageRank,messageId,handle);
  return(*this);
}

/// Write a double to the MPI channel
MPIOutputStream &MPIOutputStream::operator<<(const double & Data) {
	double dat = Data;
  MPI_Send(&dat,1,MPI_DOUBLE,messageRank,messageId,handle);
  return(*this);
}

/// Write a string to the MPI channel
MPIOutputStream &MPIOutputStream::operator<<(const string & Data) {
  int stringLength;
  stringLength = Data.length();
  MPI_Send(&stringLength,1,MPI_INT,messageRank,messageId,handle);
  MPI_Send((char*)(Data.c_str()),stringLength,MPI_CHAR,messageRank,messageId,handle);
  return(*this);
}

/// Default constructor of an MPIStream
MPIStream::MPIStream(void) : MPIOutputStream(),MPIInputStream() {}

/// Constructor of an MPIStream that uses an MPI_Comm
MPIStream::MPIStream(MPI_Comm X) : MPIOutputStream(X),MPIInputStream(X) {}

