# igm-osg-practica2

## Makefile

El Makefile esta compuesto por tres _targets_:
  + **compile**: este _target_ crea el ejecutable `spinningcube`.
  + **run**: llama al _target_ `compile` y acto seguido ejecuta el binario.
  + **clear**: _target_ que elimina el ejecutable si existe.

## Dependencias

En debian 12 fue necesario instalar los siguientes paquetes para que funcionase la libreria de OSG
```
apt install openscenegraph libopenscenegraph-dev
```