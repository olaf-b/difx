/***************************************************************************
 *   Copyright (C) 2006 by Adam Deller                                     *
 *                                                                         *
 *   This program is free for non-commercial use: see the license file     *
 *   at http://astronomy.swin.edu.au:~adeller/software/difx/ for more      *
 *   details.                                                              *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 ***************************************************************************/
//===========================================================================
// SVN properties (DO NOT CHANGE)
//
// $Id$
// $HeadURL$
// $LastChangedRevision$
// $Author$
// $LastChangedDate$
//
//============================================================================

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include "configuration.h"
#include "fxmanager.h"
#include "core.h"
#include "datastream.h"
#include "mk5.h"
#include "nativemk5.h"
#include <sys/utsname.h>
//includes for socket stuff - for monitoring
#include "string.h"
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <difxmessage.h>
#include "alert.h"

//act on an XML command message which was received
bool actOnCommand(Configuration * config, DifxMessageGeneric * difxmessage) {
  string paramname, paramvalue;

  //Only act on parameter setting commands
  //cout << "received a message" << endl;
  if (difxmessage->type == DIFX_MESSAGE_PARAMETER) {
    DifxMessageParameter * pmessage = &((difxmessage->body).param);
    paramname = string(pmessage->paramName);
    paramvalue = string(pmessage->paramValue);
    cinfo << startl << "Received a parameter message for parameter " << paramname << " and value " << paramvalue << ", targetmpiid is " << pmessage->targetMpiId << endl;
    //is it for me
    if ((pmessage->targetMpiId == config->getMPIId()) || 
        (pmessage->targetMpiId == DIFX_MESSAGE_ALLMPIFXCORR) || 
        ((pmessage->targetMpiId == DIFX_MESSAGE_ALLCORES) && config->isCoreProcess()) ||
        ((pmessage->targetMpiId == DIFX_MESSAGE_ALLDATASTREAMS) && config->isDatastreamProcess())) {
      //is it a shutdown message?
      if (paramname == "keepacting" && paramvalue == "false")
        return false;
      //otherwise set a config parameter if we know how
      if (paramname == "dumpsta")
        config->setDumpSTAState((paramvalue == "true") || (paramvalue == "True"));
      else if (paramname == "dumplta")
        config->setDumpLTAState((paramvalue == "true") || (paramvalue == "True"));
      else if (paramname == "stachannels")
        config->setSTADumpChannels(atoi(pmessage->paramValue));
      else if (paramname == "ltachannels")
        config->setLTADumpChannels(atoi(pmessage->paramValue));
      //else if (pmessage->paramname == "clock stuff")
      else {
        cwarn << startl << config->getMPIId() << ": warning - received a parameter instruction regarding " <<  pmessage->paramName << " which cannot be honored and will be ignored!" << endl;
      }
    }
  }
  return true;
}

//setup message receive thread
void * launchCommandMonitorThread(void * c) {
  Configuration * config = (Configuration*) c;
  int socket, bytesreceived = 1;
  char message[DIFX_MESSAGE_LENGTH];
  char sendername[DIFX_MESSAGE_PARAM_LENGTH];
  bool keepacting = true;
  DifxMessageGeneric * genericmessage = (DifxMessageGeneric *)malloc(sizeof(DifxMessageGeneric));

  cinfo << startl << "About to open receive socket" << endl;
  socket = difxMessageReceiveOpen();
  //struct timeval tv;
  //tv.tv_sec = 9;
  //tv.tv_usec = 0;
  //setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  cinfo << startl << "Receive socket opened - socket is " << socket << endl;
  if (socket < 0) {
    csevere << startl << "Could not open command monitoring socket! Aborting message receive thread." << endl;
    keepacting = false;
  }
  config->setCommandThreadInitialised();
  cinfo << startl << "Command thread initialised has been set" << endl;
  while (keepacting) {
    bytesreceived = difxMessageReceive(socket, message, DIFX_MESSAGE_LENGTH, sendername);
    if(bytesreceived > 0) {
      //cinfo << startl << "Received a message" << endl;
      difxMessageParse(genericmessage, message);
      keepacting = actOnCommand(config, genericmessage);
    }
    //else {
    //  csevere << startl << "Problem receiving message! Bytesreceived was " << bytesreceived << ". Sendername was " << sendername << ". Aborting message receive thread." << endl;
    //  keepacting = false;
    //}
  }
  free(genericmessage);
  if(socket >= 0)
    difxMessageReceiveClose(socket);
  cinfo << startl << "Command monitor thread shutting down" << endl;
  return 0;
}

//setup monitoring socket
int setup_net(char *monhostname, int port, int window_size, int *sock) {
  int status;
  unsigned long ip_addr;
  struct hostent     *hostptr;
  struct linger      linger = {1, 1};
  struct sockaddr_in server;    /* Socket address */

  hostptr = gethostbyname(monhostname);
  if (hostptr==NULL) {

    cerror << startl << "Failed to look up monhostname " << monhostname << endl;
    return(1);
  }
  
  memcpy(&ip_addr, (char *)hostptr->h_addr, sizeof(ip_addr));
  memset((char *) &server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons((unsigned short)port); 
  server.sin_addr.s_addr = ip_addr;
  
  cinfo << startl << "Connecting to " << inet_ntoa(server.sin_addr) << endl;
    
  *sock = socket(AF_INET, SOCK_STREAM, 0);
  if (*sock==-1) {
    perror("Failed to allocate socket");
    return(1);
  }

  /* Set the linger option so that if we need to send a message and
     close the socket, the message shouldn't get lost */
  status = setsockopt(*sock, SOL_SOCKET, SO_LINGER, (char *)&linger,
                      sizeof(struct linger)); 
  if (status!=0) {
    close(*sock);
    perror("Setting socket options");
    return(1);
  }

  /* Set the window size to TCP actually works */
  status = setsockopt(*sock, SOL_SOCKET, SO_SNDBUF,
                      (char *) &window_size, sizeof(window_size));
  if (status!=0) {
    close(*sock);
    perror("Setting socket options");
    return(1);
  }
  status = setsockopt(*sock, SOL_SOCKET, SO_RCVBUF,
                      (char *) &window_size, sizeof(window_size));
  if (status!=0) {
    close(*sock);
    perror("Setting socket options");
    return(1);
  }
    
  status = connect(*sock, (struct sockaddr *) &server, sizeof(server));
  if (status!=0) {
    perror("Failed to connect to server");
    return(1);
  }
  return(0);
} /* Setup Net */

static void generateIdentifier(const char *inputfile, int myID, char *identifier)
{
  int i, l, s=0;

  for(i = 0; inputfile[i]; i++)
  {
    if(inputfile[i] == '/')
    {
      s = i+1;
    }
  }

  if(inputfile[s] == 0)
  {
    s = 0;
  }

  strcpy(identifier, inputfile+s);
  l = strlen(identifier);
  
  // strip off ".input"
  for(i = l-1; i > 0; i--)
  {
    if(identifier[i] == '.')
    {
      identifier[i] = 0;
      break;
    }
  }
}


//main method - run by everyone
int main(int argc, char *argv[])
{
  MPI_Comm world, return_comm;
  int numprocs, myID, numdatastreams, numcores, perr;
  double t1, t2;
  Configuration * config;
  FxManager * manager = 0;
  Core * core = 0;
  DataStream * stream = 0;
  int * coreids;
  int * datastreamids;
  bool monitor = false;
  string monitoropt;
  pthread_t commandthread;
  int nameslength = 1;
  char * monhostname = new char[nameslength];
  int port=0, monitor_skip=0, namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  char difxMessageID[128];

  cout << "About to run MPIInit" << endl;

  MPI_Init(&argc, &argv);
  world = MPI_COMM_WORLD;
  MPI_Comm_size(world, &numprocs);
  MPI_Comm_rank(world, &myID);
  MPI_Comm_dup(world, &return_comm);
  MPI_Get_processor_name(processor_name, &namelen);

  cinfo << startl << "MPI Process " << myID << " is running on host " << processor_name << endl;
  
  if(argc == 3)
  {
    if(!(argv[2][0]=='-' && argv[2][1]=='M'))
    {
      cfatal << startl << "Error - invoke with mpifxcorr <inputfilename> [-M<monhostname>:port[:monitor_skip]]" << endl;
      MPI_Barrier(world);
      MPI_Finalize();
      return EXIT_FAILURE;
    }
    monitor = true;
    monitoropt = string(argv[2]);
    size_t colindex1 = monitoropt.find_first_of(':');
    size_t colindex2 = monitoropt.find_last_of(':');
    if(colindex2 == string::npos) 
      // BUG: This does not work and skip ends up equaling port!!!!
    {
      port = atoi(monitoropt.substr(colindex1 + 1).c_str());
      monitor_skip = 1;	
    }
    else
    {
      port = atoi(monitoropt.substr(colindex1 + 1, colindex2-colindex1-1).c_str());
      monitor_skip = atoi(monitoropt.substr(colindex2 + 1).c_str());

    }
    strcpy(monhostname, monitoropt.substr(2,colindex1-2).c_str());
  }
  else if(argc != 2)
  {
    cfatal << startl << "Error - invoke with mpifxcorr <inputfilename> [-M<monhostname>:port[:monitor_skip]]" << endl;
    MPI_Barrier(world);
    MPI_Finalize();
    return EXIT_FAILURE;
  }

  generateIdentifier(argv[1], myID, difxMessageID);
  difxMessageInit(myID, difxMessageID);

  cverbose << startl << "About to process the input file.." << endl;
  //process the input file to get all the info we need
  config = new Configuration(argv[1], myID);
  if(!config->consistencyOK())
  {
    //There was a problem with the input file, so shut down gracefully
    cfatal << startl << "Config encountered inconsistent setup in config file - aborting correlation" << endl;
    MPI_Barrier(world);
    MPI_Finalize();
    return EXIT_FAILURE;
  }

  //handle difxmessage setup for sending and receiving
  perr = pthread_create(&commandthread, NULL, launchCommandMonitorThread, (void *)(config));
  if (perr != 0)
    csevere << startl << "Error creating command monitoring thread!" << endl;
  else {
    //wait for commandmonthread to be initialised
    while(!config->commandThreadInitialised())
      usleep(10);
  }
  numdatastreams = config->getNumDataStreams();
  numcores = numprocs - (fxcorr::FIRSTTELESCOPEID + numdatastreams);
  if(numcores < 1)
  {
    cfatal << startl << "Error - must be invoked with at least " << fxcorr::FIRSTTELESCOPEID + numdatastreams + 1 << " processors (was invoked with " << numprocs << " processors) - aborting!!!" << endl;
    MPI_Barrier(world);
    MPI_Finalize();
    return EXIT_FAILURE;
  }

  //create the ID arrays
  coreids = new int[numcores];
  datastreamids = new int[numdatastreams];
  for(int i=0;i<numcores;i++)
    coreids[i] = fxcorr::FIRSTTELESCOPEID + numdatastreams + i;

  for(int i=0;i<numdatastreams;i++)
    datastreamids[i] = fxcorr::FIRSTTELESCOPEID + i;

  //wait until everyone has caught up
  MPI_Barrier(world);

  try
  {
    //work out what process we are and run accordingly
    if(myID == fxcorr::MANAGERID) //im the manager
    {
      manager = new FxManager(config, numcores, datastreamids, coreids, myID, return_comm, monitor, monhostname, port, monitor_skip);
      MPI_Barrier(world);
      t1 = MPI_Wtime();
      manager->execute();
      t2 = MPI_Wtime();
      cinfo << startl << "Total wallclock time was **" << t2 - t1 << "** seconds" << endl;
    }
    else if (myID >= fxcorr::FIRSTTELESCOPEID && myID < fxcorr::FIRSTTELESCOPEID + numdatastreams) //im a datastream
    {
      int datastreamnum = myID - fxcorr::FIRSTTELESCOPEID;
      if(config->isMkV(datastreamnum)) {
        stream = new Mk5DataStream(config, datastreamnum, myID, numcores, coreids, config->getDDataBufferFactor(), config->getDNumDataSegments());
      } else if(config->isNativeMkV(datastreamnum))
        stream = new NativeMk5DataStream(config, datastreamnum, myID, numcores, coreids, config->getDDataBufferFactor(), config->getDNumDataSegments());
      else
        stream = new DataStream(config, datastreamnum, myID, numcores, coreids, config->getDDataBufferFactor(), config->getDNumDataSegments());
      stream->initialise();
      MPI_Barrier(world);
      stream->execute();
    }
    else //im a processing core
    {
      core = new Core(myID, config, datastreamids, return_comm);
      MPI_Barrier(world);
      core->execute();
    }
    MPI_Barrier(world);
  }
  catch (MPI::Exception e)
  {
    cerror << startl << "Caught an exception!!! " << e.Get_error_string() << endl;
    return EXIT_FAILURE;
  }
  MPI_Finalize();
  delete [] coreids;
  delete [] datastreamids;

  if(myID == 0) difxMessageSendDifxAlert("Will this work?", 3);
  if(myID == 0) difxMessageSendDifxParameter("keepacting", "false", DIFX_MESSAGE_ALLMPIFXCORR);
  if(myID == 0) difxMessageSendDifxAlert("Not expecting this one!?", 3);
  perr = pthread_join(commandthread, NULL);
  if(perr != 0) csevere << startl << "Error in closing commandthread!!!" << endl;
  if(manager) delete manager;
  if(stream) delete stream;
  if(core) delete core;
  delete config;

  cinfo << startl << "MPI ID " << myID << " says BYE!" << endl;
  return EXIT_SUCCESS;
}
