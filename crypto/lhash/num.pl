<<<<<<< HEAD
#! /usr/bin/env perl
# Copyright 1995-2016 The OpenSSL Project Authors. All Rights Reserved.
#
# Licensed under the OpenSSL license (the "License").  You may not use
# this file except in compliance with the License.  You can obtain a copy
# in the file LICENSE in the source distribution or at
# https://www.openssl.org/source/license.html
=======
#!/usr/local/bin/perl
>>>>>>> origin/master

#node     10 ->   4

while (<>)
	{
	next unless /^node/;
<<<<<<< HEAD
	s|\R$||;                # Better chomp
=======
	chop;
>>>>>>> origin/master
	@a=split;
	$num{$a[3]}++;
	}

@a=sort {$a <=> $b } keys %num;
foreach (0 .. $a[$#a])
	{
	printf "%4d:%4d\n",$_,$num{$_};
	}
