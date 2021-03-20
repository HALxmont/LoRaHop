###############################################################################
# Author K. Herman
# kherman@ubiobio.cl
# Main Application for the LoRa oint Thomas node
# 07.02.2021
#                               Includes
###############################################################################
using Sockets;
using Dates;
using CSV;
using DataFrames;
using LibSerialPort;

###############################################################################
#                           Global Variables
###############################################################################

# hostname definition
hostname = "BB-3";
# main server listening port
port = 2037;
# local IP address
ipaddr = "localhost";
# serial port 
uart = "/dev/ttyS0";
baudrate = 9600;
# condition to trigger tasks
srvwrite1 = Condition();
srvwrite2 = Condition();
srvwrite3 = Condition();
Sock1Ch = Channel(128);
Sock2Ch = Channel(128);
Sock3Ch = Channel(128);
UartCond = Condition();
UartDataCh = Channel(128);


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
			wait(srvwrite1);
			msg = take!(Sock1Ch);
			write(sock, msg);     
		end
    end
end

@async begin
    server = listen(port+1);
    while true
    	sock = accept(server);
	@async while isopen(sock);
				wait(srvwrite2);
				msg = take!(Sock2Ch);
		      	write(sock, msg); 
	       end
    end
end

@async begin
    server = listen(port+2);
    while true
    	sock = accept(server);
	@async while isopen(sock);
				wait(srvwrite3);
				msg = take!(Sock3Ch);
		      	write(sock, msg); 
	       end
    end
end

@async begin
    server = listen(port+3);
    while true
    	sock = accept(server);
	@async while isopen(sock);
	       line = readline(sock, keep=true)
	       println("$line");
	end
    end
end
###############################################################################


###############################################################################
#                                 Serial Port Task
###############################################################################
#asynchronous task for the serial port
@async begin
msg = "";
sp = LibSerialPort.open(uart, baudrate);
    while true
	   if (bytesavailable(sp) > 0)
			msg = readline(sp, keep=true);
			put!(UartDataCh, msg);
			notify(UartCond);
	   end
	   sleep(0.001);
	end 
end
###############################################################################

###############################################################################
#Function to decode the Uart data
function UartDecode()
	msg = take!(UartDataCh);
	msg = String(msg);
	# Puchalski statuon
	if (msg[1] == 'P' && msg[2] == '2')
		put!(Sock1Ch, msg);
		notify(srvwrite1);
	end
    # Gucio
	if (UInt8(msg[1]) == 2 && UInt8(msg[2]) == 0)
		put!(Sock2Ch, msg);
		notify(srvwrite2);
	end

	# Panda
	if (UInt8(msg[1]) == 1 && UInt8(msg[2]) == 0)
			put!(Sock3Ch, msg);
			notify(srvwrite3);
	end
end
# make it asynchronous
@async begin
	while true
		wait(UartCond);
		UartDecode();
	end
end
###############################################################################


