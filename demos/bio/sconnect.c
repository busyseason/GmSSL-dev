<<<<<<< HEAD
/*
 * Copyright 1998-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */
=======
/* NOCW */
/* demos/bio/sconnect.c */
>>>>>>> origin/master

/*-
 * A minimal program to do SSL to a passed host and port.
 * It is actually using non-blocking IO but in a very simple manner
 * sconnect host:port - it does a 'GET / HTTP/1.0'
 *
 * cc -I../../include sconnect.c -L../.. -lssl -lcrypto
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
<<<<<<< HEAD
#include <string.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

#define HOSTPORT "localhost:4433"
#define CAFILE "root.pem"

=======
#include <openssl/err.h>
#include <openssl/ssl.h>

>>>>>>> origin/master
extern int errno;

int main(argc, argv)
int argc;
char *argv[];
{
<<<<<<< HEAD
    const char *hostport = HOSTPORT;
    const char *CAfile = CAFILE;
    char *hostname;
    char *cp;
    BIO *out = NULL;
=======
    char *host;
    BIO *out;
>>>>>>> origin/master
    char buf[1024 * 10], *p;
    SSL_CTX *ssl_ctx = NULL;
    SSL *ssl;
    BIO *ssl_bio;
    int i, len, off, ret = 1;

<<<<<<< HEAD
    if (argc > 1)
        hostport = argv[1];
    if (argc > 2)
        CAfile = argv[2];

    hostname = OPENSSL_strdup(hostport);
    if ((cp = strchr(hostname, ':')) != NULL)
        *cp = 0;
=======
    if (argc <= 1)
        host = "localhost:4433";
    else
        host = argv[1];
>>>>>>> origin/master

#ifdef WATT32
    dbug_init();
    sock_init();
#endif

    /* Lets get nice error messages */
    SSL_load_error_strings();

    /* Setup all the global SSL stuff */
    OpenSSL_add_ssl_algorithms();
<<<<<<< HEAD
    ssl_ctx = SSL_CTX_new(TLS_client_method());

    /* Enable trust chain verification */
    SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_PEER, NULL);
    SSL_CTX_load_verify_locations(ssl_ctx, CAfile, NULL);
=======
    ssl_ctx = SSL_CTX_new(SSLv23_client_method());
>>>>>>> origin/master

    /* Lets make a SSL structure */
    ssl = SSL_new(ssl_ctx);
    SSL_set_connect_state(ssl);

<<<<<<< HEAD
    /* Enable peername verification */
    if (SSL_set1_host(ssl, hostname) <= 0)
        goto err;

=======
>>>>>>> origin/master
    /* Use it inside an SSL BIO */
    ssl_bio = BIO_new(BIO_f_ssl());
    BIO_set_ssl(ssl_bio, ssl, BIO_CLOSE);

    /* Lets use a connect BIO under the SSL BIO */
    out = BIO_new(BIO_s_connect());
<<<<<<< HEAD
    BIO_set_conn_hostname(out, hostport);
=======
    BIO_set_conn_hostname(out, host);
>>>>>>> origin/master
    BIO_set_nbio(out, 1);
    out = BIO_push(ssl_bio, out);

    p = "GET / HTTP/1.0\r\n\r\n";
    len = strlen(p);

    off = 0;
    for (;;) {
        i = BIO_write(out, &(p[off]), len);
        if (i <= 0) {
            if (BIO_should_retry(out)) {
                fprintf(stderr, "write DELAY\n");
                sleep(1);
                continue;
            } else {
                goto err;
            }
        }
        off += i;
        len -= i;
        if (len <= 0)
            break;
    }

    for (;;) {
        i = BIO_read(out, buf, sizeof(buf));
        if (i == 0)
            break;
        if (i < 0) {
            if (BIO_should_retry(out)) {
                fprintf(stderr, "read DELAY\n");
                sleep(1);
                continue;
            }
            goto err;
        }
        fwrite(buf, 1, i, stdout);
    }

    ret = 1;
<<<<<<< HEAD
    goto done;

 err:
    if (ERR_peek_error() == 0) { /* system call error */
        fprintf(stderr, "errno=%d ", errno);
        perror("error");
    } else
        ERR_print_errors_fp(stderr);
 done:
    BIO_free_all(out);
    SSL_CTX_free(ssl_ctx);
    return (ret == 1);
=======

    if (0) {
 err:
        if (ERR_peek_error() == 0) { /* system call error */
            fprintf(stderr, "errno=%d ", errno);
            perror("error");
        } else
            ERR_print_errors_fp(stderr);
    }
    BIO_free_all(out);
    if (ssl_ctx != NULL)
        SSL_CTX_free(ssl_ctx);
    exit(!ret);
    return (ret);
>>>>>>> origin/master
}
