#!/usr/bin/perl -w

#################################################################################
# Fourth script trims name_list.txt and bfe_list.txt to remove extra params     #
#################################################################################

use Text::Trim qw(trim);
use strict;

# Trim name_list and bfe_list for removing extra params

my $output_file1 = "c:\\University\\workspace\\NewBloomFilter\\firstLast\\name_list.txt";
open WRITENAME1, ">$output_file1" or die "Cannot open $output_file1: $!\n";

my $output_file2 = "c:\\University\\workspace\\NewBloomFilter\\firstLast\\bfe_list.txt";
open WRITENAME2, ">$output_file2" or die "Cannot open $output_file2: $!\n";

my $file1= 'c:\\University\\workspace\\NewBloomFilter\\name_list.txt';
my $file2 = 'c:\\University\\workspace\\NewBloomFilter\\bfe_list.txt'; 

my $size1=-s $file1;
my $size2=-s $file2;

my %seen; # to track duplicate entries in file

# ***** read name_list file and remove extra params  ******

open(FILE1,$file1) || die "Can't open $file2\n";
while($_ = <FILE1>){

    $_ =~ s/\d+$//g;
    trim $_;
    #print "$_\n";
    print WRITENAME1 $_."\n";
}

close(FILE1);
close(WRITENAME1);

# ***** read bfe_list file and remove extra params  ******

open(FILE2,$file2) || die "Can't open $file2\n";
while($_ = <FILE2>){

    $_ =~ s/\d+$//g;
    trim $_;
    #print "$_\n";
    print WRITENAME2 $_."\n";
}

close(FILE2);
close(WRITENAME2);
