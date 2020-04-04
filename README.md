# Compression - méthode de Huffman

Permet la compression/décompression, sans perte, d'un fichier passé en paramètre en utilisant le codage de Huffman.

## Codage de Huffman

Ce codage utilise un code à longueur variable pour représenter chaque symbole de la source. Il est déterminé à partir du nombre d'occurrences de ce symbole dans la source.
On associe un code court aux symboles les plus fréquents. 

## Compilation et utilisation

Pour compiler les différents fichiers, il suffit d'exécuter la commande :

	make

Compression du fichier vingtmille.txt :

	compression vingtmille.txt vingtmille

Décompression du fichier compressé vingtmille : 

	decompression vingtmille vingtmille.txt
	
## Exemples de résultats

|                      | vingmille.txt     | Caillou.bmp | chaton.jpg|
| ---------------------|---------|-------|--------------|
| Taille avant | 920 Ko  | 2,19 Mo    |129 Ko|
| Taille après  | 537 Ko | 1,27 Mo   |130 Ko|