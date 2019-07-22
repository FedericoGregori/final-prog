#define _GNU_SOURCE /* Needed to use fcloseall() */
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int carrera;
    int asignatura;
    int ano;
    int legajo;
} inscripciones;

typedef struct {
    int carrera;
    int asignatura;
    int correlativa;
} correlativas;

typedef struct {
    int alumno;
    int carrera;
    int fecha;
    int asignatura;
    int nota;
} notas;

void agregarInscripcion();
void agregarCorrelativa();
void agregarNota();
void procesarInscripciones();

/* Realizar un programa que guarde en un nuevo archivo las inscripciones de los
 * alumnos que están en condiciones de rendir de tener aprobada la correlativa
 * (o por no tener correlativa)  */

int main(void) {
    int opc = -1;

    while (opc != 0) {
        printf("\nIngrese la opción deseada: ");
        printf("\n1- Ingresar inscripciones.");
        printf("\n2- Ingresar correlativas.");
        printf("\n3- Ingresar notas.");
        printf("\n4- Procesar inscripciones finales.\n");

        printf("\n0- Salir.\n");

        printf("Ingrese elección: ");
        scanf("%d", &opc);

        switch (opc) {
        case 1:
            agregarInscripcion();
            break;
        case 2:
            agregarCorrelativa();
            break;
        case 3:
            agregarNota();
            break;
        case 4:
            procesarInscripciones();
            break;
        default:
            break;
        }
    }
}

void agregarInscripcion() {
    inscripciones inscripcion;
    FILE *fp;

    if ((fp = fopen("inscripciones.bin", "ab+")) != NULL) {
        printf("\n\tMenú ingreso de Inscripción:");
        printf("\nIngrese carrera: ");
        scanf("%d", &inscripcion.carrera);
        printf("\nIngrese asignatura: ");
        scanf("%d", &inscripcion.asignatura);
        printf("\nIngrese año: ");
        scanf("%d", &inscripcion.ano);
        printf("\nIngrese legajo: ");
        scanf("%d", &inscripcion.legajo);
        fwrite(&inscripcion, sizeof(inscripciones), 1, fp);
        fclose(fp);
        fp = NULL;
    } else {
        printf("\nError al procesar el archivo \"inscripciones.bin\"");
    }
}

void agregarCorrelativa() {
    correlativas correlativa;
    FILE *fp;

    if ((fp = fopen("correlativas.bin", "ab+")) != NULL) {
        printf("\n\tMenú ingreso de Correlativa:");
        printf("\nIngrese carrera: ");
        scanf("%d", &correlativa.carrera);
        printf("\nIngrese asignatura: ");
        scanf("%d", &correlativa.asignatura);
        printf("\nIngrese correlativa: ");
        scanf("%d", &correlativa.correlativa);
        fwrite(&correlativa, sizeof(correlativas), 1, fp);
        fclose(fp);
        fp = NULL;
    } else {
        printf("\nError al procesar el archivo \"correlativas.bin\"");
    }
}

void agregarNota() {
    notas nota;
    FILE *fp;

    if ((fp = fopen("notas.bin", "ab+")) != NULL) {
        printf("\n\tMenú ingreso de Notas:");
        printf("\nIngrese alumno: ");
        scanf("%d", &nota.alumno);
        printf("\nIngrese carrera: ");
        scanf("%d", &nota.carrera);
        printf("\nIngrese fecha: ");
        scanf("%d", &nota.fecha);
        printf("\nIngrese asignatura: ");
        scanf("%d", &nota.asignatura);
        printf("\nIngrese nota: ");
        scanf("%d", &nota.nota);
        fwrite(&nota, sizeof(notas), 1, fp);
        fclose(fp);
        fp = NULL;
    } else {
        printf("\nError al procesar el archivo \"notas.bin\"");
    }
}

void procesarInscripciones() {
    inscripciones i;
    correlativas c;
    notas n;

    FILE *ip, *cp, *np, *idefp;

    if ((ip = fopen("inscripciones.bin", "rb")) != NULL) {
        if ((cp = fopen("correlativas.bin", "rb")) != NULL) {
            if ((np = fopen("notas.bin", "rb")) != NULL) {
                if ((idefp = fopen("inscripcionesDefinitivas.bin", "ab+")) !=
                    NULL) {
                    fread(&i, sizeof(inscripciones), 1, ip);
                    fread(&c, sizeof(correlativas), 1, cp);
                    fread(&n, sizeof(notas), 1, np);
                    while (!feof(ip)) {
                        if ((i.asignatura == c.asignatura) &&
                            (i.carrera == c.carrera)) {

                            if ((c.correlativa == 0)) {
                                fwrite(&i, sizeof(inscripciones), 1, idefp);
                            } else {
                                while (!feof(cp)) {
                                    while (!feof(np)) {
                                        if (((n.alumno == i.legajo) &&
                                             (c.correlativa == n.asignatura)) &&
                                            (n.nota >= 4)) {
                                            // Seguir mirando la lista de
                                            // correlativas, e inscribir en caso
                                            // de haber recorrido toda la lista
                                            // y que la nota rendida de las
                                            // mismas sea >= 4.
                                            fwrite(&i, sizeof(inscripciones), 1,
                                                   idefp);
                                        }
                                        fread(&n, sizeof(notas), 1, np);
                                    }
                                    rewind(np);
                                    fread(&c, sizeof(correlativas), 1, cp);
                                }
                                rewind(cp);
                            }
                        }
                    }
                    fread(&i, sizeof(inscripciones), 1, ip);
                }
            }
        }
    }
    /* exit(-1); */
    /* exit()[stdlib.h] closes and flushes all stdio streams */
    fcloseall();
}