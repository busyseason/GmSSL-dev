<<<<<<< HEAD
/*
 * Copyright 1995-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <stdio.h>
#include "internal/cryptlib.h"
#include <openssl/rsa.h>
=======
/* crypto/evp/p_enc.c */
/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 *
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 *
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes cryptographic software written by
 *     Eric Young (eay@cryptsoft.com)"
 *    The word 'cryptographic' can be left out if the rouines from the library
 *    being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from
 *    the apps directory (application code) you must include an acknowledgement:
 *    "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 *
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */

#include <stdio.h>
#include "cryptlib.h"
#include <openssl/rand.h>
#ifndef OPENSSL_NO_RSA
# include <openssl/rsa.h>
#endif
>>>>>>> origin/master
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/x509.h>

<<<<<<< HEAD
int EVP_PKEY_encrypt_old(unsigned char *ek, const unsigned char *key,
                         int key_len, EVP_PKEY *pubk)
{
    int ret = 0;

#ifndef OPENSSL_NO_RSA
    if (EVP_PKEY_id(pubk) != EVP_PKEY_RSA) {
#endif
        EVPerr(EVP_F_EVP_PKEY_ENCRYPT_OLD, EVP_R_PUBLIC_KEY_NOT_RSA);
#ifndef OPENSSL_NO_RSA
        goto err;
    }
    ret =
        RSA_public_encrypt(key_len, key, ek, EVP_PKEY_get0_RSA(pubk),
                           RSA_PKCS1_PADDING);
 err:
#endif
    return (ret);
}
=======
/* GMSSL: EVP_PKEY_encrypt_old() is modified */
int EVP_PKEY_encrypt_old(unsigned char *out, const unsigned char *in,
	int inlen, EVP_PKEY *pkey)
{
	int ret = 0;
	EVP_PKEY_CTX *ctx = NULL;
	size_t size;

	if (pkey->type == EVP_PKEY_RSA) {
		if ((ret = RSA_public_encrypt(inlen, in, out, pkey->pkey.rsa,
			RSA_PKCS1_PADDING)) < 0) {
			EVPerr(EVP_F_EVP_PKEY_ENCRYPT_OLD,
				EVP_R_RSA_PUBLIC_ENCRYPT_FAILED);
			return 0;
		}
	} else {
		if (!(ctx = EVP_PKEY_CTX_new(pkey, NULL))) {
			EVPerr(EVP_F_EVP_PKEY_ENCRYPT_OLD, ERR_R_EVP_LIB);
			return 0;
		}
		if (!EVP_PKEY_encrypt_init(ctx)) {
			EVPerr(EVP_F_EVP_PKEY_ENCRYPT_OLD, ERR_R_EVP_LIB);
			return 0;
		}
		if (!EVP_PKEY_CTX_set_ec_enc_type(ctx, NID_sm_scheme)) {
			EVPerr(EVP_F_EVP_PKEY_ENCRYPT_OLD, ERR_R_EVP_LIB);
			goto end;
		}
		/* FIXME: this old API lost input buffer length */
		size = inlen + 256;
		if (!EVP_PKEY_encrypt(ctx, out, &size, in, inlen)) {
			EVPerr(EVP_F_EVP_PKEY_ENCRYPT_OLD, ERR_R_EVP_LIB);
			goto end;
		}
		ret = (int)size;
	}
end:
	EVP_PKEY_CTX_free(ctx);
	return ret;
}

>>>>>>> origin/master
