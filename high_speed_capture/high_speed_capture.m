function buff=high_speed_capture(mode)
% Basic capture program for experimentation and initial bringup

    if nargin<1
        mode='c';
    end
    mode=lower(mode(1));
    continuous_flag=false;
    switch mode
        % Continuous capture mode.        
      case 'c' 
        continuous_flag=true; 
        warning('Running in continuous capture mode.  Close the capture window to exit the program and close serial interface.');
      case 's' % Single capture mode
      otherwise
        warning('Unrecognized or unspecific capture mode.  Defaulting to continuous capture.');
    end

    disp('Starting capture...')
    
    % Open Serial interface
    s=serial('/dev/cu.usbmodem2422561');
    s.BaudRate=115200;
    disp(s.BaudRate);    
    fopen(s);
    
    [f,ax]=generate_plots();
    
    % Main Read loop
    t=0;
    t_prev=0;
    loop_count=1;
    draw_count=1;
    
    % Capture buffer
    buff=zeros(1,512*1000,'uint8');        
    
    draw_buff=zeros(1,512*10,'uint8');
    x_buff=1:(512*1000);
    
    t=tic;
    while loop_count<1000              
        
%         if mod(loop_count,10)==0 
%             if loop_count~=10
%                 update_plots(ax,draw_buff,x_buff((loop_count-10)*512+1:(loop_count)*512));
%             end
%             draw_count=1;
%         end

        % Grab bytes         
        flushinput(s);
        while s.BytesAvailable<512
        end
        tmp=fread(s,512);
        tmp=uint8(tmp);

        buff(512*(loop_count-1)+1:512*loop_count)=tmp;
        %draw_buff((512*(draw_count-1)+1):512*draw_count)=tmp;
        
        loop_count=loop_count+1;
        %draw_count=draw_count+1;
    end
    
    plot(buff);
    
    disp(toc(t));    
    
    % Close the serial interface
    fclose(s);
    delete(s);

end

function [f,ax]=generate_plots()
    % Plotting 
    f=figure;
    ax=axes;
    set(ax,'color',[0 0 0],'xlim',[0 512*1000],'ylim',[0 16]);
    %h_plt=plot(0,0,'g');
end

function update_plots(ax,buff,x)

% We can
%   (1) initialize a new line each time

    l=line(x,buff,'parent',ax);
    set(l,'color','g');

%   (2) add new points to an existing line?
    %set(h_plt,'xdata',1:numel(tmp),'ydata',tmp);
    drawnow;
end