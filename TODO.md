# DOC:
- TCP protocole
- UDP new protocol
- ECS update


##########################################################GABI :
- tag death + systeme qui play les animations de death

- IU basic (nb joeurs + jauge de tir), points, level
- Coller les niveau sur gimp + ecran noir avec ecris niveau 2, niveau 3 etc

- LEVEL DESIGN collisions
- MOB : dasn playership.cpp ne pas collider avec les bords du niveau / + check le tag "mob" et ne pas cllider avec le decord

- enum pour les udp !!!
- essayer de passer un tableau static aux unpack & pack sans new


#########################################################MAXIME :
spawn les mobs depuis le serv & send les pos toutes les 0.5s

##########################################################EDOUARD : 

kill les mobs quands le serveur dis qu'ils sont touchées
kill les enemy quand ils sont touchés

colide les joueurs avec les murs + animation d'explosion
kill les joueurs quand ils touchent les murs + animation explosion (sparse_array<spawn> explosion ?)
kill les joueurs quand les mobs le touche

degats player -> mob
degars mob -> player
mob IA
mob spawn de bonus
Bonus effets