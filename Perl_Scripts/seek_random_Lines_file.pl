#!/usr/bin/perl -w
use File::Slurp;
use strict;

#########################################################################
#  First script to create a random database of voters list              #
#########################################################################

my $output_file1 = "voters_sample.txt"; # contains a random subset of voters list


open WRITE, ">$output_file1" or die "Cannot open $output_file1: $!\n";

my $file='voters_list.txt'; # contains full voters list

my $count=shift || 100000;
my $size=-s $file;

open(FILE,$file) || die "Can't open $file\n";

while ($count--) {
   seek(FILE,int(rand($size)),0);
   $_= readline(FILE);                         # ignore partial line
   redo unless defined ($_ = readline(FILE)); # catch EOF
   print WRITE $_;
}
close(WRITE);
