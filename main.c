#include "tdas/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  char nombre[50];
  int edad;
  char sintoma[50];
} Paciente;

typedef struct {
  time_t H_llegada;
  char prioridad[6];
  Paciente *paciente;
} Atencion;


// Función para limpiar la pantalla
void limpiarPantalla() { system("clear"); }

void presioneTeclaParaContinuar() {
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Sistema de Gestión Hospitalaria");
  puts("========================================");

  puts("1) Registrar paciente");
  puts("2) Asignar prioridad a paciente");
  puts("3) Mostrar lista de espera");
  puts("4) Atender al siguiente paciente");
  puts("5) Mostrar pacientes por prioridad");
  puts("6) Salir");
}
void asignar_prioridad(List *pacientes) {
    
    char nombres[50];
    char prioridad[6];

    // Pedir al usuario el número de llegada del paciente y la prioridad deseada
    printf("Ingrese nombre del paciente ");
    scanf("%s", nombres);
    printf("Ingrese la prioridad deseada (Bajo, Medio o Alto): ");
    scanf("%s", prioridad);

    // Buscar al paciente por su número de llegada en la lista
    void *current_node = list_first(pacientes);
    while (current_node != NULL) {
        Atencion *atencion = (Atencion *)current_node;
        if (strcmp(atencion->paciente->nombre, nombres) == 0) {
            // Actualizar la prioridad del paciente
            strcpy(atencion->prioridad, prioridad);
            printf("Prioridad asignada correctamente al paciente %s.\n", nombres);
            return;
        }
        current_node = list_next(pacientes);
    }

    // Si no se encuentra al paciente con el número de llegada especificado
    printf("No se encontró al paciente con el nombre %s.\n", nombres);
}

void registrar_paciente(List *pacientes) {
  
  // Asignar memoria para la nueva estructura de paciente
  Atencion *nuevo_paciente = (Atencion *)malloc(sizeof(Atencion));
  nuevo_paciente->paciente = (Paciente *)malloc(sizeof(Paciente));
  // Asignar el número de llegada al nuevo paciente
  time(&nuevo_paciente->H_llegada);
  printf("Registrar nuevo paciente\n");
  printf("Nombre del paciente: ");
  scanf("%49s", nuevo_paciente->paciente->nombre); // Limitar la entrada a 49 caracteres
  printf("Edad del paciente: ");
  scanf("%d", &nuevo_paciente->paciente->edad); // Leer la edad del paciente
  printf("Síntoma del paciente: ");
  scanf("%49s", nuevo_paciente->paciente->sintoma); // Limitar la entrada a 49 caracteres
  // Agregar el nuevo paciente a la lista de espera
  list_pushFront(pacientes, nuevo_paciente);
  // Asignar nivel de prioridad inicial "Bajo" al paciente
  
  strcpy(nuevo_paciente->prioridad, "Bajo");
  printf("Paciente registrado con éxito.\n");
}
void atender_siguiente_paciente(List *pacientes) {
  
  // Obtener el primer paciente de la lista
  Atencion *paciente_a_atender = list_popFront(pacientes);

  // Convertir el tiempo de llegada a una estructura tm
  struct tm *tm_info = localtime(&paciente_a_atender->H_llegada);

  // Imprimir la hora del paciente atendido
  printf("Atendiendo al siguiente paciente:\n");
  printf("Hora de llegada: %02d:%02d\n", tm_info->tm_hour, tm_info->tm_min);

  // Imprimir el resto de la información del paciente atendido
  printf("Nombre: %s\n", paciente_a_atender->paciente->nombre);
  printf("Edad: %d\n", paciente_a_atender->paciente->edad);
  printf("Síntoma: %s\n", paciente_a_atender->paciente->sintoma);
  printf("Prioridad: %s\n", paciente_a_atender->prioridad);

  // Liberar la memoria asignada al paciente atendido
  free(paciente_a_atender->paciente);
  free(paciente_a_atender);

  printf("Paciente atendido con éxito.\n");
}

int comparar_pacientes_prioridad(const void *a, const void *b) {
    const Atencion *atencion1 = *(const Atencion **)a;
    const Atencion *atencion2 = *(const Atencion **)b;

    // Asignar valores numéricos a las prioridades
    int prioridad1, prioridad2;
    if (strcmp(atencion1->prioridad, "Alto") == 0) {
        prioridad1 = 3;
    } else if (strcmp(atencion1->prioridad, "Medio") == 0) {
        prioridad1 = 2;
    } else {
        prioridad1 = 1; // Si no es Alta ni Media, asumimos que es Baja
    }
    if (strcmp(atencion2->prioridad, "Alto") == 0) {
        prioridad2 = 3;
    } else if (strcmp(atencion2->prioridad, "Medio") == 0) {
        prioridad2 = 2;
    } else {
        prioridad2 = 1; // Si no es Alta ni Media, asumimos que es Baja
    }

    // Comparar las prioridades numéricas
    if (prioridad1 > prioridad2) {
        return -1;
    } else if (prioridad1 < prioridad2) {
        return 1;
    } else {
      // Si las prioridades son iguales, comparar por tiempo de llegada
      if (difftime(atencion1->H_llegada, atencion2->H_llegada) < 0) {
          return -1; // atencion1 llegó antes que atencion2
      } else if (difftime(atencion1->H_llegada, atencion2->H_llegada) > 0) {
          return 1; // atencion2 llegó antes que atencion1
      } else {
          return 0; // Ambos tienen la misma prioridad y tiempo de llegada
      }
  }
}


void mostrar_lista_pacientes(List *pacientes,size_t talla) {

  int size = talla;
  Atencion **array = (Atencion **)malloc(size * sizeof(Atencion *));
  if (array == NULL) {
      printf("Error: No se pudo asignar memoria para el arreglo.\n");
      return;
  }

  // Llenar el arreglo con los elementos de la lista
  int index = 0;
  void *current;
  for (current = list_first(pacientes); current != NULL; current = list_next(pacientes)) {
      array[index++] = (Atencion *)current;
      if (index >= size) {
          size *= 2; // Duplicar el tamaño del arreglo si se llena
          array = (Atencion **)realloc(array, size * sizeof(Atencion *));
          if (array == NULL) {
              printf("Error: No se pudo asignar memoria para el arreglo.\n");
              return;
          }
      }
  }

  // Ordenar el arreglo utilizando qsort
  qsort(array, index, sizeof(Atencion *), comparar_pacientes_prioridad);

  // Limpiar la lista para insertar los elementos ordenados
  list_clean(pacientes);

  // Insertar los elementos ordenados de vuelta en la lista
  for (int i = 0; i < index; i++) {
      list_pushBack(pacientes, array[i]);
  }
  free(array);
  
  // Mostrar la lista ordenada de pacientes
  printf("Pacientes ordenados por prioridad y tiempo de llegada:\n");
  void *curr_node_ordenado = list_first(pacientes);
  while (curr_node_ordenado != NULL) {
      Atencion *atencion = (Atencion *)curr_node_ordenado;
      Paciente *paciente = atencion->paciente;
      // Mostrar la hora de llegada
      struct tm *tm_info = localtime(&atencion->H_llegada);
      printf("Hora de llegada: %02d:%02d\n", tm_info->tm_hour, tm_info->tm_min); // Muestra correctamente la hora y los minutos
      printf("Nombre: %s\n", paciente->nombre);
      printf("Edad: %d\n", paciente->edad);
      printf("Síntoma: %s\n", paciente->sintoma);
      printf("Prioridad: %s\n", atencion->prioridad);
      printf("------------------------------------\n");
      curr_node_ordenado = list_next(pacientes);
  }
  
}

// Función para mostrar pacientes ordenados por prioridad y número de llegada
void mostrar_pacientes_por_prioridad(List *pacientes) {
  char priori[6];
  void *current = list_first(pacientes);

  printf("Ingrese la prioridad deseada (Bajo, Medio o Alto):");
  scanf("%s", priori);

  while(current != NULL)
    {
      Atencion *atencion = (Atencion *)current;
      Paciente *paciente = atencion->paciente;
      if(strcmp(atencion->prioridad, priori) == 0){
        printf("Nombre: %s\n", paciente->nombre);
        printf("Edad: %d\n", paciente->edad);
        printf("Síntoma: %s\n", paciente->sintoma);
        printf("------------------------------------\n");
      }
      current = list_next(pacientes);
    
    }
}

int main() {
  char opcion;
  size_t talla = 0;
  
  List *pacientes =
      list_create(); // puedes usar una lista para gestionar los pacientes

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion); // Nota el espacio antes de %c para consumir el
                           // newline anterior

    switch (opcion) {
    case '1':
      registrar_paciente(pacientes);
      talla++;
      break;
    case '2':
      // Lógica para asignar prioridad
      asignar_prioridad(pacientes);
      break;
    case '3':
      mostrar_lista_pacientes(pacientes,talla);
      break;
    case '4':
      atender_siguiente_paciente(pacientes);
      // Lógica para atender al siguiente paciente
      break;
    case '5':
      // Lógica para mostrar pacientes por prioridad
      mostrar_pacientes_por_prioridad(pacientes);
      break;
    case '6':
      puts("Saliendo del sistema de gestión hospitalaria...");
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '6');

  // Liberar recursos, si es necesario
  list_clean(pacientes);

  return 0;
}
