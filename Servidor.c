#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


MYSQL *con;

void finish_with_error(MYSQL *con)
{
	fprintf(stderr, "%s\n", mysql_error(con));
	mysql_close(con);
	exit(1);        
}

// Login
int login(char* usuario, char* contrasena) {
	char query[200];
	sprintf(query, "SELECT * FROM JUGADOR WHERE NOMBRE='%s' AND PASSWORD='%s'", usuario, contrasena);
	
	if (mysql_query(con, query)) {
		finish_with_error(con);
	}
	MYSQL_RES *result = mysql_store_result(con);
	
	if (result == NULL) {
		finish_with_error(con);
	}
	
	int num_rows = mysql_num_rows(result);
	
	mysql_free_result(result);
	
	return num_rows;
}

// Registro
int registro(char* usuario, char* contrasena, int jugadorid) {
	char query[200];
	sprintf(query, "INSERT INTO JUGADOR(JUGADORID, NOMBRE, PASSWORD) VALUES(%d, '%s', '%s')", jugadorid, usuario, contrasena);
	if (mysql_query(con, query)) {
		finish_with_error(con);
	}
	
	return 1;
}

//Consulta1 (Devuelve las partidas que haya ganado el jugador)
int consulta1(MYSQL *conn, char* id) {
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	int err;
	
	char consulta[80];
	sprintf(consulta, "SELECT COUNT(*) FROM PARTIDA WHERE GANADORPARTIDA = %s", id);
	
	err = mysql_query(conn, consulta);
	if (err != 0) {
		printf("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	resultado = mysql_store_result(conn);
	row = mysql_fetch_row(resultado);
	
	if (row == NULL)
		printf("No se han obtenido datos en la consulta\n");
	else
		printf("El jugador con id %s ha ganado %s partidas\n", id, row[0]);
	
	return 0;
}



int main()
{
	char usuario[20];
	char contrasena[20];

	con = mysql_init(NULL);
	
	if (con == NULL) 
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}
	
	if (mysql_real_connect(con, "localhost", "root", "mysql", 
						   "Juego", 0, NULL, 0) == NULL) 
	{
		finish_with_error(con);
	}
    //Registro
	
	printf("¿Quieres registrarte? (s/n): ");
	char opcion;
	scanf(" %c", &opcion); 
	
	if (opcion == 's' || opcion == 'S') {
		printf("Introduce tu nombre de usuario para registrarte: ");
		scanf("%s", usuario);
		
		printf("Introduce tu contrasena para registrarte: ");
		scanf("%s", contrasena);
		
		printf("Introduce tu ID de jugador para registrarte: ");
		int jugadorid;
		scanf("%d", &jugadorid);
		
		int registro_exitoso = registro(usuario, contrasena, jugadorid);
		
		if (registro_exitoso) {
			printf("Registro exitoso. Ahora puedes iniciar sesion y realizar consultas en la base de datos.\n");
		}
	}
		
	
	// Solicitar al usuario que introduzca su nombre de usuario y contrasena
	
	
	printf("Introduce tu nombre de usuario: ");
	scanf("%s", usuario);
	
	printf("Introduce tu contrasena: ");
	scanf("%s", contrasena);
	// Llamar a la funcion login con estos datos
	int login_exitoso = login(usuario, contrasena);
	
	// Si el inicio de sesion es correcto, se le permite al usuario realizar consultas
	if (login_exitoso) {
		// Solicitar al usuario que introduzca un ID
			char id[20];
		printf("Introduce el ID del jugador para consultar el numero de partidas ganadas: ");
		scanf("%s", id);
		
		// Llamar a la funcion consulta1 con este ID
		consulta1(con, id);("Inicio de sesion exitoso. Ahora puedes realizar consultas en la base de datos.\n");
		
	
	}
}



