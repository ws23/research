#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"

UsageEnvironment *env;

// To make the second and subsequent client for each stream reuse the same
// input stream as the first client (rather than playing the file from the
// start for each client), change the following "False" to "True":
Boolean reuseFirstSource = True;

// To stream *only* MPEG-1 or 2 video "I" frames
// (e.g., to reduce network bandwidth),
// change the following "False" to "True":
Boolean iFramesOnly = False;

static void announceStream(RTSPServer *rtspServer, ServerMediaSession *sms, char const *streamName, char const *inputFileName);
static char newDemuxWatchVariable;

// Special code for handling Matroska files: 
static MatroskaFileServerDemux *matroskaDemux;
static void onMatroskaDemuxCreation(MatroskaFileServerDemux *newDemux, void* /*clientData*/) {
  matroskaDemux = newDemux;
  newDemuxWatchVariable = 1;
}
// End Special code for handling Matroska files

int main(int argc, char **argv) {
  // Begin by setting up our usage environment:
  TaskScheduler *scheduler = BasicTaskScheduler::createNew();
  env = BasicUsageEnvironment::createNew(*scheduler);

  UserAuthenticationDatabase *authDB = NULL;

  // Create the RTSP server:
  RTSPServer *rtspServer = RTSPServer::createNew(*env, 8554, authDB);
  if (rtspServer == NULL) {
    *env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
    exit(1);
  }

  char const *descriptionString = "Session streamed by \"RTSPServer\"";

  // Set up each of the possible streams that can be served by the
  // RTSP server.  Each such stream is implemented using a
  // "ServerMediaSession" object, plus one or more
  // "ServerMediaSubsession" objects for each audio/video substream.

  // A MPEG-1 or 2 audio+video program stream:
  {
    char const *streamName = "test.mpg";
    char const *inputFileName = "test.mpg";
    // NOTE: This *must* be a Program Stream; not an Elementary Stream
    ServerMediaSession *sms = ServerMediaSession::createNew(*env, streamName, streamName, descriptionString);
    MPEG1or2FileServerDemux *demux = MPEG1or2FileServerDemux::createNew(*env, inputFileName, reuseFirstSource);
    sms->addSubsession(demux->newVideoServerMediaSubsession(iFramesOnly));
    sms->addSubsession(demux->newAudioServerMediaSubsession());
    rtspServer->addServerMediaSession(sms);

    announceStream(rtspServer, sms, streamName, inputFileName);
  }

  // A MP3 audio stream (actually, any MPEG-1 or 2 audio file will work):
  {
    char const* streamName = "test.mp3";
    char const* inputFileName = "test.mp3";
    ServerMediaSession *sms = ServerMediaSession::createNew(*env, streamName, streamName, descriptionString);
    Boolean useADUs = False;
    Interleaving *interleaving = NULL;
    sms->addSubsession(MP3AudioFileServerMediaSubsession::createNew(*env, inputFileName, reuseFirstSource, useADUs, interleaving));
    rtspServer->addServerMediaSession(sms);

    announceStream(rtspServer, sms, streamName, inputFileName);
  }

  // A Matroska ('.mkv') file, with video+audio+subtitle streams:
  {
    char const *streamName = "test.mkv";
    char const *inputFileName = "test.mkv";
    ServerMediaSession *sms = ServerMediaSession::createNew(*env, streamName, streamName, descriptionString);

    newDemuxWatchVariable = 0;
    MatroskaFileServerDemux::createNew(*env, inputFileName, onMatroskaDemuxCreation, NULL);
    env->taskScheduler().doEventLoop(&newDemuxWatchVariable);

    Boolean sessionHasTracks = False;
    ServerMediaSubsession *smss;
    while ((smss = matroskaDemux->newServerMediaSubsession()) != NULL) {
      sms->addSubsession(smss);
      sessionHasTracks = True;
    }
    if (sessionHasTracks) {
      rtspServer->addServerMediaSession(sms);
    }
    // otherwise, because the stream has no tracks, we don't add a ServerMediaSession to the server.

    announceStream(rtspServer, sms, streamName, inputFileName);
  }

  // A WebM ('.webm') file, with video(VP8)+audio(Vorbis) streams:
  // (Note: ".webm' files are special types of Matroska files, so we use the same code as the Matroska ('.mkv') file code above.)
  {
    char const *streamName = "test.webm";
    char const *inputFileName = "test.webm";
    ServerMediaSession *sms = ServerMediaSession::createNew(*env, streamName, streamName, descriptionString);

    newDemuxWatchVariable = 0;
    MatroskaFileServerDemux::createNew(*env, inputFileName, onMatroskaDemuxCreation, NULL);
    env->taskScheduler().doEventLoop(&newDemuxWatchVariable);

    Boolean sessionHasTracks = False;
    ServerMediaSubsession *smss;
    while ((smss = matroskaDemux->newServerMediaSubsession()) != NULL) {
      sms->addSubsession(smss);
      sessionHasTracks = True;
    }
    if (sessionHasTracks) {
      rtspServer->addServerMediaSession(sms);
    }
    // otherwise, because the stream has no tracks, we don't add a ServerMediaSession to the server.

    announceStream(rtspServer, sms, streamName, inputFileName);
  }

  env->taskScheduler().doEventLoop(); // does not return

  return 0; // only to prevent compiler warning
}

static void announceStream(RTSPServer *rtspServer, ServerMediaSession *sms, char const *streamName, char const *inputFileName) {
  char *url = rtspServer->rtspURL(sms);
  UsageEnvironment& env = rtspServer->envir();
  env << "\n\"" << streamName << "\" stream, from the file \"" << inputFileName << "\"\n";
  env << "Play this stream using the URL \"" << url << "\"\n";
  delete[] url;
}
