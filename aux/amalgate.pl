#!/usr/bin/env perl

use strict;
use warnings;
use File::Spec;

sub slurpfile {
    my $filename = shift;
    open(my $fh, '<', $filename) or die "File $filename: $!";
    my @lines = <$fh>;
    close($fh);
    return @lines;
}

sub processfile {
    my $filename = shift;
    my @lines = @_;
    my $output = '';

    foreach my $line (@lines) {
        $output .= processline($filename,$line);
    }

    return $output;
}

sub processline {
    my $filename = shift;
    my $line = shift;

    if($line !~ /^#include\s+"/) {
        return $line;
    }

    my ($volume, $dir, $file) = File::Spec->splitpath($filename);
    $dir = $volume . $dir;
    my ($newfile) = ( $line =~ /^#include\s+"([^"]+)"/);
    $newfile = File::Spec->catfile($dir,$newfile);

    print STDERR "Including $newfile (from $filename)\n";

    my @lines = slurpfile($newfile);
    return processfile($newfile,@lines);
}

if(@ARGV < 1) {
    print STDERR "Usage: amalage.pl /path/to/source.c\n";
    exit(1);
}

my @lines = slurpfile($ARGV[0]);
my $output = processfile($ARGV[0],@lines);

print $output;
