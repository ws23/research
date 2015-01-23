#include <BasicUsageEnvironment.hh>
#include "DynamicRTSPServer.hh"

int main(int argc, char *argv[]){
	// Begin by setting up our usage environment
	TaskScheduler *scheduler = BasicTaskScheduler::createNew();
	UsageEnvironment *env = BasicUsageEnvironment::createNew(*scheduler);

	// Create the RTSP server 
	RTSPServer *rtspServer;
	portNumBits rtspServerPortNum = 8554;
	UserAuthenticationDatabase *authDB = NULL; 
	rtspServer = DynamicRTSPServer::createNew(*env, rtspServerPortNum, authDB);
	if (rtspServer == NULL){
		*env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
		exit(1);
	}

	// Show Server Msg
	*env << "LIVE555 Media Server\n"; 
	*env << " (LIVE555 Streaming Media library version " << LIVEMEDIA_LIBRARY_VERSION_STRING << ").\n";

	char *urlPrefix = rtspServer->rtspURLPrefix();
	*env << "Play streams from this server using the URL\n\t" << urlPrefix << "<filename>\nwhere <filename> is a file present in the current directory.\n";
	*env << "Each file's type is inferred from its name suffix:\n";
	*env << "\t\".mkv\" => a Matroska audio+video+(optional)subtitles file\n";
	*env << "\t\".mp3\" => a MPEG-1 or 2 Audio file\n";
	*env << "\t\".mpg\" => a MPEG-1 or 2 Program Stream (audio+video) file\n";

	env->taskScheduler().doEventLoop(); // does not return

	return 0; // only to prevent compiler warning
}
