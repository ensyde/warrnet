#!/usr/bin/perl -w
foreach $file (@ARGV) {
	open(FLE,$file) or die ("open $!");
	my @lines = <FLE>;
	close FLE;
	open(FLE,">$file") or die ("open $!");
	foreach my $line (@lines) {
		$line =~ s/(\ {3}?)/\t/ while ($line =~ /(\ {3}?)\w/);
		print FLE $line;
	}
	close FLE;
}


