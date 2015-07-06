%# Author: Vasundhara Sharma email: vvasundh@uwo.ca
%# Script to Measure Degree of Anonymity for BFE Method of Record Linkage

clearvars;
clc;

%# Read possibility set file to count the number of possible names found for each bfe
pwd = 'C:\University\workspace\NewBloomFilter';
addpath(genpath(pwd));

vec =[];

startRow = 1;
endRow = inf;

fid = fopen('out.txt','r');  % Open text file

%# Reading File in Blocks to capture information about the possibility set size
Block = 1;                                             % Initialize block index
while (~feof(fid))                                     % For each block:

   sprintf('Block: %s', num2str(Block));              % Display block number
   InputText = textscan(fid,'%s',4,'delimiter','\n'); % Read header line
   HeaderLines{Block,1} = InputText{1};
   %# disp(HeaderLines{Block});

   InputText = textscan(fid,'End list Size:%d');          % Read parameter value
   Data{Block,1} = num2str(InputText{1});
   %# disp(Data{Block});                                   % prints possibility set count
   
   eob = textscan(fid,'%s',1,'delimiter','\n');       % Read and discard EOB
   % marker (Here, 'EOF')
   
   
   
   Block = Block+1;                                   % Increment block index
end


%# Read the matrix of possibility set count to compute Degree of Anonymity

% get total count 
   total = length(Data) - 1
  
sumval=0;   
 %# display all values
 for i = 1:total
 disp(Data{i}); 
 X = str2double(Data{i}); 
 Y = (total-X+1)/total;
 disp(Y);
 T = Y * log10(Y);
 disp(T);
 sumval = sumval +T;
 %sumval = sumval+((1/Y)*log(Y)); # default log(x) returns ln(x)
 end
 
 HX = sumval  % obtained entropy value based on the current possibility set
 HM = log10(1/total) % standard manimum entropy associated with all values in possbility set; maxm possible entropy for anonumity

%# Compute Degree of Anonymity equation 

d = HM/HX

