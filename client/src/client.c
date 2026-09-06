#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Leemos los valores del config usando config_get_string_value
    ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTO");
    valor = config_get_string_value(config, "CLAVE");

    // Loggeamos el valor de config que acabamos de leer
    log_info(logger, "Lecura de config - Clave: %s", valor);

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
    conexion = crear_conexion(ip, puerto);

    // Enviamos al servidor el valor de CLAVE como mensaje
    enviar_mensaje(valor, conexion);

    // Armamos y enviamos el paquete
    paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "CLIENTE", true, LOG_LEVEL_INFO);
    return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");
    
    // IMPORTANTE: Como pide la guía, siempre hay que chequear que no devuelva NULL por si el archivo no existe
    if (nuevo_config == NULL) {
        // No se pudo crear el config, terminamos el programa
        abort();
    }

    return nuevo_config;
}

void leer_consola(t_log* logger)
{
    char* leido;

    // La primera te la dejo de yapa
    leido = readline("> ");

    // El resto, las vamos leyendo y logueando hasta recibir un string vacío
    while (strcmp(leido, "") != 0) {
        log_info(logger, "%s", leido);
        free(leido);
        leido = readline("> ");
    }

    // ¡No te olvides de liberar las lineas antes de regresar!
    free(leido);
}

void paquete(int conexion) //para leer de consola y mandar el paquete
{
	// 1. Creamos el paquete
    t_paquete* paquete = crear_paquete();

    char* leido;
    // 2. Volvemos a leer de consola de la misma forma para ir agregando las líneas al paquete
    leido = readline("> ");
    while (strcmp(leido, "") != 0) {
        // Agregamos cada línea al paquete (con su '\0' sumándole 1 al tamaño)
        agregar_a_paquete(paquete, leido, strlen(leido) + 1);
        free(leido);
        leido = readline("> ");
    }
    free(leido); // Liberamos la última línea vacía

    // 3. Enviamos el paquete a través de la conexión
    enviar_paquete(paquete, conexion);

    // 4. ¡Muy importante! No te olvides de eliminar el paquete para liberar memoria
    eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
    // Destruimos el logger y el config
    log_destroy(logger);
    config_destroy(config);
    
    // Cerramos la conexión con el servidor (esta función ya viene creada en utils.c)
    liberar_conexion(conexion);
}