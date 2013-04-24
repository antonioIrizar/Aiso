#include "clipboard.h"

/* Variables globales */
extern struct list_head lista_clipboards;
extern unsigned int elemento_actual;
extern unsigned int tam;
// Este contador dice a memcpy cuantos cararcteres quieres leer
/*
unsigned long caracteres_copiar; //buffer_size
static char buffer_seleccionado[TAM_MAX_BUFFER];
*/

/* ----------------------------------------------------------- */

/** 
 * Funcion que se llama cuando leemos del archivo /proc/aisoclip/selection
 *
 * @param buffer donde copiaremos los datos
 * @param offset posicion actual del archivo
 * @return 0 si se ha acabado el archivo; valor negativo en caso de error
 */
extern int leer_indice(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
    int terminado;
	char mi_buff[10];
    int caracteres_copiar;
    printk(KERN_INFO "leer_indice. Seleccionado: %d\n", elemento_actual);
   caracteres_copiar = snprintf(mi_buff,10,"%d\n",elemento_actual);

     printk(KERN_INFO "lo q tiene el bufer en 0 %s\n",mi_buff);
    /* determinar si hemos terminado de escribir */
    if (offset > 0) {
        terminado = 0;
    } else {
        /* copiar el elemento_actual en el buffer del sistema */ 
        printk(KERN_INFO "entramos a copiar \n");
        memcpy(buffer, mi_buff, caracteres_copiar);
        terminado = caracteres_copiar;
    }
    
    return terminado;
}

/** 
 * Funcion que se llama cuando leemos del archivo /proc : "cat"
 *
 * @param buffer donde copiaremos los datos
 * @param offset posicion actual del archivo
 * @return 0 si se ha acabado el archivo; valor negativo en caso de error
 */
extern int leer_clipboard(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
    int terminado;
    struct clipstruct *seleccionado = NULL;
    printk(KERN_INFO "leer_clipboard. Seleccionado: %d\n", elemento_actual);
    
    /* determinar si hemos terminado de escribir */
    /*preguntar porque entra dos veces en el offset*/
    if (offset > 0) {
    	printk(KERN_INFO "entramos en el offset\n");
        terminado = 0;
    } else {
        /* copiar el contendido del buffer del clipboard en el buffer del sistema */   
        seleccionado = encontrar_clipboard();    
        memcpy(buffer, seleccionado->buffer, seleccionado->num_elem);
        terminado = seleccionado->num_elem;
    }
    
    return terminado;
}

/**
 * En buffer se almacena la entrada del usuario seguido de un salto de linea
 * Ejemplo: 34 => [3|4|\n|········]
 *  
 * En codigo ASCI 
 *
 * | char | codigo | 
 * |:----:|:------:|
 * | '0'  |  48    |
 * | '1'  |  49    |
 * | '2'  |  50    |
 * | '9'  |  57    |
 * | '\n' |  10    |
 *
 * @param buffer cadena de entrada
 * @param count numero de caracteres a copiar
 * @return caracteres copiados
 */
extern int modificar_indice(struct file *file, const char *buffer, unsigned long count, void *data)
{
    int nuevo_elemento = 0;
    
    /* transformar el buffer de entrada en int y comprobar q e sun numero correcto*/
	if ((nuevo_elemento = mi_atoi(buffer))== -1)
		return -EINVAL;

    /* Comprobar que nos han llamado con un id existente */
    if (nuevo_elemento < 1 || nuevo_elemento > tam){
    	printk(KERN_INFO "Numero fuera de rango de los clipboard, maximo tamaño = %d\n",tam);
    	return -EINVAL;
    }
  
    elemento_actual = nuevo_elemento;
    printk(KERN_INFO "Elemento_actual = %d\n", elemento_actual);
    
	//el numeroq pongas en ese return si es menor q el numero de count entonces vulve a llamar a la funcion con lo q le queda
	// return 4;  // tamaño de 1 byte 
    return count;
}

/**
 * Funcion que se llama cuando escribimos en /proc : "echo"
 *
 * @param buffer del sistema cadena de entrada
 * @param count numero de caracteres a copiar
 * @return caracteres copiados
 */
extern int escribir_clipboard(struct file *file, const char *buffer, unsigned long count, void *data)
{
    struct clipstruct *seleccionado;
    printk(KERN_INFO "escribir_clipboard. Seleccionado: %d\n", elemento_actual);
    
    /* encontrar el buffer en el que vamos a escribir */
    seleccionado = encontrar_clipboard();
    printk(KERN_INFO "Encontrado la entrada con id: %d\n", seleccionado->id);
    
    /* copiar en el buffer seleccionado <= buffer */
    seleccionado->num_elem = count;
    if (seleccionado->num_elem > TAM_MAX_BUFFER) {
        seleccionado->num_elem = TAM_MAX_BUFFER;
    }

    if ( copy_from_user(seleccionado->buffer, buffer, seleccionado->num_elem) ) {
        return -EFAULT;
    }
    printk(KERN_INFO "Salimos de escribir_clipboard\n");
    
    return seleccionado->num_elem;
}

struct clipstruct* encontrar_clipboard(void)
{
    struct clipstruct *tmp = NULL;
    struct list_head *pos;

    list_for_each(pos, &lista_clipboards) {
        tmp = list_entry(pos, struct clipstruct, lista);
        if (tmp->id == elemento_actual){
            break;        
        }
    }    
    
    return tmp;
}

