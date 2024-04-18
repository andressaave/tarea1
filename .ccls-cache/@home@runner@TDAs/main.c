#include "tdas/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char nombre[50];
  int edad;
  char sintoma[50];
} Paciente;

typedef struct {
  int num_llegada;
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
    printf("Ingrese la prioridad deseada (Baja, Media o Alta): ");
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
        current_node = list_next(current_node);
    }

    // Si no se encuentra al paciente con el número de llegada especificado
    printf("No se encontró al paciente con el nombre %s.\n", nombres);
}

void registrar_paciente(List *pacientes) {
  static int numero_llegada = 0;
  // Incrementar el contador global para el número de llegada
  numero_llegada++;

  // Asignar memoria para la estructura Atencion
  Atencion *nuevo_paciente =(Atencion*)malloc(sizeof(Atencion));
  nuevo_paciente->paciente =(Paciente*)malloc(sizeof(Paciente));
  

  // Asignar el número de llegada al nuevo paciente
  nuevo_paciente->num_llegada = numero_llegada;

  printf("Registra nuevo paciente\n");
  printf("Nombre del paciente: ");
  scanf("%49s", nuevo_paciente->paciente->nombre); // Limitar la entrada a 49 caracteres
  printf("Edad del paciente: ");
  scanf("%d", &nuevo_paciente->paciente->edad); // Leer la edad del paciente
  printf("Síntoma del paciente: ");
  scanf("%49s", nuevo_paciente->paciente->sintoma); // Limitar la entrada a 49 caracteres

  // Agregar el nuevo paciente a la lista de espera
  list_pushBack(pacientes, nuevo_paciente);

  strcpy(nuevo_paciente->prioridad,"baja");

  printf("Paciente registrado con éxito.\n");
}
void atender_siguiente_paciente(List *pacientes) {
  
  // Obtener el primer paciente de la lista
  Atencion *paciente_a_atender = list_popFront(pacientes);

  // Mostrar información del paciente atendido
  printf("Atendiendo al siguiente paciente:\n");
  printf("Número de llegada: %d\n", paciente_a_atender->num_llegada);
  printf("Nombre: %s\n", paciente_a_atender->paciente->nombre);
  printf("Edad: %d\n", paciente_a_atender->paciente->edad);
  printf("Síntoma: %s\n", paciente_a_atender->paciente->sintoma);
  printf("Prioridad: %s\n", paciente_a_atender->prioridad);

  // Liberar la memoria asignada al paciente atendido
  free(paciente_a_atender->paciente);
  free(paciente_a_atender);

  printf("Paciente atendido con éxito.\n");
}

int comparar_prioridad(void *data1, void *data2) {
    Atencion *atencion1 = (Atencion *)data1;
    Atencion *atencion2 = (Atencion *)data2;

    // Primero comparamos las prioridades
    int comparacion_prioridad = strcmp(atencion1->prioridad, atencion2->prioridad);
    if (comparacion_prioridad != 0) {
        return comparacion_prioridad;
    }

    // Si las prioridades son iguales, comparamos los números de llegada
    if (atencion1->num_llegada < atencion2->num_llegada) {
        return -1;
    } else if (atencion1->num_llegada > atencion2->num_llegada) {
        return 1;
    } else {
        return 0;
    }
}

void mostrar_lista_pacientes(List *pacientes) {
  // Crear una nueva lista temporal para almacenar los pacientes ordenados
  List *pacientes_ordenados = list_create();

  // Iterar sobre la lista de pacientes y agregarlos ordenadamente a la lista temporal
  void *current_node = list_first(pacientes);
  while (current_node != NULL) {
      Atencion *atencion = (Atencion *)current_node;
      // Insertar el paciente actual en la lista ordenada temporal
      list_sortedInsert(pacientes_ordenados, atencion, comparar_prioridad);
      current_node = list_next(current_node);
  }

  // Copiar los pacientes ordenados de la lista temporal a la lista original
  list_clean(pacientes); // Limpiar la lista original para evitar duplicados
  current_node = list_first(pacientes_ordenados);
  while (current_node != NULL) {
      Atencion *atencion = (Atencion *)current_node;
      list_pushBack(pacientes, atencion); // Insertar el paciente en la lista original
      current_node = list_next(current_node);
  }

  // Mostrar la lista ordenada de pacientes
  printf("Pacientes ordenados por prioridad:\n");
  current_node = list_first(pacientes);
  while (current_node != NULL) {
      Atencion *atencion = (Atencion *)current_node;
      Paciente *paciente = atencion->paciente;

      printf("Nombre: %s\n", paciente->nombre);
      printf("Edad: %d\n", paciente->edad);
      printf("Síntoma: %s\n", paciente->sintoma);
      printf("Prioridad: %s\n", atencion->prioridad);
      printf("------------------------------------\n");
      current_node = list_next(current_node);
  }

  // Liberar la memoria utilizada por la lista temporal de pacientes ordenados
  list_clean(pacientes_ordenados);
  free(pacientes_ordenados);
}

// Función para mostrar pacientes ordenados por prioridad y número de llegada
void mostrar_pacientes_por_prioridad(List *pacientes) {
  char priori[6];
  void *current = list_first(pacientes);

  printf("Ingrese la prioridad deseada (Baja, Media o Alta):");
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
      current = list_next(current);
    
    }
}

int main() {
  char opcion;
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
      break;
    case '2':
      // Lógica para asignar prioridad
      asignar_prioridad(pacientes);
      break;
    case '3':
      mostrar_lista_pacientes(pacientes);
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
