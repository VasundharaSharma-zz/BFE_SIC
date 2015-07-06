#!/usr/bin/perl -w

#########################################################################
#  Second Script to create name list, bigrams, compute dice  values     #
#########################################################################



use List::MoreUtils qw(uniq);
use Lingua::EN::Bigram;
use File::Sort qw(sort_file);

$filename = 'voters_sample.txt'; # current database file
$write_file = "c:\\University\\workspace\\NewBloomFilter\\name_list.txt"; # creates full list of names as per database size
$bigram_size_file = "c:\\University\\workspace\\NewBloomFilter\\bigram_size.txt";
$dice_file = "c:\\University\\workspace\\NewBloomFilter\\firstLast\\dice_name.txt";

# delete previously created files 
unlink glob "c:\\University\\workspace\\NewBloomFilter\\Data_Files\\*.txt";
       
    open WRITE, ">$write_file" or die "Cannot open $write_file: $!\n";
    open DICE, ">$bigram_size_file" or die "Cannot open $bigram_size_file: $!\n";
    open FILE,"<$filename" or die "Cannot read $filename: $!\n";
    open DICEFILE,">$dice_file" or die "Cannot open $dice_file: $!\n";

    $total_nlines = 0;
    $total_nwords = 0;
    
    while ($line = <FILE>)
    {
        @words = split("\"",$line);
        # $words[3] contains first+middle+last name
        @param = split(" ",$words[3]);
        
        if ($param[0] =~ /\,$/) { chop $param[0]; } #remove extra trailing chars from names
        if ($param[1] =~ /\,$/) { chop $param[1]; } #remove extra trailing chars from names
        $combined_name = $param[1]." ".$param[0];
        print "$combined_name\n"; # print check for correct filtered first and last names
       
        our @namelist;
        
        # Filter out unique names in the list and remove duplicates
        if (grep {$_ eq $combined_name} @namelist) {
        print "Duplicate '$combined_name' found!\n" ;
        next;
        } else {
        push @namelist,$combined_name;	
        #print WRITE $combined_name."\n";
        }
        @letters = split("",$combined_name);
        #print "@letters\n"; # check for the correct names
        $nwords = @letters; # count the letters for each of the names
        
    } 
    
    close(FILE);
    print "This is the name list @namelist\n";
    

# Level 1 : Create Bigrams for unique names

    # Code below implements the trimmed list (database riving)
    our @sortlist = sort { length($a) <=> length($b) } @namelist;
    $list = @sortlist;
    my $n = $list*0.04;
    my @first_n = @sortlist[0..$n];
    my @last_n = @sortlist[-$n..-1];
    our @fullist = (@first_n, @last_n);
    foreach(@fullist){
    	print WRITE $_."\n";
    }  
    close(WRITE);
    
    $list = @fullist; # pick size of full name list
    
    # Code proceeds to implement bigram calculation
    for ($i = 0; $i < $list; $i = $i + 1)
       {
       	
       	my @name_var = split(" ",$fullist[$i]);
       	# Calculate individual bigrams for first and last names and combine them 
       	
       	my @bigrams1 = CalcBigrams($name_var[0]);
       	my @bigrams2 = CalcBigrams($name_var[1]);
       	my @bigrams = (@bigrams1, @bigrams2);
       	#% print "Final Bigrams: @bigrams \n";
  
       	@{$new_hash{$fullist[$i]}} = @bigrams;
       	
       	
        $total_nwords = $total_nwords + $nwords;

        $total_nlines = $total_nlines + 1;
        
       }
     for (keys %new_hash) {
     my @value_array = @{$new_hash{$_}};
     print "Name is $_ and Bigrams are @value_array \n";
     }
   print "$filename contains $total_nlines lines, $total_nwords words.\n"; 
   
    # Level 2 : Create and calculate Jaccard distance vectors between plaintext names 
   
    #print scalar keys %new_hash; # print size of the name hash
    my %size_hash = ();
    
    for (keys %new_hash) {
     my @value_array1 = @{$new_hash{$_}};
     my $key1 = $_;
     our @unique_bigrams1 = uniq @value_array1;
     my $bigram_size = @unique_bigrams1;
     
    #print "Name is $_ and Unique Bigrams are @unique_bigrams1 \n"; #check for unique bigrams
    # populate hash for name list based on size of bigrams
    if (exists $size_hash{$bigram_size}){
        push (@{$size_hash{$bigram_size}}, $key1);
       } else {
       @{$size_hash{$bigram_size}} = $key1; #add new name elements to the $size_hash{bigram_size}
       }
    
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
       my $bigram_count_1 = @unique_bigrams1;
       my $bigram_count_2 = @unique_bigrams2;
       my $unique_bigrams_count = @unique_bigrams1 + @unique_bigrams2;
       
       # calculate Jaccard Distance between two strings
       $jaccard = $count1/$count2;
       #print Jaccard Distance: $jaccard."\n";
       
       # calculate Dice's coefficient between two strings
       
       $dice = 2 * $count1 / $unique_bigrams_count;
       #print "Dice's coefficient: $dice \n";
       
       # create hash of Dice's coefficient with the respective names
       
       push @jc,$key1,$key2;
       
       if ($dice > 0 && $dice != 1) {
       print DICEFILE "$dice :$key1,$key2 | *$bigram_count_1*$bigram_count_2*$count1* #\n";
       my $i = $bigram_count_1;
       if($i){
       $dice_name = "c:\\University\\workspace\\NewBloomFilter\\Data_Files\\dice_name".$i.".txt";
       open DICEVAL, ">>$dice_name" or die "Cannot open $dice_name: $!\n";
       print DICEVAL "$dice :$key1,$key2 | *$bigram_count_1*$bigram_count_2*$count1* #\n";
       close DICEVAL;
             }
       }
       
       # Create list of names found for unique Dice Values
       
       if (exists $check_hash{$dice}){
        push (@{$check_hash{$dice}}, @jc);
       } else {
       @{$check_hash{$dice}} = @jc; #add new name elements to the $check_hash{dice}
       }
     }
    }
    
   # check if the Bigram Size Hash is populated correctly 
   foreach (sort { $a <=> $b } keys(%size_hash) ){
     my @name_array = @{$size_hash{$_}};
     my @unique_array = uniq @name_array;
     my $name_arr_size = @name_array;
     print "Bigram Size is $_ and Names are @unique_array \n";
     print DICE "$_ :";
     for ($i = 0; $i < $name_arr_size; $i = $i + 1)
       {
         #%print DICE "$_ : @unique_array -  \n";
         print DICE "$name_array[$i]*";
       }
       print DICE "|\n";
       
     }
     close(DICE);
     close(DICEFILE);
     
     
    sub CalcBigrams {
    	my $inputlist = $_[0];
    	my $white = "_";
    	
    	@letters = split("",$inputlist);
       	my $size =  @letters;
       	my @bigram_arr = (); # for restting the array before setting bigrams for each name
       	$j = 0;
       	$first = $white.$letters[0];
       	$last = $letters[$size-1].$white;
       	
       	while ($j < $size-1){
       	
       	$next = $letters[$j].$letters[$j+1];
       	$j++;
       	push @bigram_arr,$next; 	
       	
       	}
       	push @bigram_arr,$last;
       	unshift @bigram_arr,$first;
       	return @bigram_arr;
    }
    