%%
clear all
clc
filename = 'data/kenwood/PowerRcvr.txt';
data = read_ir_signals(filename);

%%
% 1D00B946
% Decoded NEC: 1D00B946 (32 bits)
% Raw (68): 10538 8900 -4400 600 -550 550 -550 550 -550 550 -1700 550 -1650 550 -1700 550 -550 550 -1650 550 -550 600 -550 550 -550 550 -550 550 -550 600 -550 550 -550 550 -550 550 -1650 600 -550 550 -1650 550 -1700 550 -1650 550 -550 600 -550 550 -1650 550 -550 600 -1650 550 -550 550 -550 550 -550 600 -1650 550 -1650 600 -550 550 
data(1)

%%
clc
for i=1:length(data)
    data(i).code_dec = hex2dec(data(i).code);
    data(i).code_bin = dec2bin(data(i).code_dec, data(i).bits);
end

%% > function
clc
figure(), grid on, hold on
colors = 'rgbymk';
for i=1:length(data)   
    n = data(i).length/2;
    y = repmat(1-[1  0 0 1],1,n);
    x = cumsum(double(abs(data(i).raw)));
    x = [x x]'; x = x(:);
    x = x - x(1);
    plot(x,y+2*(i-1),'Color',colors(mod(i-1,length(colors))+1) );
end
set(gca,'YTick',1:2:2+2*(i-1));
set(gca,'YTickLabel',{data.code});
ylim([0 2+2*(i-1)])

%% > function
clc
figure(), grid on, hold on
for i=1:length(data)   
    n = data(i).length/2;
    y = repmat(1-[1  0 0 1],1,n);
    x = cumsum(double(abs(data(i).raw)));
    x = [x x]'; x = x(:);
    x = x - x(1);
    plot(x,y+0.5,'k.-');
end
ylim([0 2])
text(1e4,0.4,data(end).code_bin)