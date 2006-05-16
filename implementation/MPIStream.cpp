#include "../MPIStream.h"

int message_communicator::size(void)
{
  return(Communicator.Get_size());
}

int message_communicator::rank(void)
{
  return(Communicator.Get_rank());
}

void message_communicator::messageId(int X)
{
  MessageId = X;
}

int message_communicator::messageId(void)
{
 return(MessageId);
}


void message_communicator::messageRank(int X)
{
  MessageRank = X;
}

int message_communicator::messageRank(void)
{
  return(MessageRank);
}

message_communicator::message_communicator(void) : Communicator(MPI::COMM_WORLD)
{}

message_communicator::message_communicator(MPI_Comm X) : Communicator(X)
{}

impi_stream::impi_stream(void) : message_communicator(MPI::COMM_WORLD), istream(NULL)
{}

impi_stream::impi_stream(MPI_Comm X) : message_communicator(X), istream(NULL)
{}

impi_stream &impi_stream::operator>>(int &Data)
{
  Communicator.Recv(&Data,1,MPI::INT,MessageRank,MessageId,Status);
  return(*this);
}

impi_stream &impi_stream::operator>>(float &Data)
{
  Communicator.Recv(&Data,1,MPI::FLOAT,MessageRank,MessageId,Status);
  return(*this);
}

impi_stream &impi_stream::operator>>(string &Data)
{
  char *Value;
  int Size;
  Communicator.Recv(&Size,1,MPI::INT,MessageRank,MessageId,Status);
  Value = new char[Size];
  Communicator.Recv(Value,Size,MPI::CHAR,MessageRank,MessageId,Status);
  Data.assign(Value);
  delete Value;
  return(*this);
}

impi_stream &impi_stream::operator>>(vector<string> &X)
{
  int NumWords;
  *this >> NumWords;
  string Data;
  int N;
  for(N = 0;N < NumWords; N++)
  {
    *this >> Data;
    X.push_back(Data);
  }

  return(*this);
}

ompi_stream::ompi_stream(void) : message_communicator(MPI::COMM_WORLD), ostream(NULL)
{}

ompi_stream::ompi_stream(MPI_Comm X): message_communicator(X), ostream(NULL)
{}

ompi_stream &ompi_stream::operator<<(int Data)
{
  Communicator.Send(&Data,1,MPI::INT,MessageRank,MessageId);
  return(*this);
}


ompi_stream &ompi_stream::operator<<(float Data)
{
  Communicator.Send(&Data,1,MPI::FLOAT,MessageRank,MessageId);
  return(*this);
}


ompi_stream &ompi_stream::operator<<(string Data)
{
  int Size;
  Size = Data.length();
  Communicator.Send(&Size,1,MPI::INT,MessageRank,MessageId);
  Communicator.Send((char *)Data.c_str(),Size,MPI::CHAR,MessageRank,MessageId);
  return(*this);
}

ompi_stream &ompi_stream::operator<<(vector<string> &X)
{
  int Size = X.size();
  *this << Size;
  for(unsigned N = 0;N < X.size(); N++)
  {
    *this << X[N];
  }
  return(*this);
}

mpi_stream::mpi_stream(void) : ompi_stream(MPI::COMM_WORLD),impi_stream(MPI::COMM_WORLD)
{}

mpi_stream::mpi_stream(MPI_Comm X) : ompi_stream(X),impi_stream(X)
{}

