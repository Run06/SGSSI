#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main() {
    char fecha[11];
    char comando[1024];

    const char *origen = "/home/marcos/Documentos/Seguridad";
    const char *destino = "/var/tmp/Backups";

    /* Obtener fecha YYYY-MM-DD */
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(fecha, sizeof(fecha), "%Y-%m-%d", &tm);

    /* Comando rsync incremental */
    snprintf(comando, sizeof(comando),
        "ULTIMA=$(ls -1 %s | sort | tail -n 1); "
        "if [ -d \"%s/$ULTIMA\" ]; then "
        "rsync -av --link-dest=%s/$ULTIMA %s/ %s/%s/; "
        "else "
        "rsync -av %s/ %s/%s/; "
        "fi",
        destino, destino, destino, origen, destino, fecha,
        origen, destino, fecha
    );

    system(comando);

    return 0;
}

