#include <stdio.h>
#include "atm.h"

int main(void) {
    printf("=================================================\n");
    printf("       WELCOME TO THE ATM MACHINE SIMULATOR      \n");
    printf("=================================================\n");

    for (;;) {
        int user = atm_login();
        if (user >= 0) {
            atm_session(user);
        } else {
            /* Login failed after allowed attempts — ask to retry or exit */
            char ans[8];
            printf("\nLogin failed. Try again? (y/n): ");
            if (!fgets(ans, sizeof(ans), stdin)) return 0;
            if (ans[0] != 'y' && ans[0] != 'Y') {
                atm_goodbye();
                return 0;
            }
            continue;
        }

        /* After logout, ask whether to exit app or go back to login */
        char ans[8];
        printf("Would you like to exit the app? (y/n): ");
        if (!fgets(ans, sizeof(ans), stdin)) return 0;
        if (ans[0] == 'y' || ans[0] == 'Y') {
            atm_goodbye();
            break;
        }
    }

    return 0;
}
