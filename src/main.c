#include "gps.h"
#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PORT 8888

// Funzione per servire il contenuto del file (ad esempio app.js)
static enum MHD_Result serve_file(const char *file_path, struct MHD_Connection *connection) {
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        return MHD_NO; // Non è stato possibile aprire il file
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *buffer = malloc(file_size);
    fread(buffer, 1, file_size, file);
    fclose(file);

    struct MHD_Response *response = MHD_create_response_from_buffer(file_size, buffer, MHD_RESPMEM_MUST_FREE);
    MHD_add_response_header(response, "Content-Type", "application/javascript"); // Imposta il tipo di contenuto per JavaScript
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

// Funzione per gestire tutte le richieste HTTP
static enum MHD_Result handle_request(void *cls, struct MHD_Connection *connection,
                          const char *url, const char *method,
                          const char *version, const char *upload_data,
                          size_t *upload_data_size, void **con_cls)
{
    if (strcmp(url, "/gps") == 0) {
        return serve_gps(cls, connection, url, method, version, upload_data, upload_data_size, con_cls);
    }
    
    if (strcmp(url, "/app.js") == 0) {
        return serve_file("public/app.js", connection); // Assicurati di utilizzare il percorso corretto di app.js
    }

    const char *page = "<html><body><h1>La tua mappa</h1>"
                       "<div id='map' style='height: 100vh;'></div>"
                       "<script src='/app.js'></script></body></html>";
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(page), 
                                                                    (void *) page, 
                                                                    MHD_RESPMEM_PERSISTENT);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

int main()
{
    struct MHD_Daemon *daemon;

    // Avvia il server sulla porta 8888
    daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL, 
                              &handle_request, NULL, MHD_OPTION_END);

    if (NULL == daemon)
        return 1;

    printf("Server in esecuzione su http://localhost:%d\n", PORT);

    // Mantieni il server in esecuzione
    getchar();

    MHD_stop_daemon(daemon);

    return 0;
}
