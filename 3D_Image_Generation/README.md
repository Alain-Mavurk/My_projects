Nom: Mavurk Alain 
Numéro étudiant: 21502543

Binôme: Aka Aka Evrard 
Numéro étudiant de mon binôme: 21506167

Précision dans le fichier Json:
Dans la fonction json_get, une variable (key) a été allouée mais il n'y avait pas de free correspondant, on a donc rajouter le free(key).

Dans le fichier Main, il nous manque la fonction anti-aliasing.

Nous n'avons pas inclus les args.h et args.c dans le Makefile car il n'y avait pas la documentation pour le lancement avec option.

Pour que notre application fonctionne convenablement, il faut au préalable un fichier Json avec les champs "scene3D" et "camera" remplis. Vous pouvez bien evidemment modifier les données de la scène ou de la caméra par la suite.



