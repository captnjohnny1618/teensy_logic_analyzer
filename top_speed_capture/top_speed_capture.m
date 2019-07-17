function [data,cycles]=top_speed_capture()

    fprintf(1,'Opening interface...');
    
    % Open Serial interface
    s=serial('/dev/cu.usbmodem2422561');
    s.BaudRate=115200;
    disp(s.BaudRate);    
    fopen(s);
    fprintf(1,'OPEN\n');    

    % Test that we have communication
    fprintf(1,'Testing communication...\n');
    fwrite(s,'b')
    fprintf(1,'If you did not see the Teensy blink, serial communication NOT established.\n');

    pause(5);
    
    % Start a new capture (Capture should only take 0.01s at most)
    fprintf(1,'Capture begin...');
    fwrite(s,'r');
    pause(0.5);
    fprintf(1,'COMPLETE\n');    
    
    % Get the cycles information (for timing)
    fprintf(1,'Reading total number of cycles...\n');
    fwrite(s,'c');
    cycles=fread(s,1,'uint32');
    cycles=int32(cycles);

    % Get the buffer data
    fprintf(1,'Reading capture data...\n');    
    data=read_data_serial(s);
    fprintf(1,'COMPLETE\n');

    % Close serial communication
    fprintf(1,'Closing interface...');
    fclose(s);
    delete(s);
    fprintf(1,'CLOSED\n');
    
    %plot(linspace(1,double(cycles),numel(capture_buffer)),capture_buffer);

end

function data=read_data_serial(interface)

    fwrite(interface,'d');

    data    = zeros(0,1,'uint8');

    % Wait for data to become available if it's not
    while interface.BytesAvailable==0
        pause(0.01)
    end

    % Check that the first character sent is the "start" character
    char_start=fread(interface,1);
    if isequal(char_start,'<')
        fwrite(interface,'y');        
    else
        warning('Received character that was not a "start" character. Not reading any more data.');
        data=[];
        return;
    end

    % Main read loop
    not_done=true;    
    while not_done

        pause(.01);
        
        % Teensy will send 512 bytes every time
        bytes=interface.BytesAvailable;
        
        if bytes>0 && bytes~=512
            % Check for condition where we have our terminator
            if bytes==1
                character=fread(interface,1,'uint8');
                if isequal(character,'>')
                    not_done=false;
                    fwrite(interface,'y');
                    break;
                else
                    continue;
                end
            else
                continue;
            end            
        else
            try 
                tmp=fread(interface,bytes,'uint8');
            catch
                continue;
            end
            if numel(tmp)==512
                fwrite(interface,'y');
                data=[data; tmp];
            else
                warning('I read a strange amount of data, and I don''t know how or why.');
                data=[];
                return;
            end
        end        
    end    
end