#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>

int main(int argc, char** argv) {
    double *datos;
    int fd;
    size_t filesize;
    /* Si no indicamos el archivo a usar, fallamos la ejecución */
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo>\n", argv[0]);
        return 1;
    }
    /* Tratamos de crear el archivo nuevo */
    fd = open(argv[1], O_RDWR | O_CREAT | O_EXCL, S_IRWXU);
    /* Si falla la creación tratamos de abrirlo */
    if (fd == -1) {
        struct stat st;
        /* Abrimos el archivo previo */
        fd = open(argv[1], O_RDWR);
        assert(fd != -1);
    }
    /* Reservamos la memoria que deseemos */
    filesize = 12000 * sizeof(double);
    /* Creamos el archivo al tamaño deseado */
    ftruncate(fd, filesize);
    /* Mapeamos el contenido del archivo en memoria */
    datos = (double *)mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE, fd, 0);
    assert(datos != MAP_FAILED);
    printf("Datos iniciales: %lf\n", datos[0]);
    datos[0] = 3.141592;
    /* Desmapeamos el archivo de la memoria */
    int rc = munmap(datos, filesize);
    assert(rc == 0);
    /* Cerramos el archivo */
    close(fd);
}
