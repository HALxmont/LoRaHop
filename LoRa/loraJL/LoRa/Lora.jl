###############################################################################
# Author K. Herman
# kherman@ubiobio.cl
# Main Application for the Nocturnal Warming System
#                               Includes
###############################################################################
using Sockets;
using Dates;
using CSV;
using DataFrames;

###############################################################################
#                           Global Variables
###############################################################################

# hostname definition
hostname = "BB-3";
# main server listening port
port = 2037;
# local IP address
ipaddr = "localhost";
# telnet port  to be read
#rport = 28700;
# log handler
# condition to trigger frame analysis
analyze = Condition();
syncTime = Condition();
cmdcond = Condition();
MeasureTrigger = Condition();
dBaseDump = Condition();
cancheck = Condition();
cancheckcon = Condition();
#dg18b20 = Condition();
#datachannel = Channel(5);
#cmdchannel = Channel(5);
#cancheckch = Channel(15);

###############################################################################

#                            Custom Function Reads
###############################################################################

###############################################################################

#                            DataFrame Definitions
###############################################################################


###############################################################################




###############################################################################
#                                 Main Server
###############################################################################
# asynchronous task for the server
@async begin
    server = listen(port);
    while true
    	sock = accept(server);
		@async while isopen(sock);
		      	write(sock, "$hostname Taking Photo\n");
		        close(sock);
		end
    end
end
###############################################################################

