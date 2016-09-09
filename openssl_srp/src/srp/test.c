#include "SrpClient.h"
#include "SrpServer.h"

#define S_USERNAME      "alice"
#define S_PASSWORD      "password123"

#define C_USERNAME      "alice"
#define C_PASSWORD      "password123"

#define MODULUS         "3072"

int main()
{
    SrpServer *server = NULL;
    SrpClient *client = NULL;
    char *s = NULL;
    char *B = NULL;
    char *A = NULL;
    char *server_u = NULL;
    char *server_S = NULL;
    char *client_u = NULL;
    char *client_S = NULL;
    size_t s_len = 0;
    size_t B_len = 0;
    size_t A_len = 0;
    size_t server_u_len = 0;
    size_t server_S_len = 0;
    size_t client_u_len = 0;
    size_t client_S_len = 0;

    server = SrpServer_New(MODULUS, S_USERNAME, S_PASSWORD);
    if (server == NULL)
    {
        printf("SrpServer_New failed\n");
        return 0;
    }

    client = SrpClient_New(MODULUS, C_USERNAME, C_PASSWORD);
    if (client == NULL)
    {
        printf("SrpClient_New failed\n");
        return 0;
    }

    /**
     * STEP 1. Server generate: s & B
     */
    printf("STEP 1. Server generate: s & B\n");

    if (RET_FAILED(SrpServer_generate_s(server, &s, &s_len)))
    {
        printf("SrpServer_generate_s failed\n");
        return 0;
    }
    printf("    s = %s\n", s);

    if (RET_FAILED(SrpServer_generate_B(server, &B, &B_len)))
    {
        printf("SrpServer_generate_B failed\n");
        return 0;
    }
    printf("    B = %s\n", B);

    /**
     * STEP 2. Server send s & B -> client
     */
    printf("STEP 2. Server send s & B -> client\n");

    /**
     * STEP 3. Client generate A
     */
    printf("STEP 3. Client generate A\n");

    if (RET_FAILED(SrpClient_set_s(client, s)))
    {
        printf("SrpClient_set_s failed");
        return 0;
    }

    if (RET_FAILED(SrpClient_generate_A(client, &A, &A_len)))
    {
        printf("SrpClient_generate_A failed\n");
        return 0;
    }
    printf("    A = %s\n", A);

    /**
     * STEP 4. Client send A -> Server
     */
    printf("STEP 4. Client send A -> Server\n");

    /**
     * STEP 5. Server compute u(Random Scrambling Parameter) & S(Premaster Secret)
     */
    printf("STEP 5. Server compute u(Random Scrambling Parameter) & S(Premaster Secret)\n");

    if (RET_FAILED(SrpServer_compute_u(server, A, &server_u, &server_u_len)))
    {
        printf("SrpServer_compute_u failed\n");
        return 0;
    }
    printf("    server u = %s\n", server_u);

    if (RET_FAILED(SrpServer_compute_S(server, &server_S, &server_S_len)))
    {
        printf("SrpServer_compute_u failed\n");
        return 0;
    }
    printf("    server S = %s\n", server_S);

    /**
     * STEP 6. client compute u(Random Scrambling Parameter) & S(Premaster Secret)
     */
    printf("STEP 6. client compute u(Random Scrambling Parameter) & S(Premaster Secret)\n");

    if (RET_FAILED(SrpClient_compute_u(client, B, &client_u, &client_u_len)))
    {
        printf("SrpClient_compute_u failed\n");
        return 0;
    }
    printf("    client u = %s\n", client_u);

    if (RET_FAILED(SrpClient_compute_S(client, &client_S, &client_S_len)))
    {
        printf("SrpClient_compute_S failed\n");
        return 0;
    }
    printf("    client S = %s\n", client_S);

    /**
     * STEP 7. check u & S
     */
    printf("STEP 7. check u & S\n");

    if (strcmp(client_u, server_u) == 0) {
        printf("    u is equal!\n");
    }
    else {
        printf("    u is not equal!\n");
    }

    if (strcmp(client_S, server_S) == 0) {
        printf("    S is equal!\n");
    }
    else {
        printf("    S is not equal!\n");
    }

    return 0;
}