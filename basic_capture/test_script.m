% Open Serial interface

s=serial('/dev/cu.usbmodem2422561');
fopen(s);

fprintf(s,'*IDN?')
out = fscanf(s);

while (s.BytesAvailable~=0)
    flushinput(s);
    pause(.025);
    tmp=fscanf(s); 
    fprintf(1,'%s',strrep(tmp,char(10),''));
end


%%
fclose(s);
delete(s);
clear s;
