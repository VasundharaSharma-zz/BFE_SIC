#!/usr/bin/perl -w

#################################################################################
# Script to calculate Degree of Anonymity for given possibility set             #
#################################################################################
 
my @matrix;

print "Enter total size of BFE List: (enter value -10 than original) \n";
my $total = <STDIN>;
chomp $total;

for($i=1; $i<=$total; $i++)
{

# read output file and parse for values
my $input_file1 = "c:\\Users\\Hiii\\Dropbox\\Research_Experiment\\DoA\\overlap_1\\300bfe_Firstname_Lastname\\Fixed_Data\\test\\trim_count_".$i.".txt";
my $input_file2 = "c:\\Users\\Hiii\\Dropbox\\Research_Experiment\\DoA\\overlap_1\\300bfe_Firstname_lastname-Zipcode\\Fixed_Data\\test\\trim_count_".$i.".txt";

#print "Choose the Experiment: (1: f+l ; 2: f+l+z)\n";
#my $input = <STDIN>;
#chomp $input;
#print "You chose: $input\n";

open FILE,"<$input_file1" or die "Cannot read $input_file1: $!\n";
#open FILE,"<$input_file2" or die "Cannot read $input_file2: $!\n" if($input==2);

$sumval=0, $count=0;
$grand=2495;

while ($line = <FILE>)
{
  chomp($line);
  #print "$line\n";
  if ($line =~ /.*End list Size:(\d+)\s*$/)
  {
  	if ($1 == 0) {
  		$value = 0;
  	} else {
  	$value = $1 - 1; 
  	}
  	$count++;
  	$DoA = $value / ($grand -1);
  	$DoA= sprintf("%.3f", $DoA); # to trim the output to 3 decimal places
    #print "DoA for element $count: $DoA \n";
    $sumval = $sumval + $DoA;
  }
  
} 

close(FILE);

# calculate DoA for the given values

$avg = $sumval/$count;
$avg= sprintf("%.3f", $avg); # to trim the output to 3 decimal places
print "Average Degree of Anonymity: $avg \n";
 
$matrix[$i][0] = $i;
$matrix[$i][1] = $avg;

print "Matrix Elements: $matrix[$i][0] -- $matrix[$i][1]\n";

}

