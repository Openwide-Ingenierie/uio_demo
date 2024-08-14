# UIO Demo

Ce projet a été créé avec l'article de blog "Comment exposer au userland des interruption avec UIO"
https://linuxembedded.fr/2024/07/comment-exposer-au-userland-des-interruption-avec-uio

Il contient le code illustrant l'utilisation de UIO pour le développement de drive dans le Userland.

Ce repo créé un driver et une appli de test:
- le driver écoute les interrution clavier. 
  Il les rend disponible sur le /dev/uioX en read().
  Le compte d'interrutions est écrit dans le permier "int" d'un mmap.
- L'application attends les interruption sur le device /dev/uio0 avec read.
  Elle va lire le nombre d'interruption dans le premier "int" du mmap.
  Elle s'arrête au bout de 10 interruptions.

Pour utiliser le projet:

- cloner : git clone
- installer les header kernel : sudo apt install linux-headers-$(uname -r)
- builder le module : make
- builder l'appli de test : make uio_app
- charger uio : sudo modprobe uio
- charger le driver : sudo insmod uio_kbd.ko
- lancer l'appli de test : sudo ./uio_app
- taper sur le clavier (principal) doit afficher des messages:
    Read interrupt count : 14 
    Read interrupt count : 15...
  Et dmesg doit afficher des traces:
    [xxxx.xxxx] UIO handler
- décharger le module : sudo rmmod uio_kbd



