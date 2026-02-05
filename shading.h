/**
 * @file shading.h
 * @author Max Nonfried (nonfried@students.zcu.cz)
 * @brief Hlavičkový soubor pro soubor shading.c
 * @version 1.0
 * @date 2021-11-29
 */

#ifndef shading
#define shading

#define MAX_OF_SHADE_COUNTER 250

/**
 * @brief Vybere sousedy daného prvku. Uloží je do mf->neighbours_values.
 * @param mf ukazatel na strukturu myfile.
 * @param i index prvku, ke kterému hledáme sousedy.
 */
void choose_neighbours_values(myfile *mf, int i);

/**
 * @brief Pomocná funkce pro quick sort řazení.
 * @param a uchar číslo k porovnání.
 * @param b uchar číslo k porovnání.
 * @return int, vrací rozdíl parametrů (čísel) - porovnání.
 */
int compare(const void* a, const void* b);

/**
 * @brief Náhodně vybere nový, nepoužitý odstín.
 * @param mf ukazatel na strukturu myfile.
 * @param labels_counter počítadlo odstínů od začátku běhu programu.
 * @param shades_counter počítadlo aktuálně použitých odstínů.
 * @return -1, pokud aktuální počet použitých odstínů je roven 250
 *         nový odstín, pokud je vše v pořádku.
 */
int choose_shade(myfile *mf, int *labels_counter, int *shades_counter);

/**
 * @brief Obarví daný pixel podle pravidel prvního kola CCL.
 * @param mf ukazatel na strukturu myfile.
 * @param i index, kde se v datech právě nacházíme.
 * @param labels_counter očítadlo odstínů od začátku běhu programu.
 * @param shades_counter počítadlo aktuálně použitých odstínů.
 * @return -1, pokud došly nové odstíny.
 *          0, pokud je vše v pořádku.
 */
int shade_data_round1(myfile *mf, int i, int *labels_counter, int *shades_counter);

/**
 * @brief Zjednoduší tabulku ekvivalence tak, aby se odstíny přepisovaly přímo
 *        na konečný odstín (odstín, který se přepisuje sám na sebe).
 * @param mf ukazatel na strukturu myfile.
 */
void simplify_equivalency(myfile *mf);

/**
 * @brief Spočte počet nulových sousedů.
 * @param mf ukazatel na strukturu myfile.
 * @return počet nulových sousedů
 */
int get_zero_neighbours_count(myfile *mf);

/**
 * @brief Provede druhé kolo obarvení obrázku do indexu i.
 *        Odstíny se přepíší na příslušné odstíny, podle tabulky ekvivalence.
 * @param mf ukazatel na strukturu myfile.
 * @param i index, kde se právě v datech nacházíme.
 */
void shade_data_round2(myfile *mf, int i);

/**
 * @brief Řídí obarvení pgm obrázku.
 * @param mf ukazatel na strukturu myfile.
 */
void handle_shading(myfile *mf);

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
void sort_neighbours_by_labels(myfile *mf, int zero_neighbours_count);

/**
 * @brief Odstínům, které se nepřepisují samy na sebe, přiřadí 0.
 * @param mf ukazatel na strukturu myfile.
 */
void clear_equivalency(myfile *mf);

/**
 * @brief Spočte počet aktuálně použitých odstínů.
 *        Vrací správný výsledek pouze, pokud je zavolána po funkci clear_equivalency.
 * @param mf ukazatel na strukturu myfile.
 * @return Vrací počet aktuálně použitých odstínů.
 */
int get_equivalency_count(myfile *mf);

#endif
