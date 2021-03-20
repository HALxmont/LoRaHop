###############################################################################
# Author K. Herman
# kherman@ubiobio.cl
# Main Application for the Nocturnal Warming System
#                               Includes
###############################################################################
using Sockets;
using Syslogs;
#using JuliaDB;
using Dates;
using CSV;
using DataFrames;

#using SQLite;
###############################################################################
#                           Global Variables
###############################################################################

# hostname definition
hostname = "BB-1";
# main server listening port
port = 2037;
# local IP address
ipaddr = "localhost";
# telnet port  to be read
rport = 28700;
# log handler
logg = Syslog();
# condition to trigger frame analysis
analyze = Condition();
syncTime = Condition();
cmdcond = Condition();
MeasureTrigger = Condition();
dBaseDump = Condition();
cancheck = Condition();
cancheckcon = Condition();
dg18b20 = Condition();
datachannel = Channel(5);
cmdchannel = Channel(5);
cancheckch = Channel(15);

dgsb20in = 0.0;
dgsb20out = 0.0;
canpresent = Vector{Int}();
###############################################################################

#                            Custom Function Reads
###############################################################################

#include("canread.jl");
#include("MLX90614.jl");
include("DS18B20read.jl");
include("settime.jl");
###############################################################################

#                            DataFrame Definitions
###############################################################################

dfbiolpattern = DataFrame(Date=String[], NodeID=Int16[], FrameID = Int8[], MeasurementCounter = Int16[], PlantTemp1=Float32[], PlantTemp2=Float32[], PlantTemp3=Float32[], PlantTemp4=Float32[], PlantTemp5=Float32[], PlantTemp6=Float32[], AirTempLoc=Float32[], Humidity=Float32[], AirTemp=Float32[]);
dfsyspattern  = DataFrame(Date=String[], NodeID=Int16[], FrameID = Int8[], MeasurementCounter = Int16[], SystemVoltage=Int16[], CoreTemperature=Int16[],
EI2C=Int8[], ECAN=Int8[], EFAT=Int8[], LEC=Int8[], TEC=Int8[], REC = Int8[]);

dfbiol = dfbiolpattern;
dfsys  = dfsyspattern;
###############################################################################




###############################################################################
#                                 Main Server
###############################################################################
# asynchronous task for the server
@async begin
	server = listen(port);
    while true
	    sock = accept(server);
        try
	    @async while isopen(sock);
	     		line = readline(sock, keep=true)
			if line == "takephoto\n"
		        	write(sock, "$hostname Taking Photo\n");
			    elseif line == "takemeasure\n"
				   write(sock, "$hostname Taking Measurement\n");
				   notify(MeasureTrigger);
		            elseif line ==  "measureDG18B20\n"
				   notify(dg18b20);
				   write(sock, "$hostname Measuring DG18B20\n");
		            elseif line ==  "DumpDB\n"
                                   notify(dBaseDump);
                                   write(sock, "$hostname DB dump to file\n");
		            elseif line ==  "SyncTime\n"
                        	   notify(syncTime);
			           write(sock, "$hostname TimeSynced\n");
		            elseif line ==  "cancheck\n"
                                   notify(cancheck);
				   write(sock, "$hostname CAN check\n");
			    elseif line == "close\n"
				   write(sock, "Close and Exit Task\n");
		                   close(sock);
		           	   close(server);
		           	   break;
			    else
				   write(sock, "$hostname Invalid Command\n");
		            end
		       end
		catch  err
			   println(logg, :info, "$hostname srv log $err");
               close(sock);
               close(server);
        end
	end
end
###############################################################################


###############################################################################

#                            TELNET Client
###############################################################################
@async begin
	try
		rsock = connect(ipaddr, rport);
		while true
		  tmp = readline(rsock);
	      raw_frame = tmp[26:length(tmp)];
          addr = raw_frame[1:3];
		  frame = raw_frame[5:6];
		  addr = parse(Int16, addr, base=16);
          frame = parse(Int8, frame, base=16);
		  if addr >=10 && addr <= 25
			if  frame >=16 && frame <= 19
		     put!(datachannel, raw_frame)
		     notify(analyze);
            end

			if frame >= 48 && frame <= 80
             put!(cmdchannel, raw_frame)
		     notify(cmdcond);
			end

            if frame == 52
                put!(cancheckch, raw_frame);
                notify(cancheckcon);
            end 
		  end

    end
		close(rsock);
    catch err
		println(logg,:err,"$hostname Unable to launch CAN telnet client $err");
    end

end

##############################  Functions  ####################################
###############################################################################
# prototype of the frame analyzing function
function FrameAnalyzer()

	let
		dfb = dfbiolpattern;
		dfs  = dfsyspattern;
		biol = Vector{Union{Missing, Any}}(missing , 9);
		sys  = Vector{Union{Missing, Any}}(missing , 8);
		addr = 0;
		frameid = 0;
		id = 0;
		dc = datachannel;
		CurrentTime = Dates.now();
        CurrentTime = Dates.format(CurrentTime, "yyyy-mm-dd HH-MM-SS");
	    while isready(dc)

			data = take!(dc);
			addr = parse(Int16, data[1:3], base=16);
			data = data[5:end];
			frameid = parse(Int, data[1:2], base = 16);

			if  frameid == 16

				biol[1]  = parse(Int,data[3:6],base=16);            # measurement counter
				sys[1]   = parse(Int,data[7:10],base=16)/1000;      # system voltage
				ct       = parse(Int,data[11:14],base=16);          # core temperature
                if (ct < 327)
                    sys[2] = ct;
                else 
                    sys[2] = ct - 65535;
                end
				sys[3]   = parse(Int,data[15:16],base=16);         	# EI2C
				dfb = DataFrame(Date=CurrentTime, NodeID=addr, FrameID = frameid, MeasurementCounter = biol[1], PlantTemp1=biol[2], PlantTemp2=biol[3], PlantTemp3=biol[4], PlantTemp4=biol[5], PlantTemp5=biol[6], PlantTemp6=biol[7], AirTempLoc=biol[8], Humidity=biol[9], AirTemp=dgsb20in);
				dfs  = DataFrame(Date=CurrentTime, NodeID=addr, FrameID = frameid, MeasurementCounter = biol[1], SystemVoltage=sys[1], CoreTemperature=sys[2], EI2C=sys[3], ECAN=sys[4], EFAT=sys[3], LEC=sys[6], TEC=sys[7], REC=sys[8]);
				global dfbiol = vcat( dfbiol, dfb);
				global dfsys  = vcat( dfsys, dfs);

			elseif frameid == 17
                
			    mlx   = parse(Int,data[3:6],base=16)/100;       # MLX1
                if (mlx < 327)
                    biol[2] = mlx;
                else 
                    biol[2] = mlx - 655.35;
                end
				mlx   = parse(Int,data[7:10],base=16)/100;      # MLX2
				if (mlx < 327)
                    biol[3] = mlx;
                else 
                    biol[3] = mlx - 655.35;
                end
                mlx   = parse(Int,data[11:14],base=16)/100;     # MLX2
                if (mlx < 327)
                    biol[4] = mlx;
                else 
                    biol[4] = mlx - 655.35;
                end

				sys[4]    = parse(Int,data[15:16],base=16);         # ECAN
				dfb = DataFrame(Date=CurrentTime, NodeID=addr, FrameID = frameid, MeasurementCounter = biol[1], PlantTemp1=biol[2], PlantTemp2=biol[3], PlantTemp3=biol[4], PlantTemp4=biol[5], PlantTemp5=biol[6], PlantTemp6=biol[7], AirTempLoc=biol[8], Humidity=biol[9], AirTemp=dgsb20in);
				dfs  = DataFrame(Date=CurrentTime, NodeID=addr, FrameID = frameid, MeasurementCounter = biol[1], SystemVoltage=sys[1], CoreTemperature=sys[2], EI2C=sys[3], ECAN=sys[4], EFAT=sys[3], LEC=sys[6], TEC=sys[7], REC=sys[8]);
				global dfbiol = vcat( dfbiol, dfb);
				global dfsys  = vcat( dfsys, dfs);

			elseif frameid == 18

				mlx   = parse(Int,data[3:6],base=16)/100;        # MLX4
                if (mlx < 327)
                    biol[5] = mlx;
                else 
                    biol[5] = mlx - 655.35;
                end
				mlx   = parse(Int,data[7:10],base=16)/100;       # MLX5
                if (mlx < 327)
                    biol[6] = mlx;
                else 
                    biol[6] = mlx - 655.35;
                end
				mlx   = parse(Int,data[11:14],base=16)/100;      # MLX6
                if (mlx < 327)
                    biol[7] = mlx;
                else 
                    biol[7] = mlx - 655.35;
                end
				sys[5]    = parse(Int,data[15:16],base=16);          # EFAT
				dfb = DataFrame(Date=CurrentTime, NodeID=addr, FrameID = frameid, MeasurementCounter = biol[1], PlantTemp1=biol[2], PlantTemp2=biol[3], PlantTemp3=biol[4], PlantTemp4=biol[5], PlantTemp5=biol[6], PlantTemp6=biol[7], AirTempLoc=biol[8], Humidity=biol[9], AirTemp=dgsb20out);
				dfs  = DataFrame(Date=CurrentTime, NodeID=addr, FrameID = frameid, MeasurementCounter = biol[1], SystemVoltage=sys[1], CoreTemperature=sys[2], EI2C=sys[3], ECAN=sys[4], EFAT=sys[3], LEC=sys[6], TEC=sys[7], REC=sys[8]);
				global dfbiol = vcat( dfbiol, dfb);
				global dfsys  = vcat( dfsys, dfs);

			elseif frameid == 19

				mlx   = parse(Int,data[3:6],base=16)/100;            # SHTT
                if (mlx < 327)
                    biol[8] = mlx;
                else 
                    biol[8] = mlx - 655.35;
                end
				tmp       = parse(Int,data[7:10],base=16);               # SHTH
				biol[9]   = -2.0462 +  0.0367*tmp - 1.5955*10^(-6)*tmp.^2;
				sys[6]    = parse(Int,data[11:12],base=16);          # LEC
				sys[7]    = parse(Int,data[13:14],base=16);          # TEC
                sys[8]    = parse(Int,data[15:16],base=16);          # REC
				dfb = DataFrame(Date=CurrentTime, NodeID=addr, FrameID = frameid, MeasurementCounter = biol[1], PlantTemp1=biol[2], PlantTemp2=biol[3], PlantTemp3=biol[4], PlantTemp4=biol[5], PlantTemp5=biol[6], PlantTemp6=biol[7], AirTempLoc=biol[8], Humidity=biol[9], AirTemp=dgsb20out);
				dfs  = DataFrame(Date=CurrentTime, NodeID=addr, FrameID = frameid, MeasurementCounter = biol[1], SystemVoltage=sys[1], CoreTemperature=sys[2], EI2C=sys[3], ECAN=sys[4], EFAT=sys[3], LEC=sys[6], TEC=sys[7], REC=sys[8]);
				global dfbiol = vcat( dfbiol, dfb);
				global dfsys  = vcat( dfsys, dfs);

			end

	   end   # end while
	end      #end let

end
# make the function asynchronous
	@async begin
		while true
			wait(analyze);
			FrameAnalyzer();
		end
	end
###############################################################################

###############################################################################
function TakeMeasurements()
    
    atemp = 0;
    a = floor(dgsb20out*100);

    if (a >= 0)
       atemp = UInt16(a);
    else
       atemp = UInt16(2^16 + a);
    end
    s = string(atemp, base=16, pad=4);
    for i in 10:25
           
           data = parse(UInt8, "$i", base=10);
           data = string(data, base=16, pad=2);
           c = "02";
           cmd = `cansend can0  004#$data$c$s`;
           run(cmd);
           sleep(0.5);
    end
    sleep(2);
    for i in 10:25
           
           data = parse(UInt8, "$i", base=10);
           data = string(data, base=16, pad=2);
           dat = "03";
           cmd = `cansend can0  004#$data$dat`;
           run(cmd);
           sleep(0.5);
    end

end

# make the function asynchronous
    @async begin
	    while true
			wait(MeasureTrigger);
			TakeMeasurements();
	    end
    end
###############################################################################

###############################################################################
function BaseDump()

	dfbc  = dfbiol;
	dt = Dates.format(Dates.now(),"yyyymmdd HH-MM-SS");
	global dfbiol  = dfbiolpattern;
	dfbs  = dfsys;
	global dfsys  = dfsyspattern;
        if isempty(dfbc) == false
			CSV.write("dfbiol-$dt.csv", dfbc);
        end

		if isempty(dfbs) == false
			CSV.write("dfsys-$dt.csv", dfbs);
        end
end
# make the function asynchronous
	@async begin
		while true
		   wait(dBaseDump);
		   BaseDump();
		end
	end
###############################################################################


###############################################################################
function MeasureDG18B20()

    # in is outside out is inside the box
  try
	global dgsb20in = DS18B20read(1);
  catch err
	println(logg,:err,"$hostname DG18B20  $err");
  end

  try
	global dgsb20out = DS18B20read(2);
  catch err
	println(logg,:err,"$hostname DG18B20  $err");
  end
    
    

end
# make the function asynchronous
	@async begin
		while true
		   wait(dg18b20);
		   MeasureDG18B20();
		end
	end
###############################################################################

function canselfcheck()

    global canpresent  = Vector{Int}();

    for i in 11:25
           
           data = parse(UInt8, "$i", base=10);
           data = string(data, base=16, pad=2);
           dat = "08";
           cmd = `cansend can0  004#$data$dat`;
           run(cmd);
           sleep(0.5);
    end
    sleep(2);  
    x = canpresent;
 for i in 11:25
    
   res =  findall(x->x == i, vec(x))

    if isempty(res) 
           data = parse(UInt8, "$i", base=10);
           data = string(data, base=16, pad=2);
           dat = "ff";
           cmd = `cansend can0  004#$data$dat`;
           run(cmd);
           sleep(0.5);
     end        
  end
    

end 

@async begin 
     while true
        wait(cancheck);
        canselfcheck();
     end
end      
###############################################################################
function cancheckanalyzer()

	let
	addr = 0;
    frameid = 0;
    addr

		while isready(cancheckch)
			data = take!(cancheckch);
			addr = parse(Int16, data[1:3], base=16);
            append!(canpresent, addr);
        end
     end
end
# make the function asynchronous
	@async begin
		while true
		   wait(cancheckcon );
		   cancheckanalyzer();
		end
	end
###############################################################################


###############################################################################
function CmdAnalyzer()

	let
	addr = 0;
    frameid = 0;

		while isready(cmdchannel)
			data = take!(cmdchannel);
			addr = parse(Int16, data[1:3], base=16);
			data = data[5:end];
			frameid = parse(Int, data[1:2], base = 16);

			if 	   frameid == 48
				i2c = data[3:4];
				can = data[5:6];
				fat = data[7:8];
				println("I2C Error  = 0x$i2c");
				println("CAN Error  = 0x$can");
				println("FAT Error  = 0x$fat");
			elseif frameid == 49
				mc  = parse(Int,data[3:6],base=16);
				println("$addr Node: Measurement Counter  = $mc");
			elseif frameid == 50
				tmp  = parse(Int,data[3:6],base=16)/1000;
				println("$addr Node: System Voltage  = $tmp");
			elseif frameid == 51
				tmp  = parse(Int,data[3:6],base=16);
				println("$addr Node: Core Temperature  = $tmp");
			elseif frameid == 52
				tmp  = parse(Int,data[3:6],base=16)/100;
				println("$addr Node: SHT temperature  = $tmp");
			elseif frameid == 53
				tmp  = parse(Int,data[3:6],base=16);
				thum  = -2.0462 +  0.0367*tmp - 1.5955*10^(-6)*tmp.^2;
				println("$addr Node: SHT Humidity  = $thum");
			elseif frameid == 54
				tmp  = parse(Int,data[3:6],base=16)/100;
				println("$addr Node: MLX1 = $tmp");
			elseif frameid == 55
				tmp  = parse(Int,data[3:6],base=16)/100;
				println("$addr Node: MLX2 = $tmp");
			elseif frameid == 56
				tmp  = parse(Int,data[3:6],base=16)/100;
				println("$addr Node: MLX3 = $tmp");
			elseif frameid == 57
				tmp  = parse(Int,data[3:6],base=16)/100;
				println("$addr Node: MLX4 = $tmp");
			elseif frameid == 58
				tmp  = parse(Int,data[3:6],base=16)/100;
				println("$addr Node: MLX5 = $tmp");
			elseif frameid == 59
				tmp  = parse(Int,data[3:6],base=16)/100;
				println("$addr Node: MLX6 = $tmp");
			elseif frameid == 60
				tmp  = parse(Int,data[3:4],base=16);
				println("$addr Node: TEC = $tmp");
			elseif frameid == 61
				tmp  = parse(Int,data[3:4],base=16);
				println("$addr Node: REC = $tmp");
			elseif frameid == 62
				YY1  = parse(Int,data[3:4],base=16);
				YY2  = parse(Int,data[5:6],base=16);
				MO  = parse(Int,data[7:8],base=16);
				DD  = parse(Int,data[9:10],base=16);
				HH  = parse(Int,data[11:12],base=16);
				MIN  = parse(Int,data[13:14],base=16);
				SEC  = parse(Int,data[15:16],base=16);
				println("$addr Node: Time = $YY1$YY2-$MO-$DD $HH-$MIN-$SEC ");

            elseif frameid == 70
                dc1 = 2^16-1 - (parse(Int,data[3:4],base=16)*256 + parse(Int,data[5:6],base=16));
                dc2 = 2^16-1 - (parse(Int,data[7:8],base=16)*256 + parse(Int,data[9:10],base=16));
                dc3 = 2^16-1 - (parse(Int,data[11:12],base=16)*256 + parse(Int,data[13:14],base=16));
                println("DC1 = $dc1, DC2 = $dc2, DC3 = $dc3");
		    elseif frameid == 71	
                dc4 = 2^16-1 - (parse(Int,data[3:4],base=16)*256 + parse(Int,data[5:6],base=16));
                dc5 = 2^16-1 - (parse(Int,data[7:8],base=16)*256 + parse(Int,data[9:10],base=16));
                dc6 = 2^16-1 - (parse(Int,data[11:12],base=16)*256 + parse(Int,data[13:14],base=16));
                println("DC4 = $dc4, DC5 = $dc5, DC6 = $dc6");
            else
    		end
		end
    end
end
# make the function asynchronous
	@async begin
		while true
		   wait(cmdcond );
		   CmdAnalyzer()
		end
	end
###############################################################################
# make the function asynchronous
    @async begin 
        while true
            wait(syncTime);
            SendTime();
        end
    end     
################################### END #######################################
