#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "hashmap.h"
#include "list.h"


typedef struct libro{
int id;
char *titulo;
int numPalabras;
int numCaracteres;
HashMap* frecuencias;
}libro;

char *lista_bloqueo[] = {"to", "at", "before", "under", "fits", "with", "against", "of", "from", "during", "in", "between", "towards", "until", "through", "for", "by", "according", "without", "so", "about", "after", "versus", "via", "and", "the", "none", "what", "him", "do", "you", "they", "as", "he", "ones", "on", "lo", "not", "it", "have", "i", "that", "a", "be"};

int esBloqueada(char *palabra) {
    int n = sizeof(lista_bloqueo) / sizeof(lista_bloqueo[0]); 
    for (int i = 0; i < n; i++) { 
        if (strcmp(palabra, lista_bloqueo[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char *strlwr(char *str){
  char *p = str; 
  while (*p != '\0') { 
    *p = tolower(*p); 
    p++; 
  }
  return str; 
}

libro* crearLibro(){
  libro *libros = (libro*)malloc(sizeof(libro));
  libros->id = 0;
  libros->titulo=NULL;
  libros->numPalabras=0;
  libros->numCaracteres=0;
  libros->frecuencias=createMap(1024);
  return libros;
}

libro* cargarLibros(char* nombreArchivo){
  FILE *archivo = fopen(nombreArchivo, "r");
  if(archivo == NULL){
    return NULL;
  }
  libro* libros = crearLibro();
  char linea[1024];
  int lineaNum=0;

  fseek(archivo, 0, SEEK_SET);

  while(fgets(linea, sizeof(linea), archivo) != NULL){
    lineaNum++;
    if(lineaNum==1){
      libros->titulo = strdup(linea);
    }else{
      char *palabra = strtok(linea, " \n\t.,;:?!\"\'()[]{}");
      while(palabra != NULL){
        palabra = strlwr(palabra);
        if(!esBloqueada(palabra)){
          libros->numPalabras++;
          libros->numCaracteres+=strlen(palabra);
          Pair* frecuencia = searchMap(libros->frecuencias, palabra);
          if(frecuencia == NULL){
            frecuencia = malloc(sizeof(Pair));
            frecuencia->key = strdup(palabra);
            frecuencia->value = malloc(sizeof(int));
            *(int *) frecuencia->value = 1;
            insertMap(libros->frecuencias, palabra, frecuencia);
          }else{
            int valor = *(int *) frecuencia->value;
            *(int *) frecuencia->value = valor + 1;
          }
        }
        palabra = strtok(NULL, " \n\t.,;:?!\"\'()[]{}");
      }
    }
  }
  fclose(archivo);
  return libros;
}

List* buscar_libro_por_titulo(List* libros, char* titulo){
  List* librosEncontrados = createList();
  for (int i = 0; i < sizeList(libros); i++) {
    libro* libros = list_get(libros, i);
    if (strcasecmp(libros->titulo, titulo) == 0) {
      pushBack(librosEncontrados, libros);
    }
  }
  return librosEncontrados;
}


int main(void) {
  List* libros = createList();
  int opcion = 0;
  do {
        switch (opcion) {
      case 1: {
        char nombreArchivo[256];
        printf("Ingrese el nombre del archivo a cargar:\n");
        scanf("%s", nombreArchivo);
        List* nuevosLibros = cargarLibros(nombreArchivo);
        pushBack(libros, nuevosLibros);
        printf("Se han cargado %d libros.\n", sizeList(nuevosLibros));
        break;
      }
      case 2: {
        mostrar_libros_ordenados(libros);
        break;
      }
      case 3: {
        char titulo[256];
        printf("Ingrese el título del libro a buscar:\n");
        scanf("%s", titulo);
        List* librosEncontrados = buscar_libro_por_titulo(libros, titulo);
        printf("Se han encontrado %d libros con el título ingresado.\n", list_size(librosEncontrados));
        for (int i = 0; i < list_size(librosEncontrados); i++) {
          libro* libros = list_get(librosEncontrados, i);
          printf("%d. %s\n", libro->id, libro->titulo);
        }
        break;
      }
      case 4: {
        mostrar_palabras_con_mayor_frecuencia(libros);
        break;
      }
      case 5: {
        mostrar_palabras_mas_relevantes(libros);
        break;
      }
      case 6: {
        char palabra[256];
        printf("Ingrese la palabra a buscar:\n");
        scanf("%s", palabra);
        mostrar_resultados_busqueda(libros, palabra);
        break;
      }
      case 7: {
        char palabra[256];
        printf("Ingrese la palabra a buscar:\n");
        scanf("%s", palabra);
        mostrar_contexto(libros, palabra);
        break;
      }
      case 0: {
        printf("Gracias por usar la aplicación.\n");
        break;
      }
      default: {
        printf("Opción no válida.\n");
        break;
      }
    }
  } while (opcion != 0);

  return 0;
}