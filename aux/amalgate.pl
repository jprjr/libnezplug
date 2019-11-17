#!/usr/bin/env perl

use strict;
use warnings;
use File::Spec;

my $defines = {};
my @stack;

sub slurpfile {
    my $filename = shift;
    open(my $fh, '<', $filename) or die "File $filename: $!";
    my @lines = <$fh>;
    close($fh);
    return @lines;
}

sub processfile {
    my $filename = shift;
    my $parent = shift;
    my @lines = @_;
    my $output = '';

    if(defined($parent)) {
        push(@stack,"$parent => $filename");
    } else {
        push(@stack,$filename);
    }

    # check of ifndef-guard
    my ($def1) = ( $lines[0] =~ /^#ifndef\s+(.+)$/);
    if(defined($def1)) {
        chomp($def1);
        my ($def2) = ( $lines[1] =~ m/^#define\s+(.+)$/);
        if(defined($def2)) {
            chomp($def2);

            if($def1 eq $def2) {
                if(exists($defines->{$def1})) {
                    return '';
                }
                else {
                    $defines->{$def1} = 1;
                }
            }
        }
    }

    my $stack_info = "#if 0\nBEGIN $filename\nSTACK\n";
    foreach my $filename (@stack) {
        $stack_info .= "$filename\n";
    }
    $stack_info .= "#endif\n\n";

    $output .= $stack_info;

    foreach my $i (0..$#lines) {
        $output .= processline($filename,$lines[$i], $i + 1);
    }


    pop(@stack);

    return $output;
}

sub processline {
    my $filename = shift;
    my $line = shift;
    my $linenum = shift;

    if($line !~ /^#include\s+"/) {
        return $line;
    }

    my ($volume, $dir, $file) = File::Spec->splitpath($filename);
    $dir = $volume . $dir;
    my ($newfile) = ( $line =~ /^#include\s+"([^"]+)"/);
    $newfile = File::Spec->catfile($dir,$newfile);

    my @lines = slurpfile($newfile);
    return processfile($newfile,$filename.'[' . $linenum . ']',@lines);
}

if(@ARGV < 1) {
    print STDERR "Usage: amalage.pl /path/to/source.c ..\n";
    exit(1);
}

my $output = '';
# $output .= "#define PROTECTED static\n";
# $output .= "#define PROTECTED_VAR static\n\n";
# $output .= "#ifdef __GNUC__\n";
# $output .= "#define Inline __attribute__((always_inline)) inline\n";
# $output .= "#else\n";
# $output .= "#define Inline\n";
# $output .= "#endif\n\n";
# 
# $output .= "#define External static Inline\n\n";

foreach my $file (@ARGV) {
    @stack = ();
    my @lines = slurpfile($file);
    $output .= processfile($file,undef,@lines);
}

print $output;
