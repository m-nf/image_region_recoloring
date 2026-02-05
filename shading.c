/**
 * @file shading.c
 * @author Max Nonfried (nonfried@students.zcu.cz)
 * @brief Soubor zajišťující obarvení pgm souboru.
 * @version 1.0
 * @date 2021-11-29
 */

#include "definitions.h"
#include "checking_tools.h"
#include "shading.h"

/**
 * @brief Pomocná funkce pro quick sort řazení.
 * @param a uchar číslo k porovnání.
 * @param b uchar číslo k porovnání.
 * @return int, vrací rozdíl parametrů (čísel) - porovnání.
 */
int compare(const void *a, const void *b) {
    return *(uchar*)a - *(uchar*)b ;
}

/**
 * @brief Vybere sousedy daného prvku. Uloží je do mf->neighbours_values.
 * @param mf ukazatel na strukturu myfile.
 * @param i index prvku, ke kterému hledáme sousedy.
 */
void choose_neighbours_values(myfile *mf, int i) {
    memset(mf->neighbours_values, 0, sizeof(mf->neighbours_values[0]) * NUMBER_OF_NEIGHBOURS);
    
    /* první řádek */
    if (i < mf->columns) {
        mf->neighbours_values[0] = mf->data[i - 1];
    }
    /* první sloupec */
    else if (i % mf->columns == 0) {
        mf->neighbours_values[0] = mf->data[i - mf->columns];        
        mf->neighbours_values[1] = mf->data[i - mf->columns + 1];
    }
    /* poslední sloupec */
    else if ((i % mf->columns) == (mf->columns - 1)) {
        mf->neighbours_values[0] = mf->data[i - 1];
        mf->neighbours_values[1] = mf->data[i - mf->columns];
        mf->neighbours_values[2] = mf->data[i - mf->columns - 1];
    }    
    /* ostatní */
    else {
        mf->neighbours_values[0] = mf->data[i - mf->columns - 1];
        mf->neighbours_values[1] = mf->data[i - mf->columns];
        mf->neighbours_values[2] = mf->data[i - mf->columns + 1];
        mf->neighbours_values[3] = mf->data[i - 1];
    }    
}

/**
 * @brief Do mf->neigbours_values[3] uloží (podle popisku odstínu, na který se soused momentálně
 *        přebarvuje) souseda, jehož odstín, na který se přebarvuje je nejstarší (tedy odstín, který
 *        má v mf->shades_labels nejmenší hodnotu).
 *        
 *        Uložení do mf->neigbours_values[3] probíhá výměnou. Tedy například soused na 
 *        pozici [3] se uloží na pozici [2] a soused na pozici [2] se uloží na pozici [3].
 * 
 *        Při vstupu jsou sousedé seřazeni od nejmenšího po největší (podle jejich hodnoty).
 *        Např. 0, 0, 25, 165.
 * @param mf ukazatel na strukturu myfile.
 * @param zero_neighbours_count počet nulových sousedů.
 */
void sort_neighbours_by_labels(myfile *mf, int zero_neighbours_count) {
    int x3 = 0, x2 = 0, x1 = 0, x0 = 0;
    uchar tmp = 0;    
    
    x3 = mf->shades_labels[mf->equivalency[mf->neighbours_values[3]]];
    x2 = mf->shades_labels[mf->equivalency[mf->neighbours_values[2]]];
    x1 = mf->shades_labels[mf->equivalency[mf->neighbours_values[1]]];
    x0 = mf->shades_labels[mf->equivalency[mf->neighbours_values[0]]];

    switch (zero_neighbours_count) {
        case 2:
            if (x3 >= x2) {
                tmp = mf->neighbours_values[3];
                mf->neighbours_values[3] = mf->neighbours_values[2];
                mf->neighbours_values[2] = tmp;
            }
            break;
        case 1:
            if ((x3 <= x2) && (x3 <= x1)) {
                ;
            }
            else if ((x2 <= x3) && (x2 <= x1)) {                
                tmp = mf->neighbours_values[3];
                mf->neighbours_values[3] = mf->neighbours_values[2];
                mf->neighbours_values[2] = tmp;
            }
            else if ((x1 <= x3) && (x1 <= x2)) {                
                tmp = mf->neighbours_values[3];
                mf->neighbours_values[3] = mf->neighbours_values[1];
                mf->neighbours_values[1] = tmp;
            }
            
            break;
        case 0:
            if ((x3 <= x2) && (x3 <= x1) && (x3 <= x0)) {
                ;
            }
            else if ((x2 <= x3) && (x2 <= x1) && (x2 <= x0)) {                
                tmp = mf->neighbours_values[3];
                mf->neighbours_values[3] = mf->neighbours_values[2];
                mf->neighbours_values[2] = tmp;
            }
            else if ((x1 <= x3) && (x1 <= x2) && (x1 <= x0)) {                
                tmp = mf->neighbours_values[3];
                mf->neighbours_values[3] = mf->neighbours_values[1];
                mf->neighbours_values[1] = tmp;
            }
            else if ((x0 <= x3) && (x0 <= x2) && (x0 <= x1)) {
                tmp = mf->neighbours_values[3];
                mf->neighbours_values[3] = mf->neighbours_values[0];
                mf->neighbours_values[0] = tmp;
            }
            break;
        default:
            break;
    }    
}

/* prostor pro zlepseni - nedavat nahdone ty cisla, ale poporade - takhle to muze vyhledavat velmi douho v pripade
   ze hodne barev je zabrano, udelat vyjimku, pokud jsou vsechy vybrany */

/**
 * @brief Náhodně vybere nový, nepoužitý odstín.
 * @param mf ukazatel na strukturu myfile.
 * @param labels_counter počítadlo odstínů od začátku běhu programu.
 * @param shades_counter počítadlo aktuálně použitých odstínů.
 * @return -1, pokud aktuální počet použitých odstínů je roven 250
 *         nový odstín, pokud je vše v pořádku.
 */
int choose_shade(myfile *mf, int *labels_counter, int *shades_counter) {
    int rnd;
    
    if (*shades_counter == MAX_OF_SHADE_COUNTER) {        
        return -1;        
    }
    
    do {
        /* 0 is backround, generates number from 0 to 254, then increments by 1 */
        rnd = rand() % (NUMBER_OF_SHADES - 1);
        ++rnd;         
    } 
    while (mf->equivalency[rnd] != 0); /* if a shade isn't used == 0 */      
    
    ++(*shades_counter);
    ++(*labels_counter);    

    return rnd;
}

/**
 * @brief Spočte počet nulových sousedů.
 * @param mf ukazatel na strukturu myfile.
 * @return počet nulových sousedů
 */
int get_zero_neighbours_count(myfile *mf) {
    int j = 0, zero_neighbours_count = 0;

    for (j = 0; j < NUMBER_OF_NEIGHBOURS; j++) {
        if (mf->neighbours_values[j] == 0) {
            ++zero_neighbours_count;
        }
    }
    return zero_neighbours_count;
}

/**
 * @brief Obarví daný pixel podle pravidel prvního kola CCL.
 * @param mf ukazatel na strukturu myfile.
 * @param i index, kde se v datech právě nacházíme.
 * @param labels_counter očítadlo odstínů od začátku běhu programu.
 * @param shades_counter počítadlo aktuálně použitých odstínů.
 * @return -1, pokud došly nové odstíny.
 *          0, pokud je vše v pořádku.
 */
int shade_data_round1(myfile *mf, int i, int *labels_counter, int *shades_counter) {
    int new_shade = 0, zero_neighbours_count = 0;   

    zero_neighbours_count = get_zero_neighbours_count(mf);
    
    /* seřadí sousedy vzestupně*/
    qsort(mf->neighbours_values, NUMBER_OF_NEIGHBOURS, sizeof(uchar), compare);    

    switch (zero_neighbours_count) {
        case 4: /* žádný nenulový soused -> nový odstín */
            new_shade = choose_shade(mf, labels_counter, shades_counter);
            if (new_shade == -1) {
                /* nové odstíny došly */
                return -1;
            }            

            /* obarví pixel */
            mf->data[i] = new_shade;            
            /* nový odstín se přepisuje sám na sebe */
            mf->equivalency[new_shade] = new_shade;            
            /* uložení popisku nového odstínu, tedy jeho stáří (pořadí) */
            mf->shades_labels[new_shade] = *labels_counter;             

            break;    
        case 3: /* jeden nenulový soused */    
            /* pixel se obarví na odstín, na který se přepisuje jeho soused */
            mf->data[i] = mf->equivalency[mf->neighbours_values[3]]; 
            break;        
        case 2:
            /* vybere nejpovolanějšího souseda, viz popis dané funkce */
            sort_neighbours_by_labels(mf, zero_neighbours_count);       
            /* pixel se obarví na odstín, na který se přepisuje nejpovolanější soused */
            mf->data[i] = mf->equivalency[mf->neighbours_values[3]];

            /* ostatní sousedi se budou přepisovat na odstín, na který se přepisuje nejpovolanější soused */
            mf->equivalency[mf->neighbours_values[2]] = mf->equivalency[mf->neighbours_values[3]];        
            break;        
        case 1:            
            sort_neighbours_by_labels(mf, zero_neighbours_count);           
            mf->data[i] = mf->equivalency[mf->neighbours_values[3]];
            
            mf->equivalency[mf->neighbours_values[2]] = mf->equivalency[mf->neighbours_values[3]];
            mf->equivalency[mf->neighbours_values[1]] = mf->equivalency[mf->neighbours_values[3]];
            break;
        case 0:
            sort_neighbours_by_labels(mf,zero_neighbours_count);
            mf->data[i] = mf->equivalency[mf->neighbours_values[3]];
           
            mf->equivalency[mf->neighbours_values[2]] = mf->equivalency[mf->neighbours_values[3]];
            mf->equivalency[mf->neighbours_values[1]] = mf->equivalency[mf->neighbours_values[3]];
            mf->equivalency[mf->neighbours_values[0]] = mf->equivalency[mf->neighbours_values[3]];
            break;
        default:
            break;
    }
    return 0;
}

/**
 * @brief Provede druhé kolo obarvení obrázku do indexu i.
 *        Odstíny se přepíší na příslušné odstíny, podle tabulky ekvivalence.
 * @param mf ukazatel na strukturu myfile.
 * @param i index, kde se právě v datech nacházíme.
 */
void shade_data_round2(myfile *mf, int i) {
    int j = 0;
    
    /* j = 1, protože první pixel nelze na nic přepsat */ 
    for (j = 1; j < i; j++) {
        if (mf->data[j] != 0) {            
            mf->data[j] = mf->equivalency[mf->data[j]];           
        }
    }
}

/**
 * @brief Zjednoduší tabulku ekvivalence tak, aby se odstíny přepisovaly přímo
 *        na konečný odstín (odstín, který se přepisuje sám na sebe).
 * @param mf ukazatel na strukturu myfile.
 */
void simplify_equivalency(myfile *mf) {
    int i = 0;
    
    for (i = 1; i < NUMBER_OF_SHADES; i++) {
        /* dokud se odstín i nebude přepisovat na odstín, který se přepisuje sám na sebe */
        while (mf->equivalency[i] != mf->equivalency[mf->equivalency[i]]) {
            mf->equivalency[i] = mf->equivalency[mf->equivalency[i]];           
        }           
    }    
}

/**
 * @brief Odstínům, které se nepřepisují samy na sebe, přiřadí 0.
 * @param mf ukazatel na strukturu myfile.
 */
void clear_equivalency(myfile *mf) {
    int i = 0;

    for (i = 1; i < NUMBER_OF_SHADES; i++) {
        if (i != mf->equivalency[i]) {
            mf->equivalency[i] = 0;
        }
    }     
}

/**
 * @brief Spočte počet aktuálně použitých odstínů.
 *        Vrací správný výsledek pouze, pokud je zavolána po funkci clear_equivalency.
 * @param mf ukazatel na strukturu myfile.
 * @return Vrací počet aktuálně použitých odstínů.
 */
int get_equivalency_count(myfile *mf) {
    int i = 0, counter = 0;
    
    for (i = 1; i < NUMBER_OF_SHADES; i++) {
        if (i == mf->equivalency[i]) {
            ++counter;
        }
    }    
    return counter;
}

/**
 * @brief Řídí obarvení pgm obrázku.
 * @param mf ukazatel na strukturu myfile.
 */
void handle_shading(myfile *mf) {
    int i = 0, new_shade = 0; 
    int labels_counter = 0; /* počítadlo nových odstínů - kolikátý nový odstín se vygeneroval od začátku běhu programu */
    int shades_counter = 0; /* počítadlo aktuálně používaných odstínů */
    
    /* srand(time(NULL)); */
    
    /* případné obarvení prvního prvku (nemá sousedy) */
    if(mf->data[0] != 0) {
        new_shade = choose_shade(mf, &labels_counter, &shades_counter);
        mf->data[0] = new_shade;
        mf->equivalency[new_shade] = new_shade;
        mf->shades_labels[new_shade] = labels_counter;
    }    
    /* hlavní smyčka pro obarvování */
    for (i = 1; i < mf->data_length; i++) {
        if (mf->data[i] != 0) { /* pokud se nejedná o nulový pixel */
            choose_neighbours_values(mf, i);                                    
            
            if (shade_data_round1(mf, i, &labels_counter, &shades_counter) == -1) {            
                /* došly nové ostíny -> provede se druhé kolo, aby se uvolnily */
                simplify_equivalency(mf);
                shade_data_round2(mf, i);
                clear_equivalency(mf);
                shades_counter = get_equivalency_count(mf);
                
                if (shades_counter == MAX_OF_SHADE_COUNTER) {
                    /* nové odstíny došly úplně -> konec programu, obrázek je uložen ve stávajícím, rozpracovaném stavu */
                    printf("ERROR - too many objects in the picture");
                    return;
                }
                
                choose_neighbours_values(mf, i); /* aktuální sousedi se nám přebarvili, je potřeba je vybrat znovu */
                shade_data_round1(mf, i, &labels_counter, &shades_counter);
            }
        }
    }
    /* závěrečné druhé kolo */
    simplify_equivalency(mf);
    shade_data_round2(mf, i);
}