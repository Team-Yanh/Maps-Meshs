# Maps-Meshs
Projet S4 - Epita

Lorsque l'on exécute le projet, une interface s'ouvre et permet de gérer tout le traitement de l'image jusqu'à la génération 3D.
On dispose de plusieurs boutons:
- Le premier permet d'ouvrir une image.
- Le second de lancer le traitement actuel avec l'image placée à gauche.
- Le troisième de lancer le traitement suivant avec l'image qui se situe sur la droite.
- Le quatrième permet de dessiner sur les images avec la couleur de dessin.
- Le cinquième permet de relier deux points avec la coueleur de dessin.
- Les deux sixièmes permettent d'afficher soit l'image des lignes topologiques traitées soit l'image des rivières.
- Le premier slider permet de modifier la taille du trait du dessin.
- Le deuxième slider correspond à l'interval de prise en compte pendant le traitement des lignes (plus l'interval est grand plus une couleur éloignée de celle sélectionnée peut être conservée).
- Ensuite les 3 autres emplacements servent à récupérer une couleur sur les images pour les lignes topologiques à traitées ou encore les rivières ou la couleur de dessin.
- Enfin les champs RGB permettent de changer la couleur de dessin avec des valeurs RGB.
Une fois que le traitement est finis, la génération 3D se lance. Si on quitte le générateur 3D, on revient sur l'interface si l'on souhaite obtenir un meilleur résultat 3D en modifiant l'image précédemment traitée.

Une fois le rendu 3D lancé, l'utilisateur peut se déplacer librement dans l'espace.
- Pour avancer, il faut appuyer sur la touche z
- Pour reculer, il faut appuyer sur la touche s
- Pour aller vers la droite, il faut appuyer sur la touche d
- Pour aller vers la gauche, il faut appuyer sur la touche q
- Pour aller vers le haut, il faut appuyer sur la touche espace
- Pour aller vers le bas, il faut appuyer sur la touche shift gauche.
Déplacer la souris va permettre de changer l'orientation dans laquelle l'utilisateur regarde
La molette de la souris permet de zoomer ou dézoomer jusqu'à une certaine valeure
Les touches flèche du haut et flèche du bas permettent (respectivement) d'augmenter et de baisser la vitesse de déplacement de la caméra

