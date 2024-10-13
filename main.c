#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gps.h"

#define PORT 8888

// Funzione per servire file statici
static enum MHD_Result serve_file(const char *file_path, struct MHD_Connection *connection) {
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("File not found: %s\n", file_path);
        return MHD_NO; // File non trovato
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *file_content = malloc(file_size);
    fread(file_content, 1, file_size, file);
    fclose(file);

    struct MHD_Response *response = MHD_create_response_from_buffer(file_size, file_content, MHD_RESPMEM_MUST_FREE);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    printf("Served file: %s (%zu bytes)\n", file_path, file_size);
    return ret;
}

// Funzione per gestire le richieste
static enum MHD_Result handle_request(void *cls, struct MHD_Connection *connection,
                                      const char *url, const char *method,
                                      const char *version, const char *upload_data,
                                      size_t *upload_data_size, void **con_cls) {
    if (strcmp(url, "/gps") == 0) {
        return serve_gps(cls, connection, url, method, version, upload_data, upload_data_size, con_cls);
    }

    // Servire file statici dalla cartella res
    if (strcmp(url, "/res/ol.css") == 0) {
        return serve_file("res/ol.css", connection);
    }
    if (strcmp(url, "/res/ol.js") == 0) {
        return serve_file("res/ol.js", connection);
    }

    // Pagina principale
    const char *page = "<html><head><title>La tua mappa</title>"
                       "<link rel='stylesheet' href='/res/ol.css'>"
                       "<script src='/res/ol.js'></script>"
                       "<style>#map { width: 100%; height: 100vh; }</style>"
                       "</head><body>"
                       "<h1>La tua mappa</h1>"
                       "<div id='map'></div>"
                       "<script>"
                       "const map = new ol.Map({"
                       "    target: 'map',"
                       "    layers: ["
                       "        new ol.layer.Tile({"
                       "            source: new ol.source.OSM()"
                       "        })"
                       "    ],"
                       "    view: new ol.View({"
                       "        center: ol.proj.fromLonLat([-0.1278, 51.5074])," // Londra
                       "        zoom: 12"
                       "    })"
                       "});"
                       "function updatePosition() {"
                       "    fetch('/gps')"
                       "        .then(response => {"
                       "            if (!response.ok) {"
                       "                throw new Error('Network response was not ok');"
                       "            }"
                       "            return response.json();"
                       "        })"
                       "        .then(data => {"
                       "            const latitude = parseFloat(data.latitude);"
                       "            const longitude = parseFloat(data.longitude);"
                       "            const coordinates = ol.proj.fromLonLat([longitude, latitude]);"
                       "            map.getView().setCenter(coordinates);"
                       "        })"
                       "        .catch(error => {"
                       "            console.error('Errore nel recupero dei dati GPS:', error);"
                       "        });"
                       "}"
                       "updatePosition();"
                       "</script></body></html>";

    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(page), 
                                                                (void *) page, 
                                                                MHD_RESPMEM_PERSISTENT);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

int main() {
    struct MHD_Daemon *daemon;

    // Avvia il server sulla porta 8888
    daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL, 
                              &handle_request, NULL, MHD_OPTION_END);

    if (NULL == daemon) {
        fprintf(stderr, "Errore: impossibile avviare il server\n");
        return 1;
    }

    printf("Server in esecuzione su http://localhost:%d\n", PORT);

    // Mantieni il server in esecuzione
    getchar();

    MHD_stop_daemon(daemon);

    return 0;
}
