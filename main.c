#include <stdio.h>
#include "atm.h"

int main(void) {
    printf("=================================================\n");
    printf("       WELCOME TO THE ATM MACHINE SIMULATOR      \n");
    printf("=================================================\n");

    /* First login — atm_login enforces exactly 2 attempts (username+PIN) */
    int user = atm_login();
    if (user < 0) {
        /* Exit the app immediately after two failed attempts */
        printf("\nToo many failed attempts. Exiting application...\n");
        atm_goodbye();
        return 0;
    }

    /* Successful login -> run session */
    atm_session(user);

    /* After logout, allow new sessions; still exit immediately on 2 failed attempts */
    for (;;) {
        char ans[8];
        printf("Would you like to exit the app? (y/n): ");
        if (!fgets(ans, sizeof(ans), stdin)) break;
        if (ans[0] == 'y' || ans[0] == 'Y') {
            atm_goodbye();
            break;
        }

        user = atm_login();
        if (user < 0) {
            printf("\nToo many failed attempts. Exiting application...\n");
            atm_goodbye();
            break;
        }
        atm_session(user);
    }

    return 0;
}