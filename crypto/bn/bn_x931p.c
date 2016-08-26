<<<<<<< HEAD
/*
 * Copyright 2011-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
=======
/* bn_x931p.c */
/*
 * Written by Dr Stephen N Henson (steve@openssl.org) for the OpenSSL project
 * 2005.
 */
/* ====================================================================
 * Copyright (c) 2005 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.OpenSSL.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    licensing@OpenSSL.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.OpenSSL.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
>>>>>>> origin/master
 */

#include <stdio.h>
#include <openssl/bn.h>
<<<<<<< HEAD
#include "bn_lcl.h"
=======
>>>>>>> origin/master

/* X9.31 routines for prime derivation */

/*
 * X9.31 prime derivation. This is used to generate the primes pi (p1, p2,
 * q1, q2) from a parameter Xpi by checking successive odd integers.
 */

static int bn_x931_derive_pi(BIGNUM *pi, const BIGNUM *Xpi, BN_CTX *ctx,
                             BN_GENCB *cb)
{
<<<<<<< HEAD
    int i = 0, is_prime;
=======
    int i = 0;
>>>>>>> origin/master
    if (!BN_copy(pi, Xpi))
        return 0;
    if (!BN_is_odd(pi) && !BN_add_word(pi, 1))
        return 0;
    for (;;) {
        i++;
        BN_GENCB_call(cb, 0, i);
<<<<<<< HEAD
        /* NB 27 MR is specified in X9.31 */
        is_prime = BN_is_prime_fasttest_ex(pi, 27, ctx, 1, cb);
        if (is_prime < 0)
            return 0;
        if (is_prime)
=======
        /* NB 27 MR is specificed in X9.31 */
        if (BN_is_prime_fasttest_ex(pi, 27, ctx, 1, cb))
>>>>>>> origin/master
            break;
        if (!BN_add_word(pi, 2))
            return 0;
    }
    BN_GENCB_call(cb, 2, i);
    return 1;
}

/*
 * This is the main X9.31 prime derivation function. From parameters Xp1, Xp2
 * and Xp derive the prime p. If the parameters p1 or p2 are not NULL they
 * will be returned too: this is needed for testing.
 */

int BN_X931_derive_prime_ex(BIGNUM *p, BIGNUM *p1, BIGNUM *p2,
                            const BIGNUM *Xp, const BIGNUM *Xp1,
                            const BIGNUM *Xp2, const BIGNUM *e, BN_CTX *ctx,
                            BN_GENCB *cb)
{
    int ret = 0;

    BIGNUM *t, *p1p2, *pm1;

    /* Only even e supported */
    if (!BN_is_odd(e))
        return 0;

    BN_CTX_start(ctx);
    if (!p1)
        p1 = BN_CTX_get(ctx);

    if (!p2)
        p2 = BN_CTX_get(ctx);

    t = BN_CTX_get(ctx);

    p1p2 = BN_CTX_get(ctx);

    pm1 = BN_CTX_get(ctx);

<<<<<<< HEAD
    if (pm1 == NULL)
        goto err;

=======
>>>>>>> origin/master
    if (!bn_x931_derive_pi(p1, Xp1, ctx, cb))
        goto err;

    if (!bn_x931_derive_pi(p2, Xp2, ctx, cb))
        goto err;

    if (!BN_mul(p1p2, p1, p2, ctx))
        goto err;

    /* First set p to value of Rp */

    if (!BN_mod_inverse(p, p2, p1, ctx))
        goto err;

    if (!BN_mul(p, p, p2, ctx))
        goto err;

    if (!BN_mod_inverse(t, p1, p2, ctx))
        goto err;

    if (!BN_mul(t, t, p1, ctx))
        goto err;

    if (!BN_sub(p, p, t))
        goto err;

    if (p->neg && !BN_add(p, p, p1p2))
        goto err;

    /* p now equals Rp */

    if (!BN_mod_sub(p, p, Xp, p1p2, ctx))
        goto err;

    if (!BN_add(p, p, Xp))
        goto err;

    /* p now equals Yp0 */

    for (;;) {
        int i = 1;
        BN_GENCB_call(cb, 0, i++);
        if (!BN_copy(pm1, p))
            goto err;
        if (!BN_sub_word(pm1, 1))
            goto err;
        if (!BN_gcd(t, pm1, e, ctx))
            goto err;
<<<<<<< HEAD
        if (BN_is_one(t)) {
=======
        if (BN_is_one(t)
>>>>>>> origin/master
            /*
             * X9.31 specifies 8 MR and 1 Lucas test or any prime test
             * offering similar or better guarantees 50 MR is considerably
             * better.
             */
<<<<<<< HEAD
            int r = BN_is_prime_fasttest_ex(p, 50, ctx, 1, cb);
            if (r < 0)
                goto err;
            if (r)
                break;
        }
=======
            && BN_is_prime_fasttest_ex(p, 50, ctx, 1, cb))
            break;
>>>>>>> origin/master
        if (!BN_add(p, p, p1p2))
            goto err;
    }

    BN_GENCB_call(cb, 3, 0);

    ret = 1;

 err:

    BN_CTX_end(ctx);

    return ret;
}

/*
<<<<<<< HEAD
 * Generate pair of parameters Xp, Xq for X9.31 prime generation. Note: nbits
 * parameter is sum of number of bits in both.
=======
 * Generate pair of paramters Xp, Xq for X9.31 prime generation. Note: nbits
 * paramter is sum of number of bits in both.
>>>>>>> origin/master
 */

int BN_X931_generate_Xpq(BIGNUM *Xp, BIGNUM *Xq, int nbits, BN_CTX *ctx)
{
    BIGNUM *t;
    int i;
    /*
     * Number of bits for each prime is of the form 512+128s for s = 0, 1,
     * ...
     */
    if ((nbits < 1024) || (nbits & 0xff))
        return 0;
    nbits >>= 1;
    /*
     * The random value Xp must be between sqrt(2) * 2^(nbits-1) and 2^nbits
     * - 1. By setting the top two bits we ensure that the lower bound is
     * exceeded.
     */
<<<<<<< HEAD
    if (!BN_rand(Xp, nbits, BN_RAND_TOP_TWO, BN_RAND_BOTTOM_ANY))
        goto err;
=======
    if (!BN_rand(Xp, nbits, 1, 0))
        return 0;
>>>>>>> origin/master

    BN_CTX_start(ctx);
    t = BN_CTX_get(ctx);

    for (i = 0; i < 1000; i++) {
<<<<<<< HEAD
        if (!BN_rand(Xq, nbits, BN_RAND_TOP_TWO, BN_RAND_BOTTOM_ANY))
            goto err;
=======
        if (!BN_rand(Xq, nbits, 1, 0))
            return 0;
>>>>>>> origin/master
        /* Check that |Xp - Xq| > 2^(nbits - 100) */
        BN_sub(t, Xp, Xq);
        if (BN_num_bits(t) > (nbits - 100))
            break;
    }

    BN_CTX_end(ctx);

    if (i < 1000)
        return 1;

    return 0;

<<<<<<< HEAD
 err:
    BN_CTX_end(ctx);
    return 0;
=======
>>>>>>> origin/master
}

/*
 * Generate primes using X9.31 algorithm. Of the values p, p1, p2, Xp1 and
 * Xp2 only 'p' needs to be non-NULL. If any of the others are not NULL the
 * relevant parameter will be stored in it. Due to the fact that |Xp - Xq| >
 * 2^(nbits - 100) must be satisfied Xp and Xq are generated using the
 * previous function and supplied as input.
 */

int BN_X931_generate_prime_ex(BIGNUM *p, BIGNUM *p1, BIGNUM *p2,
                              BIGNUM *Xp1, BIGNUM *Xp2,
                              const BIGNUM *Xp,
                              const BIGNUM *e, BN_CTX *ctx, BN_GENCB *cb)
{
    int ret = 0;

    BN_CTX_start(ctx);
    if (!Xp1)
        Xp1 = BN_CTX_get(ctx);
    if (!Xp2)
        Xp2 = BN_CTX_get(ctx);

<<<<<<< HEAD
    if (!BN_rand(Xp1, 101, BN_RAND_TOP_ONE, BN_RAND_BOTTOM_ANY))
        goto error;
    if (!BN_rand(Xp2, 101, BN_RAND_TOP_ONE, BN_RAND_BOTTOM_ANY))
=======
    if (!BN_rand(Xp1, 101, 0, 0))
        goto error;
    if (!BN_rand(Xp2, 101, 0, 0))
>>>>>>> origin/master
        goto error;
    if (!BN_X931_derive_prime_ex(p, p1, p2, Xp, Xp1, Xp2, e, ctx, cb))
        goto error;

    ret = 1;

 error:
    BN_CTX_end(ctx);

    return ret;

}
