#include "../MPIStream.h"

int message_communicator::getSize(void) {
  return size;
}

int message_communicator::getRank(void) {
  return rank;
}

void message_communicator::setMessageId(int X) {
  messageId = X;
}

int message_communicator::getMessageId(void) {
 return messageId;
}


void message_communicator::setMessageRank(int X) {
  messageRank = X;
}

int message_communicator::getMessageRank(void) {
  return(messageRank);
}

//message_communicator::message_communicator(void) : message_communicator(MPI::COMM_WORLD) {}

message_communicator::message_communicator(MPI_Comm X) : handle(X) {
	MPI_Comm_size(handle, &size);
	MPI_Comm_rank(handle, &rank);
}

impi_stream::impi_stream(void) : message_communicator()  {}

impi_stream::impi_stream(MPI_Comm X) : message_communicator(X)  {}

impi_stream &impi_stream::operator>>(int &Data) {
  MPI_Recv(&Data,1,MPI_INT,messageRank,messageId,handle,&status);
  messageRank = status.MPI_SOURCE;
  messageId = status.MPI_TAG;
  return(*this);
}

impi_stream &impi_stream::operator>>(unsigned &Data) {
  MPI_Recv(&Data,1,MPI_UNSIGNED,messageRank,messageId,handle,&status);
  messageRank = status.MPI_SOURCE;
  messageId = status.MPI_TAG;
  return(*this);
}

impi_stream &impi_stream::operator>>(float &Data) {
  MPI_Recv(&Data,1,MPI_FLOAT,messageRank,messageId,handle,&status);
  messageRank = status.MPI_SOURCE;
  messageId = status.MPI_TAG;
  return(*this);
}

impi_stream &impi_stream::operator>>(double &Data) {
  MPI_Recv(&Data,1,MPI_DOUBLE,messageRank,messageId,handle,&status);
  messageRank = status.MPI_SOURCE;
  messageId = status.MPI_TAG;
  return(*this);
}

impi_stream &impi_stream::operator>>(string &Data) {
  char *value;
  int stringLength;
  MPI_Recv(&stringLength,1,MPI_INT,messageRank,messageId,handle,&status);
  value = new char[stringLength];
  MPI_Recv(value,stringLength,MPI_CHAR,messageRank,messageId,handle,&status);
  messageRank = status.MPI_SOURCE;
  messageId = status.MPI_TAG;
  Data.assign(value);
  delete value;
  return(*this);
}

ompi_stream::ompi_stream(void) : message_communicator() {}

ompi_stream::ompi_stream(MPI_Comm X): message_communicator(X) {}

ompi_stream &ompi_stream::operator<<(const int & Data) {
	int dat = Data;
	MPI_Send(&dat,1,MPI_INT,messageRank,messageId,handle);
	return(*this);
}

ompi_stream &ompi_stream::operator<<(const unsigned & Data) {
  unsigned dat = Data;
  MPI_Send(&dat,1,MPI_UNSIGNED,messageRank,messageId,handle);
  return(*this);
}


ompi_stream &ompi_stream::operator<<(const float & Data) {
  float dat = Data;
  MPI_Send(&dat,1,MPI_FLOAT,messageRank,messageId,handle);
  return(*this);
}

ompi_stream &ompi_stream::operator<<(const double & Data) {
	double dat = Data;
  MPI_Send(&dat,1,MPI_DOUBLE,messageRank,messageId,handle);
  return(*this);
}


ompi_stream &ompi_stream::operator<<(const string & Data) {
  int stringLength;
  stringLength = Data.length();
  MPI_Send(&stringLength,1,MPI_INT,messageRank,messageId,handle);
  MPI_Send((char*)(Data.c_str()),stringLength,MPI_CHAR,messageRank,messageId,handle);
  return(*this);
}

mpi_stream::mpi_stream(void) : ompi_stream(),impi_stream() {}

mpi_stream::mpi_stream(MPI_Comm X) : ompi_stream(X),impi_stream(X) {}

