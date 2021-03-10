%% C语言运行结果
clc;
clear;
close all;


%% 导入数据
fid = fopen('fft_ifft.txt');
lines = textscan(fid,'%s');
fclose(fid);


%% 分析数据
str_singal = strsplit(lines{1}{1}, ',');
str_singal_fft_c = strsplit(lines{1}{2}, ',');
str_singal_ifft_c = strsplit(lines{1}{3}, ',');

N = length(str_singal);

for i = 1: N
    singal(i) = str2num(str_singal{i});
    singal_fft_c(i) = str2num(str_singal_fft_c{i});
    singal_ifft_c(i) = str2num(str_singal_ifft_c{i});
end


%% MATLAB 频谱变换
singal_fft_m = abs(fft(singal, N)) * 2 / N; 
singal_fft_m(1) = singal_fft_m(1) / 2;


%% 绘制图形
figure;
plot(singal, 'r');hold on;
plot(singal_ifft_c, 'b-.');hold on;
legend('singal', 'singal\_ifft\_c');

figure;
plot(singal_fft_m, 'r');hold on;
plot(singal_fft_c, 'b-.');hold on;
legend('singal\_fft\_m', 'singal\_fft\_c');


%% End