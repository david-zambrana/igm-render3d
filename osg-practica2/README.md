# igm-osg-practica2

## Makefile

El Makefile esta compuesto por tres _targets_:
  + **compile**: este _target_ crea el ejecutable `spinningcube`.
  + **run**: llama al _target_ `compile` y acto seguido ejecuta el binario.
  + **clear**: _target_ que elimina el ejecutable si existe.

## Dependencias

En debian 12 fue necesario instalar los siguientes paquetes para que funcionase la libreria de OSG
```
apt install openscenegraph libopenscenegraph-dev graphviz
```

## Grafos

Los grafos se generaron usando los ficheros `.ogst` generados al ejecutar `spinningcube`.

Se siguieron los siguientes pasos:
1. Ejecutar `spinningcube` para generar el archivo `grafo_ejX.ogst`
2. Ejecutar el comando **osgconv grafo_ejX.osgt grafo_ejX.dot**
3. Generar la imgen del grafo ejecutando el comando **dot -Tpng -o grafo_ejX.png grafo_ejX.dot**
