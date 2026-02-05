/**
 * @file ccl.c
 * @author Max Nonfried (nonfried@students.zcu.cz)
 * @brief Hlavní soubor obsahující funkci main.
 * @version 1.0
 * @date 2021-11-29
 */

#include "definitions.h"
#include "checking_tools.h"
#include "shading.h"
#include "time.h"

/**
 * @brief Získá počet sloupců a řádků z hlavičky pgm souboru a uloží je
 *        do &mf->columns, &mf->rows.
 *        Ukazatel pro čtecí pozici v pgm souboru je po vykonání fuknce
 *        na stejném místě jako před ní.
 * @param mf ukazatel na strukturu myfile
 */
void parse_header(myfile *mf) {    
    char header[MAX_HEADER_ROW_LENGTH] = {0};

    fseek((FILE*) mf->fp, 3, SEEK_SET);
    fgets(header, MAX_HEADER_ROW_LENGTH, (FILE*) mf->fp);
    sscanf(header, "%d %d\n", &mf->columns, &mf->rows);
    fseek((FILE*) mf->fp, 4, SEEK_CUR);
}

/**
 * @brief Vybere největší hodnotu odstínu, která se vyskytuje v pgm souboru po obarvení.
 * @param mf ukazatel na strukturu the myfile
 * @return uchar největší hodnotu odstínu.
 */
uchar choose_maximal_shade(myfile *mf) {
    int i = 0;
    uchar max = mf->equivalency[1];

    for (i = 2; i < NUMBER_OF_SHADES; i++) {
        if (mf->equivalency[i] > max) {
            max = mf->equivalency[i];
        }
    }

    return max;   
}

/**
 * @brief Zapíše hlavičku a data do výstupního pgm souboru.
 * @param mf ukazatel na strukturu myfile
 */
void write_data(myfile *mf) {
    char tmp[50] = {0};
    int max = choose_maximal_shade(mf);

    sprintf(tmp, "P5\n%d %d\n%d\n", mf->columns, mf->rows, max);
    fputs(tmp, (FILE*) mf->fp);    
    
    fwrite(mf->data, sizeof(uchar), mf->data_length,(FILE*) mf->fp);
}

/**
 * @brief Zajistí kontrolu hlavičky a načte z ní data.
 *        Jestliže hlavička není v pořádku, uklidí a skončí program.
 * @param mf ukazatel na strukturu myfile.
 */
void prepare_header(myfile *mf) {
    /* kontrola hlavičky souboru */
    if (check_header(mf) == 1) {
        printf("ERROR - invalid header\n");
        fclose(mf->fp);
        free(mf);
        exit(EXIT_FAILURE);
    }
    parse_header(mf);
}

/**
 * @brief Načte data a zajistí jejich zkontrolování.
 *        Jestliže data nejsou v pořádku, uklidí a ukončí program.
 * @param mf ukazatel na strukturu myfile.
 */
void prepare_data(myfile *mf) {
    mf->data_length = mf->columns * mf->rows;
    mf->data = malloc(sizeof(uchar) * mf->data_length);

    fread(mf->data, sizeof(uchar), mf->data_length, mf->fp);

    /* kontrola dat */
    if (check_data(mf) == 1) {
        printf("ERROR - invalid data\n");        
        free(mf->data);
        fclose(mf->fp);
        free(mf);
        exit(EXIT_FAILURE);
    }
}   

/**
 * @brief Zkontroluje argumenty. Zajistí načtení a zkontrolování vstupu.
 * @param argc počet argumentů.
 * @param argv argumenty.
 * @param mf ukazatel na strukturu myfile.
 */
void prepare(int argc, char *argv[], myfile *mf) {
    /* kontrola argumentů*/
    if (check_argument(argc, argv) == 1) {
        printf("ERROR - invalid extension of input file or invalid number of arguments\n");
        free(mf);
        exit(EXIT_FAILURE);
    }

    /* otevírání vstupního souboru */
    mf->fp =fopen(argv[1], "r");    
    if (mf == NULL) {
        printf("Input file loading ERROR\n");
        free(mf);
        exit(EXIT_FAILURE);
    }
    
    prepare_header(mf);
    prepare_data(mf);

    /* zavření vstupního souboru */
    fclose(mf->fp);    

    /* otevírání výstupního souboru*/
    mf->fp =fopen(argv[2], "w");    
    if (mf->fp == NULL) {
        printf("Output file loading ERROR\n");
        free(mf->data);
        free(mf);
        exit(EXIT_FAILURE);
    }   
}

/**
 * @brief Řídí hlavní chod programu.
 *        Spustí obarvování, zajistí zapsání do souboru, uvolní alokovanou pamět.
 * @param mf na strukturu myfile
 */
void run(myfile *mf) {
    memset(mf->equivalency, 0, sizeof(mf->equivalency[0]) * NUMBER_OF_SHADES);
    memset(mf->neighbours_values, 0, sizeof(mf->equivalency[0]) * NUMBER_OF_NEIGHBOURS);
    memset(mf->shades_labels, 0, sizeof(mf->shades_labels[0]) * NUMBER_OF_SHADES);    

    handle_shading(mf);

    write_data(mf);    
    
    fclose(mf->fp);
    free(mf->data);
    free(mf);
}

/**
 * @brief Hlavní přístupový bod aplikace. Zde je deklarován a inicializován ukazatel na strukturu 
 *        myfile *mf, se kterým se pracuje v celém programu.
 * @param argc počet argumentů
 * @param argv argumenty
 * @return Vrací vždy EXIT_SUCCES.
 */
int main(int argc, char *argv[]) {
    clock_t start = clock();
    clock_t end;
    myfile *mf = malloc(sizeof(myfile));   

    prepare(argc, argv, mf);
    run(mf);    

    end = clock();    
    printf("Program successfully shaded the picture.\n");
    printf("Running time of programm was %f seconds.\n", ((float)(end - start) / CLOCKS_PER_SEC));
    return EXIT_SUCCESS;
}