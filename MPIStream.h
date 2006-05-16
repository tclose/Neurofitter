//From : Simplifying Cluster (PVM and MPI) Application Programming Using Interface Classes: Part 2 


#include <string>
#include <vector>
#include <iostream>
#include <mpi.h>

using namespace MPI;
using namespace std;

class message_communicator{
public:
  message_communicator(void);
  message_communicator(MPI_Comm X);
  void messageRank(int X);
  int messageRank(void);
  int rank(void);
  int messageId(void);
  void messageId(int X);
  int size(void);
protected:
  Intracomm Communicator;
  int MessageRank;
  int MessageId;

};

class impi_stream : public virtual message_communicator, public istream {
public:
  impi_stream(void);
  //impi_stream(streambuf * sb) : istream(sb) {};
  impi_stream(MPI_Comm X);
  impi_stream &operator>>(int &Data);
  impi_stream &operator>>(string &Data);
  impi_stream &operator>>(float &Data);
  impi_stream &operator>>(vector<string> &X);
private:
  MPI::Status Status;
};


class ompi_stream : public virtual message_communicator, public ostream {
public:
  ompi_stream(void);
  //ompi_stream(streambuf * sb) : ostream(sb) {};
  ompi_stream(MPI_Comm X);
  ompi_stream &operator<<(int Data);
  ompi_stream &operator<<(string Data);
  ompi_stream &operator<<(float Data);
  ompi_stream &operator<<(vector<string> &X);
private:
  MPI::Status Status;
};


class mpi_stream : public ompi_stream, public impi_stream{
public:
  mpi_stream(void);
  mpi_stream(MPI_Comm X);
};
