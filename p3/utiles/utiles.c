#include "utiles.h"



// -------------------------------------------------
// Funciones para crear - destruir entradas en /proc
// -------------------------------------------------

/**
 * Crea el directorio especificado que colgara de /proc
 * @return (struct proc_dir_entry *)
 */
struct proc_dir_entry * crear_directorio(const char* nombre_directorio)
{
	return crear_sub_directorio(nombre_directorio, NULL);
}

/**
 *
*/
struct proc_dir_entry * crear_sub_directorio(const char* nombre_directorio, struct proc_dir_entry * directorio_padre)
{
	struct proc_dir_entry * directorio;
	
	if (directorio_padre== NULL)
		printk(KERN_INFO "el directorio padre es null para el dir %s por lo que se creara en la raiz /proc", nombre_directorio);
   	directorio = proc_mkdir(nombre_directorio, directorio_padre); 	
    
    // comprobacion de errores
	if (directorio == NULL) {
		remove_proc_entry(nombre_directorio, directorio_padre);
		printk(KERN_ALERT "Error: No se pudo crear el directorio /%s\n", nombre_directorio);
		return NULL;
	}

   // printk(KERN_INFO "Creado el directorio /proc/%s/%s \n", nombre_directorio);
    return directorio;
}

/**
 * Crea la entrada especificada en el directorio /proc/directorio especificado
 *
 * @return int exito
 */
int crear_entrada(const char * nombre_entrada, struct proc_dir_entry *directorio,
				   int (*leer) (char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data),
				   int (*escribir) (struct file *file, const char *buffer, unsigned long count, void *data) )
{
    struct proc_dir_entry * nueva_entrada;
    
    /* creamos la entrada principal */
    // Permisos de lectura y escritura para todos es decir 0666
    nueva_entrada = create_proc_entry(nombre_entrada,  S_IFREG | S_IRUGO | S_IWOTH | S_IWGRP | S_IWUSR, directorio);
    
    /* Rellenar la estructura */
    nueva_entrada->read_proc = leer;
    nueva_entrada->write_proc = escribir;
   // no hace falta ya asigno los permisos al crear la entrada
    //nueva_entrada->mode = S_IFREG | S_IRUGO | S_IWOTH;
    nueva_entrada->uid = 0; // id usuario 
    nueva_entrada->gid = 0; // id grupo
   
    return 0;
}

/** 
 *
 */
inline void eliminar_entrada(char * entrada){ 
	eliminar_sub_entrada(entrada, NULL); 
}

inline void eliminar_sub_entrada(char * entrada, struct proc_dir_entry * parent)
{
    remove_proc_entry(entrada, parent);
    printk(KERN_INFO "Eliminada la entrada %s", entrada);
}

// -------------------------------------------------
// Funciones auxiliares
// -------------------------------------------------

int mi_atoi(const char* p)
{
    int numero_leido;
    int i = 0; 
    int numero = 0;
    
    while (1) {
      numero_leido = (int)p[i];
        
      if (numero_leido == 10) {
        return numero;
      } 
      
      if ( (numero_leido< 48) || (numero_leido > 57) ) {
        printk(KERN_INFO "No introduciste un numero%s\n",p);
        return -1;
      }
      
      numero_leido = numero_leido - 48;
      
      if (i == 0) {
        numero = numero_leido;
      } else {
        numero = numero * 10 + numero_leido;
      }
      
      i++;
    }
}

int foo (char c){
    int i = (int) c;
    if (i >= 48 && i <=57) return 1;
    if (i>= 65 && i<=90) return 1;
    if (i >=97 && i<=122) return 1;
    return 0;
}

