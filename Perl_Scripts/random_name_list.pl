#!/usr/bin/perl -w
use strict;

#################################################################################
# Third script creates the target subset list from the database name list       #
#################################################################################

# Create a subset of name-list from the entire chunk of name database 

my $output_file2 = "c:\\University\\workspace\\NewBloomFilter\\bfe_list.txt";
open WRITENAME, ">$output_file2" or die "Cannot open $output_file2: $!\n";

my $file1='c:\\University\\workspace\\NewBloomFilter\\name_list.txt';
my $count1=shift || 325; # 0.01 of the total initial size of dB
my $size1=-s $file1;
my %seen; # to track duplicate entries in file

open(FILE,$file1) || die "Can't open $file1\n";

while ($count1--) {
   seek(FILE,int(rand($size1)),0); # seek random line in the file 
   $_= readline(FILE);   # ignore partial line and read file at seek
   #% next if $seen{$_}++; # checks for duplicate entries and prevents creating them
   redo unless defined ($_ = readline(FILE)); # catch EOF
   print WRITENAME $_ if not $seen{$_}++ ;
}
close(WRITENAME);
