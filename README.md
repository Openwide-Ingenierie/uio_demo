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

- cloner : _git clone_
- installer les header kernel : _sudo apt install linux-headers-$(uname -r)_
- builder le module : _make_
- builder l'appli de test : _make uio_app_
- charger uio : _sudo modprobe uio_
- charger le driver : _sudo insmod uio_kbd.ko_
- lancer l'appli de test : _sudo ./uio_app_
- taper sur le clavier (principal) doit afficher des messages:  
    _Read interrupt count : 14_  
    _Read interrupt count : 15..._  
  Et dmesg doit afficher des traces:  
    _[xxxx.xxxx] UIO handler_  
- décharger le module : _sudo rmmod uio_kbd_




