#!/usr/bin/perl -w

use List::MoreUtils qw 'uniq';
use Lingua::EN::Bigram;

$filename = $ARGV[0];
$write_file = "name_list.txt";

    open WRITE, ">$write_file" or die "Cannot open $write_file: $!\n";
    open FILE,"<$filename" or die "Cannot read $filename: $!\n";

    $total_nlines = 0;
    $total_nwords = 0;

    while ($line = <FILE>)
    {
        @words = split(" ",$line);
        
        @firstname = split("\"",$words[1]);
        if ($firstname[0] =~ /\,$/) { chop $firstname[0]; } #remove extra trailing chars from names
        #print "$firstname[0]\n"; # print check for correct filtered first names
      
        our @namelist;
        
        # Filter out unique names in the list and remove duplicates
        if (grep {$_ eq $firstname[0]} @namelist) {
        print "Duplicate '$firstname[0]' found!\n" ;
        next;
        } else {
        push @namelist,$firstname[0];	
        print WRITE $firstname[0]."\n";
        }
        @letters = split("",$firstname[0]);
        #print "@letters\n"; # check for the correct names
        $nwords = @letters; # count the letters for each of the names
        
    } 

    close(WRITE);
    close(FILE);
    
    print "This is the name list @namelist\n";

    $list = @namelist;
    for ($i = 0; $i < $list; $i = $i + 1)
       {
       	
       	my $white = "_";
       	my $name_var = $namelist[$i];
       	@letters = split("",$namelist[$i]);
       	print "@letters\n";
       	my $size =  @letters;
       	#print $size;
       	our @bigrams = (); # for restting the array before setting bigrams for each name
       	$j = 0;
       	$first = $white.$letters[0];
       	$last = $letters[$size-1].$white;
       	
       	while ($j < $size-1){
       	
       	$next = $letters[$j].$letters[$j+1];
       	$j++;
       	push @bigrams,$next; 	
       	
       	}
       	push @bigrams,$last;
       	unshift @bigrams,$first;
       	@{$new_hash{$namelist[$i]}} = @bigrams;
       	
        $total_nwords = $total_nwords + $nwords;

        $total_nlines = $total_nlines + 1;
        
       }
     for (keys %new_hash) {
     my @value_array = @{$new_hash{$_}};
     print "Key is $_ and Second Element of array is @value_array \n";
     }
      print "$filename contains $total_nlines lines, $total_nwords words.\n"; 
      
   