<<<<<<< HEAD
#! /usr/bin/env perl
# Copyright 1995-2016 The OpenSSL Project Authors. All Rights Reserved.
#
# Licensed under the OpenSSL license (the "License").  You may not use
# this file except in compliance with the License.  You can obtain a copy
# in the file LICENSE in the source distribution or at
# https://www.openssl.org/source/license.html

# Generate progs.h file by looking for command mains in list of C files
# passed on the command line.

use strict;
use warnings;
use configdata qw/@disablables/;

my %commands = ();
my $cmdre = qr/^\s*int\s+([a-z_][a-z0-9_]*)_main\(\s*int\s+argc\s*,/;

foreach my $filename (@ARGV) {
	open F, $filename or die "Coudn't open $_: $!\n";
	foreach (grep /$cmdre/, <F>) {
		my @foo = /$cmdre/;
		$commands{$1} = 1;
	}
	close F;
}

@ARGV = sort keys %commands;

print <<'EOF';
/*
 * WARNING: do not edit!
 * Generated by apps/progs.pl
 *
 * Copyright 1995-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

typedef enum FUNC_TYPE {
    FT_none, FT_general, FT_md, FT_cipher, FT_pkey,
    FT_md_alg, FT_cipher_alg
} FUNC_TYPE;

typedef struct function_st {
    FUNC_TYPE type;
    const char *name;
    int (*func)(int argc, char *argv[]);
    const OPTIONS *help;
} FUNCTION;

DEFINE_LHASH_OF(FUNCTION);

EOF

foreach (@ARGV) {
	printf "extern int %s_main(int argc, char *argv[]);\n", $_;
}

print "\n";

foreach (@ARGV) {
	printf "extern OPTIONS %s_options[];\n", $_;
}

print "\n#ifdef INCLUDE_FUNCTION_TABLE\n";
print "static FUNCTION functions[] = {\n";
my %cmd_disabler = (
    ciphers  => "sock",
    genrsa   => "rsa",
    rsautl   => "rsa",
    gendsa   => "dsa",
    dsaparam => "dsa",
    gendh    => "dh",
    dhparam  => "dh",
    ecparam  => "ec",
    pkcs12   => "des",
    );
foreach my $cmd (@ARGV) {
	my $str="    { FT_general, \"$cmd\", ${cmd}_main, ${cmd}_options },\n";
	if ($cmd =~ /^s_/) {
		print "#ifndef OPENSSL_NO_SOCK\n${str}#endif\n";
	} elsif (grep { $cmd eq $_ } @disablables) {
		print "#ifndef OPENSSL_NO_".uc($cmd)."\n${str}#endif\n";
	} elsif (my $disabler = $cmd_disabler{$cmd}) {
		print "#ifndef OPENSSL_NO_".uc($disabler)."\n${str}#endif\n";
	} else {
		print $str;
	}
}

my %md_disabler = (
    blake2b512 => "blake2",
    blake2s256 => "blake2",
    );
foreach my $cmd (
	"md2", "md4", "md5",
	"gost",
	"sha1", "sha224", "sha256", "sha384", "sha512",
	"mdc2", "rmd160", "blake2b512", "blake2s256"
) {
        my $str = "    { FT_md, \"".$cmd."\", dgst_main},\n";
        if (grep { $cmd eq $_ } @disablables) {
                print "#ifndef OPENSSL_NO_".uc($cmd)."\n${str}#endif\n";
        } elsif (my $disabler = $md_disabler{$cmd}) {
                print "#ifndef OPENSSL_NO_".uc($disabler)."\n${str}#endif\n";
        } else {
                print $str;
        }
}

my %cipher_disabler = (
    des3  => "des",
    desx  => "des",
    cast5 => "cast",
    );
foreach my $cmd (
=======
#!/usr/local/bin/perl

print "/* apps/progs.h */\n";
print "/* automatically generated by progs.pl for openssl.c */\n\n";

grep(s/^asn1pars$/asn1parse/,@ARGV);

foreach (@ARGV)
	{ printf "extern int %s_main(int argc,char *argv[]);\n",$_; }

print <<'EOF';

#define FUNC_TYPE_GENERAL	1
#define FUNC_TYPE_MD		2
#define FUNC_TYPE_CIPHER	3
#define FUNC_TYPE_PKEY		4
#define FUNC_TYPE_MD_ALG	5
#define FUNC_TYPE_CIPHER_ALG	6

typedef struct {
	int type;
	const char *name;
	int (*func)(int argc,char *argv[]);
	} FUNCTION;
DECLARE_LHASH_OF(FUNCTION);

FUNCTION functions[] = {
EOF

foreach (@ARGV)
	{
	push(@files,$_);
	$str="\t{FUNC_TYPE_GENERAL,\"$_\",${_}_main},\n";
	if (($_ =~ /^s_/) || ($_ =~ /^ciphers$/))
		{ print "#if !defined(OPENSSL_NO_SOCK)\n${str}#endif\n"; } 
	elsif ( ($_ =~ /^speed$/))
		{ print "#ifndef OPENSSL_NO_SPEED\n${str}#endif\n"; }
	elsif ( ($_ =~ /^engine$/))
		{ print "#ifndef OPENSSL_NO_ENGINE\n${str}#endif\n"; }
	elsif ( ($_ =~ /^rsa$/) || ($_ =~ /^genrsa$/) || ($_ =~ /^rsautl$/)) 
		{ print "#ifndef OPENSSL_NO_RSA\n${str}#endif\n";  }
	elsif ( ($_ =~ /^dsa$/) || ($_ =~ /^gendsa$/) || ($_ =~ /^dsaparam$/))
		{ print "#ifndef OPENSSL_NO_DSA\n${str}#endif\n"; }
	elsif ( ($_ =~ /^ec$/) || ($_ =~ /^ecparam$/))
		{ print "#ifndef OPENSSL_NO_EC\n${str}#endif\n";}
	elsif ( ($_ =~ /^dh$/) || ($_ =~ /^gendh$/) || ($_ =~ /^dhparam$/))
		{ print "#ifndef OPENSSL_NO_DH\n${str}#endif\n"; }
	elsif ( ($_ =~ /^pkcs12$/))
		{ print "#if !defined(OPENSSL_NO_DES) && !defined(OPENSSL_NO_SHA1)\n${str}#endif\n"; }
	elsif ( ($_ =~ /^cms$/))
		{ print "#ifndef OPENSSL_NO_CMS\n${str}#endif\n"; }
	elsif ( ($_ =~ /^ocsp$/))
		{ print "#ifndef OPENSSL_NO_OCSP\n${str}#endif\n"; }
	elsif ( ($_ =~ /^srp$/))
		{ print "#ifndef OPENSSL_NO_SRP\n${str}#endif\n"; }
	else
		{ print $str; }
	}

foreach ("md2","md4","md5","sha","sha1","mdc2","rmd160","sm3")
	{
	push(@files,$_);
	printf "#ifndef OPENSSL_NO_".uc($_)."\n\t{FUNC_TYPE_MD,\"".$_."\",dgst_main},\n#endif\n";
	}

foreach (
>>>>>>> origin/master
	"aes-128-cbc", "aes-128-ecb",
	"aes-192-cbc", "aes-192-ecb",
	"aes-256-cbc", "aes-256-ecb",
	"camellia-128-cbc", "camellia-128-ecb",
	"camellia-192-cbc", "camellia-192-ecb",
	"camellia-256-cbc", "camellia-256-ecb",
	"base64", "zlib",
	"des", "des3", "desx", "idea", "seed", "rc4", "rc4-40",
<<<<<<< HEAD
	"rc2", "bf", "cast", "rc5",
=======
	"rc2", "bf", "cast", "rc5", "sms4",
>>>>>>> origin/master
	"des-ecb", "des-ede",    "des-ede3",
	"des-cbc", "des-ede-cbc","des-ede3-cbc",
	"des-cfb", "des-ede-cfb","des-ede3-cfb",
	"des-ofb", "des-ede-ofb","des-ede3-ofb",
	"idea-cbc","idea-ecb",    "idea-cfb", "idea-ofb",
	"seed-cbc","seed-ecb",    "seed-cfb", "seed-ofb",
	"rc2-cbc", "rc2-ecb", "rc2-cfb","rc2-ofb", "rc2-64-cbc", "rc2-40-cbc",
	"bf-cbc",  "bf-ecb",     "bf-cfb",   "bf-ofb",
	"cast5-cbc","cast5-ecb", "cast5-cfb","cast5-ofb",
<<<<<<< HEAD
	"cast-cbc", "rc5-cbc",   "rc5-ecb",  "rc5-cfb",  "rc5-ofb"
) {
	my $str="    { FT_cipher, \"$cmd\", enc_main, enc_options },\n";
	(my $algo= $cmd) =~ s/-.*//g;
        if ($cmd eq "zlib") {
                print "#ifdef ZLIB\n${str}#endif\n";
        } elsif (grep { $algo eq $_ } @disablables) {
                print "#ifndef OPENSSL_NO_".uc($algo)."\n${str}#endif\n";
        } elsif (my $disabler = $cipher_disabler{$algo}) {
                print "#ifndef OPENSSL_NO_".uc($disabler)."\n${str}#endif\n";
	} else {
		print $str;
	}
}

print "    { 0, NULL, NULL}\n};\n";
print "#endif\n";
=======
	"cast-cbc", "rc5-cbc",   "rc5-ecb",  "rc5-cfb",  "rc5-ofb",
	"sms4-cbc", "sms4-ecb", "sms4-cfb", "sms4-ofb", "zuc")
	{
	push(@files,$_);

	$t=sprintf("\t{FUNC_TYPE_CIPHER,\"%s\",enc_main},\n",$_);
	if    ($_ =~ /des/)  { $t="#ifndef OPENSSL_NO_DES\n${t}#endif\n"; }
	elsif ($_ =~ /aes/)  { $t="#ifndef OPENSSL_NO_AES\n${t}#endif\n"; }
	elsif ($_ =~ /camellia/)  { $t="#ifndef OPENSSL_NO_CAMELLIA\n${t}#endif\n"; }
	elsif ($_ =~ /idea/) { $t="#ifndef OPENSSL_NO_IDEA\n${t}#endif\n"; }
	elsif ($_ =~ /seed/) { $t="#ifndef OPENSSL_NO_SEED\n${t}#endif\n"; }
	elsif ($_ =~ /rc4/)  { $t="#ifndef OPENSSL_NO_RC4\n${t}#endif\n"; }
	elsif ($_ =~ /rc2/)  { $t="#ifndef OPENSSL_NO_RC2\n${t}#endif\n"; }
	elsif ($_ =~ /bf/)   { $t="#ifndef OPENSSL_NO_BF\n${t}#endif\n"; }
	elsif ($_ =~ /cast/) { $t="#ifndef OPENSSL_NO_CAST\n${t}#endif\n"; }
	elsif ($_ =~ /rc5/)  { $t="#ifndef OPENSSL_NO_RC5\n${t}#endif\n"; }
	elsif ($_ =~ /sms4/)  { $t="#ifndef OPENSSL_NO_SMS4\n${t}#endif\n"; }
	elsif ($_ =~ /zuc/)  { $t="#ifndef OPENSSL_NO_ZUC\n${t}#endif\n"; }
	elsif ($_ =~ /zlib/)  { $t="#ifdef ZLIB\n${t}#endif\n"; }
	print $t;
	}

print "\t{0,NULL,NULL}\n\t};\n";
>>>>>>> origin/master
