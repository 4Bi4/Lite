esto no es un README.md pero lo será algún día

NORMAS DEL REPO:
	1. TERMINANTEMENTE PROHIBIDO Y PENADO POR LA LEY SUBIR CAMBIOS SIN HABER LIMPIADO CON FCLEAN (queda feo)
	2. SOLO SE PUSHEARA AL MAIN VERSIONES ESTABLES (las de testeo en la rama de cada uno y luego ya mergearemos)
	3. IDK SOLO SE ME OCURREN 2 NORMAS

LIBRERÍAS NECESARIAS:
	todas las librerías está incluidas en la carpeta vendor

MAKEFILE:
	make		- compila el programa (pero aun no hay programa)
	make test	- compila un miniprograma de testeo (para comprobar librerias y tal)
	make fclean	- elimina los ejecutables o objetos para limpiar la carpeta


TO DO LIST:

	[x] Fijar Resolución Lógica: Usar SDL_SetRenderLogicalPresentation para que el tamaño de la ventana deje de importar.

	[x] Definir constantes del Mundo: Establecer el ancho y alto máximo del mapa.

	[x] Sistema de Cámara:
		Crear una estructura Camera.
		Hacer que la cámara siga al jugador (con suavizado si quieres).
		Modificar el Render de tus objetos para que usen las coordenadas de la cámara.

	[x] Pintar el Suelo: Cargar una textura pequeña y repetirla (tiling) por todo el mapa para que el usuario sienta que se está moviendo.

	[ ] Sistema de Spawning de Enemigos: Crear una clase Enemy y un EnemyManager que los haga aparecer en los bordes de la pantalla y los dirija hacia el jugador.

	[x] IA Simple de Persecución: Los enemigos deben calcular su vector de dirección hacia la x, y del jugador (usando la misma normalización que ya hiciste).

	[ ] Detección de Colisiones (AABB): Implementar una función que compruebe si dos SDL_FRect se solapan. La necesitarás para:
		Jugador vs Enemigo (recibir daño).
		Proyectil vs Enemigo (matar).

	[ ] Sistema de Disparo Automático: Un temporizador que busque al enemigo más cercano y cree un objeto Projectile que se mueva hacia él.

BONUS:
	[x] Soporte para mandos (tested on a PS5 controller)