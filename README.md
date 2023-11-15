# Taller MPI

Daniela Brenes Otárola - Oscar Méndez Granados

## Especificaciones utilizadas

A continuanción se presentan las especificaciones en las que se ejecutó el taller.

- Sistema operativo: Linux Ubuntu 22.04.3 LTS
- Se utilizó VirtualBox en su versión 6.1.38_Ubuntu r153438
- Las VM se realizaron con:
  - Ubuntu 18.04.6 LTS (bionic) (console version) (2 nodos slaves)
  - Ubuntu 18.04.6 LTS (bionic) (complete version) (1 nodo master)
  - GCC Compiler: gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0

## Descripción de los archivos del taller:
- `arraySum.c` código con la implementación de la suma de los elementos de un arreglo utilizando MPI.
- `reference.py` programa de referencia que verifica el resultado de la suma de los elementos del arreglo.
- `script.py` script que ejecuta el programa `arraySum.c` 1000 veces con distintos valores de <BATCHES>.
- `graph.py` programa que grafica la relación tiempo de ejecución vs. tamaño de mensaje/número de <BATCHES>.

## Modo de ejecución

### Compilación y ejecución de los archivos

#### Programa MPI
- Compilar el archivo del código fuente: `mpicc arraySum.c -o arraySum`
- Ejecución del programa MPI `mpiexec -f hostfile.txt -n 3 ./arraySum <BATCHES>`
  En el comando anterior, el "hostfile.txt" es un archivo que contiene las direcciones IP específicas de las máquinas a ser utilizadas por MPI. El argumento <BATCHES> hace referencia a las distintas porciones en las que puede ser dividido el arreglo para cada slave, por lo cual se pueden colocar números enteros como 1, 2, 3, 4, 5... Un batch de 1 se refiere a que cada slave realizar su suma parcial a lo largo de solo una porción del array (se divide en mitades entre cada slave). Un batch de 2 por lo tanto implica que cada slave realiza dos sumas parciales a través de dos batches/porciones del array. Por lo tanto, a mayor número de <BATCHES>, menor es el tamaño del mensaje transmitido.

#### Script y Gráfica
- Con el comando `python3 script.py` se ejecutan las distintas corridas del programa MPI. Esto llenará un txt llamado "results.txt" con los resultados de cada ejecución. Este paso es indispensable para que el próximo programa pueda hacer su trabajo.
- Al utilizar `python3 graph.py` se grafica la relación entre tiempos de ejecución vs. número de <BATCHES>. Este programa utiliza como input el archivo "results.txt".

#### Programa de referencia
- Este programa realiza la suma de los elementos del array de largo 10000. `python3 reference.py`.

## Link del repositorio

[Link del repositorio](https://github.com/Oscarmendez825/MPI-Taller.git)