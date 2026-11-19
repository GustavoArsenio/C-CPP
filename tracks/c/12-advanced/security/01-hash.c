#include <openssl/evp.h>
#include <stdio.h>

int main(void) {
    static const unsigned char message[] = "hello";
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_length = 0;

    if (EVP_Digest(message, sizeof(message) - 1, digest, &digest_length,
                   EVP_sha256(), NULL) != 1) {
        fputs("Unable to compute SHA-256 digest.\n", stderr);
        return 1;
    }

    for (unsigned int i = 0; i < digest_length; ++i) {
        printf("%02x", digest[i]);
    }
    putchar('\n');
    return 0;
}
