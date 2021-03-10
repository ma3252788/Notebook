%% C语言运行结果
clc;
clear;
close all;


%% 导入数据
fid = fopen('fft_integr.txt');
lines = textscan(fid,'%s');
fclose(fid);


%% 分析数据
str_singal = strsplit(lines{1}{1}, ',');
str_singal_integr_c = strsplit(lines{1}{2}, ',');
str_singal_integr_t = strsplit(lines{1}{3}, ',');

N = length(str_singal);

for i = 1: N
    singal(i) = str2num(str_singal{i});
    singal_integr_c(i) = str2num(str_singal_integr_c{i});
    singal_integr_t(i) = str2num(str_singal_integr_t{i});
end

%% 频谱分析
singal_integr_fft_c = abs(fft(singal_integr_c)) * 2 / N;
singal_integr_fft_c(1) = singal_integr_fft_c(1) / 2;

singal_integr_fft_t = abs(fft(singal_integr_t)) * 2 / N;
singal_integr_fft_t(1) = singal_integr_fft_t(1) / 2;


%% 绘制图形
figure;
% plot(singal, 'r');hold on;
plot(singal_integr_c, 'g');hold on;
plot(singal_integr_t, 'b-.');hold on;

% legend('singal', 'singal\_integr\_c', 'singal\_integr\_t');
legend('singal\_integr\_c', 'singal\_integr\_t');


figure;
plot(singal_integr_fft_c, 'g');hold on;
plot(singal_integr_fft_t, 'b-.');hold on;
legend('singal\_integr\_fft\_c', 'singal\_integr\_fft\_t');


%% End