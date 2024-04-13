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
  printf("Prioridad del paciente: ");
  scanf(" %5s", nuevo_paciente->prioridad); // Leer la prioridad del paciente
  
  if (*(nuevo_paciente->prioridad) == '\n') {
    strcpy(nuevo_paciente->prioridad, "Baja");
  }

  // Agregar el nuevo paciente a la lista de espera
  list_pushBack(pacientes, nuevo_paciente);

  printf("Paciente registrado con éxito.\n");
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
      break;
    case '3':
      mostrar_lista_pacientes(pacientes);
      break;
    case '4':
      // Lógica para atender al siguiente paciente
      break;
    case '5':
      // Lógica para mostrar pacientes por prioridad
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
