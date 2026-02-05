/**
 * @file checking_tools.c
 * @author Max Nonfried (nonfried@students.zcu.cz)
 * @brief Soubor obsahuje nástroje pro kontrolu vstupu.
 * @version 1.0
 * @date 2021-11-29
 */

#include "definitions.h"
#include "checking_tools.h"

/**
 * @brief Zkontroluje počet argumentů, jejich správnost.
 *        Jestliže soubor obsahuje příponu, musí to být přípona pgm. 
 *        Jestliže příponu nemá, bere se jako pgm soubor.
 * @param argc počet argumentů.
 * @param arg první argument
 * @return 0, jestliže je vše v pořádku.
 *         1, jestliže nejsou splněny požadavky.
 */
int check_argument(int argc, char *argv[]) {       
    char *index;
    
    /* count of arguments check */
    if (argc != ARGUMENTS_COUNT) {
        return 1;
    }

    index = strrchr(argv[1],'.');

    if (index == NULL) {
        return 0;
    }    
    if (strcmp(index,".pgm") == 0) {
        return 0;
    }
    return 1;
}

/**
 * @brief Zkontroluje hlavičku pgm souboru. Hlavička musí být ve tvaru
 *        P5\n
 *        "počet sloupců>0" "počet řádek>0"\n
 *        255\n
 * @param mf ukazatel na strukturu myfile.
 * @return 0, jestliže je vše v pořádku.
 *         1, jestliže nejsou splněny požadavky.
 */
int check_header(myfile *mf) {
    int number1 = 0, number2 = 0;
    char header[MAX_HEADER_ROW_LENGTH] = {0};

    /* kontrola P5\n */
    fgets(header, MAX_HEADER_ROW_LENGTH, (FILE*) mf->fp);
    if (strcmp(header, "P5\n") != 0) {
        return 1;
    }

    /* kontrola druhé řádky, zda se jedná o čísla */
    fgets(header, MAX_HEADER_ROW_LENGTH, (FILE*) mf->fp);
    if (sscanf(header, "%d %d\n", &number1, &number2) == 0) {
   		return 1;
	}
    /* kontrola druhé řádky, zda jsou čísla větší než nula */
    if (number1 <= 0 || number2 <= 0) {
        return 1;
    }

    /* zda třetí řádka obsahuje jen a pouze 255\n */
    fgets(header, MAX_HEADER_ROW_LENGTH, (FILE*) mf->fp);
    if (strcmp(header, "255\n") != 0) {
        return 1;
    }
    return 0;
    
}

/**
 * @brief Zkontroluje, zda všechna data pgm souboru jsou hodnoty 0, nebo 255. 
 * @param mf ukazatel na strukturu myfile.
 * @return 0, pokud je vše v pořádku - tj. hodnoty jsou pouze 0, nebo 255.
 *         1, pokud nejsou splněny požadavky - tj. data nabývají i jiných hodnot.
 */
int check_data(myfile *mf) {
    int i = 0;
    
    for (i = 0; i < mf->data_length; i++) {
        if ((mf->data[i] != 0) && (mf->data[i] != 255)) {
            return 1;
        }        
    }
    return 0;
}