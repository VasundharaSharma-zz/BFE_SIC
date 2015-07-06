#!/usr/bin/perl -w

use List::MoreUtils qw(uniq);
use Lingua::EN::Bigram;

$filename = $ARGV[0];
$write_file = "name_list.txt";
$dice_coeff_file = "dice_coeff.txt";

    open WRITE, ">$write_file" or die "Cannot open $write_file: $!\n";
    open DICE, ">$dice_coeff_file" or die "Cannot open $dice_coeff_file: $!\n";
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

# Level 1 : Create Bigrams for unique names

    $list = @namelist;
    
    for ($i = 0; $i < $list; $i = $i + 1)
       {
       	
       	my $white = "_";
       	my $name_var = $namelist[$i];
       	@letters = split("",$namelist[$i]);
       
       	my $size =  @letters;
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
     print "Name is $_ and Bigrams are @value_array \n";
     }
   print "$filename contains $total_nlines lines, $total_nwords words.\n"; 
    
    # Level 2 : Create ad calculate Jaccard distance vectors between plaintext names 
   
    #print scalar keys %new_hash; # print size of the name hash
    for (keys %new_hash) {
     my @value_array1 = @{$new_hash{$_}};
     my $key1 = $_;
     our @unique_bigrams1 = uniq @value_array1;
     #print "Name is $_ and Unique Bigrams are @unique_bigrams1 \n"; #check for unique bigrams
     for (keys %new_hash) {
     	my @value_array2 = @{$new_hash{$_}};
        my $key2 = $_;
        our @jc = ();
        my %original = ();
        my @isect = ();
        our $jaccard;
        our $dice;
        
        my @unique_bigrams2 = uniq @value_array2;

        map { $original{$_} = 1 } @unique_bigrams1;
        @isect = grep { $original{$_} } @unique_bigrams2;
        my @join_array = (@value_array1, @value_array2); 

       my $count1 =  @isect; 
       my $count2 = uniq @join_array;
       my $unique_bigrams_count = @unique_bigrams1 + @unique_bigrams2;
       
       # calculate Jaccard Distance between two strings
       $jaccard = $count1/$count2;
       #print Jaccard Distance: $jaccard."\n";
       
       # calculate Dice's coefficient between two strings
       
       $dice = 2 * $count1 / $unique_bigrams_count;
       #print "Dice's coefficient: $dice \n";
       
       # create hash of Dice's coefficient with the respective names
       
       push @jc,$key1,$key2;
       if (exists $check_hash{$dice}){
        push (@{$check_hash{$dice}}, @jc);
       } else {
       @{$check_hash{$dice}} = @jc; #currently it replaces the key element
       }
       #print "@jc \n";
     } 
    }
    
   # check if the Dice's Coeff Hash is populated correctly 
   for (keys %check_hash) {
     my @name_array = @{$check_hash{$_}};
     my @unique_array = uniq @name_array;
     print "Dice's Coeff is $_ and Names are @unique_array \n";
     print DICE "$_ : @unique_array -  \n";
     }
     close(DICE);
    