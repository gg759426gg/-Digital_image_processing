clc;clear;
data=csvread('histogram.csv');
x=1:256;
bar(x,data),title('Gray Level Histogram');
ylabel('Number'),xlabel('Gray Level');