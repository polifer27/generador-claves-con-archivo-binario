#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 300
#define TAM 80
#define NUM_CARACTERES 8

void inic_lista(void);
int menu(void);
int intro(void);
int busca_libre(void);
char *generate_key(int max);
void borrar(void);
void listar(void);
int guardar(void);
void cargar(void);
void salir(void);


//Declara un arreglo de estructuras
struct dir {
    char aplicacion[30];
    char usuario[40];
    char clave[20];
}lista_dir[MAX];

// Variable global que evita guardar datos antes de cargar
char condicion = -1;

int main()
{
    char opcion;
    inic_lista(); // inicializa el array de estructuras

    for(;;){
       opcion = menu();
       switch(opcion){
           case 1:
               intro();
               break;
           case 2:
               borrar();
               break;
           case 3:
               listar();
               break;
           case 4:
               guardar();
               break;
           case 5:
               cargar();
               break;
           case 6:
               salir();
               break;
           default:
               printf("Introduzca una opción valida\n");
        }
    }
    return 0;
}

/** Inicializar la lista **/
void inic_lista(void)
{
    register int t;

    for(t = 0; t < MAX; ++t)
        lista_dir[t].aplicacion[0] = '\0';
}

/** Seleccionar una operacion **/
int menu(void)
{
    char s[80];
    int c;

    printf("1. Introducir aplicacion y usuario.\n");
    printf("2. Borrar un registro.\n");
    printf("3. Ver datos guardados o antes de guardar.\n");
    printf("4. Guardar el archivo.\n");
    printf("5. Cargar el archivo para ver datos guardados.\n");
    printf("6. Salir\n");

    do{
        printf("\nIntroduce tu opcion: ");

        fgets(s, sizeof s, stdin);
        c = atoi(s);
    }while(c < 0 || c > 6);

    return c;
}

/** Introduce direcciones en la lista **/
int intro(void)
{
    int sitio;
    char *p;

    if(condicion == -1){
        printf("**************************************\n");
        printf("* Primero se debe cargar el archivo. *\n");
        printf("**************************************\n");
        return 0;
    }
    sitio = busca_libre();
    if(sitio == -1){
        printf("\nLista llena");
        return 0;
    }

    printf("Introduce el nombre de la aplicacion: ");
    fgets(lista_dir[sitio].aplicacion, sizeof lista_dir[sitio].aplicacion, stdin);

    printf("Introduce el usuario: ");
    fgets(lista_dir[sitio].usuario, sizeof lista_dir[sitio].usuario, stdin);

    p = generate_key(NUM_CARACTERES);

    for(int i = 0; *p != '\0'; i++, p++){
        lista_dir[sitio].clave[i] = *p;
    }
    printf(" **********************************************************\n");
    printf(" * Oprima guardar en caso contrario se pierden los datos. *\n");
    printf(" **********************************************************\n");

    return 0;
}

/** Buscar una estructura no usada **/
int busca_libre(void)
{
    register int t;

    // lista_dir[t].nombre[0] es verdadero mientras esta lleno
    // entonces a la primera ocurrencia vacio sale del for
    for(t = 0; lista_dir[t].aplicacion[0] && t < MAX; ++t)
         ;

    if(t == MAX)
        return -1;

    return t;
}

/** Genera una clave al azar **/
char *generate_key(int max)
{

    int stime;
    long ltime;
    int i;
    // diccionario de caracteres
    char diccionario[TAM] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
        'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', ' ',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
        'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
        'W', 'X', 'Y', 'Z',
        '!', '?','#', '@', '%', '&', '*', '-', '{', '}', '[', ']',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };

    ltime = time(NULL);
    stime = (unsigned)ltime / 2;

    srand(stime); // genero una semilla para poder tener un numero random
    char *key = malloc(sizeof(char)+max); // le doy la cantidad de memoria a la variable que guardara la clave
    int len = (int)strlen(diccionario); // obtengo la longitud de caracteres del diccionario

    for(i = 0; i < max; i++){
        key[i] = diccionario[rand() % len]; // se genera la clave aleatoria
    }

    return key;
}

/** Eliminar una direccion **/
void borrar(void)
{
    register int sitio;
    char s[1000];

    printf("Introduce el numero de registro: ");
    fgets(s, sizeof s, stdin);
    sitio = atoi(s);
    sitio -= 1;

    if(sitio >= 0 && sitio < MAX)
        lista_dir[sitio].aplicacion[0] = '\0';
    else printf("Introduzca un numero de registro valido.\n");
}

/** Mostrar la lista en la pantalla **/
void listar(void)
{
    register int t;
    int j;
    system("clear");
    for(t = 0; t < MAX; ++t){
        j = t;
        if(lista_dir[t].aplicacion[0]){
            // Al mostrarlo empieza desde el registro 1 pero
            // internamente sigue siendo 0 el primer registro
            printf("      Registro número %d\n", j+1);
            printf("      Aplicacion: %s\n", lista_dir[t].aplicacion);
            printf("      Usuario: %s\n", lista_dir[t].usuario);
            printf("      Contraseña: %s\n", lista_dir[t].clave);
            printf("**********************************\n");
        }
    }
    printf("\n");
}

/** salvar la lista **/
int guardar(void)
{
    FILE *fp;
    register int i;
    if(condicion == -1){
        printf("**************************************\n");
        printf("* Primero se debe cargar el archivo. *\n");
        printf("**************************************\n");
        return 0;
    }
    if((fp = fopen("registros", "wb")) == NULL){
        printf("No se puede abrir el archivo.\n");
        return 0;
    }
    for(i = 0; i < MAX; i++)
        if(*lista_dir[i].aplicacion)
            if(fwrite(&lista_dir[i], sizeof(struct dir), 1, fp) != 1)
                printf("Error de escritura en el archivo.\n");
    fclose(fp);
    return 0;
}

/** cargar el archivo **/
void cargar(void)
{
    FILE *fp;
    register int i;
    int j;
    system("clear");
    //Abre o crea si no existe el archivo binario con nombre registros
    if((fp = fopen("registros", "a+b")) == NULL){
        printf("No se puede abrir el archivo.\n");
        return;
    }
    inic_lista();
    for(i = 0; i < MAX; i++){
        j = i;
        if(fread(&lista_dir[i], sizeof(struct dir), 1, fp) != 1){
        //printf("Error en la lectura del archivo.\n");
        ;
        }
        if(lista_dir[i].aplicacion[0]){
            // j+1 lo mismo que listar lo deja mas prolijo
            // para el usuario pero el primer registro internamente sigue siendo el cero
            printf("      Registro número %d\n", j+1);
            printf("      Aplicacion: %s\n", lista_dir[i].aplicacion);
            printf("      Usuario: %s\n", lista_dir[i].usuario);
            printf("      Contraseña: %s\n", lista_dir[i].clave);
            printf("**********************************\n");
        }
    }
    condicion = 0;
    fclose(fp);
}

void salir(void)
{
    exit(0);
}


