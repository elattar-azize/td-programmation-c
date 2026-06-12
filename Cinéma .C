#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define CAPACITE_VIP 30
#define CAPACITE_NORMALE 70
#define MAX_RESERVATIONS 100


typedef struct {
    int id;
    char nom[50];
    char prenom[50];
    char telephone[15];
    char film[50];
    char date[11];
    char heure[6];
    char salle[10];
    int numPlace;
    char categorie[15];
    int tarif;
} Reservation;

Reservation liste[MAX_RESERVATIONS];
int nbReservations = 0;
int prochainID = 1;


void viderBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int calculerTarif(char* salle, char* cat) {
    if (strcmp(salle, "VIP") == 0) {
        if (strcmp(cat, "Etudiant") == 0) return 180;
        if (strcmp(cat, "Enfant") == 0) return 170;
        return 195;
    } else {
        if (strcmp(cat, "Etudiant") == 0) return 70;
        if (strcmp(cat, "Enfant") == 0) return 60;
        return 85;
    }
}

int estPlaceOccupee(int place, char* salle, char* date, char* heure) {
    int i;
    for (i = 0; i < nbReservations; i++) {
        if (liste[i].numPlace == place &&
            strcmp(liste[i].salle, salle) == 0 &&
            strcmp(liste[i].date, date) == 0 &&
            strcmp(liste[i].heure, heure) == 0) {
            return 1;
        }
    }
    return 0;
}



void reserverBillet(void) {
    Reservation r;
    int limite;

    if (nbReservations >= MAX_RESERVATIONS) {
        printf("\nErreur : Base de donnees pleine.\n");
        return;
    }

    r.id = prochainID++;

    printf("\n--- Nouvelle Reservation ---\n");
    printf("Nom du film : "); viderBuffer(); scanf("%[^\n]s", r.film);
    printf("Date (JJ/MM/AAAA) : "); scanf("%s", r.date);
    printf("Heure (HH:MM) : "); scanf("%s", r.heure);
    printf("Salle (VIP/Normale) : "); scanf("%s", r.salle);
    printf("Numero de place : "); scanf("%d", &r.numPlace);

    limite = (strcmp(r.salle, "VIP") == 0) ? CAPACITE_VIP : CAPACITE_NORMALE;

    if (r.numPlace < 1 || r.numPlace > limite || estPlaceOccupee(r.numPlace, r.salle, r.date, r.heure)) {
        printf("\nERREUR : Place invalide ou occupee !\n");
        prochainID--;
        return;
    }

    printf("Nom : "); scanf("%s", r.nom);
    printf("Prenom : "); scanf("%s", r.prenom);
    printf("Telephone : "); scanf("%s", r.telephone);
    printf("Categorie (Etudiant/Normal/Enfant) : "); scanf("%s", r.categorie);

    r.tarif = calculerTarif(r.salle, r.categorie);
    liste[nbReservations++] = r;
    printf("\nOK ! ID: %d | Tarif: %d dh\n", r.id, r.tarif);
}

void annulerReservation(void) {
    int id, trouve, i;
    trouve = -1;

    printf("\nID a annuler : ");
    scanf("%d", &id);

    for (i = 0; i < nbReservations; i++) {
        if (liste[i].id == id) {
            trouve = i;
            break;
        }
    }

    if (trouve != -1) {
        for (i = trouve; i < nbReservations - 1; i++) {
            liste[i] = liste[i + 1];
        }
        nbReservations--;
        printf("Annule avec succes.\n");
    } else {
        printf("ID introuvable.\n");
    }
}

void afficherReservations(void) {
    int i;
    if (nbReservations == 0) {
        printf("\nVide.\n");
        return;
    }
    printf("\n--- Liste ---\n");
    for (i = 0; i < nbReservations; i++) {
        printf("ID:%d | %s | Film:%s | Place:%d\n",
               liste[i].id, liste[i].nom, liste[i].film, liste[i].numPlace);
    }
}

void rechercherReservation(void) {
    char nomCherche[50];
    int i, trouve;
    trouve = 0;

    printf("\nNom client : ");
    scanf("%s", nomCherche);

    for (i = 0; i < nbReservations; i++) {
        if (strcmp(liste[i].nom, nomCherche) == 0) {
            printf("ID:%d | Film:%s\n", liste[i].id, liste[i].film);
            trouve = 1;
        }
    }
    if (!trouve) printf("Rien trouve.\n");
}

void afficherPlacesDisponibles(void) {
    char salle[10], date[11], heure[6];
    int i, limite;

    printf("\nSalle (VIP/Normale) : "); scanf("%s", salle);
    printf("Date : "); scanf("%s", date);
    printf("Heure : "); scanf("%s", heure);

    limite = (strcmp(salle, "VIP") == 0) ? CAPACITE_VIP : CAPACITE_NORMALE;
    for (i = 1; i <= limite; i++) {
        if (!estPlaceOccupee(i, salle, date, heure)) {
            printf("%d ", i);
        }
    }
    printf("\n");
}


int main() {
    int choix;
    do {
        printf("\n1.Reserver \n2.Annuler \n3.Liste \n4.Dispo \n5.Chercher \n6.Quitter\nChoix : ");
        if (scanf("%d", &choix) != 1) {
            viderBuffer();
            continue;
        }

        switch(choix) {
            case 1: reserverBillet(); break;
            case 2: annulerReservation(); break;
            case 3: afficherReservations(); break;
            case 4: afficherPlacesDisponibles(); break;
            case 5: rechercherReservation(); break;
        }
    } while (choix != 6);
    return 0;
}
