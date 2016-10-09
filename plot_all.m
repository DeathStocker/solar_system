% List of all files of the format "body(i).dat"
x = 'body*.dat';
dir(x);
num_files = length(dir(x));

% Operating on all the files and plotting their X and Y coordinates.
for i = 0 : num_files - 1
  name = sprintf('body%d.dat', i);
  fileObj = load(name);
  x = fileObj(:,2);
  y = fileObj(:,3);
  % Plotting with random colours.
  plot(x, y, 'color', rand(1, 3), 'linewidth', 1.5);
  hold on;
endfor;
clear;
clc;
