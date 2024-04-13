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
    // Verificar si hay pacientes en espera
    if (pacientes == NULL || pacientes->head == NULL) {
        printf("No hay pacientes en espera.\n");
        return;
    }

    int num_llegada;
    char prioridad[6];

    // Pedir al usuario el número de llegada del paciente y la prioridad deseada
    printf("Ingrese el número de llegada del paciente: ");
    scanf("%d", &num_llegada);
    printf("Ingrese la prioridad deseada (Baja, Media o Alta): ");
    scanf("%s", prioridad);

    // Buscar al paciente por su número de llegada en la lista
    struct Node *current_node = pacientes->head;
    while (current_node != NULL) {
        Atencion *atencion = (Atencion *)current_node->data;
        if (atencion->num_llegada == num_llegada) {
            // Actualizar la prioridad del paciente
            strcpy(atencion->prioridad, prioridad);
            printf("Prioridad asignada correctamente al paciente %d.\n", num_llegada);
            return;
        }
        current_node = current_node->next;
    }

    // Si no se encuentra al paciente con el número de llegada especificado
    printf("No se encontró al paciente con el número de llegada %d.\n", num_llegada);
}

void registrar_paciente(List *pacientes) {
  static int numero_llegada = 0;
  // Incrementar el contador global para el número de llegada
  numero_llegada++;

  // Asignar memoria para la estructura Atencion
  Atencion *nuevo_paciente = malloc(sizeof(Atencion));
  

  // Asignar memoria para la estructura Paciente dentro de Atencion
  nuevo_paciente->paciente = malloc(sizeof(Paciente));
  

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

  printf("Paciente registrado con éxito.\n");
}
void atender_siguiente_paciente(List *pacientes) {
  // Verificar si hay pacientes en espera
  if (pacientes == NULL || pacientes->head == NULL) {
    printf("No hay pacientes en espera para atender.\n");
    return;
  }

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

void mostrar_lista_pacientes(List *pacientes) {
  // Mostrar pacientes en la cola de espera
  printf("Pacientes en espera: \n");
  // Aquí implementarías la lógica para recorrer y mostrar los pacientes
  printf("Pacientes en espera: \n");

  if (pacientes == NULL || pacientes->head == NULL) {
      printf("No hay pacientes en espera.\n");
      return;
  }

  struct Node *current_node = pacientes->head; // Cambiado a head en lugar de current
  while (current_node != NULL) {
      Atencion *atencion = (Atencion *)current_node->data;
      Paciente *paciente = atencion->paciente;
      printf("Número de llegada: %d\n", atencion->num_llegada);
      printf("Nombre: %s\n", paciente->nombre);
      printf("Edad: %d\n", paciente->edad);
      printf("Síntoma: %s\n", paciente->sintoma);
      printf("Prioridad: %s\n", atencion->prioridad);
      printf("------------------------------------\n");

      current_node = current_node->next;
  }

  printf("Fin de la lista de pacientes.\n");
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

// Función para mostrar pacientes ordenados por prioridad y número de llegada
void mostrar_pacientes_por_prioridad(List *pacientes) {
    // Crear una nueva lista para almacenar los pacientes ordenados por prioridad y número de llegada
    List *pacientes_ordenados = list_create();

    // Iterar sobre la lista de pacientes y agregarlos ordenadamente a la lista ordenada
    struct Node *current_node = pacientes->head;
    while (current_node != NULL) {
        Atencion *atencion = (Atencion *)current_node->data;
        // Insertar el paciente actual en la lista ordenada
        list_sortedInsert(pacientes_ordenados, atencion, comparar_prioridad);
        current_node = current_node->next;
    }

    // Mostrar la lista ordenada de pacientes
    printf("Pacientes ordenados por prioridad y número de llegada:\n");
    current_node = list_first(pacientes_ordenados);
    while (current_node != NULL) {
        Atencion *atencion = (Atencion *)current_node->data;
        Paciente *paciente = atencion->paciente;
        printf("Número de llegada: %d\n", atencion->num_llegada);
        printf("Nombre: %s\n", paciente->nombre);
        printf("Edad: %d\n", paciente->edad);
        printf("Síntoma: %s\n", paciente->sintoma);
        printf("Prioridad: %s\n", atencion->prioridad);
        printf("------------------------------------\n");
        current_node = list_next(pacientes_ordenados);
    }

    // Liberar la memoria utilizada por la lista de pacientes ordenados
    list_clean(pacientes_ordenados);
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
      registrar_paciente(pacientes);
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
