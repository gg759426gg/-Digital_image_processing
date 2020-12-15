clc;clear;
data=csvread('gray_level_number.csv');
x=1:255;
bar(x,data),title('Gray Level Histogram');
ylabel('Number'),xlabel('Gray Level');