%% C语言运行结果
clc;
clear;
close all;


%% 导入数据
fid = fopen('fft_differ.txt');
lines = textscan(fid,'%s');
fclose(fid);


%% 分析数据
str_singal = strsplit(lines{1}{1}, ',');
str_singal_differ_c = strsplit(lines{1}{2}, ',');
str_singal_differ_t = strsplit(lines{1}{3}, ',');

N = length(str_singal);

for i = 1: N
    singal(i) = str2num(str_singal{i});
    singal_differ_c(i) = str2num(str_singal_differ_c{i});
    singal_differ_t(i) = str2num(str_singal_differ_t{i});
end

%% 频谱分析
singal_differ_fft_c = abs(fft(singal_differ_c)) * 2 / N;
singal_differ_fft_c(1) = singal_differ_fft_c(1) / 2;

singal_differ_fft_t = abs(fft(singal_differ_t)) * 2 / N;
singal_differ_fft_t(1) = singal_differ_fft_t(1) / 2;


%% 绘制图形
figure;
plot(singal, 'r');hold on;
plot(singal_differ_c, 'g');hold on;
plot(singal_differ_t, 'b-.');hold on;
legend('singal', 'singal\_differ\_c', 'singal\_differ\_t');


figure;
plot(singal_differ_fft_c, 'g');hold on;
plot(singal_differ_fft_t, 'b-.');hold on;
legend('singal\_differ\_fft\_c', 'singal\_differ\_fft\_t');

%% End