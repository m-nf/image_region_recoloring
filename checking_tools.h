/**
 * @file checking_tools.h
 * @author Max Nonfried (nonfried@students.zcu.cz)
 * @brief Hlavičkový soubor k souboru checking_tools.c.
 * @version 1.0
 * @date 2021-11-29
 * 
 * Soubor obsahuje deklarace funkcí a jejich popis. 
 */

#ifndef checking_tools
#define checking_tools

/**
 * @brief Zkontroluje počet argumentů, jejich správnost.
 *        Jestliže soubor obsahuje příponu, musí to být přípona pgm. 
 *        Jestliže příponu nemá, bere se jako pgm soubor.
 * @param argc počet argumentů
 * @param arg 
 * @return 0, jestliže je vše v pořádku
 *         1, jestliže nejsou splněny požadavky
 */
int check_argument(int argc, char *argv[]);

/**
 * @brief Zkontroluje hlavičku pgm souboru. Hlavička musí být ve tvaru
 *        P5\n
 *        číslo>0 číslo>0\n
 *        255\n
 * @param mf ukazatel na strukturu themyfile
 * @return 0, jestliže je vše v pořádku
 *         1, jestliže nejsou splněny požadavky
 */
int check_header(myfile *mf);

/**
 * @brief Zkontroluje, zda všechna data pgm souboru jsou hodnoty 0, nebo 255. 
 * @param mf ukazatel na strukturu themyfile
 * @return 0, pokud je vše v pořádku - tj. hodnoty jsou pouze 0, nebo 255.
 *         1, pokud nejsou splněny požadavky - tj. data nabývají i jiných hodnot.
 */
int check_data(myfile *mf);

#endif