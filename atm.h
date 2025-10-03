#ifndef ATM_H
#define ATM_H

/* Public API for the ATM module */
int  atm_login(void);        /* returns user index on success, -1 on failure */
void atm_session(int user);  /* runs the per-user menu loop */
void atm_goodbye(void);      /* prints a farewell message */

#endif /* ATM_H */
