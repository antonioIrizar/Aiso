#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <linux/module.h>	        /* modulo */
#include <linux/kernel.h>	        /* kernel */
#include <linux/proc_fs.h>	        /* struct proc_dir_entry */
#include <linux/list.h>             /* listas del kernel */
#include <linux/vmalloc.h>          /* funcion vmalloc */
#include <asm/uaccess.h>            /* function copy_from_user */
#include <linux/moduleparam.h>      /* paso de parametros */
#include "../utiles/utiles.h"       /* funciones utiles */
#include "clipstruct.h"             /* struct clipstruct */
#include "clipthread.h"             /* definicion del thread */

/* Declaracion de macros */
#define nombre_clipboard "clipboard"
#define nombre_selector "selection"
#define nombre_periodo "periodo"
#define CAMBIO_CLIPBOARD 4
#define ESCRITURA_CLIPBOARD 5
#define TAM_MAX_BUFFER 4096

/* Funciones de carga/descagar del modulo */
int modulo_init(void);

void modulo_clean(void);

/* Funciones para manipular la lista de clipboards */
int crear_lista(void);

void liberar_lista(void);


/* Funciones de callback */
int leer_indice(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data);

int leer_clipboard(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data);

int leer_periodo(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data);

int escribir_indice(struct file *file, const char *buffer, unsigned long count, void *data);

int escribir_clipboard(struct file *file, const char *buffer, unsigned long count, void *data);

int escribir_periodo(struct file *file, const char *buffer, unsigned long count, void *data);


/* Funciones auxiliares */
struct clipstruct* encontrar_clipboard(int id);

struct clipstruct* insertar_nuevo_clipboard(int id);


#endif /* CLIPBOARD_H */

