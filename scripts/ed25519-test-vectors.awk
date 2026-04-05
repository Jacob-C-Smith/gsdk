BEGIN {
    FS=":";
    printf ("/** !\n"\
" * ed25519 test vectors\n"\
" * \n"\
" * @file src/core/ed25519/ed25519_test.h\n"\
" * \n"\
" * @author Jacob Smith\n"\
" */\n"\
"\n"\
"// gsdk\n"\
"/// core\n"\
"#include <core/ed25519.h>\n"\
"\n"\
"struct ed25519_test_vector_s\n"\
"{\n"\
"    ed25519_public_key public_key;\n"\
"    ed25519_private_key private_key;\n"\
"    int len;\n"\
"    char _input[1024];\n"\
"    ed25519_signature signature;\n"\
"};\n"\
"\n"\
"typedef struct ed25519_test_vector_s ed25519_test_vector;\n"\
"\n"\
"ed25519_test_vector _ed25519_test_vectors[] = {\n"\
);
}

{
    PRIV = substr($1, 1, 64);
    PUB = $2;
    INP = $3;
    LEN = length(INP) / 2;
    EXP = substr($4, 1, 128);
    
    printf("\t[%d] = (ed25519_test_vector) {\n", NR-1);
    printf("\t\t.public_key = { ");
    for (I=1; I <= 64; I+=2)
        printf("0x%s, ", substr(PUB, I, 2));
    printf("},\n");

    printf("\t\t.private_key = { ");
    for (I=1; I <= 64; I+=2)
        printf("0x%s, ", substr(PRIV, I, 2));
    printf("},\n");

    printf("\t\t.len = %d,\n", LEN);

    printf("\t\t._input = { ");
    for (I=1; I < LEN*2; I+=2)
        printf("0x%s, ", substr(INP, I, 2));
    printf("},\n");

    printf("\t\t.signature = { ");
    for (I=1; I <= 128; I+=2)
        printf("0x%s, ", substr(EXP, I, 2));
    printf("},\n");

    printf("\t},\n");
}

END {
    printf("};\n");
}