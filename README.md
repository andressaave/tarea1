# codigo para la gestión de pacientes de un hospital  
Para ejecutar el main primero debemos compilar (en la carpeta raíz)
````
gcc tdas/*.c main.c -Wno-unused-result -o main
````
 
Y luego ejecutar:
```` 
./main
````
#### struct principales
````c

// Definición de la estructura para representar a un paciente
typedef struct {
  char nombre[50];
  int edad;
  char sintoma[50];
} Paciente;

// Definición de la estructura para representar la atención que requiere un paciente
typedef struct {
  time_t H_llegada;
  char prioridad[6];
  Paciente *paciente;
} Atencion;

````
#### funciones principales
````c
 // Funciones auxiliares para la gestión del programa

void asignar_prioridad(List *pacientes);
     //funcion que muestra la prioridad de todos los pacientes para poder ingresar el nombre del paciente y cambiar su prioridad, tiene que ser exactamente el nombre y las prioridades que se muestra en pantalla.

void registrar_paciente(List *pacientes);
     // funcion que tiene intrucciones de que datos ingresar del paciente, en caso contrario puede causar errores.

void atender_siguiente_paciente(List *pacientes);
    // funcion que toma el primero de la lista para atenderlo, se tiene que usar despues de usar la funcion mostrar_lista_pacientes para antenderlos en el orden adecuado.

void mostrar_lista_pacientes(List *pacientes, size_t talla);
    // funcion que muestra la lista ordenado de prioridad Alto a Bajo, donde usar un arreglo para poder ordenarlo con un qsort.

void mostrar_pacientes_por_prioridad(List *pacientes);
    // funcion que se le ingresa la prioridad que desea para mostrando todos los pacientes que tiene esa prioridad, se tiene que usar despues de usar la funcion mostrar_lista_pacientes para mostralos en el orden de llegada.

````
<br>
<hr>

#### funcion para poder ordenar
 para ordenar la lista se realiza un arreglo con los contedios de la lista, donde se realiza un qsort gracias a una funcion comparar_pacientes_prioridad y devolver los valores ya ordenados a la lista pacientes.
````c
 int comparar_pacientes_prioridad(const void *a, const void *b)
// contiene las condiciones de que ordene de Alto a Bajo, y por hora de llegada en el caso de que las prioridades coinciden.
   // a cada prioridad le asigna un numero para poder comparar
   // compara la hora de llegada con la funcion difftime

*-----funcion mostrar_lista_pacientes-----*
// asigna memoria al array
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
  // libera el array
  free(array);
 
````
