#!/usr/bin/env perl
use strict;
use warnings;
use Cwd;
use File::Find;

if(@ARGV<1) {
    print("USAGE: ./run.pl [option]   avec  -1 / 0:vis  1:run1  2:run2 \n");
    die;
}

my $compil = "make";
system($compil);

if($ARGV[0] == 0) {
my $novis = "/afs/ihep.ac.cn/users/d/diane/g4work/bin/Linux-g++/MDCalib 1 test.root";
print($novis);
system($novis);
}

if($ARGV[0] == 1) {
my $vis = "/afs/ihep.ac.cn/users/d/diane/g4work/bin/Linux-g++/MDCalib 5 test5.root";
print($vis);
system($vis);
}

if($ARGV[0] == -1) {
my $vis = "/afs/ihep.ac.cn/users/d/diane/g4work/bin/Linux-g++/MDCalib run/vis2.mac";
print($vis);
system($vis);
}




if($ARGV[0] == 1) {
my $run1 = "/afs/ihep.ac.cn/users/d/diane/g4work/bin/Linux-g++/MDCalib run/run1.mac";
print($run1);
system($run1);
}


if($ARGV[0] == 2) {
my $run2 = "/afs/ihep.ac.cn/users/d/diane/g4work/bin/Linux-g++/MDCalib run/run2.mac";
print($run2);
system($run2);
}

if($ARGV[0] == 3) {
my $run = "/afs/ihep.ac.cn/users/d/diane/g4work/bin/Linux-g++/MDCalib";
print($run);
system($run);
}
